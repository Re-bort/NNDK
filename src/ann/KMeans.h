#ifndef KMeans_h
#define KMeans_h

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

#define	VECTOR std::vector<double>
#define MATRIX std::matrix<double>
#define KEY	std::vector<unsigned>

class KMeans
{
	public:
		class Cluster;
		
    public:
								KMeans(const MATRIX& X);
								KMeans(const MATRIX& X, 
                                  const unsigned means);
                                KMeans(const MATRIX& X,
							      const MATRIX& codebook);
					         	KMeans(const KMeans& kmeans);
								~KMeans();
		
		// KMeans access methods
		Cluster& 				operator [] (const unsigned i);
        const unsigned			dimension(void) const;

        Cluster&				bmc(void);
        const unsigned			bmcIndex(void) const;
		const Cluster&          cluster(const unsigned i) const;
		
		const unsigned			K() const;
		void					setK(unsigned k);

        // Algorithm methods
        void					initialise(void);
        void                    randomise(void);
        void 					clear(void);
        void                    orderClustersByDistance(void);
        void                    orderClustersByIndex(void);
        void                    shuffle();
        void                    query(const MATRIX& X);
        const double            query(const VECTOR& v);
        
        const double&			learningRate() const;
        const unsigned&			epochs() const;
        void					setLearningRate(double learningRate);
        void					setEpochs(unsigned epochs);
        void                    setDefaultParameters(void);
        void                    setDefaultSize(void);
        void                    setX(const MATRIX& X);
       
        void                    train(void);
        void                    train(const double& learningRate, 
                                  const unsigned& epochs);
        // Reporting methods
        const double			ssintra() const;
		VECTOR					frequency() const;
		VECTOR					histogram() const;        
		MATRIX		            clustering(void) const;
		MATRIX		            membership(void) const;
		MATRIX		            codebook(void) const;
		
    protected:
                                KMeans(){}    
        
    private:
		const MATRIX *          X_;
        std::vector<Cluster>	clusters_;
        unsigned				K_;
        double					learningRate_;
        unsigned				epochs_;
        
        const double            (* distance)(const VECTOR& u, const VECTOR& v);
        		
		unsigned				bmcIndex_;
                                
        void                    redim(unsigned dimension);
        void					updateCentroid(Cluster& cluster);

    friend std::ostream& operator << (std::ostream& os, const KMeans& kmeans);    
    friend class KMplex;    

};

class KMeans::Cluster
{
    friend class KMeans;
    public:
                                Cluster();
                                Cluster(const Cluster& cluster);
                                Cluster(const unsigned dimension);
                                Cluster(const VECTOR& v);
                                ~Cluster();
        // Access methods        
        VECTOR&					m();
        const double            distance(unsigned i) const;
        const VECTOR&           distances() const;
        const bool              empty(void) const;
        const unsigned          index(unsigned i) const;
        const KEY&              indices(void) const;
        const bool              singleton(void) const;      
        const unsigned          size(void) const;

        // Mutate methods
        void                    clear(void);
        void                    push_back(const unsigned i, const double d);
        
    private:
        VECTOR					m_;
        KEY                     indices_;
        VECTOR                  distances_;
};

#undef VECTOR
#undef MATRIX
#undef KEY

#endif // KMeans_h
