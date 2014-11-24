#ifndef rng_uniform_h
#define rng_uniform_h

#include <math.h>

#include "Random.h"

template<typename T>
const T uniform(const T& min, const T& max, 
			   RAND_FUNC_PTR = &RAND, 
			   const int rand_max = RAND_MAX)
{	
	return (T)((double)(max - min)*rand()/(double)rand_max + (double)min);
}

namespace rng {

template<class T>
class Uniform
	: public Random<T>
{
	public:
								Uniform(const T& min = T(0), 
								  const T& max = T(1),
								  RAND_FUNC_PTR = &RAND,
								  const int rand_max = RAND_MAX);
	
		T&						min(void);
		T&						max(void);
		const T&				operator () (void) const;
	
	private:
		T						min_;
		T						max_;
};

template<class T>
Uniform<T>::Uniform(const T& min, const T& max, RAND_FUNC_PTR, 
				  const int rand_max)
	: Random<T>(rand, rand_max), min_(min), max_(max)
{}

template<class T>
T& Uniform<T>::min(void)
{
	return this->min_;
}

template<class T>
T& Uniform<T>::max(void)
{
	return this->max_;
}

template<class T>
const T& Uniform<T>::operator () (void) const
{
	this->value_ = uniform(this->min_, this->max_, this->rand_, this->rand_max_);
	return this->value_;
}

} // namespace rng

#endif // rng_Uniform_h
