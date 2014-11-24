#include <iostream>
#include <fstream>
#include <util/Command.h>
#include <data/SomSampler.h>
#include <data/Dataset.h>
#include <data/Scaler.h>

using namespace std;

int main(int argc, char ** argv)
{
    Command command(argc, argv);

	if(command.size() < 6)
    {
        cerr << "sample: Must specify 5 filenames: data, codebook, training, testing, validation, log." 
             << endl;
        return 1;
    }

    unsigned seed;
    if(command.find("-r") != command.end())
    {    
        command.get("-r", seed);
    
    } else {
        seed = 0;
    }

    rng::seed(seed);

    Dataset data;
    data.read(command.arg(0));
 
    // Create matrix of data for SOM mapping based on inputs or inputs:output
    matrix<double> X(data.X);
	if(command.find("-y") != command.end())
    {
       X.push_backc(data.y);
    } 
       
    // Create a linear SOM based on pre-existing codebook
    matrix<double> codebook;
    ifstream ifs(command.arg(1).c_str(), ios::in);
    ifs >> codebook;
    ifs.close();
    Som som(X, 1, codebook.rows(), codebook);
    // Note: dimensions of SOM are not important for sampling and a simple 
    // 1 x k map is used instead, where k is the number of codebook vectors.

    // Map data onto the SOM to populate cells
    som.query(X);
    
    // Optional ordering of points by distance to codebook
    if(command.find("-o") != command.end())
        som.orderClustersByDistance();
    else
        som.shuffle();

    // Create an instance of a SOM-based sampler using SOM
    SomSampler sampler(data, som);

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
    
    if(command.find("-a") != command.end())
    {
    	unsigned a;
    	command.get("-a", a);	
    	sampler.setOutlierThreshold(a);
    }

	// Apply sample allocation rule: Proportional, Neyman, or single (default)
    if(command.find("-p") != command.end())
        sampler.resample(som.frequency());
    else if(command.find("-n") != command.end())
        sampler.resample(som.neyman());
    else
		sampler.resample();

    ofstream fout(command.arg(5).c_str(), ios::out);
    if(command.find("-p") != command.end())
    	fout << "Sampling: Random Proportional" << endl;
	else if(command.find("-n") != command.end())
        fout << "Sampling: Random Neyman" << endl;
	else if(command.find("-o") != command.end())
		fout << "Sampling: Single Nearest" << endl;  
	else if(command.find("-d") != command.end())
		fout << "Sampling: DUPLEX" << endl;
	else 
		fout << "Sampling: Single Random" << endl;
        
  	fout << "Training frac.:\t" << sampler.trainingFraction() << endl;
	fout << "Testing frac.:\t" << sampler.testingFraction() << endl;
	fout << "Validating frac.:\t" << sampler.validatingFraction() << endl;
	fout << "\nTraining\n" << sampler.training().statistics() << endl;
	fout << "\n\nTesting\n" << sampler.testing().statistics() << endl;
	fout << "\nValidating\n" << sampler.validating().statistics() << endl;
	fout << "SOM Frequency Matrix\n " << som.frequency();
	fout.close();
    
    // Optional (verbose) output
    if(command.find("-v") != command.end())
    {
    	if(command.find("-p") != command.end())
        	cout << "Sampling: Random Proportional" << endl;
   		else if(command.find("-n") != command.end())
	        cout << "Sampling: Random Neyman" << endl;
		else if(command.find("-o") != command.end())
			cout << "Sampling: Single Nearest" << endl;       
    	else
			cout << "Sampling: Single Random" << endl;
        
      	cout << "Training frac.:\t" << sampler.trainingFraction() << endl;
		cout << "Testing frac.:\t" << sampler.testingFraction() << endl;
		cout << "Validating frac.:\t" << sampler.validatingFraction() << endl;
		cout << "\nTraining\n" << sampler.training().statistics() << endl;
		cout << "\n\nTesting\n" << sampler.testing().statistics() << endl;
		cout << "\nValidating\n" << sampler.validating().statistics() << endl;
    }

    // Write training and testing sample data files
    sampler.training().write(command.arg(2));
    sampler.testing().write(command.arg(3));
    sampler.validating().write(command.arg(4));

    return 0;
}




