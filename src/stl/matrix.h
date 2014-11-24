#ifndef stl_matrix_h
#define stl_matrix_h

#include <vector>
#include <iostream>
#include <string>
#include <sstream>

namespace std {

template <class T>
class matrix
	: public vector< vector<T> >
{
	public:
		typedef typename vector< vector<T> >::iterator iterator;
		typedef typename vector< vector<T> >::const_iterator const_iterator;
	
								matrix();
								matrix(unsigned rows,
							      unsigned columns, 
								  T value);
								matrix(const vector<T>& v);
                                matrix(const vector<T>& v,
                                  unsigned rows,
								  unsigned columns);
								matrix(const matrix<T>& A);
								matrix(const pair<unsigned, unsigned>& size,
								  const T& value);
		 						//~matrix(); // Use default
		// Access methods
		unsigned				rows(void) const;
		unsigned				columns(void) const;
const	pair<unsigned,unsigned> size(void) const;
		string					size_str(void) const;
		bool					empty(void) const;
		vector<T>				row(unsigned i) const;
		vector<T>				column(unsigned j) const;
		matrix<T>				transpose(void) const;
		// Overloaded push/insert methods (with vector size checking)
		void					push_back(const vector<T>& v);
		void					insert(unsigned pos, const vector<T>& v);
        void                    erase(unsigned pos);
		void					concat(const matrix<T>& M);
        void                    interlace(const matrix<T>& M, 
                                  const unsigned k = 0);
		// Column push/pop/erase/insert methods
		void					clearc(void);
		void					push_backc(const vector<T>& v);
		vector<T>				backc(void) const;
		void					pop_backc(void);
		void					insertc(unsigned pos, const vector<T>& v);
		void					erasec(unsigned pos);
		void					concatc(const matrix<T>& M);
        void                    interlacec(const matrix<T>& M, 
                                  const unsigned k = 0);
		// Other methods
		void					resize(unsigned rows, unsigned columns);
        
        using vector< vector<T> >::erase;
};

template <class T>
matrix<T>::matrix()
	: vector< vector<T> >()
{}

	
template <class T>
matrix<T>::matrix(unsigned rows, unsigned columns, T value = 0)
	: vector< vector<T> >(rows, vector<T>(columns, value))
{}

template <class T>
matrix<T>::matrix(const matrix<T>& A)
	: vector< vector<T> >(A.rows(), vector<T>(A.columns()))
{
	for(unsigned i = 0; i < this->rows(); ++i)
		for(unsigned j = 0; j < this->columns(); ++j)
			(*this)[i][j] = A[i][j];
}

template <class T>
matrix<T>::matrix(const vector<T>& v)
    : vector< vector<T> > (1, v)
{}

template <class T>
matrix<T>::matrix(const vector<T>& v, unsigned rows, unsigned columns)
	: vector< vector<T> >(rows, vector<T>(columns, 0))
{
	if(rows*columns != v.size()) 
		return; 
	typename matrix<T>::iterator row;
	typename vector<T>::iterator element;
	typename vector<T>::const_iterator vi = v.begin();
	for(row = this->begin(), vi = v.begin(); row != this->end(); ++row)
		for(element = row->begin(); element != row->end(); ++element, ++vi)
			*element = *vi;
}

template <class T>
matrix<T>::matrix(const pair<unsigned, unsigned>& size, const T& value = 0)
	: matrix<T>(size.first, size.second, value)
{}

template <class T>
inline bool matrix<T>::empty() const
{	
	if(this->rows() > 0 && this->columns() > 0)
        return false;
    else
        return true;
}

template <class T>
inline unsigned matrix<T>::rows() const
{
	return vector< vector<T> >::size();
}

template <class T>
inline unsigned matrix<T>::columns() const
{
	if(this->rows() == 0) 
		return 0;
	else
		return (*this)[0].size();
}

template <class T>
inline const pair<unsigned, unsigned> matrix<T>::size() const
{
	return pair<unsigned, unsigned>(this->rows(), this->columns());
}

template <class T>
string matrix<T>::size_str() const
{
	std::stringstream size;
	size << this->rows() << 'x' << this->columns();
	return size.str();
}

template <class T>
inline void matrix<T>::resize(unsigned rows, unsigned columns)
{
	typename matrix<T>::iterator row;
	if(rows < this->rows())
	{
		// Erase unused rows
		while(rows != this->rows())
		{
			this->erase(this->end() - 1);
		}
		for(row = this->begin(); row != this->end(); ++row)
			row->resize(columns);
	}
	else
	{
		while(rows != this->rows())
		{
			push_back(vector<T>(columns, 0));
		}
		for(row = this->begin(); row != this->end(); ++row)
			row->resize(columns);
	}
}

template <class T>
inline vector<T> matrix<T>::row(unsigned i) const
{
	return (*this)[i];
}

template <class T>
vector<T> matrix<T>::column(unsigned j) const
{
	vector<T> column(this->rows());
	for(unsigned i = 0; i != this->rows(); ++i)
		column[i] = (*this)[i][j];
	return column;
}

template <class T>
matrix<T> matrix<T>::transpose() const
{
	matrix<T> transpose(this->columns(), this->rows());
	for(unsigned i = 0; i < this->rows(); ++i)
		for(unsigned j = 0; j < columns(); ++j)
			transpose[j][i] = (*this)[i][j];
	return transpose;
}

template <class T>
void matrix<T>::push_back(const vector<T>& v)
{
	if(this->rows() > 0 && v.size() != this->columns())
		return;
	vector< vector<T> >::push_back(v);
}

template <class T>
void matrix<T>::insert(unsigned pos, const vector<T>& v)
{
	if(this->rows() > 0 && v.size() != this->columns())
		return;
	vector< vector<T> >::insert(this->begin() + pos, v);
}

template <class T>
void matrix<T>::erase(unsigned pos)
{
    vector< vector<T> >::erase(this->begin() + pos);
}

template <class T>
void matrix<T>::concat(const matrix<T>& M)
{
	for(unsigned i = 0; i < M.rows(); ++i)
		push_back(M[i]);
}

template <class T>
void matrix<T>::clearc()
{
	typename matrix<T>::iterator row;
	for(row = this->begin(); row != this->end(); ++row)
		row->clear();
}

template<class T>
void matrix<T>::push_backc(const vector<T>& v)
{
	typename matrix<T>::iterator row = this->begin();
	typename vector<T>::const_iterator vi = v.begin();
	while(row != this->end() && vi != v.end())
	{
		row->push_back(*vi);
		++row;
		++vi;
	}
}

template<class T>
vector<T> matrix<T>::backc() const
{
	vector<T> v;
	typename matrix<T>::const_iterator row;
	for(row = this->begin(); row != this->end(); ++row)
		v.push_back(row->back());
	return v;
}

template<class T>
void matrix<T>::pop_backc(void)
{
	for(typename matrix<T>::iterator row = this->begin(); row != this->end(); ++row)
		row->pop_back();
}

template <class T>
void matrix<T>::erasec(unsigned pos)
{
	for(typename matrix<T>::iterator row = this->begin(); row != this->end(); ++row)
		row->erase(row->begin() + pos);
}

template <class T>
void matrix<T>::insertc(unsigned pos, const vector<T>& v)
{
	typename matrix<T>::iterator row = this->begin();
	typename vector<T>::const_iterator vi = v.begin();
	while(row != this->end() && vi != v.end())
	{
		row->insert(row->begin() + pos, *vi);
		++row;
		++vi;
	}
}

template <class T>
void matrix<T>::concatc(const matrix<T>& M)
{
	for(unsigned i = 0; i < M.columns(); ++i)
		push_backc(M.column(i));
}

template <class T>
void matrix<T>::interlace(const matrix<T>& M, const unsigned k) 
{
    push_back(M[M.rows() - 1]);
    for(int i = M.rows() - 2; i >= 0;  --i)
        insert((i + 1) * (k + 1), M[i]);
}

template <class T>
void matrix<T>::interlacec(const matrix<T>& M, const unsigned k)
{
    push_backc(M.column(M.columns() - 1));    
    for(int i = M.columns() - 2; i >= 0;  --i)
        insertc((i + 1) * (k + 1), M.column(i));
}

} // namespace std

#endif // stl_matrix_h
