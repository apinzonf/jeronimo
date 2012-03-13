#include "sjStateContext.h"
#include "sjKernelPlugin.h"

using namespace sj;

sjStateContext::sjStateContext():
state(0), 
m_params(0){
	state = (sjState *)(sjKernelPlugin::getInstance().getSystem(sjKernelPlugin::SYS_INIT_INDEX_SYSTEM));
	m_init_system = false;
}

bool sjStateContext::evolve(int delta_frame){
	return state->evolve(this);
}

void sjStateContext::setState(sjState * a_state){
	state  = a_state;
}

sjState * sjStateContext::getState(){
	return state;
}

void sjStateContext::proccesEvent(sjEvent * evt){
	state->proccesEvent(evt);
}

sjPolyhedronPipe::PolyhedronType * sjStateContext::iterate(){
	if(m_init_system == false){
		this->mesh_G = * input_pipe->read();
		m_init_system = true;
	}
	if(this->evolve(1)){
		return & (this->getMesh());
	}else{
		//No more iterations
	}
	return NULL;
}

void sjStateContext::setParameters(sjPolyhedronPipe::ParametersType * a_params){
	m_params = a_params;
}

sjPolyhedronPipe::ParametersType * sjStateContext::getParameters(){
	return this->m_params;
}

void sjStateContext::setMesh(sjPolyhedronPipe::PolyhedronType meshG){	
	this->mesh_G = meshG;
}

size_t sjStateContext::getNumberOfVertex(){
	return mesh_G.size_of_vertices();
}

sjPolyhedronPipe::PolyhedronType sjStateContext::getMesh(){
	return mesh_G;
}

void sjStateContext::setRings (vector< vector<sjVertex_handle> > a_rings){
	m_rings = a_rings;
}

vector< vector<sjVertex_handle> > sjStateContext::getRings(){
	return m_rings ;
}