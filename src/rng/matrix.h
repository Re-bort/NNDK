#ifndef rng_matrix_h
#define rng_matrix_h

#include "Random.h"
#include "vector.h"
#include "../matrix"

namespace rng {

template <class T>
class matrix
	: public std::matrix<T>
{
	using std::matrix<T>::push_back;   // Unhides base method of this name
    using std::matrix<T>::push_backc;  
    
    public:
		typedef typename matrix<T>::iterator iterator;
		typedef typename matrix<T>::const_iterator const_iterator;
		
								matrix(const Random<T>& variable,
                                       unsigned rows = 1,
									   unsigned columns = 1);
								~matrix(){}
		
		std::matrix<T>&			operator()(void);
		const Random<T>& 		variable(void) const;
		void					setVariable(const Random<T>& variable);
		void					draw(void);
		void					push_back(void);
		void					push_backc(void);
				
	private:
		const Random<T> *     	variable_;
};

template<class T>
matrix<T>::matrix(const Random<T>& variable, unsigned rows, unsigned columns)
	: std::matrix<T>(rows, columns, 0.0), variable_(&variable) 
{
	draw();
}

template<class T>
std::matrix<T>& matrix<T>::operator () ()
{
	draw();
	return *this;
}

template<class T>
const rng::Random<T>& matrix<T>::variable() const
{
	return *variable_;	
}

template<class T>
void matrix<T>::setVariable(const Random<T>& variable)
{
	variable_ = &variable;
}

template<class T>
void matrix<T>::draw()
{
	iterator row;
	typename std::vector<T>::iterator vi;
	for(row = this->begin(); row != this->end(); ++row)
		for(vi = row->begin(); vi != row->end(); ++vi)
			*vi = (*variable_)();
}

template<class T>
void matrix<T>::push_back(void)
{
	rng::vector<T> v(this->columns(), *variable_);
	this->push_back(v());
}

template<class T>
void matrix<T>::push_backc(void)
{
	rng::vector<T> v(this->rows(), *variable_);
	this->push_backc(v());
}

} // namespace rng

#endif // rng_matrix_h
