#ifndef kde_Scott_h
#define kde_Scott_h

#include <math.h>
#include "../math/statistics.h"

namespace kde {

inline const double scottBandwidth(const double n, const double d, const double s)
{
    return pow(4.0 /(d + 2.0), 1.0/(d + 4.0)) * s * pow(n, -1.0/(d + 4.0));
}

} // namespace kde

#endif // kde_Scott_h
