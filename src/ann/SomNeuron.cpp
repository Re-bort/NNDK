#include "Som.h"

#define VECTOR std::vector<double>
#define INDEX std::vector<unsigned>

Som::Neuron::Neuron()
{}

Som::Neuron::Neuron(const Som::Neuron& neuron)
	: VECTOR(neuron), indices_(neuron.indices_), 
	  distances_(neuron.distances_)
{}

Som::Neuron::Neuron(const unsigned dimension)
	: VECTOR(dimension, 0)
{}

Som::Neuron::Neuron(const VECTOR& v)
	: VECTOR(v)
{}

Som::Neuron::~Neuron()
{}

const INDEX& Som::Neuron::indices() const
{
	return indices_;
}

const unsigned Som::Neuron::index(unsigned i) const
{
	return indices_[i];
}

const std::vector<double>& Som::Neuron::distances() const
{
	return distances_;
}

const double Som::Neuron::distance(unsigned i) const
{
	return distances_[i];
}

const unsigned Som::Neuron::size() const
{
	return indices_.size();
}

const bool Som::Neuron::empty() const
{
	return indices_.empty();
}

const bool Som::Neuron::singleton() const
{
	if (indices_.size() == 1)
		return true; 
	else
		return false;
}

void Som::Neuron::push_back(const unsigned i, const double d)
{
	indices_.push_back(i);
	distances_.push_back(d);
}

void Som::Neuron::clear()
{
	indices_.clear();
	distances_.clear();
}



#undef VECTOR
#undef INDEX
