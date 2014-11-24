#ifndef math_dgp_ArProcess_h
#define math_dgp_ArProcess_h

#include "Process.h"
#include "../rng/rng.h"

namespace dgp {

template <int Inputs>
class ArProcess
	: public Process<Inputs, 1>
{
	public:
									ArProcess();

		const std::vector<double>&	operator() (void);
		virtual void				initialise(void);

	protected:
		void						shift();
		const double				nextOutput();
};

} // namespace dgp

#endif //math_dgp_ArProcess_h
