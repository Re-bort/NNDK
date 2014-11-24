#ifndef math_func_mapping_h
#define math_func_mapping_h

template <class InIter, class OutIter, class Func>
void map(InIter first, InIter last, OutIter result, Func func)
{
	for(; first != last; ++first, ++result)
		(*result) = func(*first);
}

#endif // math_func_mapping_h
