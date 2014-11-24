#ifndef sequence_h
#define sequence_h

#include <vector>

namespace std {

  template <class T>
  class sequence
	  : public std::vector<T>
  {
	  public:
	  							sequence(T a, T b, T w = (T)1);
  };


  template <class T>
  sequence<T>::sequence(T a, T b, T d)
	  : std::vector<T>((unsigned)((double)(b - a)/(double)d + 0.5))
  {
	  for(unsigned i = 0; i < this->size(); ++i)
		  (*this)[i] = i * d + a;
  }

} // namespace std

#endif //sequence_h
