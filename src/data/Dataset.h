#ifndef DataSet_h
#define DataSet_h

#include <iostream>
#include <fstream>
#include <string>

#include "../table"
#include "../util/matio.h"
#include "../math/statistics.h"

#define TABLE std::table<double>
#define SERIES std::series<double>

class Dataset
{
    public:
                                Dataset();
                                Dataset(const Dataset& dataset);
                                Dataset(const TABLE& X);
                                Dataset(const TABLE& X, const SERIES& y);
                                Dataset(const std::string& filename);
                                ~Dataset();
    
        const unsigned          length() const;
        const TABLE             statistics() const;
        const unsigned          dim() const;
        const TABLE             data() const;
        
        void                    read(std::string filename);
        void                    write(std::string filename) const;
        void 					writefort(std::string filename) const;
    
    public:
        TABLE                   X;
        SERIES                  y;
};

std::ostream& operator << (std::ostream& os, const Dataset& dataset);
std::istream& operator >> (std::istream& is, Dataset& dataset);

#undef TABLE
#undef SERIES

#endif // Dataset_h
