#ifndef nndk_sort_h
#define nndk_sort_h

#include <vector>

namespace std {

template <class K, class T>
void sort(std::vector<K>& key, std::vector<T>& val)
{
    K tempKey;
    T tempVal;
    for(unsigned i = 0; i < key.size(); ++i)
    {
        for(unsigned j = i + 1; j < key.size(); ++j)
        {
            if(key[j] < key[i])
            {
                tempKey = key[i]; 
                key[i] = key[j]; 
                key[j] = tempKey;
                tempVal = val[i]; 
                val[i] = val[j]; 
                val[j] = tempVal;
            }
        }
    }
}

} // namespace std

#endif // nndk_sort_h
