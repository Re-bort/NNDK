#ifndef statistics_h
#define statistics_h

#include <algorithm>

#include "../matrix"
#include "constants.h"
#include "vecops.h"

template <typename T>	
T min(const std::vector<T>& v)
{
	T min = v.at(0);
	typename std::vector<T>::const_iterator vi = v.begin();
	for(; vi != v.end(); ++vi)
		if (*vi < min)
			min = *vi;
	return min;
}

template <typename T>	
T max(const std::vector<T>& v)
{
	T max = v.at(0);
	typename std::vector<T>::const_iterator vi = v.begin();
	for(; vi != v.end(); ++vi)
		if (*vi > max)
			max = *vi;
	return max;
}

template <typename T>	
T range(const std::vector<T>& v)
{
	return max(v) - min(v);
}

template <typename T>
T sum(const std::vector<T>& v)
{
	T  sum(*v.begin());
	typename std::vector<T>::const_iterator vi = v.begin() + 1;
	for(; vi != v.end(); ++vi)
		sum += *vi;
	return sum;
}

template <typename T>	
T ave(const std::vector<T>& v)
{
	return (T)sum(v)/(double)v.size();
}

template <typename T>	
T median(std::vector<T> v)
{
	T median;
	sort(v.begin(), v.end());
	unsigned n = v.size();
	if ((n % 2) != 0) 	// n is odd 
		median = v[n/2]; 					
	else			// n is even
		median = 0.5*(v[(n/2)] + v[(n/2) + 1]); 	
	return median;	
}

template <typename T>
double mad(const std::vector<T>& v)
{
	T v50 = median(v);
	std::vector<double> d(v.size());
	typename std::vector<T>::const_iterator vi = v.begin(); 
	typename std::vector<T>::iterator di = d.begin();
	while(vi != v.end())
	{
		*di = (T)fabs(*vi - v50);
		++vi;
		++di;
	}
	return median(d);
}

template <typename T>
double covar(const std::vector<T>& u, const std::vector<T>& v)
{
	// SAFETY: Can't calculate covariance of 0 length vectors
	// or vectors of different lengths;
	if(u.size() != v.size() || u.size() == 0)
		return 0;
	
	double u_ = ave(u);
	double v_ = ave(v);
	typename std::vector<T>::const_iterator ui = u.begin();
	typename std::vector<T>::const_iterator vi = v.begin();
	double covar = 0.0;
	for(; ui != u.end() && vi != v.end(); ++ui, ++vi)
		covar += (*ui - u_)*(*vi - v_);
	covar /= (double)v.size();
	return covar;
}

template <typename T>	
inline T var(const std::vector<T>& v)
{
	return covar(v, v);
}

template <typename T>
inline double stdev(const std::vector<T>& v)
{
	//SAFETY: Can't calculate stdev for 0 variance
	double s2 = var(v);
	if(s2 == 0.0)
		return 0;
	else
		return sqrt(s2);
}

template <typename T>
inline double stdev(const std::matrix<T>& X)
{
    double stdev = 0.0;
    std::vector<double> x(X.rows());
    for(unsigned i = 0; i < X.columns(); ++i)
    {    
        x = X.column(i);
        stdev += var(x);
    }
    return sqrt(stdev);    
}

template <typename T>  
T sumabs(const std::vector<T>& v)
{
    T sum(0);
    typename std::vector<T>::const_iterator vi = v.begin();
    for(; vi != v.end(); ++vi)
        if (*vi < T(0))
        	sum -= *vi;
        else
        	sum += *vi;
	return sum;
}

template <typename T>  
T sumsq(const std::vector<T>& v)
{
    T sum(0);
    typename std::vector<T>::const_iterator vi = v.begin();
    for(; vi != v.end(); ++vi)
            sum += (*vi)*(*vi);
    return sum;
}


