#include <iostream>
#include <fstream>
#include <util/Command.h>
#include <data/Sampler.h>
#include <data/SystematicSampler.h>
#include <data/DuplexSampler.h>
#include <data/Standardiser.h>
#include <table>
#include <random>

using namespace std;

void write(Dataset& dataset, string filename);
void read(Dataset& dataset, string filename);

double evaluate(const Dataset& training, const Dataset& test, const Dataset& validation);

int main(int argc, char ** argv)
{
    Command command(argc, argv);
    
    // Check no. of mandatory arguments (3)
    if(command.size() < 5)
    {
        cerr << "sample: Must specify 5 filenames: data, training, testing, validation, and summary report." 
             << endl;
        return 1;
    }
        
    Dataset dataset;
    dataset.read(command.arg(0));
    
    Sampler * sampler;
    if(command.find("--systematic") != command.end())
       sampler = new SystematicSampler(dataset, dataset.y);
    else if(command.find("--duplex") != command.end())
       sampler = new DuplexSampler(dataset);
    else
       sampler = new Sampler(dataset);
    
    sampler->setTrainingFraction(0.50);
    sampler->setTestingFraction(0.25);
    sampler->setDataset(dataset);
     
    // Parse any optional command arguments
    if(command.size() > 5)
    {
        // Parse optional training fraction argument
        if(command.find("-T") != command.end())
        {
            double T;
            command.get("-T", T);
            sampler->setTrainingFraction(T);
        }
    
        // Parse optional sampling fraction argument
        if(command.find("-t") != command.end())
        {
            double t;
            command.get("-t", t);
            sampler->setTestingFraction(t);
        }
    
        // Parse optional random number seed argument
        if(command.find("-r") != command.end())
        {
            unsigned seed;
            command.get("-r", seed);
            rng::seed(seed);   
        }
        else rng::seed();
    }
    
    // Optional sample optimisation (simple random sampling only)
//    if(command.find("-o") != command.end() 
//    	&& command.find("--sys") == command.end())
//	{
//        unsigned iterations = 100;
//        command.get("-o", iterations);
//        vector<unsigned> seeds(iterations);
//        generate(seeds.begin(), seeds.end(), std::rand);
//        
//        double score, bestScore = 1000;
//        vector<unsigned>::iterator seed = seeds.begin(), bestSeed = seed;
//        for(; seed != seeds.end(); ++seed)
//        {
//            rng::seed(*seed);
//            sampler->resample();
//                       
//            score = evaluate(sampler->training(), sampler->testing(), 
//            	sampler->validating());
//            if(score < bestScore)
//            {
//                bestScore = score;
//                bestSeed = seed;
//            }
//        }
//        cout << "Seed: " << *bestSeed << endl;
//        rng::seed(*bestSeed);
//	}
    
    sampler->resample();
    sampler->training().write(command.arg(1));
    sampler->testing().write(command.arg(2));
    sampler->validating().write(command.arg(3));
    
    ofstream fout(command.arg(4).c_str(), ios::out);
    if(command.find("--systematic") != command.end())
       fout << "Method: Systematic" << endl;
    else if(command.find("--duplex") != command.end())
       fout << "Method: DUPLEX" << endl;
    else
       fout << "Method: Uniform Random" << endl;
    
	fout << "Training frac.:\t" << sampler->trainingFraction() << endl;
	fout << "Testing frac.:\t" << sampler->testingFraction() << endl;
	fout << "Validating frac.:\t" << sampler->validatingFraction() << endl;
	fout << "\nTraining\n" << sampler->training().statistics() << endl;
	fout << "\n\nTesting\n" << sampler->testing().statistics() << endl;
	fout << "\nValidating\n" << sampler->validating().statistics() << endl;
    fout.close();
    
    // Optional (verbose) output
    if(command.find("-v") != command.end())
    {
    	if(command.find("--systematic") != command.end())
       		cout << "Method: Systematic" << endl;
    	else if(command.find("--duplex") != command.end())
       		cout << "Method: DUPLEX" << endl;
    	else
       		cout << "Method: Uniform Random" << endl;
    
      	cout << "Training frac.:\t" << sampler->trainingFraction() << endl;
		cout << "Testing frac.:\t" << sampler->testingFraction() << endl;
		cout << "Validating frac.:\t" << sampler->validatingFraction() << endl;
		cout << "\nTraining\n" << sampler->training().statistics() << endl;
		cout << "\n\nTesting\n" << sampler->testing().statistics() << endl;
		cout << "\nValidating\n" << sampler->validating().statistics() << endl;
    }
    
    return 0;
}

double evaluate(const Dataset& training, const Dataset& test, const Dataset& validation)
{
	matrix<double> trainingStatistics = training.statistics();
	matrix<double> testStatistics = test.statistics();
	matrix<double> validationStatistics = validation.statistics();
	
	double sum = 0.0;
	for(unsigned j = 0; j < trainingStatistics.rows(); ++j)
	{
		double muTraining = trainingStatistics[j][2];
		double sigmaTraining = trainingStatistics[j][4];
		double muTest = testStatistics[j][2];
		double sigmaTest = testStatistics[j][4];
	
		sum += fabs(tstat(muTraining, sigmaTraining, training.length(),
			muTest, sigmaTest, test.length()));
			
        if(validation.length() > 0)
        {
        	double muValidation = trainingStatistics[j][2];
        	double sigmaValidation = validationStatistics[j][4];
        	
         	sum += fabs(tstat(muTraining, sigmaTraining, training.length(),
				muValidation, sigmaValidation, validation.length()));
			sum += fabs(tstat(muTraining, sigmaTraining, training.length(),
				muTest, sigmaTest, test.length()));  
        }		
	}		
	
	return sum;		
}




