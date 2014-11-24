#include <iostream>
#include <fstream>
#include <util/Command.h>
#include <ann/Som.h>
#include <data/Dataset.h>
#include <data/Scaler.h>

using namespace std;

int main(int argc, char ** argv)
{
    Command command(argc, argv);
    
    // Check no. of mandatory arguments (3)
    if(command.size() < 3) 
    {
        cerr << "som-train: Insufficient arguments.\n"
            << "Usage: som-train [data] [codebook] [histogram]" << endl;
        return 1;
    }
    
	Dataset dataset;
	dataset.read(command.arg(0));
	
	matrix<double> X = dataset.X;
	
    // Option to include output variable in clustering
    if(command.find("-y") != command.end())
        X.push_backc(dataset.y);
        
    // Transform variables onto [-0.5, 0.5]
    Scaler scaler(X, -0.5, 0.5);
    scaler.encode(X);
     
    // Create SOM using default options
    Som som(X);
    
    // Parse optional arguments
    // Optional SOM size 
    if(command.find("-m") != command.end())
    {
        unsigned m, n;
        command.get("-m", m);
        if(command.find("-n") != command.end())
            command.get("-n", n);
        else
            n = m;
        som.resize(m, n);
        som.setDefaultParameters();
    }
        
    // Optional SOM algorithm parameters
    double conscience, bias;
    if(command.find("-C") != command.end())
    {
    	command.get("-C", conscience);
      	som.setConscience(conscience);
    }
        
    if(command.find("-D") != command.end())
    {
    	command.get("-D", bias);
    	som.setBias(bias);
    } 	
       
    double rate;
	unsigned nhoodSize, epochs;
	if (command.find("-L") != command.end())
	{
		command.get("-L", rate);
        som.setOrderingRate(rate);
	}
    	
    if (command.find("-H") != command.end())
    {
        command.get("-H", nhoodSize);
        som.setOrderingNhoodSize(nhoodSize);
    }
    
    if (command.find("-E") != command.end())
    {
        command.get("-E", epochs);
        som.setOrderingEpochs(epochs);    
    }
    
    if (command.find("-l") != command.end())
    {
        command.get("-l", rate);
        som.setTuningRate(rate);
    }
    
    if(command.find("-h") != command.end())
    {
        command.get("-h", nhoodSize);
        som.setTuningNhoodSize(nhoodSize);
    }
    
    if(command.find("-e") != command.end())
    {
        command.get("-e", epochs);
        som.setTuningEpochs(epochs);
    }
    
    if(command.find("-r") != command.end())
    {
        unsigned seed;
        command.get("-r", seed);
        rng::seed(seed);
    }

	cout << "SOM Parameters\n";
	cout << "Map Dimension" << endl;
	cout << "m:\t" << som.rows();
	cout << "n:\t" << som.columns() << endl;
	cout << "Learning" << endl;
	cout << "C:\t" << som.conscience() << endl;
	cout << "b:\t" << som.bias() << endl;
	cout << "E:\t" << som.orderingEpochs() << endl;
	cout << "L:\t" << som.orderingRate() << endl;
	cout << "H:\t" << som.orderingNhoodSize() << endl;
	cout << "e:\t" << som.tuningEpochs() << endl;
	cout << "l:\t" << som.tuningRate() << endl;
	cout << "h:\t" << som.tuningNhoodSize() << endl;
		
    // Train the SOM weights using the current settings:
    som.initialise();
    som.train();
    som.query(X);
       
    // Decode codebook into original data coordinate space
    matrix<double> codebook = som.codebook();
    scaler.decode(codebook);
           
    // Write the SOM codebook (in restored scale) to a file
    ofstream ofs(command.arg(1).c_str(), ios::out);
    ofs << codebook << endl;
    ofs.close();
    
    // Write the SOM histogram to a file
    ofs.open(command.arg(2).c_str(), ios::out);
    ofs << som.histogram() << endl;
    ofs.close();
    
    // Optional (verbose) output to console
    if(command.find("-v") != command.end())
    {
        cout << som << endl;
        cout << "CV statistics\n" << "SS-intra:\t " << som.ssintra() << '\n'
        	<< endl;
    }
            
    return 0;
}



