#include "sjLaplacianSmoothing.h"
#include <vector>
#include <cmath>  
#include <map>
#include <CGAL/Timer.h>
#include <fstream>
#include <iostream>

#include "sjDataType.h"
#include "sjUtils.h"
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/IO/Polyhedron_scan_OFF.h> 

#include <OGF/basic/types/counted.h>
#include <OGF/math/attributes/attribute_adapter.h>

#include <OGF/math/linear_algebra/vector.h>
#include <OGF/math/linear_algebra/matrix.h>
#include <OGF/math/geometry/types.h>
#include <OGF/math/numeric/linear_solver.h>

using namespace std;
using namespace OGF;

sjLaplacianSmoothing::sjLaplacianSmoothing():
WH_0(1.0), 
WL_0(1.0), 
SL(0), 
iteration(0), 
AVERAGE_FACE(0.0),
MAX_CONTRACTION_SQUARED( 100000.),
MAX_ATTRACTION(100000000.),
MIN_COT_ANGLE(0.00000000000000001)
{
}

void sjLaplacianSmoothing::setMeshG(sjPolyhedron meshG){
	mesh_G = meshG;
}

sjPolyhedron sjLaplacianSmoothing::getMeshG(){
	return mesh_G;
}

size_t sjLaplacianSmoothing::getNsize(){
	return mesh_G.size_of_vertices();
}

void sjLaplacianSmoothing::initIndex(){
	int i = 0;
	for ( sjVIterator v1 = mesh_G.vertices_begin(); v1 != mesh_G.vertices_end(); ++v1){
		sjHalfedge_vertex_circulator vcir = v1->vertex_begin();
		vcir->vertex()->index = i;
		vcir->vertex()->initial_ring_area = 0.0;
		i++;
	}
}

void sjLaplacianSmoothing::computeRings(){
	rings.clear();
	for ( sjVIterator v = mesh_G.vertices_begin(); v != mesh_G.vertices_end(); ++v){
		sjHalfedge_vertex_circulator vcir = v->vertex_begin();
		vector< sjVertex_handle> neighbors;
		do{
			sjVertex_handle punto = (vcir->next()->vertex());
			neighbors.push_back(punto);
		}while(++vcir != v->vertex_begin ());
		vcir->vertex()->initial_ring_area = areaRing(v, neighbors);
		rings.push_back(neighbors);
	}
}

map<int, double> sjLaplacianSmoothing::computeLaplacian(sjVIterator vi, vector< sjVertex_handle> neighbors){
	
		sjHalfedge_vertex_circulator vcir = vi->vertex_begin();
		sjPoint_3 point_i = vi->point();
		vector< sjVertex_handle> vecinos = neighbors;

		map<int, double> Lij;

		double Wij = 0.0;
		double L = 0.0;

		int j_pos, j_Alpha_pos, j_Beta_pos;
		for(j_pos = 0; j_pos < (int)(vecinos.size()); j_pos++){
			
			j_Alpha_pos = ((j_pos + 1)+vecinos.size()) % vecinos.size();
			j_Beta_pos = ((j_pos - 1)+vecinos.size()) % vecinos.size();
			sjPoint_3 point_Alpha = ((vecinos[j_Alpha_pos]))->point();
			sjPoint_3 point_Beta = ((vecinos[j_Beta_pos]))->point();
			double Alpha = angle3(point_i, point_Alpha, ((vecinos[j_pos]))->point());
			if(Alpha < MIN_COT_ANGLE)				Alpha = MIN_COT_ANGLE;
			double Beta = angle3(point_i, point_Beta, ((vecinos[j_pos]))->point());
			if(Beta < MIN_COT_ANGLE)				Beta = MIN_COT_ANGLE;
			Wij = 1.0/ std::tan(Alpha) + 1.0/std::tan(Beta);
			Lij[vecinos[j_pos]->index] = Wij;
			L = L - Wij;
		}
		Lij[vi->index] = L;
	return Lij;	
}

