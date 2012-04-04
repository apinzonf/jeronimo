#ifndef __SJ_SIMPLI_H__
#define __SJ_SIMPLI_H__

#include "sjDataType.h"
#include "sjKernelPlugin.h"
#include "sjPipeFilter.h"
#include "sjSystem.h"
#include "sjUtils.h"
#include "sjState.h"
#include <list>

using namespace std;



namespace sj{

	class sjNodeHeap {
	public:
		sjNodeHeap(int a_index, double a_value){
			index = a_index;
			value = a_value;
		}
		double value;
		int index;
		bool operator < (const sjNodeHeap& rhs){
			return value < rhs.value;
		}
	};

	class sjSimplificator: public sjPolyhedronPipe::sjFilter{
	public:
		sjSimplificator(double wa = 1.0, double wb = 0.1);
		Matrix4d getFundamentalErrorQuadric(sjHalfedge_handle);
		double calculateSamplingCost(sjHalfedge_handle);
		double calculateShapeCost(Matrix4d Qi, Matrix4d Qj,  sjHalfedge_handle he);
		double calculateTotalCost(sjHalfedge_handle he);
		Matrix4d computeInitialQ(sjVertex_handle);
		void computeAllInitialQ();
		bool isCollapseTunnel(sjHalfedge_handle);
		void computeHeapError();
		void init();
		void collapseEdge(sjHalfedge_handle he);

		void setParameters(sjPolyhedronPipe::ParametersType *){}
		void setMesh(sjPolyhedronPipe::PolyhedronType amesh ){
			mesh_G = amesh;
		}
		size_t getNumberOfVertex(){
			return mesh_G.size_of_vertices();
		}
		sjPolyhedronPipe::PolyhedronType iterate();
		sjPolyhedronPipe::PolyhedronType getMesh(){
			return mesh_G;
		}
		sjPolyhedronPipe::ParametersType * getParameters(){
			return NULL;
		}

		
	private:
		double Wa, Wb;
		map<int, Matrix4d> Qmap;
		sjPolyhedron mesh_G;
		vector< vector< sjVertex_handle > > rings;
		list<sjNodeHeap> heap_error;
		bool m_init;
	};

}

#endif //__SJ_SIMPLI_H__