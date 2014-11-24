#ifndef	som_Grid_h
#define som_Grid_h

#include "../matrix"
#include "../math/vecops.h"
#include "../math/constants.h"
#include "../math/statistics.h"
#include "../random"

#include "Neuron.h"

namespace som {

class Grid
	: public std::matrix<Neuron>
{
	public:
                                Grid(unsigned rows = 10, unsigned columns = 10,
									 double	learningRate = 0.6,
									 unsigned maxIterations = 1000);
							    Grid(unsigned rows, unsigned columns,
							         const std::matrix<double>& codebook);
					         	Grid(const Grid& grid);
								~Grid();
		// Access methods
		const unsigned			dimension(void) const;
		Neuron&					firingNeuron();
		const double			learningRate(void) const;
		const unsigned			maxIterations(void) const;
		void					redim(unsigned dimensions);
		void					resize(unsigned rows, unsigned columns);
		void					setLearningRate(double learningRate);
		void					setMaxIterations(unsigned maxIterations);
		// Query and learning methods
		void 					clear(void);
		void					query(const std::matrix<double>& database);
		void					query(const std::vector<double>& v);
		void					randomise(void);
		void					train(const std::matrix<double>& database);
		// Analysis and summary reporting methods
		std::matrix<double>		clustering(void);
		std::matrix<double>		codebook(void);
		std::vector<double>		histogram(void);
		std::matrix<double>		membership(void);
		const unsigned			objectCount(void) const;
	
	private:
		unsigned				dimension_;
		double					initialLearningRate_;
		unsigned				initialNeighbourhoodSize_;
		unsigned				maxIterations_;
		unsigned				iteration_;
		double					learningRate_;
		unsigned				neighbourhoodSize_;	
		unsigned				neighbourhoodLowerRowIndex_;
		unsigned				neighbourhoodUpperRowIndex_;
		unsigned				neighbourhoodUpperColumnIndex_;
		unsigned				neighbourhoodLowerColumnIndex_;
		unsigned 				firingObjectIndex;
		unsigned				firingRowIndex_;
		unsigned				firingColumnIndex_;
		double					firingDistance_;
		const double			(* distance)(const std::vector<double>& u,
											 const std::vector<double>& v);
											 
		void				setNeighbourhood();
		void				updateNeighbourhood(const std::vector<double>& v);
};


Grid::Grid(unsigned rows, unsigned columns, double learningRate,
	       unsigned maxIterations)
	: std::matrix<Neuron>(rows, columns, Neuron(2)), 
	  initialLearningRate_(learningRate), maxIterations_(maxIterations),
	  distance(euclidean)
{}

Grid::Grid(unsigned rows, unsigned columns,
		   const std::matrix<double>& codebook)		   
	: std::matrix<Neuron>(rows, columns, Neuron(codebook[0].size())),
	  initialLearningRate_(0.6),
	  maxIterations_(1000), distance(euclidean)
{
	for(unsigned i = 0; i < rows; ++i)
		for(unsigned j = 0; j < columns; ++j)
			(*this)[i][j].v_ = codebook[i*columns + j];
}

Grid::Grid(const Grid& grid)
	: dimension_(grid.dimension_), 
	  initialLearningRate_(grid.initialLearningRate_),
	  maxIterations_(grid.maxIterations_),  
      neighbourhoodSize_(grid.neighbourhoodSize_),
	  distance(grid.distance)
{}
		
Grid::~Grid()
{
}

inline const unsigned Grid::dimension() const
{
	return dimension_;	
}

inline void Grid::resize(const unsigned rows, const unsigned columns)
{
    (std::matrix<Neuron>)(*this) = std::matrix<Neuron>(rows, columns, Neuron(dimension_));
	randomise();
}

inline void Grid::redim(unsigned dimension)
{
	dimension_ = dimension;
	randomise();
}

inline const double Grid::learningRate() const
{
	return initialLearningRate_;
}

inline void Grid::setLearningRate(double learningRate)
{
	initialLearningRate_ = learningRate;
}

inline const unsigned Grid::maxIterations() const
{
	return maxIterations_;
}

inline void Grid::setMaxIterations(unsigned maxIterations)
{
	maxIterations_ = maxIterations;
}

void Grid::randomise()
{
	rng::Uniform<double> U(-0.5, 0.5);
    rng::vector<double> w(U, dimension_);
	for(unsigned i = 0; i < rows(); ++i)
		for(unsigned j = 0; j < columns(); ++j)
			(*this)[i][j].v_ = w();
}

void Grid::query(const std::vector<double>& v)
{
	firingDistance_ = large();
	for(unsigned i = 0; i < rows(); ++i)
		for(unsigned j = 0; j < columns(); ++j)
		{
			double d = distance((*this)[i][j].v(), v);
			if (d < firingDistance_)
			{
				firingRowIndex_ = i;
				firingColumnIndex_ = j;
				firingDistance_ = d;
			}
		}
}

inline Neuron& Grid::firingNeuron()
{
	return (*this)[firingRowIndex_][firingColumnIndex_];
}

void Grid::train(const std::matrix<double>& database)
{
	if (dimension_ != database.columns())
		redim(database.columns());
	else
		randomise();
	
	initialNeighbourhoodSize_ = max(rows(), columns());
	for(iteration_ = 0; iteration_ < maxIterations_; ++iteration_)
	{
		double decayConstant = 1.0 - (double)iteration_/(double)maxIterations_;
		learningRate_ = decayConstant*initialLearningRate_;
		neighbourhoodSize_ = (unsigned)decayConstant*initialNeighbourhoodSize_;
		for(unsigned i = 0; i < database.rows(); ++i)
		{
			query(database[i]);
			setNeighbourhood();
			updateNeighbourhood(database[i]);
		}
	}	
}

void Grid::query(const std::matrix<double>& database)
{
	clear();
	for(unsigned i = 0; i < database.rows(); ++i)
	{
		query(database[i]);
		firingNeuron().push_back(i, firingDistance_);
	}
}
		
void Grid::clear()
{
	for(unsigned i = 0; i < rows(); ++i)
		for(unsigned j = 0; j < columns(); ++j)
			(*this)[i][j].clear();
}

void Grid::setNeighbourhood()
{
	if (firingRowIndex_ < neighbourhoodSize_)
		neighbourhoodLowerRowIndex_ = 0;
	else
		neighbourhoodLowerRowIndex_ = firingRowIndex_ - neighbourhoodSize_;
	
	if ((rows() - 1) < firingRowIndex_ + neighbourhoodSize_)
		neighbourhoodUpperRowIndex_ = (rows() - 1);
	else
		neighbourhoodUpperRowIndex_ = firingRowIndex_ + neighbourhoodSize_;
	
	if (firingColumnIndex_ < neighbourhoodSize_)
		neighbourhoodLowerColumnIndex_ = 0;
	else
		neighbourhoodLowerColumnIndex_ = firingColumnIndex_ 
			- neighbourhoodSize_;
	
	if ((columns() - 1) < firingColumnIndex_ + neighbourhoodSize_)
		neighbourhoodUpperColumnIndex_ = (columns() - 1);
	else
		neighbourhoodUpperColumnIndex_ = (firingColumnIndex_
			+ neighbourhoodSize_);
}

void Grid::updateNeighbourhood(const vector<double>& v)
{
	for(unsigned i = neighbourhoodLowerRowIndex_; 
		i <= neighbourhoodUpperRowIndex_;
		++i)
	{
		for(unsigned j = neighbourhoodLowerColumnIndex_;
			j <= neighbourhoodUpperColumnIndex_;
			++j)
		{
			double h = 1.0 /
				(1.0 + sqrt(pow(j - firingColumnIndex_, 2.0) 
					+ pow(i - firingRowIndex_, 2.0)));
			(*this)[i][j].v_ += 
				h*learningRate_*(v - (*this)[i][j].v_);
		}
	}
}

std::matrix<double> Grid::clustering()
{
	// Returns a table that contains information about the
	// distribution of objects within clusters.
	// Row entries in std::matrix correspond to:
	// k, Size, Rel. Size, St. dev.
	std::matrix<double> clustering(rows()*columns(), 4);
	double n = objectCount();
	for(unsigned i = 0; i < rows(); ++i)
		for(unsigned j = 0; j < columns(); ++j)
		{
			unsigned k = i*columns() + j;
			Neuron * node(&(*this)[i][j]);
			clustering[k][0] = (i*columns()) + j;
			clustering[k][1] = (double)node->size();
			clustering[k][2] = (double)node->size()/n;
			clustering[k][3] = stdev(node->distances());
		}
	return clustering;
}

std::matrix<double> Grid::codebook()
{
	// Returns the list of 1,...,k weight vectors
	std::matrix<double> codebook(rows()*columns(), dimension());
	for(unsigned i = 0; i < rows(); ++i)
		for(unsigned j = 0; j < columns(); ++j)
		{
			unsigned k = i*columns() + j;
			for(unsigned d = 0; d < dimension(); ++d)	
				codebook[k][d] = (*this)[i][j].v()[d];
		}
	return codebook;
}

std::vector<double> Grid::histogram()
{
	// Returns a vector containing the neuron hits n(1), ..., n(k)
	std::vector<double> histogram;
	histogram.reserve(rows()*columns());
	for(unsigned i = 0; i < rows(); ++i)
		for(unsigned j = 0; j < columns(); ++j)
			histogram.push_back((*this)[i][j].size());
	return histogram;
}

std::matrix<double> Grid::membership()
{
	// Returns a table containing the assignment of data
	// from the most recent query to the grid, including
	// cluster assignment and separation
	// Index, Object, Distance
	std::matrix<double> membership(objectCount(), 3);
	unsigned  object = 0;
	for(unsigned i = 0; i < rows(); ++i)
		for(unsigned j = 0; j < columns(); ++j)
		{
			unsigned c = i*columns() + j;
			const Neuron& neuron = (*this)[i][j];
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

const unsigned Grid::objectCount() const
{
	unsigned objectCount = 0;
	for(unsigned i = 0; i < rows(); ++i)
		for(unsigned j = 0; j < columns(); ++j)
			objectCount += (*this)[i][j].size();
	return objectCount;
}

} // namespace som

#endif // ann_som_Grid_h
