//! \file automlp.cpp

//! \brief Implements and MLP training with cross-validation, and optimisation of hidden-layer size
//! according to the Akaike Information Criterion (AIC)

#include <iostream>
#include <fstream>

#include <util/Command.h>
#include <ann/Mlp.h>
#include <ann/Neuron.h>
#include <ann/Backpropagation.h>
#include <data/Dataset.h>
#include <data/Scaler.h>
#include <math/Statistics.h>

using namespace std;

void usage();

int main(int argc, char ** argv) {
	Command command(argc, argv);

	if (command.size() < 4) {
		usage();
		return 1;
	}

	rng::seed();

	Dataset training, testing, validating;
	training.read(command.arg(0));
	testing.read(command.arg(1));
	validating.read(command.arg(2));

	NeuronType neuronType = LogisticFunc;
	if (command.find("-n") != command.end()) {
		char flag;
		command.get("-n", flag);
		switch (flag) {
		case 'H':	// Hyperbolic tangent
			neuronType = TanhFunc;
			break;

		case 'L':	// Logistic (sigmoidal)
			// Fall through

		default:
			neuronType = LogisticFunc;
			break;
		}
	}

	// Get MLP properties specified at command-line
	unsigned long max_hidden_nodes = 5;
	if (command.find("-h") != command.end()) {
		command.get("-h", max_hidden_nodes);
	}

	Mlp network(training.X.columns(), 1, neuronType);

	Backpropagation trainer;
	// Apply any BPA properties specified by command-line arguments
	if (command.find("-l") != command.end()) {
		double learningRate;
		command.get("-l", learningRate);
		trainer.learningRate = learningRate;
	}

	if (command.find("-m") != command.end()) {
		double momentum;
		command.get("-m", momentum);
		trainer.momentum = momentum;
	}

	if (command.find("-t") != command.end()) {
		double tolerance;
		command.get("-t", tolerance);
		trainer.tolerance = tolerance;
	}

	if (command.find("-i") != command.end()) {
		unsigned long maxIterations;
		command.get("-i", maxIterations);
		trainer.maxIterations = maxIterations;
	}

	if (command.find("-e") != command.end()) {
		unsigned long maxEpochs;
		command.get("-e", maxEpochs);
		trainer.maxEpochs = maxEpochs;
	}

	// Scale data depending on transfer function
	double minInput, maxInput;
	switch (network.neuronType) {
	case TanhFunc:
		minInput = -0.8;
		maxInput = 0.8;
		break;

	case LogisticFunc:
		// Fallthrough

	default:
		minInput = 0.2;
		maxInput = 0.8;
		break;
	}
	double minInitialWeight = (maxInput - minInput) * 0.4 + minInput;
	double maxInitialWeight = (maxInput - minInput) * 0.6 + minInput;

	Scaler encoder(training.X, minInput, maxInput);
	encoder.encode(training.X);
	encoder.encode(testing.X);
	encoder.encode(validating.X);

	std::vector<double> mse_history(max_hidden_nodes, 0.0);
	std::vector<double> aic_history(max_hidden_nodes, 0.0);
	std::vector<long> k_history(max_hidden_nodes, 0);
	double min_aic = large();
	long int optimum_num_hidden_nodes = 1;
	for(unsigned long j = 0; j < max_hidden_nodes; ++j) {
		network.setHiddenLayerSize(j + 1);
		network.initialiseWeights(minInitialWeight, maxInitialWeight);
		trainer.initialise(network);
		trainer.trainOnlineCV(network, training.X, training.y, testing.X, testing.y);
		std::vector<double> testing_outputs = network(testing.X);
		long int num_parameters = network.W.rows() * network.W.columns() + network.biasW.size() + network.V.size() + 1;
		k_history[j] = num_parameters;
		mse_history[j] = mse(testing_outputs, testing.y);
		aic_history[j] = aic(testing_outputs, testing.y, num_parameters);
		if (aic_history[j] < min_aic) {
			min_aic = aic_history[j];
			optimum_num_hidden_nodes = j + 1;
		}
	}

	network.setHiddenLayerSize(optimum_num_hidden_nodes);
	network.initialiseWeights(minInitialWeight, maxInitialWeight);
	trainer.initialise(network);
	trainer.trainOnlineCV(network, training.X, training.y, testing.X, testing.y);

	series<double> m_training(training.y.label + "'", network(training.X));
	series<double> m_testing(testing.y.label + "'", network(testing.X));
	series<double> m_validating(validating.y.label + "'",
			network(validating.X));

	// Write output summary file
	ofstream fout(command.arg(3).c_str(), ios::out);
	fout << "Dataset\tMSE\tMAE\tR^2\n";
	fout << "Training\t" << mse(training.y, m_training) << '\t'
			<< mae(training.y, m_training) << '\t'
			<< pearson(training.y, m_training) << endl;
	fout << "Testing\t\t" << mse(testing.y, m_testing) << '\t'
			<< mae(testing.y, m_testing) << '\t'
			<< pearson(testing.y, m_testing) << endl;
	fout << "Validating\t" << mse(validating.y, m_validating) << '\t'
			<< mae(validating.y, m_validating) << '\t'
			<< pearson(validating.y, m_validating) << endl;
	fout << "Network: " << endl;
	fout << "W:\n" << network.W << endl;
	fout << "Bias0:\n" << network.biasW << endl;
	fout << "V:\n" << network.V << endl;
	fout << "Bias1:\n" << network.biasV << endl;

	fout << "\nAIC Evaluation";
	fout << "\nH\tMSE\tK\tAIC(H)\n";
	for(unsigned int j = 0; j < aic_history.size(); ++j) {
		fout << j + 1
			<< "\t" << mse_history[j]
			<< "\t" << k_history[j]
			<< "\t" << aic_history[j] << '\n';
	}
	fout << "\nMin. AIC:\t" << min_aic;
	fout << "\nOptimum size:\t" << optimum_num_hidden_nodes;
	fout.close();

	if (command.find("-v") != command.end()) {
		cout << "Network: " << endl;
		cout << "W:\n" << network.W << endl;
		cout << "Bias0:\n" << network.biasW << endl;
		cout << "V:\n" << network.V << endl;
		cout << "Bias1:\n" << network.biasV << endl;
		cout << "Dataset\tMSE\tMAE\tR^2\n";
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
	
	if (command.find("-w") != command.end()) {
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

void usage() {
	std::cerr << "Usage: mlp.exe <training> <testing> <validating> <summary>\n";
}

