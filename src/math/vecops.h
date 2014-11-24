#ifndef vectormath_h
#define vectormath_h

#include <vector>
#include <math.h>

template <class T>
void operator += (std::vector<T>& u, const std::vector<T> v)
{
	typename std::vector<T>::iterator ui = u.begin();
	typename std::vector<T>::const_iterator vi = v.begin();
	while(ui != u.end() && vi != v.end())
	{
		*ui += *vi;
		++vi;
		++ui;
	}
}

template <class T>
void operator -= (std::vector<T>& u, const std::vector<T> v)
{
	typename std::vector<T>::iterator ui = u.begin();
	typename std::vector<T>::const_iterator vi = v.begin();
	while(ui != u.end() && vi != v.end())
	{
		*ui -= *vi;
		++vi;
		++ui;
	}
}

template <class T>
void operator *= (std::vector<T>& u, const double k)
{
	typename std::vector<T>::iterator ui;
	for(ui = u.begin(); ui != u.end(); ++ui)
		*ui *= k;
}
	
template <class T>
void operator /= (std::vector<T>& u, const double k)
{
	typename std::vector<T>::iterator ui;
	for(ui = u.begin(); ui != u.end(); ++ui)
		*ui /= k;
}

template <class T>
void operator /= (std::vector<T>& u, const std::vector<T>& v)
{
	typename std::vector<T>::iterator ui = ui.begin();
	typename std::vector<T>::const_iterator vi = v.begin();
	while(ui != u.end() && vi != v.end())
	{
		*ui /= *vi;
		++ui;
		++vi;
	}
}

template <class T>
std::vector<T> operator - (const std::vector<T>& u)
{
	std::vector<T> v(u);
	typename std::vector<T>::iterator vi;
	for(vi = v.begin(); vi != v.end(); ++vi) 
		*vi = -*vi;
	return v;
}



template <class T>
const double euclidean(const std::vector<T>& u)
{
	double euclidean = 0.0;
	typename std::vector<T>::const_iterator ui;
	for(ui = u.begin(); ui != u.end(); ++ui)
		euclidean += pow(*ui, 2.0);
	return sqrt(euclidean);
}

template <class T>
const double euclidean(const std::vector<T>& u, const std::vector<T>& v)
{
	double euclidean = 0.0;
	typename std::vector<T>::const_iterator ui = u.begin();
	typename std::vector<T>::const_iterator vi = v.begin();
	while(ui != u.end() && vi != v.end())
	{
		euclidean += pow(*ui - *vi, 2.0);
		++ui;
		++vi;
	}
	return pow(euclidean, 0.5);
}

template <class T>
const double manhattan(const std::vector<T>& u) 
{
	double manhattan = 0.0;
	typename std::vector<T>::const_iterator ui;
	for(ui = u.begin(); ui != u.end(); ++ui)
	{
		manhattan += fabs(*ui);
	}
	return manhattan;
}

template <class T>
const double manhattan(const std::vector<T>& u, const std::vector<T>& v)
{
	double manhattan = 0.0;
	typename std::vector<T>::const_iterator ui = u.begin();
	typename std::vector<T>::const_iterator vi = v.begin();
	while(ui != u.end() && vi != v.end())
	{
		euclidean += fabs(*ui - *vi);
		++ui;
		++vi;
	}
	return manhattan;
}
	
template <class T>
const double minkowski(const std::vector<T>& u, double q)
{
	double minkowski = 0.0;
	typename std::vector<T>::const_iterator ui;
	for(ui = u.begin(); ui != u.end(); ++ui)
	{
		minkowski += pow(*ui, q);
	}
	return pow(minkowski, 1.0/q);
}

template <class T>
const double minkowski(const std::vector<T>& u, const std::vector<T>& v, 
					   double q)
{
	double minkowski = 0.0;
	typename std::vector<T>::const_iterator ui = u.begin();
	typename std::vector<T>::const_iterator vi = v.begin();
	while(ui != u.end() && vi != v.end())
	{
		minkowski += pow(*ui - *vi, q);
		++ui;
		++vi;
	}
	return pow(minkowski, 1.0/q);
}

template <class T>
std::vector<T> operator * (const double k, std::vector<T> u)
{ 
 	typename std::vector<T>::iterator ui;
    for(ui = u.begin(); ui != u.end(); ++ui)
    	*ui *= k;
    return u;
}

template <class T>
std::vector<T> operator * (const std::vector<T>& u, const double k)
{ 
    std::vector<T> uk;
    uk.reserve( u.size() );

    for(typename std::vector<T>::const_iterator ui = u.begin();
    	ui != u.end();
	++ui)
    {
    	uk.push_back( (*ui)*k );
    }
    
    return uk;
}

template <class T>
std::vector<T> operator / (std::vector<T> u, const double k)
{ 
	typename std::vector<T>::iterator ui = u.begin();
   	for(ui = u.begin(); ui != u.end(); ++ui)
   	   	*ui /= k;
    return u;
}

template <class T>
double operator * (const std::vector<T>& u, const std::vector<T>& v)
{
    double product = 0.0;
    typename std::vector<T>::const_iterator vi = v.begin();
    typename std::vector<T>::const_iterator ui = u.begin();
    while(ui != u.end() )
    {
    	product += (*ui)*(*vi);
    	++ui;
    	++vi;
    }
	return product;
}

template <typename T, template <class T> class V>
V<T> operator * (const V<T>& u, const V<T>& v)
{
	V<T> uv(u.size());
	typename V<T>::const_iterator ui = u.begin();
	typename V<T>::const_iterator vi = v.begin();
	typename V<T>::const_iterator uvi = uv.begin();
	for(; ui != u.end() && vi != v.end(); ++ui, ++vi, ++uvi)
		*uvi = (*ui)*(*vi);
	return uv;
}

template <class T>
std::vector<T> operator + (std::vector<T> u, const std::vector<T>& v)
{ 
	typename std::vector<T>::iterator ui = u.begin();
    typename std::vector<T>::const_iterator vi = v.begin();
    while(ui != u.end() && vi != v.end())
    {
    	*ui += *vi;
    	++ui;
    	++vi;
    }
	return u;
}

template <class T>
std::vector<T> operator - (std::vector<T> u, const std::vector<T>& v)
{ 
   	typename std::vector<T>::iterator ui = u.begin();
   	typename std::vector<T>::const_iterator vi = v.begin();
    while(ui != u.end() && vi != v.end())
    {
    	*ui -= *vi;
    	++ui;
    	++vi;
    }
    return u;
}

template <class T>
std::vector<T> midpoint(const std::vector<T>& u)
{
    return 0.5*(u);
}

template <class T>
std::vector<T> midpoint(const std::vector<T>& u, const std::vector<T>& v)
{
    return 0.5*(u + v);
}

template <class T>
std::vector<T> fabs(const std::vector<T>& u)
{
    std::vector<T> v(u.size());
    typename std::vector<T>::const_iterator ui = u.begin();
    typename std::vector<T>::iterator vi = v.begin();
    for(; ui != u.end(); ++ui, ++vi)
        *vi = fabs(*ui);
    return v;
}

#endif // std::vectormath_h
