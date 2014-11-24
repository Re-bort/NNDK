#ifndef rng_Normal_h
#define rng_Normal_h

#include <math.h>

#include "Random.h"

template <class T>
const T normal(const T& mean, const T& stdev, 
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
	return (T)(mean + stdev*(v1*sqrt((-2.0*log(s))/s)));
}

namespace rng {

template <class T>
class Normal
	: public Random<T>
{
	public:
								Normal(const T& mean = T(0.0), 
									   const T& stdev = T(1.0),
									   RAND_FUNC_PTR = &RAND,
									   const int rand_max = RAND_MAX);
	
        const T&        	    operator () (void) const;
    	const T&				mean() const;
		const T&				stdev() const;
    
    private:
        T                       mean_;
        T                       stdev_;
};

template <class T>
Normal<T>::Normal(const T& mean, const T& stdev, RAND_FUNC_PTR, 
				  const int rand_max)
	: Random<T>(rand, rand_max), mean_(mean), stdev_(stdev)
{}

template <class T>
const T& Normal<T>::mean(void) const
{
    return this->mean_;
}

template <class T>
const T& Normal<T>::stdev(void) const
{
    return this->stdev_;
}

template <class T>
const T& Normal<T>::operator () (void) const
{
	this->value_ = normal(this->mean_, this->stdev_, this->rand_, this->rand_max_);
	return this->value_;
}

} // namespace rng

#endif // rng_Normal_h
