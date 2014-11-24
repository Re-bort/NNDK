#include "Grnn.h"

#define VECTOR std::vector<double>
#define MATRIX std::matrix<double>

static const double GOLD = 1.618034;
static const double GLIMIT = 100.0;
static const double TINY = 1.0e-20;
static const int ITMAX = 100;
static const double CGOLD = 0.3819660;
static const double ZEPS = 1.0e-10;

inline void SHFT(double& a, double& b, double& c, double& d)
    { a = b, b = c, c = d; }

inline const double SIGN(const double& a, const double& b)
    { return (b < 0 ? -fabs(a) : fabs(a)); }

inline const double& FMAX(const double& a, const double& b)
    { return (b > a ? b : a); }

inline void SWP(double& a, double& b)
    { double temp = a; a = b, b = temp; }


Grnn::Trainer::Trainer(const MATRIX& X, const VECTOR& y)
    : X_(&X), y_(&y), z_(VECTOR(y.size(), 0.0))
{
    setDefaultParameters();
}

const double Grnn::Trainer::tolerance() const
{
    return tolerance_;
}

const unsigned Grnn::Trainer::maxIter() const
{
    return maxIter_;
}

const double Grnn::Trainer::minBandwidth() const
{
    return minBandwidth_;
}

const double Grnn::Trainer::maxBandwidth() const
{
    return maxBandwidth_;
}

void Grnn::Trainer::setError(Error error)
{
    error_ = error;
}

void Grnn::Trainer::setTolerance(const double tolerance)
{
    tolerance_ = tolerance;
}

void Grnn::Trainer::setMaxBandwidth(const double& bandwidth)
{
    maxBandwidth_ = bandwidth;   
}

void Grnn::Trainer::setMinBandwidth(const double& bandwidth)
{
    minBandwidth_ = bandwidth;    
    
}
 
void Grnn::Trainer::setMaxIter(const unsigned& maxIter)
{
    maxIter_ = maxIter;
}

void Grnn::Trainer::setDefaultParameters()
{
    error_ = mse;
    tolerance_ = 0.0001;
    minBandwidth_ = small();
    maxBandwidth_ = 5.0;
    maxIter_ = 100;
}

const double Grnn::Trainer::error(Grnn& grnn, const double bandwidth)
{
    grnn.setBandwidth(bandwidth);
    z_ = grnn(*X_);
    return (*error_)(*y_, z_);
}

const double Grnn::Trainer::error(Grnn& grnn)
{
    z_ = grnn(*X_);
    return (*error_)(*y_, z_);
}

const double Grnn::Trainer::train(Grnn& grnn, Algorithm algorithm)
{
    switch (algorithm)
    {
        case climb:
            trainHillClimb(grnn);
            break;
                
        case brent:
            trainBrent(grnn);
            break;
            
        default:
            trainEnumerate(grnn);
      }    
    return error(grnn, grnn.bandwidth());
}
    
void Grnn::Trainer::trainHillClimb(Grnn& grnn)
{
    double h = grnn.bandwidth();
    double dh;
    if(error(grnn, 0.8*h) < error(grnn, 1.5*h))
        dh = -tolerance_;
    else
        dh = tolerance();        
       
    double oldErr = error(grnn, grnn.bandwidth());
    double iter = 0;
    while(iter < maxIter_ && h < maxBandwidth_ && h > (minBandwidth_ + dh))
    {
        double err = error(grnn, h + dh);
        if(err >= oldErr)
            break;
        h += dh;
        oldErr = err;
    }            
}

void Grnn::Trainer::trainEnumerate(Grnn& grnn)
{   
    double h, err, hOpt, errOpt;
    errOpt = large();
    for(h = minBandwidth_; h <= maxBandwidth_; h += 0.5*tolerance_)
    {
        err = error(grnn, h);   
        if (err < errOpt)
        {
            errOpt = err;
            hOpt =  h;  
        }
    }
    grnn.setBandwidth(hOpt);
}




