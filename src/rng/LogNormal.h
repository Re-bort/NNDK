#ifndef math_rng_lognormal_h
#define math_rng_lognormal_h

#include <math.h>

#include "Random.h"

template <class T>
const T	logNormal(const T& mean, const T& stdev, 
			   	  RAND_FUNC_PTR = &RAND,
			   	  const int rand_max = RAND_MAX)							
{	
	double s;
    double v1;
    double v2;
    do
    {
    	v1 = 2.0*rand()/(double)rand_max - 1.0;
        v2 = 2.0*rand()/(double)rand_max - 1.0;
        s = v1*v1 + v2*v2;
    }
    while (s >= 1.0);
	return (T)(mean + stdev*exp(v1*sqrt(-2.0*log(s)/s)));
}

namespace rng {

template <class T>
class LogNormal
	: public Random<T>
{
	public:
								LogNormal(const T& mean = T(0.0), 
									   	  const T& stdev = T(1.0),
									   	  RAND_FUNC_PTR = &RAND,
									   	  const int rand_max = RAND_MAX);
	
		T&						mean(void);
		T&						stdev(void);
		const T&				operator () (void) const;
	
	private:
		T						mean_;
		T						stdev_;
};

template <class T>
LogNormal<T>::LogNormal(const T& mean, const T& stdev, RAND_FUNC_PTR, 
				        const int rand_max)
	: Random<T>(rand, rand_max), mean_(mean), stdev_(stdev)
{}

template <class T>
T& LogNormal<T>::mean(void)
{
	return this->mean_;
}

template <class T>
T& LogNormal<T>::stdev(void)
{
	return this->stdev_;
}

template <class T>
const T& LogNormal<T>::operator () (void) const
{
	this->value_ = logNormal(this->mean_, this->stdev_, this->rand_, this->rand_max_);
	return this->value_;
}

} // namespace rng

#endif // rng_LogNormal_h
