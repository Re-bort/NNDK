#include <iostream>
#include <fstream>

#include <util/Command.h>
#include <ann/Mlp.h>
#include <ann/Neuron.h>
#include <ann/Backpropagation.h>
#include <data/Dataset.h>
#include <data/Scaler.h>

using namespace std;

void usage();

int main(int argc, char ** argv)
{
    Command command(argc, argv);
    
    if (command.size() < 4)
    {
        usage();
        return 1;
    }
    
    rng::seed();
    
    // Read in datasets
    Dataset training, testing, validating;
    training.read(command.arg(0));
    testing.read(command.arg(1));
    validating.read(command.arg(2));
    
    // Get optional MLP properties specified at command-line
	unsigned long numHiddenLayerNodes = 1;
	if(command.find("-h") != command.end())
	{
		command.get("-h", numHiddenLayerNodes);
	}

    NeuronType neuronType = LogisticFunc;
    if(command.find("-n") != command.end())
    {
   	    char flag;
    	command.get("-n", flag);
    	switch(flag)
    	{
    		case 'H':	// Hyperbolic tangent
    			neuronType = TanhFunc;
    			break;
    			
    		case 'T':	// Threshold
    			neuronType = ThresholdFunc;
    			break;
    			
    		case 'L':	// Logistic (sigmoidal)
    			// Fall through
    		
    		default:
    			neuronType = LogisticFunc;
    			break;
    	}
    }

    Mlp network(training.X.columns(), numHiddenLayerNodes, neuronType);

    Backpropagation trainer;
    // Get optional training parameters specified by command-line arguments
    if(command.find("-l") != command.end())
    {
    	double learningRate;
    	command.get("-l", learningRate);
    	trainer.learningRate = learningRate;
    }
    
    if(command.find("-m") != command.end())
    {
	    double momentum;    	
    	command.get("-m", momentum);
    	trainer.momentum = momentum;
    }
    
    if(command.find("-t") != command.end())
    {
   	    double tolerance;
    	command.get("-t", tolerance);
    	trainer.tolerance = tolerance;
    }
    
    if(command.find("-i") != command.end())
    {
   	    unsigned long maxIterations;
    	command.get("-i", maxIterations);
    	trainer.maxIterations = maxIterations;
    }
    
    if(command.find("-e") != command.end())  {
    	unsigned long maxEpochs;
    	command.get("-e", maxEpochs);	
    	trainer.maxEpochs = maxEpochs;
    }
    
    // Scale input data depending on transfer function
    double minInput, maxInput;
    switch(network.neuronType)
    {
    	case TanhFunc:
    		minInput = -0.8;
    		maxInput = 0.8;
    		break;
    		
    	case ThresholdFunc:
    		minInput = -1.0;
    		maxInput = 1.0;
    		break;
    		
    	case LogisticFunc:
    		// Fallthrough

    	default:
			minInput = 0.2;
    		maxInput = 0.8;
    		break;
    }

    Scaler encoder(training.X, minInput, maxInput);
    encoder.encode(training.X);
    encoder.encode(testing.X);
    encoder.encode(validating.X);
    
    // Initialise network with small weights
    network.initialiseWeights(minInput/10, maxInput/10);

    // Initialise training algorithm and train network
    trainer.initialise(network);
    trainer.trainOnlineCV(network, training.X, training.y, testing.X, testing.y);
    
    series<double> m_training(training.y.label + "'", network(training.X));
    series<double> m_testing(testing.y.label + "'", network(testing.X));
    series<double> m_validating(validating.y.label + "'", network(validating.X));

	// Write output summary file
    ofstream fout(command.arg(3).c_str(), ios::out);
	fout << "Dataset\tMSE\tMAE\tR2\n";
    fout << "Training\t" 
		<< mse(training.y, m_training) << '\t'
		<< mae(training.y, m_training) << '\t'
      	<< pearson(training.y, m_training) << endl;
	fout << "Testing\t\t" 
      	<< mse(testing.y, m_testing) << '\t'
      	<< mae(testing.y, m_testing) << '\t'
      	<< pearson(testing.y, m_testing) << endl;
    fout << "Validating\t" 
		<< mse(validating.y, m_validating) << '\t'
		<< mae(validating.y, m_validating) << '\t'
		<< pearson(validating.y, m_validating) << endl;
	fout << "Network: " << endl;
	fout << "W:\n" << network.W << endl;
	fout << "Bias0:\n" << network.biasW << endl;
	fout << "V:\n" << network.V << endl;
	fout << "Bias1:\n" << network.biasV << endl;
	fout.close();
	
	if(command.find("-v") != command.end())
    {
    	cout << "Network: " << endl;
      	cout << "W:\n" << network.W << endl;
      	cout << "Bias0:\n" << network.biasW << endl;
      	cout << "V:\n" << network.V << endl;
      	cout << "Bias1:\n" << network.biasV << endl;
      	
      	cout << "Dataset\tMSE\tMAE\tR2\n";
    	cout << "Training\t" 
      		<< mse(training.y, m_training) << '\t'
      		<< mae(training.y, m_training) << '\t'
      		<< pearson(training.y, m_training) << endl;
    	cout << "Testing\t\t" 
      		<< mse(testing.y, m_testing) << '\t'
      		<< mae(testing.y, m_testing) << '\t'
      		<< pearson(testing.y, m_testing) << endl;
    	cout << "Validating\t" 
      		<< mse(validating.y, m_validating) << '\t'
      		<< mae(validating.y, m_validating) << '\t'
      		<< pearson(validating.y, m_validating) << endl;
	}
	
	if(command.find("-w") != command.end())
    {
        training.y = m_training;
        encoder.decode(training.X);
        training.write(command.arg(0));
        
        testing.y = m_testing;
        encoder.decode(testing.X);
        testing.write(command.arg(1));
        
        validating.y = m_validating;
        encoder.decode(validating.X);
        validating.write(command.arg(2));
    }

    return 0;
}

void usage()
{
    std::cerr << "Usage: mlp.exe <training> <testing> <validating> <summary>\n";
}



