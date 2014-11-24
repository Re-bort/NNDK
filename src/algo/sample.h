#ifndef nndk_sample_h
#define nndk_sample_h

#include <algorithm>
#include <vector>
#include <iostream>

#include "../rng/Random.h"

namespace std {

template <class Iter, class OutIter, class Dist>
void random_sample(Iter first, Iter last, OutIter result, Dist b)
{
    for(Dist n = 0; n < b && first != last; ++n, ++first)
		*(result + n) = *first;
	Dist t = b + 1;
	while(first != last)
	{
		Dist n = RAND() % (t);	// P ~ U[0,t);
     	if (n < b)
			*(result + n) = *first;
     	++first;
     	++t;
	}
}

template <class Iter, class OutIter, class Dist>
void block_sample(Iter first, OutIter result, Dist b)
{
	copy(first, first + b, result);	
}

template <class Iter, class KeyIter, class OutIter>
void key_sample(Iter first, KeyIter firstKey, KeyIter lastKey, OutIter result)
{
	for(KeyIter key = firstKey; key != lastKey; ++key, ++result)
        *result = *(first + *key); 
}

template <class Iter, class SizeIter, class OutIter, class Dist>
void pps_sample(Iter first, Iter last, SizeIter firstSize, OutIter result, Dist b)
{

}

template <class Iter, class SizeIter, class OutIter, class Dist>
void biased_sample(Iter first, Iter last, SizeIter firstSize, OutIter result, Dist b)
{
    typedef typename std::iterator_traits<Iter>::value_type Type;
	typedef typename std::iterator_traits<SizeIter>::value_type Size;
	
    typename std::vector<Type> values(first, last);
    typename std::vector<Size> sizes(firstSize, firstSize + values.size());
	
    Size sum = 0.0;
	for(SizeIter size = sizes.begin(); size != sizes.end(); ++size)
		sum += *size;
    
    for(Dist i = 0; i < b; ++i, ++result)
	{
		Iter value = values.begin();
		SizeIter size = sizes.begin();
		Size U = (double)sum * std::rand() / RAND_MAX;
		Size S = *size;
		for(; S < U && size < sizes.end() - 1; ++size, ++value)
			S += *size;
		*result = *value;	
		sum -= *size;
		values.erase(value);
		sizes.erase(size);
	}
}

} // namespace std

#endif // nndk_sample_h
