#ifndef nww_glr_Regression_h
#define GLR_h

#include <iostream>

#include "../math/matrix.h"
#include "../math/statistics.h"
#include "../math/matops.h"

namespace nww {
namespace glr {

class Regression	// General Linear Regression
{
public:
	Regression(Table<double>& X, Table<double>& Y);
		
	Regression(Table<double>& X, Table<double>& Y, Table<double>& Xcv, Table<double>& Ycv)
		:	tmTRInputs(X), tmTROutputs(Y),
			tmCVInputs(Xcv), tmCVOutputs(Ycv) {}
	
	inline virtual ~Regression()	{ }
		
	inline Table<double>& TRInputs()		{ return tmTRInputs; }
	inline Table<double>& TROutputs()		{ return tmTROutputs; }
	inline Table<double>& CVInputs()		{ return tmCVInputs; }
	inline Table<double>& CVOutputs()		{ return tmCVOutputs; }
	inline Matrix<double>& S()				{ return mmS; }
	inline Matrix<double>& Si()				{ return mmSi; }
	inline Matrix<double>& B()				{ return mmB; }
		
	Vector<double> operator () (Vector<double>& x);
	Vector<double> operator () (Vector<double>& x, Vector<double>& y);
	
	Table<double> operator () (Table<double>& X); // Query (w/o targets)
	Table<double> operator () (Table<double>& X, Table<double>& Y);	// Query (with targets)
	
protected:
	Table<double> tmTRInputs;		// Training (or sample) inputs
	Table<double> tmTROutputs; 		// Training (or sample) outputs
	Table<double> tmCVInputs;		// Cross-validation inputs
	Table<double> tmCVOutputs;		// Cross-validation outputs
	Matrix<double> mmS;				// Covariance matrix
	Matrix<double> mmSi;			// Inverse covariance matrix
	Matrix<double> mmB;				// RegressionCoefficients
};


class MarginalRegression // Multivariate kde Regression estimator class
{
public:
	MarginalRegression(Vector<double>& X, Vector<double>& Y)
		: b_(covar(X,Y)/(var(X))) {}

	inline virtual ~MarginalRegression()	{  }
		
	inline double b()						{ return b_; }
		
	double operator () (double& x);
	double operator () (double& x, double& y);
	Vector<double> operator() (Vector<double>& X);	
	Vector<double> operator() (Vector<double>& X, Vector<double>& Y);
		
protected:
	double b_;
};

} // namespace glr
} // namespace nww

#endif // GLR_h

