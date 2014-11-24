#include "Som.h"

#define VECTOR std::vector<double>
#define MATRIX std::matrix<double>

Som::Som(const MATRIX& X)
	: X_(&X), distance(euclidean)
{
    setDefaultSize();
    setDefaultParameters();
}

Som::Som(const MATRIX& X, const unsigned rows, const unsigned columns) 
	: X_(&X), grid_(rows, columns, Neuron(X.columns())), distance(euclidean)
{
    setDefaultSize();
    setDefaultParameters();
}

Som::Som(const MATRIX& X, const unsigned rows, const unsigned columns, 
         const MATRIX& codebook)		   
    : X_(&X), 
    grid_(rows, columns, Neuron(codebook.columns())), 
    distance(euclidean)
{
	for(unsigned i = 0; i < rows; ++i)
		for(unsigned j = 0; j < columns; ++j)
			grid_[i][j] = codebook.row(i*columns + j);
            
    setDefaultParameters();
}

Som::Som(const Som& som)
	: X_(som.X_), 
    grid_(som.grid_), 
    distance(som.distance),
    orderingRate_(som.orderingRate_),
    orderingNhoodSize_(som.orderingNhoodSize_),
    orderingEpochs_(som.orderingEpochs_),
    tuningRate_(som.tuningRate_),
    tuningNhoodSize_(som.tuningNhoodSize_),
    tuningEpochs_(som.tuningEpochs_),
    conscience_(10.0),
    bias_(0.0001),
    bmuRow_(som.bmuRow_),
    bmuColumn_(som.bmuColumn_)
{}
 
		
Som::~Som()
{}

std::vector<Som::Neuron>& Som::operator [] (const unsigned i)
{
    return grid_[i];
}

const unsigned Som::columns() const
{
    return grid_.columns();
}

const unsigned Som::rows() const
{
    return grid_.rows();
}

const unsigned Som::dimension(void) const
{
    return X_->columns();    
}
   
void Som::resize(const unsigned rows, const unsigned columns)
{
    grid_ = std::matrix<Neuron>(rows, columns, Neuron(X_->columns()));
    initialise();
}

void Som::setX(const MATRIX& X)
{
    X_ = &X;
    redim(X_->columns());
}

void Som::redim(unsigned dimension)
{
    for(unsigned i = 0; i < rows(); ++i)
        for(unsigned j = 0; j < columns(); ++j)
            grid_[i][j].resize(dimension);
	initialise();
}

const Som::Neuron& Som::neuron(const unsigned i, const unsigned j) const
{
    return grid_[i][j];
}
void Som::initialise()
{
    rng::Uniform<double> U(-0.5, 0.5);
    rng::vector<double> w(U, X_->columns());
	double bias = 1.0/(double)(rows() * columns());
	for(unsigned i = 0; i < rows(); ++i)
		for(unsigned j = 0; j < columns(); ++j)
		{
			grid_[i][j].clear();
			grid_[i][j] = w();
			grid_[i][j].bias_ = bias;
		}
}

void Som::clear()
{
	for(unsigned i = 0; i < rows(); ++i)
		for(unsigned j = 0; j < columns(); ++j)
			grid_[i][j].clear();
}

const double Som::query(const VECTOR& v)
{
    double minDistance = distance(grid_[0][0], v);
	bmuRow_ = 0;
    bmuColumn_ = 0;
    double M = rows() * columns();
    for(unsigned i = 0; i < rows(); ++i)
		for(unsigned j = 0; j < columns(); ++j)
		{
			double d = distance(grid_[i][j], v) - 
				conscience_ * (1.0/M - grid_[i][j].bias_);
            if (d < minDistance)
			{
                bmuRow_ = i;
				bmuColumn_ = j;
				minDistance = d;
			}
		}
    return minDistance;
}


void Som::updateBias()
{
	for(unsigned i = 0; i < rows(); ++i)
		for(unsigned j = 0; j < columns(); ++j)	
			if(i == bmuRow_ && j == bmuColumn_)
				grid_[i][j].bias_ += bias_ * (1.0 - grid_[i][j].bias_);
			else
				grid_[i][j].bias_ -= bias_ * grid_[i][j].bias_;
}

Som::Neuron& Som::bmu()
{
	return grid_[bmuRow_][bmuColumn_];
}

const unsigned Som::bmuRow() const
{
    return bmuRow_;
}

const unsigned Som::bmuColumn() const
{
    return bmuColumn_;
}

