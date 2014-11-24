#ifndef Sdh_h
#define Sdh_h

#include <math/matrix.h>
#include <math/vectormath.h>
#include <math/vectorutil.h>

namespace som {

class Sdh
{
	public:
	typedef enum { rank, distance } Weighting;
	
	public:
								Sdh()
								{}
								Sdh(const unsigned& smoothness,
									const Weighting& weighting);
				 				~Sdh()
				 				{}
		// Access methods
		unsigned				smoothness(void) const;
		void					setSmoothness(const unsigned& smoothness);
		Weighting				weighting(void) const;
		void					setWeighting(const Weighting& weighting);
		// Histogram methods
		vector<double>			operator ()(const matrix<double>& database,
											const matrix<double>& codebook);
											
	private:
		unsigned				smoothness_;
		Weighting				weighting_;
		// Helper methods		
		double					distanceWeight(const vector<double>& distance);
		double					rankWeight();
};

} // namespace som

#endif // Sdh_h

