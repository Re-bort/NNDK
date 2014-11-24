// PcisFilter.cpp

#include "PcisFilter.h"
#include "../glr/GLRegression.h"
#include "../math/statistics.h"
#include "Standardiser.h"

#include "../util/matio.h"

#define SERIES std::series<double>
#define TABLE std::table<double>
#define VECTOR std::vector<double>
#define MATRIX std::matrix<double>

PcisFilter::PcisFilter(const TABLE& candidates, const SERIES& response)
    : IvsFilter(candidates, response)
{}

PcisFilter::~PcisFilter()
{}

void PcisFilter::calculateScores()
{
    candidateScores_.resize(C_.columns());
    
    for(unsigned j = 0; j < C_.columns(); ++j)
    {
		VECTOR v = V_[j];
        candidateScores_[j] = fabs(pearson(v, u_));
    }
}

void PcisFilter::calculateResiduals()
{
	std::cerr << "calculateResiduals()" << std::endl;
	u_ = y_;
    V_ = C_.transpose();
    if(S_.columns() > 0)
    {
   		if (S_.columns() == 1)
    	{
    		VECTOR s = S_.column(0);
    		glr::Regression1 m(s, y_);
    		u_ -= m(s);
    		for(unsigned j = 0; j < C_.columns(); ++j)
        	{
            	glr::Regression1 m(s, V_[j]);
            	V_[j] -= m(s);
        	}
    	}
		else
    	{
    		std::cerr << "glr::Regression m(S_, y_)" << std::endl;
    		glr::Regression m(S_, y_);
        	u_ -= m(S_);
        	for(unsigned j = 0; j < C_.columns(); ++j)
        	{
        		std::cerr << "glr::Regression m(S_, V_[" << j << "])" << std::endl;
            	glr::Regression m(S_, V_[j]);
            	V_[j] -= m(S_);
        	}
    	}
    }
}

const VECTOR PcisFilter::bootstrap(const unsigned B) const
{
	std::cerr << "bootstrap()" << std::endl;
    VECTOR v(V_[s()]);
    VECTOR bootstrap(B);
    for(unsigned b = 0; b < B; ++b)
    {
        std::random_shuffle(v.begin(), v.end());
        bootstrap[b] = pearson(u_, v);
    }
    return bootstrap;
}


const double PcisFilter::aic() const 
{
    double n = (double)u_.size();
    double k = (double)S_.columns();
    double mse = (u_ * u_) / n;
    
    return  n * log(mse) + 2.0 * (k + 1); 
}

const double PcisFilter::aicK() const
{
	return this->aic();
}


const double PcisFilter::rss() const
{
	return (u_ *  u_);	
}


const double PcisFilter::hampel(const unsigned j) const
{
    // Calculate absolute deviations from median score, dj = Ij - I50:
    double med = median(candidateScores_);
    VECTOR deviations;
    for(unsigned i = 0; i < candidateScores_.size(); ++i)
        deviations.push_back( fabs(candidateScores_[i] - med));
         
    // Estimate median absolute deviation from median and calculate Z(s)
    double mad = median(deviations);
    double Z = deviations[j] / (1.4826 * mad);
    return Z;
}
