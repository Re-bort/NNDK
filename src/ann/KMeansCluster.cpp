#include "KMeans.h"

#define VECTOR std::vector<double>
#define INDEX std::vector<unsigned>

KMeans::Cluster::Cluster()
{}

KMeans::Cluster::Cluster(const KMeans::Cluster& cluster)
	: m_(cluster.m_), indices_(cluster.indices_), 
	  distances_(cluster.distances_)
{}

KMeans::Cluster::Cluster(const unsigned dimension)
	: m_(dimension, 0)
{}

KMeans::Cluster::Cluster(const VECTOR& m)
	: m_(m)
{}

KMeans::Cluster::~Cluster()
{}

VECTOR& KMeans::Cluster::m()
{
	return m_;
}

const INDEX& KMeans::Cluster::indices() const
{
	return indices_;
}

const unsigned KMeans::Cluster::index(unsigned i) const
{
	return indices_[i];
}

const std::vector<double>& KMeans::Cluster::distances() const
{
	return distances_;
}

const double KMeans::Cluster::distance(unsigned i) const
{
	return distances_[i];
}

const unsigned KMeans::Cluster::size() const
{
	return indices_.size();
}

const bool KMeans::Cluster::empty() const
{
	return indices_.empty();
}

const bool KMeans::Cluster::singleton() const
{
	if (indices_.size() == 1)
		return true; 
	else
		return false;
}

void KMeans::Cluster::push_back(const unsigned i, const double d)
{
	indices_.push_back(i);
	distances_.push_back(d);
}

void KMeans::Cluster::clear()
{
	indices_.clear();
	distances_.clear();
}

#undef VECTOR
#undef INDEX
