#ifndef som_Neuron_h
#define som_Neuron_h

#include <vector>

namespace som {

class Grid;	// Forward declaration of som::Grid class to enable friendship

class Neuron
{
	friend class Grid;
	
	public:
										Neuron();
										Neuron(const Neuron& neuron);
                                        Neuron(const unsigned dimenion);
										Neuron(const std::vector<double>& v);
										~Neuron();
		// Access methods						
		const double					distance(unsigned i) const;
		const std::vector<double>&		distances() const;
		const bool						empty() const;
		const unsigned					index(unsigned i) const;
		const std::vector<unsigned>&	indices() const;
		const bool						singleton() const;		
		const unsigned					size() const;
		const std::vector<double>&		v() const;
		// Mutate methods
		void							clear(); // Empties object lists
		void							push_back(const unsigned i, 
												  const double d);
		
	private:
		mutable std::vector<double>		v_;		// Representative vector (weight)
		mutable std::vector<unsigned> 	indices_;
		mutable std::vector<double>		distances_;
};

Neuron::Neuron()
{}

Neuron::Neuron(const Neuron& neuron)
	: v_(neuron.v()), indices_(neuron.indices()), 
	  distances_(neuron.distances())
{}

Neuron::Neuron(const unsigned dimension)
	: v_(dimension, 0)
{}

Neuron::Neuron(const std::vector<double>& v)
	: v_(v)
{}

Neuron::~Neuron()
{}

inline const std::vector<double>& Neuron::v() const
{
	return v_;
}

inline const std::vector<unsigned>& Neuron::indices() const
{
	return indices_;
}

inline const unsigned Neuron::index(unsigned i) const
{
	return indices_[i];
}

inline const std::vector<double>& Neuron::distances() const
{
	return distances_;
}

inline const double Neuron::distance(unsigned i) const
{
	return distances_[i];
}

inline const unsigned Neuron::size() const
{
	return indices_.size();
}

inline const bool Neuron::empty() const
{
	return indices_.empty();
}

const bool Neuron::singleton() const
{
	if (indices_.size() == 1)
		return true; 
	else 
		return false;
}

inline void Neuron::push_back(const unsigned i, const double d)
{
	indices_.push_back(i);
	distances_.push_back(d);
}

inline void Neuron::clear()
{
	indices_.clear();
	distances_.clear();
}


} // namespace som


#endif	// ann_som_Neuron_h