void Grnn::Trainer::bracket(Grnn& grnn, double& a, double& b, double& c)
{
    double fa = error(grnn, a);
    double fb = error(grnn, b);
    if (fb > fa)
    {
        SWP(a, b);
        SWP(fa, fb);
    }
    c = b + GOLD*(b - a);
    double fc = error(grnn, c);
    double fu;
    while (fb > fc)
    {
        double r = (b - a)*(fb - fc);
        double q = (b - c)*(fb - fa);
        double u = b - (q*(b - c) - r*(b - a))/
            (2.0*SIGN(FMAX(fabs(q - r), TINY), q - r));
        double ulim = b + GLIMIT*(c - b);
        if(a > 5.0 && b > 5.0 && c > 5.0)
        {
                a = b;
                b = u;
                fa = fb;
                fb = fu;
                return;
        }
        if ((b - u)*(u - c) > 0.0)
        {
            fu = error(grnn, u);
            if (fu < fc)
            {
                a = b;
                b = u;
                fa = fb;
                fb = fu;
                return;
            } 
            else if (fu > fb)
            {
                c = u;
                fc = fu;
                return;
            }
            u = c + GOLD*(c - b);
            fu = error(grnn, u);
        }
        else if ((c - u)*(u - ulim) > 0.0) 
        {
            fu = error(grnn, u);
            if (fu < fc)
            {
                double temp;
                temp = c + GOLD*(c - b);
                SHFT(b, c, u, temp);
                temp = error(grnn, u);
                SHFT(fb, fc, fu, temp);
            }
        }
        else if ((u - ulim)*(ulim - c) >= 0.0)
        {
            u = ulim;
            fu = error(grnn, u);
        } 
        else 
        {
            u = c + GOLD*(c - b);
            fu = error(grnn, u);
        }
        SHFT(a, b, c, u);
        SHFT(fa, fb, fc, fu);
    }
}

void Grnn::Trainer::trainBrent(Grnn& grnn)
{
    double a = tolerance();
    double b = grnn.bandwidth();
    double x = 0.5*(a + b);
    bracket(grnn, a, b, x);
       
    if (a > b)
        SWP(a, b);
        
    double fx = error(grnn, x);
    double w = x, fw = fx;
    double v = x, fv = fx;
    
    double d = 0.0, u, e = 0.0;
    for(unsigned iter = 1; iter <= maxIter(); ++iter) 
    {
        double xm = 0.5*(a + b);
        double tol1 = tolerance()*fabs(x) + ZEPS;
        double tol2 = 2.0*(tol1);
        if (fabs(x - xm) <= (tol2 - 0.5*(b - a))) 
            break;
        if (fabs(e) > tol1)
        {
            double p = (x-v)*(x-v)*(fx-fw) - (x-w)*(x-w)*(fx-fv);
            double q = 2.0*((x-v)*(fx-fw) - (x-w)*(fx-fv));
            
            if (q > 0.0) 
                p = -p;
            q = fabs(q);
            
            double etemp = e;
            e = d;
            if (fabs(p) >= fabs(0.5*q*etemp) || 
                p <= q*(a - x) || 
                p >= q*(b - x))
            {
                if (x >= xm) 
                    e = a - x; 
                else 
                    e = b - x;
                d = CGOLD*(e);
            }   
            else
            {
                d = p/q;
                u = x + d;
                if (u - a < tol2 || b - u < tol2)
                    d = SIGN(tol1, xm - x);
            }
        } 
        else 
        {
            if (x >= xm) 
                    e = a - x; 
                else 
                    e = b - x;
            d = CGOLD *(e);
        }
        
        double u;
        if(fabs(d) >= tol1)
            u = x + d;
        else
            u = x + SIGN(tol1, d);
        double fu = error(grnn, u);
        if (fu <= fx) 
        {
            if (u >= x) 
                a = x; 
            else 
                b = x;
            SHFT(v, w, x, u);
            SHFT(fv, fw, fx, fu);
        } 
        else 
        {
            if (u < x) 
                a = u; 
            else
                b = u;
            if (fu <= fw || w == x)
            {
                v = w;
                w = u;
                fv = fw;
                fw = fu;
            } 
            else if (fu <= fv || v == x || v == w) 
            {
                v = u;
                fv = fu;
            }
        }
    }
    grnn.setBandwidth(x);
}


#undef VECTOR
#undef MATRIX
