#ifndef math_matops_h
#define math_matops_h

#include "../stl/matrix.h"
#include "identity.h"

template <class T>
std::matrix<T> operator + (const std::matrix<T>& A, const std::matrix<T>& B)
{
	std::matrix<T> C(A.rows(), A.columns());
	for(unsigned i = 0; i != A.rows(); ++i)
		for(unsigned j = 0; j != A.columns(); ++j)
			C[i][j] = A[i][j] + B[i][j];
	return C;
}

template <class T>
std::matrix<T> operator % (const std::matrix<T>& A, const T& mod)
{
    std::matrix<T> C(A.rows(), A.columns());
    for(unsigned i = 0; i != A.rows(); ++i)
        for(unsigned j = 0; j != A.columns(); ++j)
            C[i][j] = A[i][j] % mod;
    return C;
}

template <class T>
std::matrix<T> operator - (const std::matrix<T>& A, const std::matrix<T>& B)
{
	std::matrix<T> C(A.rows(), A.columns());
	for(unsigned i = 0; i != A.rows(); ++i)
		for(unsigned j = 0; j != A.columns(); ++j)
			C[i][j] = A[i][j] - B[i][j];
	return C;
}

template <class S, class T>
std::matrix<T> operator * (const S& x, const std::matrix<T>& A) 
{
	std::matrix<double> B(A.rows(), A.columns());
	{
		for(unsigned i = 0; i < A.rows(); ++i)
			for(unsigned j = 0; j < A.columns(); ++j)
				B[i][j] = x * A[i][j];
	}
	return B;
}

template <class T, class S>
std::matrix<T> operator * (const std::matrix<T>& A, const S& x)
{
	std::matrix<double> B(A.rows(), A.columns());
	{
		for(unsigned i = 0; i != A.rows(); ++i)
			for(unsigned j = 0; j != A.columns(); ++j)
				B[i][j] = x*A[i][j];
	}
	return B;
}

template <class T>
std::vector<T> operator * (const std::vector<T>& x, const std::matrix<T>& A)
{
	std::vector<T> y(A.columns(), 0);
	for(unsigned i = 0; i < x.size(); ++i)
			for(unsigned j = 0; j < A.columns(); ++j)
				y[j] += x[i]*A[i][j];
	return y;
}

template <class T> 
std::vector<T> operator * (const std::matrix<T>& A, const std::vector<T>& x)
{
	std::vector<T> y(A.columns(), (T)0);
	for(unsigned i = 0; i < A.rows(); ++i)
		for(unsigned j = 0; j < A.columns(); ++j)
				y[j] += x[j]*A[i][j];
	return y;
}

template <class T>
std::matrix<T> operator * (const std::matrix<T>& A, const std::matrix<T>& B)
{
	std::matrix<T> C(A.rows(), B.columns(), 0);
	for(unsigned i = 0; i < A.rows(); ++i)
		for(unsigned j = 0; j < A.columns(); ++j)
			for(unsigned k = 0; k < B.columns(); ++k)
				C[i][k] += A[i][j]* B[i][j];
	return C;
}

template <class T>
void solveForward(const std::matrix<T>& L, std::matrix<T>& X, 
				  const std::matrix<T>& Y)
{
	for(unsigned k = 0; k < Y.columns(); ++k) 
		X[0][k] = Y[0][k]/L[0][0];
	
	for(unsigned i = 0; i < L.rows(); ++i)
		for(unsigned k = 0; k < Y.columns(); ++k)
		{
			T sum = 0;
			for(unsigned j = 0; j < i; ++j)
				sum += L[i][j]*X[j][k];
			X[i][k] = (Y[i][k] - sum)/L[i][i];
		}
}

template <class T>
void solveBackward(const std::matrix<T>& U, std::matrix<T>& X, 
				   const std::matrix<T>& Y)
{
	unsigned n = U.rows();
	for(unsigned k = 0; k < Y.columns(); ++k)
		X[n - 1][k] = Y[n - 1][k]/U[n - 1][n - 1];
	
	for(unsigned i = n - 2; i >= 0; --i)
		for(unsigned k = 0; k < Y.columns(); ++k)
		{
			T sum = 0;
			for(unsigned j = i + 1; j < n; ++j) 
				sum += U[i][j]*X[j][k];
			X[i][k] = (Y[i][k] - sum)/U[i][i];
		}
}

template <class T> 
void solveLowerUpper(const std::pair< std::matrix<T>, std::matrix<T> >& LU, 
					 std::matrix<T>& X, 
					 const std::matrix<T>& B)
{
	std::matrix<T> Y(X.rows(), X.columns());
	solveForward(LU.first, Y, B);
	solveBackward(LU.second, X, Y);
}