void Som::shuffle()
{
    rng::Uniform<double> U(0,1);
    for(unsigned i = 0; i < rows(); ++i)
        for(unsigned j = 0; j < columns(); ++j)
        {
            Neuron& neuron = grid_[i][j];
            VECTOR key = rng::vector<double> (U, neuron.size());
            std::sort(key, neuron.indices_);
        }
}

void Som::orderClustersByDistance()
{
    for(unsigned i = 0; i < rows(); ++i)
        for(unsigned j = 0; j < columns(); ++j)
        {
            Neuron& neuron = grid_[i][j];
            std::sort(neuron.distances_, neuron.indices_);
        }
}

void Som::orderClustersByIndex()
{
    for(unsigned i = 0; i < rows(); ++i)
        for(unsigned j = 0; j < columns(); ++j)
        {
            Neuron& neuron = grid_[i][j];
            std::sort(neuron.indices_, neuron.distances_);
        }
}

void Som::query(const std::matrix<double>& X)
{
    clear();
	for(unsigned i = 0; i < X.rows(); ++i)
	{
		double d = query(X[i]);
		bmu().push_back(i, d);
	}
}

void Som::setDefaultSize() 
{
    double cells = sqrt((double)X_->rows());
    unsigned rows = (unsigned)(sqrt(golden() * cells) + 0.5);
    unsigned columns = (unsigned)((double)rows / golden() + 0.5);
    resize(rows, columns);
}

const double& Som::orderingRate() const
{
    return orderingRate_;
}

const unsigned& Som::orderingNhoodSize() const
{
    return orderingNhoodSize_;
}

const unsigned& Som::orderingEpochs() const
{
    return orderingEpochs_;
}

void Som::setOrderingRate(const double& rate)
{
   orderingRate_ = rate;
}

void Som::setOrderingNhoodSize(const unsigned& nhoodSize)
{
    orderingNhoodSize_ = nhoodSize;
}

void Som::setOrderingEpochs(const unsigned& epochs)
{
    orderingEpochs_ = epochs;
}

void Som::setTuningRate(const double& rate)
{
  tuningRate_ = rate;
}

void Som::setTuningNhoodSize(const unsigned& nhoodSize) 
{
   tuningNhoodSize_ = nhoodSize;
}

void Som::setTuningEpochs(const unsigned& epochs)
{
   tuningEpochs_ = epochs;
}

void Som::setBias(const double& bias)
{
	bias_ = bias;
}

void Som::setConscience(const double& conscience)
{
	conscience_ = conscience;
}

const double& Som::tuningRate() const
{
    return tuningRate_;
}

const unsigned& Som::tuningNhoodSize() const
{
    return tuningNhoodSize_;
}

const unsigned& Som::tuningEpochs() const
{
    return tuningEpochs_;
}

const double& Som::bias() const
{
	return bias_;
}

const double& Som::conscience() const
{
	return conscience_;
}

void Som::setDefaultParameters()
{
    unsigned minLength, maxLength;
    if (rows() >= columns())
    {
        minLength = columns();
        maxLength = rows();
    }
    else
    {
        minLength = rows();
        maxLength = columns();
    }
    
    orderingRate_ = 0.8;
    orderingNhoodSize_ = (unsigned)(0.5 * (double)maxLength + 0.5);
    orderingEpochs_ = 3;
    
    tuningRate_ = 0.01;
    tuningNhoodSize_ = (unsigned)(0.25 * (double)minLength + 0.5);
    tuningEpochs_ = 4;
    
    conscience_ = 10.0;
    bias_ = 0.0001;
    
    bmuRow_= 0;
    bmuColumn_ = 0;
    tolerance_ = small();
}

void Som::train()
{
    train(orderingRate_, orderingNhoodSize_, orderingEpochs_);
    train(tuningRate_, tuningNhoodSize_, tuningEpochs_);
}

