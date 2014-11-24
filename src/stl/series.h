#ifndef stl_series_h
#define stl_series_h

#include <vector>
#include <string>
#include <iostream>
#include <sstream>

namespace std {

template <class T>
class series 
    : public vector<T>
{
	public:
    	typedef typename vector<T>::iterator iterator;
    
    							series()
    								{}
    							series(const series<T>& ser)
    								: vector<T>(ser), label(ser.label){}
    							series(const string str)
    								: label(str) {}
    							series(const vector<T>& vec)
    								: vector<T>(vec) {}	
    							series(const string str, const vector<T>& vec)
    								: vector<T>(vec), label(str) {}
                                series(const string str, unsigned n)
                                    : vector<T>(n), label(str) {}
    							series(const string str, unsigned n, T val)
    								: vector<T>(n, val), label(str) {}
	public:
    	string label;
};

template <class T>
ostream& operator << (ostream& os, const series<T>& ser)
{
    if(ser.label != "")
    	os << ser.label << '\t';
    typename series<T>::const_iterator iter;
    for(iter = ser.begin(); iter < ser.end() - 1; ++iter)
    	os << *iter << '\t';
    os << *iter;
    return os;
}

#ifndef GETLINE_SERIES
#define GETLINE_SERIES
template<class T>
unsigned getline(istream& in, series<T>& ser, char delimiter)
{
    string line;
    getline(in, line);
    unsigned count = 0;
    if(!in.fail())
    {   
        stringstream buffer;
        buffer << line;
        
        getline(buffer, ser.label, delimiter);
        if(buffer.fail())
            return count;
      
        ++count;
        ser.clear();
        while(1)
        {
            string token;    
            getline(buffer, token, delimiter);
            if(buffer.fail())
                break;
            
            T value;
            
            stringstream temp;
            temp << token;
            temp >> value;
            
            if(temp.fail())
                value = T(0);

            ser.push_back(value);
            ++count;
        }
    }
    
    return count;
}
#endif // GETLINE_SERIES


#ifndef GETLINE_VECTOR
#define GETLINE_VECTOR
template<class T>
void getline(istream& in, vector<T>& vec, char delimiter = '\t')
{
	string line;
	getline(in, line);
	if(!in.fail())
	{	
		
        stringstream buffer;
		buffer << line;
		vec.clear();
		while(1)
		{
			string token;
            getline(buffer, token, delimiter);
			if(buffer.fail())
				break;
			
            T value;

            stringstream temp;
			temp << token;
			temp >> value;
			if(temp.fail())
				break;
			
            vec.push_back(value);
		}
	}
}
#endif //GETLINE_VECTOR

} // namespace std

#endif // stl_series_h

