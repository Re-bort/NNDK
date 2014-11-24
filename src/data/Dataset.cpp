#include "Dataset.h"

#define TABLE std::table<double>
#define SERIES std::series<double>
#define MATRIX std::matrix<double>
#define VECTOR std::vector<double>

#include <iostream>

Dataset::Dataset()
{}

Dataset::Dataset(const Dataset& dataset)
    : X(dataset.X), y(dataset.y)
{}

Dataset::~Dataset()
{}

Dataset::Dataset(const TABLE& X_, const SERIES& y_)
    : X(X_), y(y_)
{}

Dataset::Dataset(const TABLE& tab)
    : X(tab)
{
    y = X.backc();
    X.pop_backc();
}

Dataset::Dataset(const std::string& filename)
{
    read(filename);   
}

const unsigned Dataset::length() const
{
    return X.rows();
}

const unsigned Dataset::dim() const
{
    return X.columns() + 1;
}

const TABLE Dataset::data() const
{
    TABLE data(X);
    data.push_backc(y);    
    return data;
}

const TABLE Dataset::statistics() const
{
    using ::summary;
    
    TABLE statistics(summary(X));
       
    statistics.header.push_back("n");
    statistics.header.push_back("min");
    statistics.header.push_back("ave");
    statistics.header.push_back("max");
    statistics.header.push_back("var");
    statistics.header.push_back("iqr");
    statistics.index.label = std::string("x");
    statistics.index = X.header;
  
    statistics.push_back(SERIES(y.label, summary(y)));
    
    return statistics;
}

std::ostream& operator << (std::ostream& out, const Dataset& dataset)
{
    TABLE data(dataset.X);
    data.push_backc(dataset.y);
    out << data << std::flush;
    return out;
}

std::istream& operator >> (std::istream& in, Dataset& dataset)
{
    in >> dataset.X;
    dataset.y = dataset.X.backc();
    dataset.X.pop_backc();
    return in;
}

void Dataset::read(std::string filename)
{
    std::ifstream fin(filename.c_str(), std::ios::in);
    fin >> (*this);
    fin.close();
}

void Dataset::write(std::string filename) const
{
    std::ofstream fout(filename.c_str(), std::ios::out);
    fout << (*this) << std::endl;
    fout.close();
}

void Dataset::writefort(std::string filename) const
{
	std::ofstream fout(filename.c_str(), std::ios::out);
	
	//KLUDGE: Can't overload << operator, so write method specifically
	//rather than derive a Fortran dataset class.
	
	// Write magic numbers: inputs, patterns, outputs (1)
	fout << X.columns() << '\t' << X.rows() << "\t1" << std::endl;
	
	// Skip header labels, and print rows without index column
	for(unsigned i = 0; i < X.rows(); ++i)
	{
		for(unsigned j = 0; j < X.columns(); ++j)
            fout << X[i][j] << '\t';
        fout << y[i] << '\n';
	}
}

//template <class T> 
//ostream& operator << (ostream& os, const table<T>& tab)
//{
//    if(tab.header.size() > 0)
//    {
//        if(tab.index.size() > 0)
//            os << tab.index.label << '\t';
//        
//        for(unsigned j = 0; j < tab.header.size() - 1; ++j) 
//            os << tab.header[j] << '\t';    
//        os << tab.header.back() << '\n';
//    }
//    
//    for(unsigned i = 0; i < tab.rows(); ++i)
//    {
//        if(tab.index.size() > 0)
//            os << tab.index[i] << '\t';
//        
//        for(unsigned j = 0; j < tab.columns() - 1; ++j)
//            os << tab[i][j] << '\t';
//        os << tab[i].back() << '\n';
//    }
//    
//    return os;
//}

#undef TABLE
#undef SERIES
#undef MATRIX
#undef VECTOR