void Som::train(const double& learningRate, 
    const unsigned& nhoodSize, 
    const unsigned& epochs)
{
    unsigned epoch = 0;
    unsigned iters = epochs * X_->rows();
    unsigned iter = 0;
    for(; epoch < epochs; ++epoch)
    {
        // Randomise ordering of samples
        std::sequence<unsigned> order(0, X_->rows(), 1);
        std::random_shuffle(order.begin(), order.end());
        std::sequence<unsigned>::const_iterator next = order.begin();
        
        for(; next != order.end(); ++next)
        {
            MATRIX::const_iterator x = X_->begin() + (*next);
                     
            // Set learning parameters for current iteration
            double decay = 1.0 - (double)iter / (double)iters;
            double alpha = decay * (learningRate);
            unsigned sigma = (unsigned)(decay * (double)nhoodSize + 0.5);
                
            query(*x);
			updateBias();
			
            unsigned iLower, iUpper, jLower, jUpper;
            if(bmuRow_ < sigma)
                iLower = 0;
            else iLower = bmuRow_ - sigma;
            if(bmuRow_ + sigma > rows() - 1)
                iUpper = rows();
            else iUpper = bmuRow_ + sigma;
            if(bmuColumn_ < sigma)
                jLower = 0;
            else jLower = bmuColumn_ - sigma;
            if(bmuColumn_ + sigma > columns() - 1)
                jUpper = columns();
            else jUpper = bmuColumn_ + sigma;
                  
            for(unsigned i = iLower; i < iUpper; ++i)
            {
                unsigned dx = i - bmuRow_;
                for(unsigned j = jLower; j < jUpper; ++j)
                {
                    unsigned dy = j - bmuColumn_;
                    
                    // Calculate map distance d to BMU
                    double d = sqrt((double)(dx*dx + dy*dy)); 
                    double h = alpha * exp(-(d * d / (2.0 * sigma * sigma)));
                    VECTOR& m = grid_[i][j];
                    m  += h * (*x - m);                    
                }
            }
            
            ++iter;
        }
        
    }
}

const double Som::ssintra() const
{
    double ssintra = 0.0;
    for(unsigned i = 0; i < rows(); ++i)
        for(unsigned j = 0; j < columns(); ++j)
            ssintra += sumsq(neuron(i,j).distances());
    return ssintra;    
}

MATRIX Som::codebook() const 
{
	// Returns the list of 1,...,k weight vectors
	MATRIX codebook(rows()*columns(), X_->columns());
	for(unsigned i = 0; i < rows(); ++i)
		for(unsigned j = 0; j < columns(); ++j)
		{
			unsigned k = i*columns() + j;
			codebook[k] = grid_[i][j];
		}
	return codebook;
}

const MATRIX Som::histogram() const
{
	// Returns a vector containing the neuron hits n(1), ..., n(k)
	MATRIX histogram(rows(), columns());
	for(unsigned i = 0; i < rows(); ++i)
		for(unsigned j = 0; j < columns(); ++j)
			histogram[i][j] = grid_[i][j].size();
	return histogram;
}

const MATRIX Som::frequency() const
{
    MATRIX frequency = histogram();
    frequency /= (double)X_->rows();
    return frequency;
}

MATRIX Som::membership() const
{
	// Returns a table containing the assignment of data
	// from the most recent query to the Som, including
	// cluster assignment and separation
	// Index, Object, Distance
	MATRIX membership(objectCount(), 3);
	unsigned  object = 0;
	for(unsigned i = 0; i < rows(); ++i)
		for(unsigned j = 0; j < columns(); ++j)
		{
			unsigned c = i*columns() + j;
			const Neuron& neuron = grid_[i][j];
			for(unsigned k = 0; k < neuron.size(); ++k)
			{
				membership[object][0] = c;
				membership[object][1] = neuron.index(k);
				membership[object][2] = neuron.distance(k);
				++object;
			}
		}
	return membership;
}

const unsigned Som::objectCount() const
{
	unsigned objectCount = 0;
	for(unsigned i = 0; i < rows(); ++i)
		for(unsigned j = 0; j < columns(); ++j)
			objectCount += grid_[i][j].size();
	return objectCount;
}

std::ostream& operator << (std::ostream& os, const Som& som)
{
    for(unsigned rowIndex = 0; rowIndex < som.rows(); ++rowIndex)
    {
        // Print leading padding
        for(unsigned p = 1; p < som.rows() - rowIndex; ++p)
            os << "   ";
        // Print elements on diagonal from lower-left to top-right
        int i = rowIndex;
        unsigned j = 0;
        while((i >= 0) && (j < som.columns()))
        {
            if (som.grid_[i][j].empty()) 
                os << "   ...";
            else
                os << "   " << std::setw(3) << std::setfill('.') 
                    << som.grid_[i][j].size();
            --i;
            ++j;
        }
        os << '\n';
    }
    for(unsigned columnIndex = 1; columnIndex < som.columns(); ++columnIndex)
    {
        // Print leading padding
        for(unsigned p = 1; p <= columnIndex; ++p)
            os << "   ";
        // Print elements on diagonal from lower-left to top-right
        int i = som.rows() - 1;
        unsigned j = columnIndex;
        while(j < som.columns() && i >= 0)
        {
            if (som.grid_[i][j].empty())
                os << "   ...";
            else
                os << "   " << std::setw(3) << std::setfill('.') 
                    << som.grid_[i][j].size();
            --i;
            ++j;
        }
        os << '\n';
    }
    return os;
}


