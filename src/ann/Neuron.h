#ifndef ann_Neuron_h
#define ann_Neuron_h

#include <math.h>
#include "../math/constants.h"

enum NeuronType
{
	ThresholdFunc = 0,
	LogisticFunc = 1,
	TanhFunc = 2
};

class Neuron
{
	public:
		virtual double operator()(double z) = 0;
		virtual double derivative() = 0;
		virtual ~Neuron(); 
			
		double 	zin;
		double 	act;
				
	protected:
		Neuron();	// Abstract class
};

class Threshold 
	: public Neuron 
{
	public:
		double operator()(double z); 
		double derivative();
};
	
class Logistic 
	: public Neuron
{
	public:
		double operator()(double z); 
		double derivative(); 
};

class Tanh 
	: public Neuron
{
	public:
		double operator()(double z); 
		double derivative(); 
};

#endif // ann_Neuron_h
