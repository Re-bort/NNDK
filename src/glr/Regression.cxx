#include "GLR.h"

using namespace glr;
using namespace data;

Regression::Regression(Table<double>& X, Table<double>& Y)
	:	tmTRInputs(X), tmTROutputs(Y)
{
	Matrix<double> XT = transpose(X);
	S() = XT*X;
	Si() = inverse(S());
	B() = Si()*XT*Y;
}

Vector<double> Regression::operator () (Vector<double>& x)
{
	Vector<double> v; return v;
}

Vector<double> Regression::operator () (Vector<double>& x, Vector<double>& y)
{
	Vector<double> v; return v;
}
	
Table<double> Regression::operator () (Table<double>& X)
{
	Table<double> Y(X, TROutputs());
	Y = X*B();
	return Y;
}

Table<double> Regression::operator () (Table<double>& X, Table<double>& Y)
{
	Table<double> E(Y); 
	E = X*B();
	
	// Estimate outputs
	Table<double>::iterator yi = Y.begin();
	Table<double>::iterator ei = E.begin();
	while( yi != Y.end())
	{
		*ei -= *yi;
		
		++yi;	
		++ei;
	}

	return E;
}




double 
glr::MarginalRegression::operator () (double& x)
{
	return b()*x;

};



double 
glr::MarginalRegression::operator () (double& x, double& y)
{
	return y - (*this)(x);
}



Vector<double> 
glr::MarginalRegression::operator() (Vector<double>& X)
{
	Vector<double> Y;
	Y.reserve(X.size());
	
	for(Vector<double>::iterator xi = X.begin(); xi != X.end(); ++xi)
		Y.push_back((*this)(*xi));
	
	return Y;
}	



Vector<double> 
glr::MarginalRegression::operator() (Vector<double>& X, Vector<double>& Y)
{
	return Y - (*this)(X);
}

