#include <iostream>
#include <fstream>
#include <util/Command.h>
#include <data/DuplexSampler.h>
#include <data/Standardiser.h>
#include <table>
#include <random>

using namespace std;

double evaluate(const Dataset& training, const Dataset& test, const Dataset& validation);

int main(int argc, char ** argv)
{
    Command command(argc, argv);
    
    // Check no. of mandatory arguments (3)
    if(command.size() < 4)
    {
        cerr << "sample: Must specify 3 filenames: data, training, testing, and validation." 
             << endl;
        return 1;
    }
        
	Dataset dataset;
    dataset.read(command.arg(0));
    
    Sampler * sampler;
    sampler = new DuplexSampler(dataset);
    sampler->setTrainingFraction(0.50);
    sampler->setTestingFraction(0.25);
     
    // Parse any optional command arguments
	// Parse optional training fraction argument
    if(command.find("-T") != command.end())
    {
		double T;
        command.get("-T", T);
        sampler->setTrainingFraction(T);
    }
    
    if(command.find("-t") != command.end())
    {
		double t;
		command.get("-t", t);
		sampler->setTestingFraction(t);
	}
    
	sampler->resample();
    sampler->training().write(command.arg(1));
    sampler->testing().write(command.arg(2));
    sampler->validating().write(command.arg(3));
    
    // Optional (verbose) output
    if(command.find("-v") != command.end())
    {
        cout << "duplex.exe: " << command.arg(0);
        cout << "n_T :\t" << sampler->trainingSampleSize() << endl;
        cout << "n_S :\t" << sampler->testingSampleSize() << endl;
        cout << "n_V :\t" << sampler->validatingSampleSize() << endl;
        cout << "\nTraining\n" << sampler->training().statistics() << endl;
        cout << "\n\nTesting\n" << sampler->testing().statistics() << endl;
        cout << "\nValidating\n" << sampler->validating().statistics() << endl;
    }
    return 0;
}





