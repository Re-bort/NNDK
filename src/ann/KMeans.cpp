#include "KMeans.h"

#define VECTOR std::vector<double>
#define MATRIX std::matrix<double>
#define KEY	std::vector<unsigned>

KMeans::KMeans(const MATRIX& X)
	: X_(&X), K_(2), learningRate_(1.0), epochs_(100), distance(euclidean)
{
}

KMeans::KMeans(const MATRIX& X, const MATRIX& codebook)		   
    : X_(&X), learningRate_(1.0), epochs_(100),  distance(euclidean)
{
	setK(codebook.rows());
	for(unsigned i = 0; i < K(); ++i)
	{
		KMeans::Cluster cluster(codebook.row(i));
		clusters_[i] = cluster;
	}
}

KMeans::KMeans(const KMeans& kmeans)
	: X_(kmeans.X_), 
    clusters_(kmeans.clusters_), 
    K_(kmeans.K_),
    learningRate_(kmeans.learningRate_),
    epochs_(kmeans.epochs_),
    distance(kmeans.distance),
    bmcIndex_(kmeans.bmcIndex_)
{}

KMeans::~KMeans()
{}

KMeans::Cluster& KMeans::operator [] (const unsigned i)
{
    return clusters_[i];
}

void KMeans::initialise()
{
    rng::Uniform<double> U(-0.5, 0.5);
    rng::vector<double> w(U, X_->columns());
	for(unsigned i = 0; i < K(); ++i)
	{
		clusters_[i].clear();
		clusters_[i] = w();
	}
}

void KMeans::clear()
{
	for(unsigned i = 0; i < K(); ++i)
		clusters_[i].clear();
}

const double KMeans::query(const VECTOR& v)
{
    double minDistance = distance(clusters_[0].m_, v);
	bmcIndex_ = 0;
    for(unsigned i = 1; i < K(); ++i)
	{		
		double d = distance(clusters_[i].m_, v);
        if (d < minDistance)
		{
			bmcIndex_ = i;
			minDistance = d;
		}
	}

    return minDistance;
}

const unsigned KMeans::K() const
{
	return K_;	
}

const unsigned KMeans::dimension(void) const
{
    return X_->columns();    
}
   
void KMeans::setK(const unsigned k)
{
	K_ = k;
    clusters_ = std::vector<Cluster>(K_);
    initialise();
}

void KMeans::setX(const MATRIX& X)
{
    X_ = &X;
    redim(X_->columns());
}

void KMeans::redim(unsigned dimension)
{
    for(unsigned i = 0; i < K(); ++i)
            clusters_[i].m_.resize(dimension);
	initialise();
}

const KMeans::Cluster& KMeans::cluster(const unsigned i) const
{
    return clusters_[i];
}

KMeans::Cluster& KMeans::bmc()
{
	return clusters_[bmcIndex_];
}

const unsigned KMeans::bmcIndex() const
{
    return bmcIndex_;
}

void KMeans::shuffle()
{
    rng::Uniform<double> U(0,1);
    for(unsigned i = 0; i < K(); ++i)
    {
        Cluster& cluster = clusters_[i];
        VECTOR key = rng::vector<double> (U, cluster.size());
        std::sort(key, cluster.indices_);
    }
}

void KMeans::orderClustersByDistance()
{
    for(unsigned i = 0; i < K(); ++i)
    {
		Cluster& cluster = clusters_[i];
        std::sort(cluster.distances_, cluster.indices_);
    }
}

void KMeans::orderClustersByIndex()
{
    for(unsigned i = 0; i < K(); ++i)
        {
            Cluster& cluster = clusters_[i];
            std::sort(cluster.indices_, cluster.distances_);
        }
}

void KMeans::query(const std::matrix<double>& X)
{
    clear();
	for(unsigned i = 0; i < X.rows(); ++i)
	{
		double d = query(X[i]);
		bmc().push_back(i, d);
	}
}

void KMeans::setDefaultSize()
{
    unsigned k = (unsigned)sqrt((double)X_->rows());
    setK(k);
}

void KMeans::setLearningRate(double learningRate)
{
	learningRate_ = learningRate;
}

void KMeans::setEpochs(unsigned epochs)
{
	epochs_ = epochs;
}
	
const double& KMeans::learningRate() const
{
    return learningRate_;
}

const unsigned& KMeans::epochs() const
{
    return epochs_;
}


void KMeans::setDefaultParameters()
{
    learningRate_ = 1.0;
    epochs_ = 100;
    bmcIndex_= 0;
}

void KMeans::updateCentroid(KMeans::Cluster& cluster)
{
	if(cluster.size() > 0)
	{
		VECTOR m(dimension(), 0.0);
		for(unsigned i = 0; i < cluster.size(); ++i)
			m += (*X_)[cluster.index(i)];
		m = (1.0/(double)cluster.size()) * m;
		
		cluster.m() = m;
	}
}

void KMeans::train()
{
    for(unsigned epoch = 0; epoch < epochs_; ++epoch)
    {
    	// Query the database using current cluster codebooks m(i)
    	query(*X_);
    	
    	// Recalculate centroids of clusters and update codebooks
    	for(unsigned i = 0; i < K(); ++i)
    		updateCentroid(clusters_[i]);
        
    }
}

const double KMeans::ssintra() const
{
    double ssintra = 0.0;
    for(unsigned i = 0; i < K(); ++i)
    	ssintra += sumsq(clusters_[i].distances());
    return ssintra;    
}

MATRIX KMeans::codebook() const 
{
	// Returns the list of 1,...,k codebook vectors
	MATRIX codebook(K(), dimension());
	for(unsigned i = 0; i < K(); ++i)
		codebook[i] = clusters_[i].m_;
	
	return codebook;
}

VECTOR KMeans::histogram() const
{
	// Returns a vector containing the neuron hits n(1), ..., n(k)
	VECTOR histogram(K());
	for(unsigned i = 0; i < K(); ++i)
		histogram[i] = clusters_[i].size();
	return histogram;
}

VECTOR KMeans::frequency() const
{
    VECTOR frequency = histogram();
    frequency /= (double)X_->rows();
    return frequency;
}

MATRIX KMeans::membership() const
{
	// Returns a table containing the assignment of data
	// from the most recent query to the Som, including
	// cluster assignment and separation
	// Index, Object, Distance
	MATRIX membership(X_->rows(), 3);
	unsigned count = 0;
	for(unsigned i = 0; i < K(); ++i)
	{
		const Cluster& cluster = clusters_[i];
		for(unsigned j = 0; j < cluster.size(); ++j)
		{
			membership[count][0] = i;
			membership[count][1] = cluster.index(j);
			membership[count][2] = cluster.distance(j);
			++count;
		}
	}
	return membership;
}


#undef VECTOR
#undef MATRIX