template <typename T>	
double percentile(std::vector<T> v, const unsigned n)
{
	double percentile;
	if (v.size() < 2)
		percentile = 0.0;
	else
	{
		double k = (double)v.size()*(double)n/100.0;
		double a = v[(int)ceil(k) - 1];
		double b = v[(int)floor(k) - 1];
		percentile = 0.5*(a + b);
	}
	return percentile;
}

template <typename T>
std::vector<T> cumulative(const std::vector<T>& v)
{
	std::vector<T> V;
	V.reserve(v.size());
	T sum(0);
	typename std::vector<T>::const_iterator vi = v.begin();
	for(; v != v.end(); ++vi)
	{
		sum += *vi;
		V.push_back(sum);	
	}
	return V;
}

template <typename T>	
inline double iqr(const std::vector<T>& v)
{
	return percentile(v, 75) - percentile(v, 25);
}

template <typename T>	
double mae(const std::vector<T>& u, const std::vector<T>& v)
{
	double mae = 0.0;
	typename std::vector<T>::const_iterator ui = u.begin();
	typename std::vector<T>::const_iterator vi = v.begin();
	while(ui != u.end() && vi != v.end())
	{
		mae += fabs(*vi - *ui);
		++ui;
		++vi;
	}
	mae /=(double)u.size();
	return mae;
}

template <typename T>	
double mare(const std::vector<T>& u, const std::vector<T>& v)
{
	double mae = 0.0;
	typename std::vector<T>::const_iterator ui = u.begin();
	typename std::vector<T>::const_iterator vi = v.begin();
	while(ui != u.end() && vi != v.end())
	{
		if(*ui < small())
			mae += fabs(*ui - *vi) / small();
		else
			mae += fabs(*ui - *vi) / *ui;
		++ui;
		++vi;
	}
	mae /=(double)u.size();
	return mae;
}

template <typename T>	
double mre(const std::vector<T>& u, const std::vector<T>& v)
{
	double mre = 0.0;
	typename std::vector<T>::const_iterator ui = u.begin();
	typename std::vector<T>::const_iterator vi = v.begin();
	while(ui != u.end() && vi != v.end())
	{
		if(*ui < small())
			mre += fabs(*ui - *vi) / small();
		else
			mre	+= fabs(*ui - *vi) / *ui;
		++ui;
		++vi;
	}
	mre /=(double)u.size();
	return mre;
}

template <typename T>	
double mse(const std::vector<T>& u, const std::vector<T>& v)
{
	if (u.size() != v.size()) {
		return 0.0;
	}
	double mse = 0.0;
	typename std::vector<T>::const_iterator ui = u.begin();
	typename std::vector<T>::const_iterator vi = v.begin();
	for(; ui != u.end(); ++ui, ++vi) {
		mse += pow(*vi - *ui, 2.0);
	}
	mse /= (double)u.size();
	return mse;
}

template <typename T>  
double rmse(const std::vector<T>& u, const std::vector<T>& v)
{  
    return sqrt(mse(u, v));
}

template <typename T>	
double sse(const std::vector<T>& u, const std::vector<T>& v)
{
	double sse = 0.0;
	typename std::vector<T>::const_iterator ui = u.begin();
	typename std::vector<T>::const_iterator vi = v.begin();
	while(ui != u.end() &&  vi != v.end())
	{
		sse += pow((*vi) - (*ui), 2.0);
		++ui;
		++vi;
	}
	return sse;
}

template <typename T>
inline double aic(const std::vector<T>& u, const std::vector<T>& v, const long k) {
	return u.size() * log( mse(u, v) ) + 2.0 * k;
}

template <typename T>
inline double aic(const std::vector<T>& v, const double k)
{
	return  (double)v.size()*log((v*v)/(double)v.size()) + 2.0*k;
}