double sjLaplacianSmoothing::averageFaces(){
	double average = 0.0, area;
	int nface = 0;

	for(sjFIterator f = mesh_G.facets_begin(); f !=mesh_G.facets_end(); ++f ){
		sjHalfedge_facet_circulator vcir = f->facet_begin();
		vector< sjVertex_handle> vertexs;
		do{
			sjVertex_handle punto = vcir->next()->vertex();
			vertexs.push_back(punto);
		}while(++vcir != f->facet_begin ());
		area = area3(vertexs[0]->point(),vertexs[1]->point(),vertexs[2]->point());
		average = average + area;
		nface = nface + 1;
	}
	return average/((double)nface);
}

void sjLaplacianSmoothing::pruebaOpenNL(){
	sjLeastSquaresSolver solver(2);
	solver.set_least_squares(true) ;
	solver.begin_system() ;
	    solver.begin_row() ;
		solver.add_coefficient(0, 0.0)  ;
		solver.add_coefficient(1, 1.0)  ;
		solver.set_right_hand_side(1.0);
		solver.end_row() ;
		solver.begin_row() ;
		solver.add_coefficient(0, 1.0)  ;
		solver.add_coefficient(1, 0.0)  ;
		solver.set_right_hand_side(2.0);
		solver.end_row() ;
		solver.begin_row() ;
		solver.add_coefficient(0, 0.0)  ;
		solver.add_coefficient(1, 1.0)  ;
		solver.set_right_hand_side(3.0);
		solver.end_row() ;
	solver.end_system() ;
	if ( solver.solve() ){
		cout<<"\n\nResolvio el sistema.";
		cout<<solver.variable(0).value();
		cout<<"\n";
		cout<<solver.variable(1).value();
	}
}

double sjLaplacianSmoothing::areaRing(sjVIterator vi, vector< sjVertex_handle> neighbors){
	int i, j, m, pos_1, pos_2;
	double area = 0.0;
	m = neighbors.size();
	for(j=0; j<m; j++){
		pos_1 = j;
		pos_2 = (j+1) % m;
		//sjPoint_3 point_i = sjPoint_3(matrix_V[i][0], matrix_V[i][1], matrix_V[i][2]);
		sjPoint_3 point_i = vi->point();
		area = area + area3(point_i,((neighbors[pos_1]))->point(),((neighbors))[pos_2]->point());
	}
	return area;
}

bool sjLaplacianSmoothing::isDegenerateVertex(sjVIterator vi, vector< sjVertex_handle> neighbors){
	int i, j, m, pos_1, pos_2;
	double area;
	m = neighbors.size();
	for(j=0; j<m; j++){
		pos_1 = j;
		pos_2 = (j+1) % m;
		sjPoint_3 point_i = vi->point();
		area = area3(point_i,((neighbors[pos_1]))->point(),((neighbors))[pos_2]->point());
		if(AVERAGE_FACE/MAX_CONTRACTION_SQUARED > area){
			return true;
		}
	}

	map<int, double> mymap = computeLaplacian(vi, neighbors);
	map<int, double>::iterator it;
	for ( it=mymap.begin() ; it != mymap.end(); it++ ){
		int index = (*it).first ;
		double value = (*it).second;
		if(value > MAX_ATTRACTION){
			return true;
		}
	}
	return false;
}

void sjLaplacianSmoothing::initLaplacianSmoothing(double a_WH0, double a_WL0, double a_SL){
	
	initIndex();
	computeRings();
	AVERAGE_FACE = averageFaces();
	WH_0 = a_WH0;
	//WL_0 = a_WL0 * std::sqrt(AVERAGE_FACE);
	WL_0 = 1.0;
	SL = a_SL;
	
}

