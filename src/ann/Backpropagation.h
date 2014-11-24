#ifndef ann_Backpropagation_h
#define ann_Backpropagation_h

#include "Mlp.h"
#include "../math/statistics.h"

#define MATRIX std::matrix<double> 
#define VECTOR std::vector<double> 

class Backpropagation
{
	public:
								Backpropagation();
								~Backpropagation();
			void				trainOnline(Mlp& network, 
									MATRIX& inputs, 
									VECTOR& targets);

			void				trainOnlineCV(Mlp& network, 
									MATRIX& trainingInputs, 
									VECTOR& trainingTargets,
									MATRIX& testInputs,
									VECTOR& testTargets);
			
			void				trainBatch(Mlp& network, 
									MATRIX& trainingInputs, 
									VECTOR& trainingTargets,
									MATRIX& testInputs,
									VECTOR& testTargets);
									
			double				learningRate;
			double				momentum;
			unsigned long		maxEpochs;
			unsigned long		maxIterations;
			unsigned long		maxTestCount;
			double 				tolerance;
			
			double				error;
			
			
			void				initialise(Mlp& network);
			
	private:
			void				getWeightUpdates(Mlp& network, VECTOR& z, double error);
			void 				applyWeightUpdates(Mlp& network);
			
			unsigned long		iteration;
			unsigned long		epoch;
			double 				errorPrev;
			double				deltaError;
			double				testError;
			double				minTestError;
			double				testCount;
			Mlp					bestNetwork;
	
			VECTOR 				delta;
			VECTOR 				deltaV;
			VECTOR 				deltaVPrev;
			double 				deltaBiasV;
			double 				deltaBiasVPrev;
			
			MATRIX 				deltaW;
			MATRIX 				deltaWPrev;
			VECTOR 				deltaBiasW;
			VECTOR 				deltaBiasWPrev;
			
			MATRIX				W;
			VECTOR				V;
			VECTOR				biasW;
			double				biasV;
};


#endif // ann_Backpropagation_h