const MATRIX Som::neyman() const
{
    MATRIX neyman(rows(), columns(), 0.0);
    double sum = 0.0;
    for(unsigned i = 0; i < rows(); ++i)
    {
        for(unsigned j = 0; j < columns(); ++j) 
        {
            const Som::Neuron& neuron = grid_[i][j];
            if(!neuron.empty())
            {
                neyman[i][j] = (double)neuron.size() * stdev(neuron.distances());
                sum += neyman[i][j];
            }
        }
    }
    neyman /= sum;
    return neyman;
}

const MATRIX Som::stdevs() const 
{
	MATRIX stdevs(rows(), columns(), 0.0);
    for(unsigned i = 0; i < rows(); ++i)
    {
        for(unsigned j = 0; j < columns(); ++j) 
        {
            const Som::Neuron& neuron = grid_[i][j];
            if(!neuron.empty())
            {
                stdevs[i][j] = stdev(neuron.distances());
            }
        }
    }
    return stdevs;
}

const MATRIX Som::sd_histogram(const unsigned smoothness) const
{
    MATRIX C = codebook();
    VECTOR f(C.rows(), 0.0);
    MATRIX::const_iterator x = X_->begin();
    
    const double c = (double)(smoothness * smoothness) / 2.0;
    
    for(; x != X_->end(); ++x)
    {
        // Calculate distance, d(x,c) for each c in codebook C.
        VECTOR d;
        std::vector<unsigned> index;
        for(unsigned k = 0; k < C.rows(); ++k)
        {
            index.push_back(k);
            d.push_back(euclidean(C[k], *x));
        }
        // Rank std::vector indices from closest to, to farthest from object x
        sort(d, index);
        // Add membership to frequency
        for(unsigned i = 0; i < smoothness && i < C.rows(); ++i)
                f[index[i]] += (double)(smoothness - i) / c;
    }
    return MATRIX(f, rows(), columns());
};

const std::matrix<unsigned> Som::coincidence() const
{
    std::matrix<unsigned> coincidence(X_->rows(), X_->rows(), 0);
    for(unsigned i = 0; i < rows(); ++i)
        for(unsigned j = 0; j < columns(); ++j)
        {
            const Neuron& neuron = grid_[i][j];
            for(unsigned p = 0; p < neuron.size(); ++p)
                for(unsigned q = p + 1; q < neuron.size(); ++q)
                    coincidence[neuron.indices_[p]][neuron.indices_[q]] = 1;
        }
    return coincidence;
}

const double Som::silhouette(std::matrix<double>& D)
{
	double S = 0.0;
	std::vector<double> s;
	for(unsigned ri = 0; ri < rows(); ++ri)
		for(unsigned ci = 0; ci < columns(); ++ci)
		{
			Neuron& Ci = grid_[ri][ci];
			if(Ci.size() > 1)
			{
				for(unsigned i = 0; i < Ci.size(); ++i)
				{
					unsigned oi = Ci.index(i);
					double ai = 0.0;
					for(unsigned j = 0; j < Ci.size(); ++j)
						ai += D[Ci.index(i)][Ci.index(j)];
					ai /= (double)(Ci.size() - 1);
									
					double bi = large();
					for(unsigned rj = 0; rj < rows(); ++rj)
						for(unsigned cj = 0; cj < columns(); ++cj)
						{
							Neuron& Cj = grid_[rj][cj];
							if(Cj.size() > 1 && Cj != Ci)
							{
								double b = 0.0;
								for(unsigned j = 0; j < Cj.size(); ++j)
								{
									unsigned oj = Cj.index(j);
									b += D[oi][oj];
								}
								b /= (double)(Cj.size());
								if(b < bi)
								{
									bi = b;
								}
							} 
						}
					
					double si;
					if(fabs(ai) > fabs(bi))
						si = (bi - ai) / fabs(ai);
					else
						si = (bi - ai) / fabs(bi);
					S += si;
				}
			}
		}
	S /= (double)X_->rows();
	return S;
}

#undef VECTOR
#undef MATRIX

