#ifndef mlp_h
#define mlp_h

#include <math.h>
#include "Neuron.h"
#include "../stl/matrix.h"
#include "../rng/Uniform.h"
#include "../math/vecops.h"
#include "../util/matio.h"

#define MATRIX std::matrix<double> 
#define VECTOR std::vector<double>

class Mlp {

	friend class Backpropagation;
	public:
								Mlp();
								~Mlp();
								Mlp(unsigned I, 
									unsigned H, 
									NeuronType neuronType);
								Mlp(MATRIX& W_, 
									VECTOR& V_,
									NeuronType neuronType);
		
		double					operator () (VECTOR& x);
		VECTOR					operator () (MATRIX& X);
		
		void					setNeuronType(NeuronType neuronType);
		void					setInputLayerSize(unsigned i);
		void					setHiddenLayerSize(unsigned j);
		void					initialiseNeurons();
		void					initialiseWeights(double min, double max);

	public:
		MATRIX					W;				// Input weight hyperplane
		VECTOR					V;				// Output weight array
		VECTOR					biasW;			// Input biase weights
		double					biasV;			// Output bias weight
		NeuronType				neuronType;
		std::vector<Neuron *>	hiddenLayer;	

	private:
		void					deleteNeurons();
								
};

#endif // mlp_h
