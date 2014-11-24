#ifndef filename_h
#define filename_h

#include <iostream>
#include <string>

namespace file {

std::string name(const std::string& str)
{
	unsigned m = str.length();
	return str.substr(0, m - 4);
}

std::string ext(const std::string& str)
{
	unsigned m = str.length();
	return str.substr(m-3, m);
}

} // namespace file

#endif // filename_h
