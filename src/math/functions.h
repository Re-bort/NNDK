#ifndef math_functions_h
#define math_functions_h

#include <math.h>
#include "function/Cowboy.h"
#include "function/Sine.h"
#include "function/Quadratic.h"
#include "function/Friedman.h"
#include "function/HillValley.h"
#include "function/FrenchCurve.h"
#include "function/Nonlinear.h"

template <class T>
T& max(const T& a, const T& b)
{
	if(b > a)
		return b;
	else 
		return a;
}

template <class T>
T& min(const T& a, const T& b)
{
	if(b < a)
		return b;
	else 
		return a;
}

template<class T>
int	sign(const T& a)
{
	if(a < (T)0)
		return -1;
	else
		return 1;
}

#endif // math_function_h
