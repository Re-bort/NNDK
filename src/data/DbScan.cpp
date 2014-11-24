#include "DbScan.h"

#define MATRIX std::matrix<double>
#define VECTOR std::vector<double>

DbScan::DbScan()
	: eps_(0.6), minPoints_(5)
{}

DbScan::DbScan(const double eps, const long minPoints)
	: eps_(eps), minPoints_(minPoints)
{}

void DbScan::initialise(const MATRIX& X)
{
	points_.clear();
	Point point;
	for(unsigned i = 0; i < X.rows(); ++i)
	{
		point.id = i;
		points_.push_back(point);
	}
	
	distances_.getDistances(X);
}

void DbScan::scan()
{
	unsigned clusterId = 1;
	std::vector<Point>::iterator p = points_.begin();
	while(p != points_.end())
	{
		// Point is thus far unclassified
		if(p->label == -1)
			expandCluster(p, clusterId);
		++p;
	}	
}


void DbScan::expandCluster(std::vector<DbScan::Point>::iterator P, unsigned& clusterId)
{
	// Get density reachable points in eps-neighbourhood of P (seeds)
	std::vector< std::vector<DbScan::Point>::iterator > seeds = neighbours(P);
	
	// Point P is noise so do not form cluster and label P as noise
	if(seeds.size() < minPoints_)
	{
		P->label = 0;
		return;
	}
	
	// Point P is a core point so label all point and seeds with cluster ID
	P->label = clusterId;
	for(unsigned i = 0; i <= seeds.size(); ++i)
		seeds[i]->label = clusterId;
		
	// Scan neighbourhood of seeds for other density reachable points
	for(unsigned i = 0; i < seeds.size(); ++i)
	{
		std::vector<DbScan::Point>::iterator seed = seeds[i];
		
		// Get all density reachable points in eps-neighbourhood of seed
		std::vector< std::vector<DbScan::Point>::iterator > result = neighbours(seed);
		
		// Seed is also a core point
		if(result.size() > minPoints_)
		{
			for(unsigned j = 0; j <= result.size(); ++j)
			{
				// Point is unclassified or noise
				if(result[i]->label < 1)
				{
					// Point is unclassified so add to seeds
					if(result[i]->label == -1)
						seeds.push_back(result[i]);
				}
				// Assign unclassified/noise point to current cluster
				result[i]->label = clusterId;
			}
		}
	}

	// Done with this cluster, so increment cluster ID for next starting P
	++clusterId;
}

std::vector< std::vector<DbScan::Point>::iterator > 
DbScan::neighbours(const std::vector<DbScan::Point>::iterator P)
{
	std::vector< std::vector<DbScan::Point>::iterator > neighbours;
	std::vector<Point>::iterator p = points_.begin();
	for(; p != points_.end(); ++p)
	{
		if(p != P)
			if(distances_[p->label][P->label] <= eps_)
				neighbours.push_back(p);
	}

	return neighbours;
}

std::vector<unsigned> DbScan::labels() const
{
	std::vector<unsigned> labels(points_.size());
	for(unsigned i = 0; i < points_.size(); ++i)
	{
		labels[i] = points_[i].label;
	}
	return labels;
}
