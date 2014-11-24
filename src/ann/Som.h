#ifndef	Som_h
#define Som_h

#include <iostream>
#include <iomanip>

#include "../matrix"
#include "../math/vecops.h"
#include "../math/constants.h"
#include "../math/statistics.h"
#include "../math/sequence.h"
#include "../algo/sort.h"
#include "../rng/Uniform.h"
#include "../rng/vector.h"
#include "../util/matio.h"

#define MATRIX std::matrix<double>
#define VECTOR std::vector<double>
#define KEY  std::vector<unsigned>

class Som
{
    public:
		class Neuron;	

    public:
								Som(const MATRIX& X);
								Som(const MATRIX& X, 
                                  const unsigned rows, 
                                  const unsigned columns);
							    Som(const MATRIX& X,
                                  const unsigned rows, 
                                  const unsigned columns,
							      const MATRIX& codebook);
					         	Som(const Som& som);
								~Som();
		
		// Som access methods
		std::vector<Neuron>&    operator [] (const unsigned i);
        const unsigned          columns(void) const;
        const unsigned			dimension(void) const;
        Neuron&					bmu(void);
        const unsigned          bmuRow(void) const;
        const unsigned          bmuColumn(void) const;
		const Neuron&           neuron(const unsigned i, const unsigned j) const;
		void					resize(unsigned rows, unsigned columns);
        const unsigned          rows(void) const;
        // Algorithm methods
        void                    autosize(unsigned n, unsigned k = 1);
        void 					clear(void);
		void                    order(void);
        void                    orderClustersByDistance(void);
        void                    orderClustersByIndex(void);
        void                    shuffle();
        const double&           orderingRate(void) const;
        const unsigned&         orderingNhoodSize(void) const;
        const unsigned&         orderingEpochs(void) const;
        void                    query(const MATRIX& X);
        const double            query(const VECTOR& v);
        void					initialise(void);
        void                    randomise(void);
        void                    setDefaultParameters(void);
        void                    setDefaultSize(void);
        void                    setOrderingRate(const double& rate);
        void                    setOrderingNhoodSize(const unsigned& nhoodSize);
        void                    setOrderingEpochs(const unsigned& epochs);
        void                    setTuningRate(const double& rate);
        void                    setTuningNhoodSize(const unsigned& nhoodSize);
        void                    setTuningEpochs(const unsigned& epochs);
        void                    setX(const MATRIX& X);
       
       	const double&			conscience() const;
       	const double&			bias() const;
      	
       	void					setBias(const double& bias);
        void					setConscience(const double& conscience);
        void					updateBias();
       
        void                    tune(void);
        const double&           tuningRate(void) const;
        const unsigned&         tuningNhoodSize(void) const;
        const unsigned&         tuningEpochs(void) const;
        
        void                    train(void);
        void                    train(const double& learningRate, 
                                  const unsigned& nhoodSize,
                                  const unsigned& epochs);
        // Reporting methods
		MATRIX		            clustering(void) const;
		MATRIX		            codebook(void) const;
		MATRIX		            membership(void)const;
		const unsigned			objectCount(void) const;
        
        const MATRIX            histogram(void) const;
        const MATRIX            frequency(void) const;
        const MATRIX            neyman(void) const;
        const MATRIX 			stdevs() const;
        const MATRIX            sd_histogram(const unsigned smoothness) const;
        const double            ssintra(void) const;

        const std::matrix<unsigned>
                                coincidence() const;
        const double			silhouette(std::matrix<double>& D);
        
    protected:
                                Som(){}    
        
    private:
		const MATRIX *          X_;
        std::matrix<Neuron>		grid_;
        const double            (* distance)(const VECTOR& u, const VECTOR& v);
        double                  orderingRate_;
        unsigned                orderingNhoodSize_;
        unsigned                orderingEpochs_;
        double                  tuningRate_;
        unsigned                tuningNhoodSize_;
        unsigned                tuningEpochs_;
		
		double					conscience_;
		double					bias_;
		
		unsigned				bmuRow_;
        unsigned                bmuColumn_;
        double                  tolerance_;
                                
        void                    redim(unsigned dimension);

    friend std::ostream& operator << (std::ostream& os, const Som& som);    
    friend class Somplex;    
};

class Som::Neuron
    : public VECTOR
{
    friend class Som;
    public:
                                Neuron();
                                Neuron(const Neuron& neuron);
                                Neuron(const unsigned dimension);
                                Neuron(const VECTOR& v);
                                ~Neuron();
        // Access methods        
        const double            distance(unsigned i) const;
        const VECTOR&           distances() const;
        const bool              empty(void) const;
        const unsigned          index(unsigned i) const;
        const KEY&              indices(void) const;
        const bool              singleton(void) const;      
        const unsigned          size(void) const;

        // Mutate methods
        void                    clear(void);
        void                    push_back(const unsigned i, 
                                          const double d);
        
    private:
        KEY                     indices_;
        VECTOR                  distances_;
        double					bias_;
};

std::ostream& operator << (std::ostream& os, const Som& som);

#undef MATRIX
#undef VECTOR
#undef KEY

#endif // ann_som_Som_h
