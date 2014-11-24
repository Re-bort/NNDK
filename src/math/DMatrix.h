#ifndef DMatrix_h
#define DMatrix_h

#include "../matrix"
#include "vecops.h"

template<class T>
class DMatrix
	: public std::matrix<T>
{
	public:
					DMatrix();
					DMatrix(const std::matrix<T>& X);
		void		getDistances(const std::matrix<T>& X);
};

template <class T>
DMatrix<T>::DMatrix()
{}

template <class T>
DMatrix<T>::DMatrix(const std::matrix<T>& X)
{
	getDistances(X);
}

template <class T>
void DMatrix<T>::getDistances(const std::matrix<T>& X)
{
	this->resize(X.rows(), X.rows());
	for(unsigned i = 0; i < X.rows(); ++ i)
		for(unsigned j = i + 1; j < X.rows(); ++j)
			(*this)[i][j] = euclidean(X[i], X[j]);
}

#endif // DMatrix_h
