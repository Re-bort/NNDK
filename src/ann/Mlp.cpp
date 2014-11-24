
#include "Mlp.h"

Mlp::Mlp(){}

Mlp::~Mlp()
{
	deleteNeurons();
}

Mlp::Mlp(const unsigned I, const unsigned H, NeuronType neuronType_)
	: W(H, I), V(H), biasW(H), biasV(0.0), neuronType(neuronType_)
{
	setHiddenLayerSize(H);
}

Mlp::Mlp(MATRIX& W_, VECTOR& V_, NeuronType neuronType_)
	:W(W_), V(V_), neuronType(neuronType_)
{
	setHiddenLayerSize(W.rows());
}

void Mlp::initialiseWeights(double minInitWeight, double maxInitWeight)
{
	rng::Uniform<double> U(minInitWeight, maxInitWeight);
    for(unsigned j = 0; j < W.rows(); ++j)
    {
    	for(unsigned i = 0; i < W.columns(); ++i)
			W[j][i] = U();
		biasW[j] = U();
    }

	for(unsigned j = 0; j < V.size(); ++j)
		V[j] = U();
	biasV = U();
}

double Mlp::operator () (VECTOR& x)
{
	double y = 0.0;
	for(unsigned j = 0; j < hiddenLayer.size(); ++j)
	{
		y += (*hiddenLayer[j])(x * W[j] + biasW[j]) * V[j];
	}
	y += biasV;
	
	return y;
}

VECTOR Mlp::operator () (MATRIX& X)
{
	VECTOR Y(X.rows());
	VECTOR::iterator yi = Y.begin();
	for(MATRIX::iterator xi = X.begin(); xi != X.end(); ++xi, ++yi)
	{
		*yi = 0;
		for(unsigned j = 0; j < hiddenLayer.size(); ++j)
		{
			*yi += (*hiddenLayer[j])(*xi * W[j] + biasW[j]) * V[j];
		}
		*yi += biasV;
	}

	return Y;
}

void Mlp::deleteNeurons()
{
	for(unsigned i = 0; i < hiddenLayer.size(); ++i)
		delete hiddenLayer[i];
}

void Mlp::setNeuronType(NeuronType neuronType_)
{
	neuronType = neuronType_;
	initialiseNeurons();
}

void Mlp::setInputLayerSize(unsigned i)
{
	W.resize(W.rows(), i);
}

void Mlp::initialiseNeurons()
{
	switch(neuronType)
	{
		case ThresholdFunc:
			for(unsigned j = 0; j < hiddenLayer.size(); ++j)
				hiddenLayer[j] = new Threshold();
			break;
		
		case TanhFunc:
			for(unsigned j = 0; j < hiddenLayer.size(); ++j)
				hiddenLayer[j] = new Tanh();
			break;
		
		case LogisticFunc:
			// Fall through
		
		default:
			for(unsigned j = 0; j < hiddenLayer.size(); ++j)
				hiddenLayer[j] = new Logistic();
			break;	
	}
}

void Mlp::setHiddenLayerSize(unsigned j)
{
	deleteNeurons();
	hiddenLayer.resize(j);
	initialiseNeurons();
	
	W.resize(j, W.columns());
	biasW.resize(j);
	V.resize(j);
}
