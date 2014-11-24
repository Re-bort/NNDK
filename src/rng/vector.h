#ifndef rng_vector_h
#define rng_vector_h

#include <vector>
#include "Random.h"

namespace rng {

template<class T>
class vector
	: public std::vector<T>
{
	public:
		typedef typename vector<T>::iterator iterator;
		typedef typename vector<T>::const_iterator const_iterator;
		
								vector(const Random<T>& variable,
                                       const unsigned n = 1);
		std::vector<T>&			operator()(void);
		const Random<T>&		variable(void) const;
		void 					setVariable(const Random<T>& variable);
		void					draw(void);
		void					push_back(void);
				
	private:
		const Random<T> *       variable_;
};

template<class T>
vector<T>::vector(const Random<T>& variable, const unsigned n)
	: std::vector<T>(n), variable_(&variable) 
{
	draw();
}

template<class T>
std::vector<T>& vector<T>::operator () ()
{
	draw();
	return *this;
}

template<class T>
const Random<T>& vector<T>::variable() const
{
	return *(this->variable_);	
}

template<class T>
void vector<T>::setVariable(const Random<T>& variable)
{
	this->variable_ = variable;
}

template<class T>
void vector<T>::draw()
{
	iterator vi;
	for(vi = this->begin(); vi != this->end(); ++vi)
		*vi = (*(this->variable_))();
}

template<class T>
void vector<T>::push_back()
{
	this->push_back(*variable_());
}

} // namespace rng

#endif // rng_vector_h
