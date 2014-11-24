#ifndef math_rng_triangular_h
#define math_rng_triangular_h

#include <math.h>

#include "Random.h"

template <class T>
const T triangular(const T& min, const T& mode, const T& max,
			   RAND_FUNC_PTR = &RAND, 
			   const int rand_max = RAND_MAX)
{	
	double u = (double)rand()/(double)rand_max;
	double total_range = max - min;
	double lower_range = mode - min;
	double upper_range = max - mode;
	if (u <= lower_range/total_range)
		return min + sqrt(u * total_range * lower_range);
	else
		return max - sqrt((1.0 - u) * total_range * upper_range);
}

namespace rng {

template <class T>
class Triangular
	: public Random<T>
{
	public:
								Triangular(const T& min = T(0.0), 
										   const T& mode = T(0.5),
									   	   const T& max = T(1.0),
									   	   RAND_FUNC_PTR = &RAND,
									   	   const int rand_max = RAND_MAX);
	
		T&						min(void);
		T&						mode(void);
		T&						max(void);
		const T&				operator () (void) const;
	
	private:
		T						min_;
		T						mode_;
		T						max_;
};

template <class T>
Triangular<T>::Triangular(const T& min, const T& mode, const T& max, 
						  RAND_FUNC_PTR, const int rand_max)
	: Random<T>(rand, rand_max), min_(min), mode_(mode), max_(max)
{}

template <class T>
T& Triangular<T>::min(void)
{
	return this->min_;
}

template <class T>
T& Triangular<T>::mode(void)
{
	return this->mode_;
}

template <class T>
T& Triangular<T>::max(void)
{
	return this->max_;
}

template <class T>
const T& Triangular<T>::operator () (void) const
{
	this->value_ = triangular(this->min_, this->mode_, this->max_, this->rand_, this->rand_max_);
	return this->value_;
}

} // namespace rng

#endif //_TRIANGULAR_H_
