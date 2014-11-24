#ifndef random_h
#define random_h

#include <iostream>
#include <cstdlib>
#include <ctime>

// NOTE: C++ standard template library algorithms and other functions that
// require a PRNG do not strictly call the C-function std::rand() in 
// cstdlib.h and instead may call rand48() or other non-standard generators.  
// Examples include std::random_shuffle(), which does not call rand() and may
// generate unexpected results when shuffling STL containers if std::srand() 
// is called to seed the generator.  For this reason the code first checks 
// whether or not the compiler has the drand48 function, which is then used as
// the default PRNG rather than the obselete std::rand().

#ifdef _GLIBCPP_HAVE_DRAND48
#define RAND lrand48
#define SRAND(seed) srand48(seed)
#define RAND_FUNC_PTR long int (*rand)() throw ()
#define RAND_FUNC_MEM_PTR long int (*rand_)() throw ()
#else
#define RAND std::rand
#define SRAND(seed) srand(seed)
#define RAND_FUNC_PTR int (*rand)(void)
#define RAND_FUNC_MEM_PTR int (*rand_)()
#endif

namespace rng {

inline void seed(int seed = -1)
{
    if (seed < 1) 
        seed = time(NULL);
    SRAND(seed);
}

template <class T>
class Random
{
	public:
								Random(RAND_FUNC_PTR = &RAND,
									   const int rand_max = RAND_MAX);
                                Random(const Random& random);
        virtual                 ~Random();
		virtual const T&		operator()(void) const = 0;
		const T&				value(void) const;
					
	protected:
		RAND_FUNC_MEM_PTR;		// Function pointer to # generator
		int				        rand_max_;	        // Number generator limit
		mutable T 			    value_;		        // Last generated value
        
};

template <class T>
Random<T>::Random(RAND_FUNC_PTR, const int rand_max)
	: rand_(rand), rand_max_(rand_max)
{}

template <class T>
Random<T>::Random(const Random<T>& random)
    : rand_(random.rand_), rand_max_(random.rand_max_)
{}

template <class T>
Random<T>::~Random()
{}

template <class T>
const T& Random<T>::value() const
{
	return this->value_;
}

} // namespace rng

#endif // rng_Random_h