void sjLaplacianSmoothing::iterateLaplacianSmoothing(){
	cout<<"Iteration "<<iteration<<endl;
	CGAL::Timer timer;
    timer.start();
	int i;
	sjVIterator v;
	int n = getNsize();
	int coord;
	vector<vector<double>> matrixV = vector<vector<double>>(n,vector<double>(3,0.0));
	cout<<"Configuration "<<endl;
	double WLi = std::pow(SL,((double)iteration)) * WL_0;
	//double WLi = 1.0;
	bool found_solution = true;

	for(coord = 0; coord<3; coord++){
		sjLeastSquaresSolver solverx(n);
		//sjLeastSquaresSolver solvery(n);
		//sjLeastSquaresSolver solverz(n);
		solverx.set_least_squares(true) ;
		//solvery.set_least_squares(true) ;
		//solverz.set_least_squares(true) ;
		i = 0;
		for ( v = mesh_G.vertices_begin(); v != mesh_G.vertices_end(); ++v){
			sjPoint_3 point_k = v->point();
			double r = point_k[coord];
			solverx.variable(i).set_value(r);		
			if(isDegenerateVertex(v, rings[i])){
				solverx.variable(i).lock();
			}
			
			i++;
		}
		solverx.begin_system();
		//solvery.begin_system();
		//solverz.begin_system();
		i = 0;
		//cout<<"Laplacian\n";
		for ( v = mesh_G.vertices_begin(); v != mesh_G.vertices_end(); ++v){


			if(!isDegenerateVertex(v, rings[i])){

			sjHalfedge_vertex_circulator vcir = v->vertex_begin();
			map<int, double> mymap = computeLaplacian(v, rings[i]);
			map<int, double>::iterator it;
			solverx.begin_row();
			//solvery.begin_row();
			//solverz.begin_row();
			for ( it=mymap.begin() ; it != mymap.end(); it++ ){
				int index = (*it).first ;
				double value = (*it).second;
				solverx.add_coefficient(index, WLi * value);
				//solvery.add_coefficient(index, WLi * value);
				//solverz.add_coefficient(index, WLi * value);
				//cout<<WLi * value<<"\t\t";

			}
			//cout<<"\n";
			solverx.set_right_hand_side(0.0);
			//solvery.set_right_hand_side(0.0);
			//solverz.set_right_hand_side(0.0);
			solverx.end_row();
			//solvery.end_row();
			//solverz.end_row();


			}


			i++;
		}
		i = 0;
		for ( v = mesh_G.vertices_begin(); v != mesh_G.vertices_end(); ++v){
			if(!isDegenerateVertex(v, rings[i])){
			sjHalfedge_vertex_circulator vcir = v->vertex_begin();
			sjPoint_3 point_i = v->point();
			double Whi;
			if(iteration ==0){
				Whi = WH_0;
			}else{
				Whi = WH_0*std::sqrt(v->initial_ring_area/areaRing(v, rings[i]));
			}
			solverx.begin_row();
			//solvery.begin_row();
			//solverz.begin_row();
			solverx.add_coefficient(i, Whi);
			//solvery.add_coefficient(i, Whi);
			//solverz.add_coefficient(i, Whi);
			solverx.set_right_hand_side(Whi*point_i[coord]);
			//solvery.set_right_hand_side(Whi*point_i[1]);
			//solverz.set_right_hand_side(Whi*point_i[2]);
			solverx.end_row();
			//solvery.end_row();
			//solverz.end_row();


			}

			i++;
		}
		solverx.end_system();
		//solvery.end_system();
		//solverz.end_system();
		cout<<"Init solve system "<<endl;
		if ( solverx.solve() ){
			//solvery.solve();
			//solverz.solve();
			cout<<"End solve system "<<endl;
			for(i=0; i<n; i++){
				matrixV[i][coord] = solverx.variable(i).value();
				//matrixV[i][1] = solvery.variable(i).value();
				//matrixV[i][2] = solverz.variable(i).value();
			}
		}else{
			cout<<"No solution found"<<endl;
			found_solution = false;
		}
	}
	if(found_solution){
		i = 0;
		for ( v = mesh_G.vertices_begin(); v != mesh_G.vertices_end(); ++v){
			
			sjPoint_3 pointn = sjPoint_3(matrixV[i][0], matrixV[i][1], matrixV[i][2]);
			//cout<<"before: "<<v->point()<<",  after: "<< pointn <<endl;
			v->point() = pointn;
			i++;
		}
		

		char filename[256];
		sprintf(filename, "Salidaoff_%d.off", iteration);

		fstream file_off(filename, ios::out);

		file_off<<mesh_G;
		file_off.close();
	}
	cout<< timer.time() << " seconds." << std::endl;
	iteration++;

}

