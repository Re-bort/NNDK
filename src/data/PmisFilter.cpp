// PmisFilter.cpp

#include "PmisFilter.h"
#include "../kde/Pdf.h"
#include "../kde/Regression.h"
#include "../kde/Gaussian.h"
#include "../math/statistics.h"
#include "Standardiser.h"

#include "../util/matio.h"

#define SERIES std::series<double>
#define TABLE std::table<double>
#define VECTOR std::vector<double>
#define MATRIX std::matrix<double>

PmisFilter::PmisFilter(const TABLE& candidates, const SERIES& response)
    : IvsFilter(candidates, response)
{}

PmisFilter::~PmisFilter()
{}

void PmisFilter::calculateScores()
{
    candidateScores_.resize(C_.columns());
    
    standardise(u_);
    kde::Pdf1D<Gaussian1D> fu(u_);
    for(unsigned j = 0; j < C_.columns(); ++j)
    {
        standardise(V_[j]);
        kde::Pdf1D<Gaussian1D> fv(V_[j]);
        kde::Pdf2D<Mahalanobis2D> fvu(V_[j], u_);
        candidateScores_[j] = mi(fv, fu, fvu);
    }
}

void PmisFilter::calculateResiduals()
{
	u_ = y_;
    V_ = C_.transpose();
    
    if(S_.columns() > 0)
    {
        kde::Regression<Gaussian> m(S_, y_);
        u_ -= m.leaveOneOut();
        for(unsigned j = 0; j < C_.columns(); ++j)
        {
            kde::Regression<Gaussian> m(S_, V_[j]);    
            V_[j] -= m.leaveOneOut();
            //V_[j] -= m(S_);
        }
    }
}

const VECTOR PmisFilter::bootstrap(const unsigned B) const
{
    kde::Pdf1D<Gaussian1D> fu(u_);
    VECTOR v(V_[s()]);
    VECTOR bootstrap(B);
    for(unsigned b = 0; b < B; ++b)
    {
        std::random_shuffle(v.begin(), v.end());
        
        kde::Pdf1D<Gaussian1D> fv(v);
        kde::Pdf2D<Mahalanobis2D> fvu(v, u_);
        
        bootstrap[b] = mi(fu(u_), fv(v), fvu(v, u_));
    }

    return bootstrap;
}


const double PmisFilter::aic() const 
{
    double n = (double)u_.size();
    double k = (double)S_.columns();
    double mse = (u_ * u_) / n;
    
    return  n * log(mse) + 2.0 * (k + 1); 
}


const double PmisFilter::trace() const
{
	double n = (double)u_.size();
    double k = (double)S_.columns();
	double h = kde::scottBandwidth(n, k, 1.0);
	Gaussian K(h);
	
	double tr = 0.0;
	for(unsigned i = 0; i < n; ++i)
	{
		double wii = K(S_[i] - S_[i]);
		double den = 0.0;
		for(unsigned j = 0; j < n; ++j)
			den += K(S_[i] - S_[j]);
		
		tr += (wii / den);
	}		
	
	return tr;
}


const double PmisFilter::aicK() const
{
	double n = (double)u_.size();
    double k = (double)S_.columns();
    double mse = (u_ *  u_) / n;
    
    if(k == 0)
    	return  n * log(mse);
    
    return  n * log(mse) + 2.0 * trace();
}


const double PmisFilter::rss() const
{
	return (u_ *  u_);	
}


const double PmisFilter::hampel(const unsigned j) const
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

#undef SERIES
#undef TABLE
#undef VECTOR
#undef MATRIX
