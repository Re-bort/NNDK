#include <iostream>
#include <fstream>
#include <util/Command.h>
#include <data/Somplex.h>
#include <data/Dataset.h>
#include <data/Scaler.h>
#include <data/Standardiser.h>

using namespace std;

int main(int argc, char ** argv)
{
	// Get the command arguments
    Command command(argc, argv);

	// Check no. of mandatory arguments (6)
    if(command.size() < 6)
    {
        cerr << "sample: Must specify 6 filenames: data, codebook, training, testing, validation, and summary report." 
             << endl;
        return 1;
    }
    
    // Seed the PRNG
    unsigned seed;
    if(command.find("-r") != command.end())
    {    
        command.get("-r", seed);
    } 
    else 
    {
        seed = 0;
    }
    
    rng::seed(seed);

	// Get the sampled database
    Dataset data;
    data.read(command.arg(0));
    
    // Include output variable in clustering
	if(command.find("-y") != command.end())
      data.X.push_backc(data.y);
   
    // Read SOM codebook vectors from file
    matrix<double> codebook;
    ifstream ifs(command.arg(1).c_str(), ios::in);
    ifs >> codebook;
    ifs.close();
    
    // Get or set SOM map dimensions
    unsigned m;
    if(command.find("-m") != command.end())
    	command.get("-m", m);
	else
		m = 1;
	
	unsigned n;
	if(command.find("-n") != command.end())
		command.get("-n", n);
	else
		n = codebook.rows();
		
	// NOTE: dimensions of SOM are not important for sampling and a simple 
    // 1 x k map is used as default, where k is the number of codebook vectors.

	// Standardise data and codebook
	//Scaler encoder(data.X, -0.5, 0.5);
	//encoder.encode(data.X);
	//encoder.encode(codebook);
	
	// NOTE: Intra-cluster distance calculations used by Duplex sampling 
	// from map units requires standardised data.
    
    // Partition clustered database on the SOM to obtain memberships
    Som som(data.X, m, n, codebook);
	som.query(data.X);
	
	//matrix<double> histogram(m, n);
	//histogram = som.histogram();
	//std::cout << histogram << std::endl;
    
    // Create instance of Somplex algorithm
    Somplex sampler(data, som);
    
    // Get sampling options from command arguments
    if(command.find("-T") != command.end())
    {
        double T;
        command.get("-T", T);
        sampler.setTrainingFraction(T);
    }

    if(command.find("-t") != command.end())
    {
        double t;
        command.get("-t", t);
        sampler.setTestingFraction(t);
    }
    
 	sampler.resample();
 	//encoder.decode(data.X);
	//encoder.decode(codebook);
	
	
	// KLUDGE: If y was appended X in the data for clustering purposes then
 	// this column needs to be removed from each dataset
 	if(command.find("-y") != command.end())
 	{
		data.X.pop_backc();
		data.X.header.pop_back();
 	}
 	
 	//sampler.setDataset(data);
 	sampler.allocate(data);
 	
 	Dataset training, testing, validating;
 	training = sampler.training();
 	testing = sampler.testing();
 	validating = sampler.validating();
 	
 	training.X.header.pop_back();
 	testing.X.header.pop_back();
 	validating.X.header.pop_back();
	
    ofstream fout(command.arg(5).c_str(), ios::out);
    fout << "Sampling: SOMPLEX" << endl;
	fout << "Training frac.:\t" << sampler.trainingFraction() << endl;
	fout << "Testing frac.:\t" << sampler.testingFraction() << endl;
	fout << "Validating frac.:\t" << sampler.validatingFraction() << endl;
	fout << "\nTraining\n" << sampler.training().statistics() << endl;
	fout << "\n\nTesting\n" << sampler.testing().statistics() << endl;
	fout << "\nValidating\n" << sampler.validating().statistics() << endl;
	fout.close();
    
    // Optional (verbose) output
    if(command.find("-v") != command.end())
    {
   		cout << "Sampling: SOMPLEX" << endl;
      	cout << "Training frac.:\t" << sampler.trainingFraction() << endl;
		cout << "Testing frac.:\t" << sampler.testingFraction() << endl;
		cout << "Validating frac.:\t" << sampler.validatingFraction() << endl;
		cout << "\nTraining\n" << sampler.training().statistics() << endl;
		cout << "\n\nTesting\n" << sampler.testing().statistics() << endl;
		cout << "\nValidating\n" << sampler.validating().statistics() << endl;
    }

    // Write training and testing sample data files
	training.write(command.arg(2));
    testing.write(command.arg(3));
    validating.write(command.arg(4));

    return 0;
}