void sjLaplacianSmoothing::iterateLaplacianSmoothingOGF(){
	cout<<"LINE 0"<<endl;
	int n = getNsize();
	LinearSolver *solver;
	//delete solver;
	SystemSolverParameters params ;
	std::string  * mipo = new std::string("SUPERLU");
	params.set_arg_value("method", *mipo) ; 
	
	solver = new LinearSolver(n) ;
	cout<<"LINE 1"<<endl;
	solver->set_system_solver(params) ;
	cout<<"LINE 2"<<endl;
	solver->set_least_squares(true) ;
	solver->set_invert_matrix(true) ;
	cout<<"LINE 3"<<endl;


	cout<<"Iteration "<<iteration<<endl;
	CGAL::Timer timer;
    timer.start();
	int i;
	sjVIterator v;
	int coord;
	vector<vector<double>> matrixV = vector<vector<double>>(n,vector<double>(3,0.0));
	cout<<"Configuration "<<endl;
	double WLi = std::pow(SL,((double)iteration)) * WL_0;
	//double WLi = 1.0;
	bool found_solution = true;

	for(coord = 0; coord<3; coord++){

		i = 0;
		cout<<"LINE 4"<<endl;
		for ( v = mesh_G.vertices_begin(); v != mesh_G.vertices_end(); ++v){
			if(isDegenerateVertex(v, rings[i])){
				solver->variable(i).lock();
			}
			sjPoint_3 point_k = v->point();
			double r = point_k[coord];
			solver->variable(i).set_value(r);		
			
			
			i++;
		}
		cout<<"LINE 5"<<endl;
		solver->begin_system();
		cout<<"LINE 6"<<endl;
		i = 0;
		cout<<"Laplacian\n";
		for ( v = mesh_G.vertices_begin(); v != mesh_G.vertices_end(); ++v){

			if(!isDegenerateVertex(v, rings[i])){
				sjHalfedge_vertex_circulator vcir = v->vertex_begin();
				map<int, double> mymap = computeLaplacian(v, rings[i]);
				map<int, double>::iterator it;
				solver->begin_row();
				for ( it=mymap.begin() ; it != mymap.end(); it++ ){
					int index = (*it).first ;
					double value = (*it).second;
					solver->add_coefficient(index, WLi * value);
					//cout<<WLi * value<<"\t\t";

				}
				//cout<<"\n";
				solver->set_right_hand_side(0.0);
				solver->end_row();

			}


			i++;
		}
		cout<<"LINE 7"<<endl;
		i = 0;
		for ( v = mesh_G.vertices_begin(); v != mesh_G.vertices_end(); ++v){
			if(!isDegenerateVertex(v, rings[i])){
			sjHalfedge_vertex_circulator vcir = v->vertex_begin();
			sjPoint_3 point_i = v->point();
			double Whi;
			if(iteration ==0){
				Whi = WH_0;
			}else{
				Whi = WH_0*std::sqrt(v->initial_ring_area/areaRing(v, rings[i]));
			}
			solver->begin_row();
			solver->add_coefficient(i, Whi);
			solver->set_right_hand_side(Whi*point_i[coord]);
			solver->end_row();


			}

			i++;
		}
		cout<<"LINE 8"<<endl;
		solver->end_system();
		cout<<"LINE 9"<<endl;
		
		cout<<"Init solve system "<<endl;
		if ( solver->solve() ){
			
			cout<<"End solve system "<<endl;
			for(i=0; i<n; i++){
				matrixV[i][coord] = solver->variable(i).value();
			}
		}else{
			cout<<"No solution found"<<endl;
			found_solution = false;
		}

		solver->restart();

	}
	if(found_solution){
		i = 0;
		for ( v = mesh_G.vertices_begin(); v != mesh_G.vertices_end(); ++v){
			
			sjPoint_3 pointn = sjPoint_3(matrixV[i][0], matrixV[i][1], matrixV[i][2]);
			//cout<<"before: "<<v->point()<<",  after: "<< pointn <<endl;
			v->point() = pointn;
			i++;
		}
		

		/*char filename[256];
		sprintf(filename, "Salidaoff_%d.off", iteration);

		fstream file_off(filename, ios::out);

		file_off<<mesh_G;
		file_off.close();*/
	}
	cout<< timer.time() << " seconds." << std::endl;
	iteration++;

}