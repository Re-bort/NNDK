#ifndef DMatrix_h
#define DMatrix_h

#include "../matrix"
#include "../math/vecops.h"

template <class T>
class DMatrix
	: public std::matrix<T>
{
	public:
		DMatrix();
		DMatrix(std::matrix<T>& X);
		
	private:
		
};

template <class T>
DMatrix<T>::DMatrix()
{}

template <class T>
DMatrix<T>::DMatrix(std::matrix<T>& X)
	: std::matrix<T>(X.rows(), X.rows())
{
	for(unsigned i = 0; i <	X.rows(); ++i)
		for(unsigned j = i + 1; j < X.rows(); ++j)
		{
			(*this)[i][j] = euclidean(X[i], X[j]);
			(*this)[j][i] = (*this)[i][j];
		}
}

#endif // DMatrix_h
