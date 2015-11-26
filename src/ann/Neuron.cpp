#include "Neuron.h"
#include <iostream>

Neuron::Neuron() : zin(0.0){}	// Abstract class
Neuron::~Neuron(){}	// Abstract class

double Threshold::operator()(double z)
{
	zin = z;	// Store neuron activation level
	if(z < 0)
		act =  -1.0;
	else
		act = 1.0;

	return act;
}

double Threshold::derivative()
{
	if(zin != 0)
		return 0.0;
	else 
		return 1.0 / small();
}

double Logistic::operator()(double z)
{
	
	zin = z;	// Store neuron activation level
	act = 1.0 / (1.0 + exp(-z));
	return act;
}

double Logistic::derivative()
{
	return (*this)(zin) * (1.0 - (*this)(zin));
}

double Tanh::operator()(double z)
{
	zin = z;	// Store neuron activation level
	act = tanh(z);
	return 	act; 
}

double Tanh::derivative()
{
	return 1.0 - tanh(zin) * tanh(zin);
}
