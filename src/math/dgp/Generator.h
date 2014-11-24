#ifndef math_dgp_Process_h
#define math_dgp_Process_h

#include <vector>
#include <string>

template <unsigned Inputs, unsigned Outputs>
class Process
{
	public:
								Process()
									: state_(Inputs + Output) {}
								~Process(){}
		
		const std::vector<double>&	operator () (void) = 0;	
		const std::vector<string>	labels() = 0;
	
	protected:
		std::vector<double>		state_;	
};

#endif //math_dgp_Process_h