template <class T>
std::pair< std::matrix<T>, std::matrix<T> > cholesky(const std::matrix<T>& A)
{
	std::cerr << "cholesky(A)" << std::endl;
	std::matrix<T> L(A.rows(), A.rows());
	std::matrix<T> U(A.rows(), A.rows());
	for(unsigned i = 0; i < A.rows(); ++i)
	{
		std::cerr << "L[" << i << "][" << i << "] = " << A[i][i] << std::endl;
		L[i][i] = A[i][i];
		for(unsigned k = 0; k < i; ++k)
		{	
			std::cerr << "L[" << i << "][" << i << "] -=" << L[i][k]*L[i][k]
				<< std::endl;
			L[i][i] -= L[i][k]*L[i][k];
		}
		std::cerr << "L[" << i << "][" << i << "] = " << L[i][i] << std::endl;
		std::cerr << "pow(L[i][i], 0.5)" << std::endl;
		L[i][i] = pow(L[i][i], 0.5);
		U[i][i] = L[i][i];
		for(unsigned j = i + 1; j < A.rows(); ++j)
		{
			L[j][i] = A[i][j];
			for(unsigned k = 0; k < i; ++k)
				L[j][i] -= L[i][k]*L[j][k];
			L[j][i] /= L[i][i];
			U[i][j] = L[j][i];
		}
	}
	return std::make_pair(L, U);
}

template <class T>
std::pair< std::matrix<T>, std::matrix<T> > crout(const std::matrix<T>& A)
{
	std::matrix<T> L(A.rows(), A.rows());
	std::matrix<T> U(A.rows(), A.rows());
	for(unsigned i = 0; i < A.rows(); ++i)
		L[i][i] = 1;
	for(unsigned j = 0; j < A.rows(); ++j)
	{
		for(unsigned i = 1; i <= j; ++i)
		{
			U[i][j] = A[i][j];
			for(unsigned k = 0; k < i; ++k)  
				U[i][j] -= L[i][k]*U[k][j];
		}
		for(unsigned i = j + 1; i < A.rows(); ++i)
		{
			L[i][j] = A[i][j];
			for(unsigned k = 0; k < j; ++k)  
				L[i][j] -= L[i][k]*U[k][j];
			L[i][j] /= U[j][j];	
		}
	}
	return std::make_pair(L, U);
}

template <class T>
std::matrix<T> inverse(const std::matrix<T>& A)
{
	std::matrix<T> Ainv(A.rows(), A.rows());
//	solveLowerUpper(crout(A), Ainv, std::identity<T>(A.rows()));
	std::cerr << "solveLowerUpper" << std::endl;
	solveLowerUpper(cholesky(A), Ainv, std::identity<T>(A.rows()));
	return Ainv;
}

template <class T>  
double determinant(const std::matrix<T>& A)
{
	std::matrix<T> U = crout(A).second;
	double d = U[0][0];
	for(unsigned i = 1; i < U.rows(); ++i)
		d *= U[i][i];
	return d;
}

template <class T>
std::matrix<T> cofactor(std::matrix<T> A, unsigned i, unsigned j)
{
	for(unsigned k = 0; k < A.rows(); ++k)
		A[k].erase(A[k].begin() + j);
	A.erase(A.begin() + i);
	return A;
}

template <class T>
std::matrix<T> covariance(const std::matrix<T>& A)
{
	std::matrix<T> S(A.columns(), A.columns());
	for(unsigned i = 0; i < A.columns(); ++i)
	{
		S[i][i] = 1;
		std::vector<T> ci = A.column(i);
		for(unsigned j = i + 1; j < A.columns(); ++j)
		{
			std::vector<T> cj = A.column(j);
			S[i][j] = covar(ci, cj);
			S[j][i] = S[i][j];
		}
	}
}

template <class T>
std::matrix<T> correlation(const std::matrix<T>& A)
{
	std::matrix<T> S(A.columns(), A.columns());
	for(unsigned i = 0; i < A.columns(); ++i)
	{
		std::vector<T> ci = A.column(i);
		for(unsigned j = i + 1; j < A.columns(); ++j)
		{
			std::vector<T> cj = A.column(j);
			S[i][j] = pearson(ci, cj);
			S[i][j] = S[j][i];
		}
	}
}

template <class T>
void normalize(std::matrix<T>& A)
{
    T den = (T)0.0;
    for(unsigned i = 0; i < A.rows(); ++i)
        den += sum(A[i]);
    for(unsigned i = 0; i < A.rows(); ++i)
        A[i] /= den;
}

#endif // math_matops_h
