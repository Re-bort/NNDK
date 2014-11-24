#ifndef Graph_h
#define Graph_h

#include <iostream>
#include <map>
#include <vector>

using namespace std;

template <class K, class T>
class Graph 
	: public map<K, T>
{
	public:
		typedef typename map<K, T>::iterator iterator;
		typedef pair<iterator, iterator> brace;

			
								Graph();
								Graph(const vector<K>& x, const vector<T>& fx);
								Graph(K * x, T * fx, unsigned n);
								~Graph();
		T						operator () (K x);
		iterator 				lower_bound(K x);
		iterator				upper_bound(K x);
		brace					bracket(K x);
		T						interpolate(K x);
};

template<class K, class T>
Graph<K, T>::Graph()
{}

template<class K, class T>
Graph<K, T>::Graph(const vector<K>& x, const vector<T>& fx)
{
	for(unsigned i = 0; i < x.size(); ++i)
		(*this)[x[i]] = fx[i];
}

template<class K, class T>
Graph<K, T>::Graph(K * x, T * fx, unsigned n)
{
	for(unsigned i = 0; i < n; ++i)
		(*this)[x[i]] = fx[i];
}

template<class K, class T>
Graph<K, T>::~Graph()
{}

template<class K, class T>
T Graph<K, T>::operator () (K x)
{
	typename map<K, T>::iterator pos = map<K, T>::find(x);
	T fx;
	if(pos == this->end()) 
		fx = interpolate(x);
	else
		fx = pos->second;
	return fx;
}

template<class K, class T>	
typename map<K, T>::iterator Graph<K, T>::lower_bound(K x)
{
	typename map<K, T>::iterator pos = this->end();
	--pos;
	while(pos != this->begin())
	{
		if(pos->first < x)
			break;
		--pos;
	}
	return pos;
}

template<class K, class T>
typename map<K, T>::iterator Graph<K, T>::upper_bound(K x)
{
	typename map<K, T>::iterator pos;
	for(pos = this->begin(); pos != this->end(); ++pos)
		if(pos->first > x)
			break;
	return pos;
}

template <class K, class T>
typename Graph<K, T>::brace Graph<K, T>::bracket(K x)
{
	typename map<K, T>::iterator lb = lower_bound(x);
	typename map<K, T>::iterator ub = upper_bound(x);
	// Check bounds and adjust for extrapolation cases
	if(ub == this->begin())
	{
		// Lower-bound extrapolation for ub == lb == begin()
		ub = this->begin();
		++ub;
	}
	else if(ub == this->end())
	{	
		// Upper-bound extrapolation ub = end, lb = end - 1	
		ub = this->end();
		--ub;	// Decrement to one from end
		lb = this->end();
		--(--lb);  //Decrement to two from end
	}
	else ; // Do nothing
	return brace(lb, ub);
}

template <class K, class T>
T Graph<K, T>::interpolate(K x)
{
	// Bracket the interval [a,b] s.t. a < x < b
	typename Graph<K, T>::brace interval = this->bracket(x);
	// Set interpolation coordinates (a,fa) and (b,fb)
	K a = interval.first->first;
	T fa = interval.first->second;
	K b = interval.second->first;
	T fb = interval.second->second;
	// Calculate slope on interval [a,b]
	double slope = (double)(fb - fa)/(double)(b - a);
	return (x - a)*slope + fa;		
}


#endif // Graph_h
