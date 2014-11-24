//	Filter.cpp
#include "IvsFilter.h"

#define SERIES std::series<double>
#define TABLE std::table<double>
#define VECTOR std::vector<double>
#define MATRIX std::matrix<double>

IvsFilter::IvsFilter(const TABLE& candidates, const SERIES& response) 
    : C_(candidates), y_(response)
{}

IvsFilter::~IvsFilter()
{}

const unsigned IvsFilter::s() const
{
	unsigned s = 0;
	double maxScore = candidateScores_[0];
	for(unsigned j = 1; j < C_.columns(); ++j)
		if (candidateScores_[j] > maxScore)
		{
			s = j;
			maxScore = candidateScores_[j];
		}
	return s;
}

void IvsFilter::select(const unsigned j)
{
    SERIES c = C_.column(j);
    if(S_.columns() == 0)
    {
        S_ = MATRIX(c, c.size(), 1);
        S_.index = C_.index;
        S_.header.push_back(c.label);
    }
    else S_.push_backc(c);
    C_.erasec(j);
    
    inputScores_.push_back(candidateScores_[j]);
    candidateScores_.erase(candidateScores_.begin() + j);
}

#undef SERIES
#undef TABLE
#undef VECTOR
#undef MATRIX
