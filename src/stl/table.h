#ifndef stl_table_h
#define stl_table_h

#include <iostream>
#include <fstream>
#include <iomanip> 

#include "matrix.h"
#include "series.h"

namespace std {

template <class T>
class table
	: public matrix<T>
{
   	public:
        						table();
                                table(const table& tab);
                                table(const table& tab, 
                                  unsigned first,
                                  unsigned last);
                                table(const matrix<T>& mat);
                                ~table();
                                        
		series<T>               back(void) const;
        series<T>               backc(void) const;
        void                    clear(void);
        void                    clearc(void);
        series<T>               column(unsigned j) const;
        void                    concat(const table<T>& tab);
        void                    concatc(const table<T>& tab);        
        void                    erase(unsigned pos);
        void                    erasec(unsigned pos);
        void                    insert(unsigned pos, const series<T>& ser);
        void                    insertc(unsigned pos, const series<T>& ser);
        void                    interlace(const table<double>& tab,
                                  unsigned k = 0);
        void                    interlacec(const table<double>& tab,
                                  unsigned k = 0);
		void                    pop_back(void);
        void                    pop_backc(void);
        void					push_back(const series<T>& ser);
        void                    push_backc(const series<T>& ser);
        void                    resize(unsigned rows);
        void                    resize(unsigned rows, unsigned columns);
        series<T>               row(unsigned i) const;
        table<T>                transpose(void) const;
        
        void                    read(const std::string filename);
        void                    write(const std::string filename) const;
        
        using matrix<T>::resize;
        using matrix<T>::transpose;
            
	public:
    	series<string>	        index;
        vector<string>          header;
};

template <class T>
table<T>::table()
{}

template <class T>
table<T>::table(const table<T>& tab)
    : matrix<T>(tab), index(tab.index), header(tab.header)
{}

template <class T>
table<T>::table(const table<T>& tab, unsigned first, unsigned last)
    : matrix<T>((last - first), tab.columns()), header(tab.header)
{
    index.resize(last - first);
    index.label = tab.index.label;
    for(unsigned i = first; i < last; ++i)
    {
        index[i - first] = tab.index[i];
        (*this)[i - first] = tab[i];
    }
}

template <class T>
table<T>::table(const matrix<T>& mat)
    : matrix<T>(mat)
{}

template <class T>
table<T>::~table()
{}

template <class T>
series<T> table<T>::back() const
{
    return series<T>(index.back(), matrix<T>::back());
}

template <class T>
series<T> table<T>::backc() const
{
    return series<T>(header.back(), matrix<T>::backc());
}

template <class T>
void table<T>::clear()
{
    matrix<T>::clear();
    index.clear();
}

template <class T>
void table<T>::clearc()
{
    matrix<T>::clearc();
    header.clear();
}

template <class T>
inline series<T> table<T>::column(unsigned i) const
{
    return series<T>(header[i], matrix<T>::column(i));
}

template <class T>
void table<T>::concat(const table<T>& t)
{
    matrix<T>::concat(t);
    for(unsigned i = 0; i < t.rows(); ++i)
        index.push_back(t.index[i]);    
}

template <class T>
void table<T>::concatc(const table<T>& tab)
{
    matrix<T>::concatc(tab);
    for(unsigned i = 0; i < tab.header.size(); ++i)
        header.push_back(tab.header[i]);    
}

template <class T>
void table<T>::erase(unsigned pos)
{
    matrix<T>::erase(pos);
    index.erase(index.begin() + pos);
}

template <class T>
void table<T>::erasec(unsigned pos)
{
    matrix<T>::erasec(pos);
    header.erase(header.begin() + pos);
}

template <class T>
void table<T>::insert(unsigned pos, const series<T>& ser)
{
    matrix<T>::insert(pos, ser);
    index.insert(index.begin() + pos, ser.label);
}

template <class T>
void table<T>::insertc(unsigned pos, const series<T>& ser)
{
    matrix<T>::insertc(pos, ser);
    header.insert(header.begin() + pos, ser.label);
}

template <class T>
void table<T>::interlace(const table<double>& tab, unsigned k)
{
    matrix<T>::push_back(tab[tab.rows() - 1]);
    index.push_back(tab.index[tab.rows() - 1]);
    for(int i = tab.rows() - 2; i >= 0;  --i)
    {
        matrix<T>::insert((i + 1) * (k + 1), tab[i]);
        index.insert(index.begin() + (i + 1) * (k + 1), tab.index[i]);
    }
}

template <class T>
void table<T>::interlacec(const table<double>& tab, unsigned k)
{
    push_backc(tab.column(tab.columns() - 1));
    for(int i = tab.rows() - 2; i >= 0;  --i)
        insertc((i + 1) * (k + 1), tab.column(i));
}

template <class T>
void table<T>::pop_back()
{
    matrix<T>::pop_back();
    index.pop_back();
}

template <class T>
void table<T>::pop_backc()
{
    matrix<T>::pop_backc();
    header.pop_back();
}

template <class T>
void table<T>::push_back(const series<T>& ser)
{
    if(ser.size() != header.size())
        return;
    matrix<T>::push_back(ser);
    index.push_back(ser.label);
}

template <class T>
void table<T>::push_backc(const series<T>& ser)
{
    matrix<T>::push_backc(ser);
    header.push_back(ser.label);
}

template <class T>
void table<T>::resize(unsigned rows)
{
    matrix<T>::resize(rows, this->columns());
    index.resize(rows);
}

template <class T>
void table<T>::resize(unsigned rows, unsigned columns)
{
    matrix<T>::resize(rows, columns);
    index.resize(rows);
    header.resize(columns);
}

template <class T>
inline series<T> table<T>::row(unsigned i) const
{
	return series<T>(index[i], (*this)[i]);
}

template <class T>
table<T> table<T>::transpose() const
{
    table<T> transpose(matrix<T>::transpose());
    transpose.header = index;
    transpose.index.label = index.label;
    transpose.index = header;
    return transpose;
}

template <class T>
void table<T>::read(const std::string filename)
{
    std::ifstream ifs(filename.c_str(), std::ios::in);
    ifs >> *this;
    ifs.close();
} 
template <class T>
void table<T>::write(const std::string filename) const
{
    std::ofstream ofs(filename.c_str(), std::ios::out);
    ofs << *this << std::endl;
    ofs.close();
}

template <class T> 
ostream& operator << (ostream& os, const table<T>& tab)
{
    if(tab.header.size() > 0)
    {
        if(tab.index.size() > 0)
            os << tab.index.label << '\t';
        
        for(unsigned j = 0; j < tab.header.size() - 1; ++j) 
            os << tab.header[j] << '\t';    
        os << tab.header.back() << '\n';
    }
    
    for(unsigned i = 0; i < tab.rows(); ++i)
    {
        if(tab.index.size() > 0)
            os << tab.index[i] << '\t';
        
        for(unsigned j = 0; j < tab.columns() - 1; ++j)
            os << tab[i][j] << '\t';
        os << tab[i].back() << '\n';
    }
    
    return os;
}

template <class T>
istream& operator >> (istream& in, table<T>& tab)
{
    series<string> header;
    getline(in, header, '\t');
    if(in.fail())
        return in;
    tab.index.label = header.label;
    tab.header = header;
    tab.clear();
    while (1)
    {
        series<T> ser;
        unsigned count = getline(in, ser, '\t');
        if(count != header.size() + 1)
            break;
        
        if(in.fail())
            break;
        tab.push_back(ser);
    }
    return in;
}

} // namespace std

#endif // stl_table_h