template <typename T>	
double pearson(const std::vector<T>& u, const std::vector<T>& v)
{
	double pearson;
	double n = u.size();
	double num =  n*(u*v) - sum(u)*sum(v);
	double den = sqrt((n*(u*u) - sum(u)*sum(u)) * (n*(v*v) - sum(v)*sum(v)));
	pearson = num/den;
	return pearson;
}

template <typename T>
double mi(const std::vector<T>& px, const std::vector<T>& py, 
		  const std::vector<T>& pxy)
{
	double mi = 0.0;
    typename std::vector<T>::const_iterator pxi = px.begin();
    typename std::vector<T>::const_iterator pyi = py.begin();
    typename std::vector<T>::const_iterator pxyi = pxy.begin();
    while(pxi != px.end() && pyi != py.end() && pxyi != pxy.end())
    {
        mi += log(*pxyi/( (*pxi)*(*pyi) ) );
        ++pxi;
        ++pyi;
        ++pxyi;
    }
    mi /= (double)pxy.size();
    return mi;
}

template <typename T>
double H(const std::vector<T>& px)
{
	double H = 0.0;
	typename std::vector<T>::const_iterator pxi = px.begin();
    while(pxi != px.end())
    {
        H += (*pxi)*log(*pxi);
        ++pxi;
    }
    H = -H/(double)px.size();
    return H;
}

template <typename T>	
const double qks(const double lambda)
{
	double f, q = 0.0;
	const unsigned qkslimit = 10000;
	
	for(unsigned j = 1; j <= qkslimit; ++j)
	{
		f = exp( -2.0*pow( (double)j, 2.0)*pow(lambda, 2.0) );
		if (f < small()) // if summation converges
		{
			return( 2.0*q );
		}
		q += pow(-1.0, (double)(j-1))*f;
	}
	
	return 1.0;
}


template <typename T>	
const double ks(std::vector<T> u, std::vector<T> v)
{
	double Fu = 0.0, Fv = 0.0, D = 0;
	
	sort(u.begin(), u.end());
	sort(v.begin(), v.end());
	
	typename std::vector<T>::iterator ui = u.begin(), vi = v.begin();
	unsigned i = 1, j = 1;
	while(ui != u.end() && vi != v.end())
	{
		Fu = (double)i/(double)u.size();
		Fv = (double)j/(double)v.size();
		
		if(fabs(Fu - Fv) > D)
			D = fabs(Fu - Fv);
		
		if (*ui <= *vi) 
		{
			++i;
			++ui;
		} else {
			++j;
			++vi;
		}
	}
	return D;
}

template <typename T>
std::vector<double> summary(const std::vector<T>& v)
{
	std::vector<T> s;
	s.push_back(v.size());
    s.push_back(min(v));
	s.push_back(ave(v));
	s.push_back(max(v));
	s.push_back(var(v));
	s.push_back(iqr(v));
	return s;
}

template <typename T>
std::matrix<T> summary(const std::matrix<T>& A)
{
    std::matrix<double> S(A.columns(), 5);
    for(unsigned j = 0; j < A.columns(); ++j)
    {
    	std::vector<double> s = A.column(j);
        S[j] = summary(s);
    }
    return S;
}


template <typename T>
const double student(const std::vector<T>& u, const std::vector<T>& v)
{
    
    return (ave(u) - ave(v)) / 
        sqrt(var(u) / (double)u.size() + var(v) / (double)v.size());    
}

template <typename T>
const double anova(const std::vector<T>& u, const std::vector<T>& v)
{
    double s1 = var(u), s2 = var(v);
    if(s1 > s2) 
        return s1 / s2;
    else
        return s2 / s1;
}


template <typename T>
const double Fstat(const std::vector<T>& u, const std::vector<T>& v)
{
	return var(u) / var(v);
}

template <typename T>
const double tstat(const T x, 
			   const T varx, 
			   const unsigned nx, 
               const T y,
               const T vary, 
               const unsigned ny)
{
    return (x - y) / sqrt((varx) / (double)nx + (vary) / (double)ny);
}


#endif // statistics_h
