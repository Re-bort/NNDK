#include "Backpropagation.h"

#include "../util/matio.h"

Backpropagation::Backpropagation()
{
	learningRate = 0.1;
	momentum = 0.8;
	tolerance = 0.00001;
	maxIterations = 100000;
	maxTestCount = 100;
	iteration = 0;
}
Backpropagation::~Backpropagation()
{}

void Backpropagation::trainBatch(Mlp& network, 
	MATRIX& trainingInputs, 
	VECTOR& trainingTargets,
	MATRIX& testingInputs,
	VECTOR& testingTargets)
{
	VECTOR trainingOutputs (trainingTargets.size(), 0.0);
	VECTOR meanInput = ave(trainingInputs);		
	while (epoch < maxEpochs)
	{
		trainingOutputs  = network(trainingInputs);
		error = sse(trainingOutputs, trainingTargets);

		getWeightUpdates(network, meanInput, error);
		applyWeightUpdates(network);
		
		++epoch;
	}
}

void Backpropagation::trainOnline(Mlp& network, MATRIX& inputs, VECTOR& targets)
{
	VECTOR outputs(targets.size(), 0.0);
	while(iteration < maxIterations)
	{
		VECTOR::iterator output = outputs.begin();
		VECTOR::iterator target = targets.begin();
		for(MATRIX::iterator input = inputs.begin(); 
			input != inputs.end(); 
			++input, ++target, ++output)
		{
			*output = network(*input);
			double err = *output - *target;
			
			getWeightUpdates(network, *input, err);
			
			applyWeightUpdates(network);
			
			++iteration;
		}
		++epoch;
	}
}

void Backpropagation::trainOnlineCV(Mlp& network, 
	MATRIX& trainingInputs, 
	VECTOR& trainingTargets,
	MATRIX& testInputs,
	VECTOR& testTargets)
{
	VECTOR trainingOutputs(trainingTargets.size(), 0.0);
	VECTOR testOutputs(testTargets.size(), 0.0);
	
	while(error > tolerance && testCount < maxTestCount)
	{
		VECTOR::iterator output = trainingOutputs.begin();
		VECTOR::iterator target = trainingTargets.begin();
		for(MATRIX::iterator input = trainingInputs.begin(); 
			input != trainingInputs.end(); 
			++input, ++target, ++output)
		{
			*output = network(*input);
			double err = *output - *target;
			
			getWeightUpdates(network, *input, err);
			
			applyWeightUpdates(network);
			
			++iteration;
			
			if(iteration >= maxIterations)
				break;
		}
		
		++epoch;
		
		error = mse(trainingTargets, trainingOutputs);
		
		// Early-stopping using test (cross-validation) error
		testOutputs = network(testInputs);
		testError = mse(testTargets, testOutputs);
		if(testError < minTestError)
		{
			// Preserve test error and network weights
			minTestError = testError;
			W = network.W;			
			V = network.V;
			biasW = network.biasW;
			biasV = network.biasV;
			testCount = 0;
		}
		else
		{
			++testCount;
		}
	}
	
	network.W = W;
	network.V = V;
	network.biasW = biasW;
	network.biasV = biasV;
	testError = minTestError;
}

void Backpropagation::getWeightUpdates(Mlp& network, VECTOR& z, double err)
{
	unsigned long J = network.W.rows();
	unsigned long I = network.W.columns();
	for(unsigned j = 0; j < J; ++j)
	{
		deltaV[j] = -err * learningRate * network.hiddenLayer[j]->act +  
				momentum * deltaVPrev[j];
	
		delta[j] = -err * network.hiddenLayer[j]->derivative() * network.V[j];
		for(unsigned i = 0; i < I; ++i)
		{
			deltaW[j][i] = learningRate * delta[j] * z[i] 
				+ momentum * deltaWPrev[j][i];
		}
		deltaBiasW[j] = learningRate * delta[j] * 1.0 + momentum * deltaBiasWPrev[j];
	}
	deltaBiasV = -err * learningRate * 1.0 + momentum * deltaBiasVPrev; 
}

void Backpropagation::applyWeightUpdates(Mlp& network)
{
		unsigned long J = network.W.rows();
		unsigned long I = network.W.columns();
		for(unsigned long j = 0; j < J; ++j)
		{
			network.V[j] += deltaV[j];
			deltaVPrev[j] = deltaV[j];
		
			for(unsigned long i = 0; i < I; ++i)
			{
				network.W[j][i] += deltaW[j][i];
				deltaWPrev[j][i] = deltaW[j][i];	
			}
			
			network.biasW[j] += deltaBiasW[j];
			deltaBiasWPrev[j] = deltaBiasW[j];
		}
		
		network.biasV += deltaBiasV;
		deltaBiasVPrev = deltaBiasV;
}

void Backpropagation::initialise(Mlp& network)
{
	unsigned long I = network.W.columns();
	unsigned long J = network.W.rows();
		
	delta = VECTOR(J, 0.0);
	deltaV = VECTOR(J, 0.0);
	deltaVPrev = VECTOR(J, 0.0);
	deltaBiasV = 0.0;
	deltaBiasVPrev = 0.0;
		
	deltaW = MATRIX(J, I, 0.0);
	deltaWPrev = MATRIX(J, I, 0.0);
	deltaBiasW = VECTOR(J, 0.0);
	deltaBiasWPrev = VECTOR(J, 0.0);
			
	error = large();
	errorPrev = large();
	minTestError = large();
	
	epoch = 0;
	iteration = 0;
	testCount = 0;
}

