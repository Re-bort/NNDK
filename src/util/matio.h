#ifndef matio_h
#define matio_h

#include <iostream>
#include <sstream>
#include <string>

#include "../stl/matrix.h"


#ifndef GETLINE_VECTOR
#define GETLINE_VECTOR

template<class T>
void getline(std::istream& is, std::vector<T>& x, char delimiter = '\t')
{
    std::string line;
    getline(is, line);
    if(!is.fail())
    {   
        std::stringstream lineBuff;
        lineBuff << line;
        while(1)
        {
            std::string token;
            getline(lineBuff, token, delimiter);
            if(lineBuff.fail())
                break;
            std::stringstream tokenBuff;
            tokenBuff << token;
            T xi;
            tokenBuff >> xi;
            if(tokenBuff.fail())
                break;
            x.push_back(xi);
        }
    }
}

#endif // GETLINE_VECTOR

template <class T>
std::ostream& operator << (std::ostream& os, const std::vector<T>& v)
{
	typename std::vector<T>::const_iterator vi;
	for(vi = v.begin(); vi != v.end() - 1; ++vi)
	{
		os << *vi << "\t";
	}
	os << v.back();
	
	return os;
}

template <class T>
std::ostream& operator << (std::ostream& os, const std::matrix<T>& X)
{
    typename std::matrix<T>::const_iterator x;
    typename std::vector<T>::const_iterator xi;
    for(x = X.begin(); x < X.end() - 1; ++x)
    {
        for(xi = x->begin(); xi < x->end() - 1; ++xi)
            os << *xi << '\t';
        os << *xi << std::endl;
    }
    for(xi = x->begin(); xi < x->end() - 1; ++xi)
        os << *xi << '\t';
    os << *xi;
    return os;
}

template <class T>
std::istream& operator >> (std::istream& is, std::matrix <T>& X)
{
    while (1)
    {
        std::vector<T> x;
        getline(is, x, '\t');
        if (is.fail())
            break;
        X.push_back(x);
    }
    return is;
}

#endif // matio_h
