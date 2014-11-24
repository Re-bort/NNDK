#ifndef DbScan_h
#define DbScan_h

#include <vector>
#include "../math/DMatrix.h"
#include "../matrix"

#define MATRIX std::matrix<double> 
#define VECTOR std::vector<double>


class DbScan
{
	private:
		class Point
		{
			public:
									Point() : label(-1), id(0) {}
									Point(unsigned i) : label(-1), id(i){}
				long				label;
				unsigned			id;
		};
	
	public:
									DbScan();
									DbScan(const double eps, 
										const long minPoints);
		void 						initialise(const MATRIX& X);
		void						scan();
		void						setEps(const double eps);
		void						setMinPoints(const unsigned minPoints);
		std::vector<unsigned>		labels() const;
		
	private:
		double						eps_;
		unsigned					minPoints_;
		DMatrix<double>				distances_;
		std::vector<Point>			points_;
		
		
		std::vector<std::vector<Point>::iterator >
									neighbours(std::vector<Point>::iterator P);
		void 						expandCluster(std::vector<Point>::iterator P, 
										unsigned& clusterId);
};

#endif //DbScan_h
