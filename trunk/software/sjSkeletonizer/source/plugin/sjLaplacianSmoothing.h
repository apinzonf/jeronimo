#ifndef __SJLAPLACE_SMOOTH__H__
#define __SJLAPLACE_SMOOTH__H__

#include "sjPipeFilter.h"
#include "sjKernelPlugin.h"
#include "sjStateContext.h"
#include "sjState.h"
#include "sjDataIO.h"

#include <string>

using namespace std;

namespace sj{

	class InitIndex: public sjState{
	public:
		bool evolve(sjStateContext * ssc);
	};
	class PluginInitIndex: public sjPlugin{
	public:
		PluginInitIndex();
		void registerPlugin(sjKernelPlugin & K);
		sjSystem * createSystem();
	};

	class ComputeRings: public sjState{
	public:
		bool evolve(sjStateContext * ssc);
	};
	class PluginComputeRings:public sjPlugin{
	public:
		PluginComputeRings();
		void registerPlugin(sjKernelPlugin & K);
		sjSystem * createSystem();
	};

	class ComputeLaplacian: public sjSystem{
	public:
		void proccesEvent(sjEvent * evt){}
		map<int, double> computeLaplacian(sjStateContext * m_ctx, sjVIterator vi, vector< sjVertex_handle> neighbors);
	};
	class PluginComputeLaplacian: public sjPlugin{
	public:
		PluginComputeLaplacian();
		void registerPlugin(sjKernelPlugin & K);
		sjSystem * createSystem();
	};

	class ComputeMeanCurvature: public sjState{
	public:
		bool evolve(sjStateContext * ssc);
		sjPoint_3 computeMeanCurvature(sjVIterator vi, vector< sjVertex_handle> neighbors);	
	};
	class PluginComputeMeanCurvature: public sjPlugin{
		public:
		PluginComputeMeanCurvature();
		void registerPlugin(sjKernelPlugin & K);
		sjSystem * createSystem();
	};

	class MeanCurvatureSmoothing: public sjState{
	public:
		bool evolve(sjStateContext * ssc);
	};
	class PluginMeanCurvatureSmoothing: public sjPlugin{
	public:
		PluginMeanCurvatureSmoothing();
		void registerPlugin(sjKernelPlugin & K);
		sjSystem * createSystem();
	};

	class IsDegenerateVertex: public sjSystem{
	public:
		IsDegenerateVertex();
		void proccesEvent(sjEvent * evt){}
		bool isDegenerateVertex(sjStateContext * m_ctx, sjVIterator vi, vector< sjVertex_handle> neighbors);
	private:
		ComputeLaplacian * m_comlapl;
	};
	class PluginIsDegenerateVertex: public sjPlugin{
	public:
		PluginIsDegenerateVertex();
		void registerPlugin(sjKernelPlugin & K);
		sjSystem * createSystem();
	};

	class ComputeLineEquations: public sjSystem{
	public:
		void proccesEvent(sjEvent * evt){}
		sjVIterator computeLineEquations(sjVIterator vi, vector< sjVertex_handle> neighbors);
	};
	class PluginComputeLineEquations:public sjPlugin{
	public:
		PluginComputeLineEquations();
		void registerPlugin(sjKernelPlugin & K);
		sjSystem * createSystem();
	};

	class InitLaplacianSmoothing: public sjState{
	public:
		bool evolve(sjStateContext * ssc);
		void initLaplacianSmoothing(double a_WH0 = 1.0, double a_WL0 = 0.01, double a_SL = 2.0);
	};
	class PluginInitLaplacianSmoothing: public sjPlugin{
		public:
		PluginInitLaplacianSmoothing();

		void registerPlugin(sjKernelPlugin & K);
		sjSystem * createSystem();
	};

	class IterateSmoothingAlgorithm: public sjState{
	public:
			virtual ~IterateSmoothingAlgorithm(){}
			virtual bool evolve(sjStateContext * ssc) = 0;
	};

	class IterateLaplacianSmoothingSeparate: public IterateSmoothingAlgorithm{
	public:
		bool evolve(sjStateContext * ssc);
	};
	class PluginIterateLaplacianSmoothingSeparate: public sjPlugin{
	public:
		PluginIterateLaplacianSmoothingSeparate();
		void registerPlugin(sjKernelPlugin & K);
		sjSystem * createSystem();
	};

	class IterateLaplacianSmoothingIntegrate: public IterateSmoothingAlgorithm{
	public:
		bool evolve(sjStateContext * ssc);
	};
	class PluginIterateLaplacianSmoothingIntegrate: public sjPlugin{
	public:
		PluginIterateLaplacianSmoothingIntegrate();
		void registerPlugin(sjKernelPlugin & K);
		sjSystem * createSystem();
	};

	class OFFLoaderSource: public sjPolyhedronPipe::sjSource{
	public:
		OFFLoaderSource(string filename){
			printf("OFFLoaderSource + %s 1\n", filename.c_str());
			m_filename = filename;
			printf("OFFLoaderSource 2\n");
		}
		sjPolyhedronPipe::PolyhedronType produce(){
			printf("OFFLoaderSource produce 1\n");
			sjDataIO dataio;
			dataio.setFileName(m_filename );
			try{
				dataio.load();
				printf("OFFLoaderSource produce 2\n");
				return dataio.getPolyhedronModel();
			}catch(std::exception e) {
			}
			return sjPolyhedronPipe::PolyhedronType();
		}
	private:
		string m_filename;
	};
}
#endif
