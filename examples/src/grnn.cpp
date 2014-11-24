#include <iostream>

#include <util/Command.h>
#include <fstream>
#include <ann/Grnn.h>
#include <data/Dataset.h>
#include <data/Standardiser.h>

using namespace std;

void usage();

int main(int argc, char ** argv)
{
    Command commandLine(argc, argv);
    
    if (commandLine.size() < 4)
    {
        usage();
        return 1;
    }
    
    if(commandLine.find("-v") != commandLine.end())
    	cout << "\ngrnn: " << commandLine.arg(0) << endl;
    
    Dataset training, testing, validating;
    training.read(commandLine.arg(0));
    testing.read(commandLine.arg(1));
    validating.read(commandLine.arg(2));
    
    Standardiser encoder(training.X);
    encoder.encode(training.X);
    encoder.encode(testing.X);
    encoder.encode(validating.X);
   
    Grnn grnn(training.X, training.y);
    if(commandLine.find("-h") != commandLine.end())
    {
        double h;
        commandLine.get("-h", h);
        grnn.setBandwidth(h);
    }
    else 
    {
        Grnn::Trainer trainer(testing.X, testing.y);
        trainer.train(grnn);
    }
    
    series<double> m_training(training.y.label, grnn(training.X));
    series<double> m_testing(testing.y.label, grnn(testing.X));
    series<double> m_validating(validating.y.label, grnn(validating.X));

	// Write output summary file
    ofstream fout(commandLine.arg(3).c_str(), ios::out);
	fout << "Bandwidth:\t" << grnn.bandwidth() << '\n' << endl;
	fout << "Dataset\tMSE\tMAE\tR\n";
    fout << "Trg\t" 
		<< mse(training.y, m_training) << '\t'
		<< mae(training.y, m_training) << '\t'
      	<< pearson(training.y, m_training) << endl;
	fout << "Tst\t\t" 
      	<< mse(testing.y, m_testing) << '\t'
      	<< mae(testing.y, m_testing) << '\t'
      	<< pearson(testing.y, m_testing) << endl;
    fout << "Vld\t" 
		<< mse(validating.y, m_validating) << '\t'
		<< mae(validating.y, m_validating) << '\t'
		<< pearson(validating.y, m_validating) << endl;
	fout.close();
	
	if(commandLine.find("-v") != commandLine.end())
    {
    	fout << "Bandwidth:\t" << grnn.bandwidth() << '\n' << endl;
    	cout << "Dataset\tMSE\tMAE\tR\n";
    	cout << "Trg\t" 
      		<< mse(training.y, m_training) << '\t'
      		<< mae(training.y, m_training) << '\t'
      		<< pearson(training.y, m_training) << endl;
    	cout << "Tst\t\t" 
      		<< mse(testing.y, m_testing) << '\t'
      		<< mae(testing.y, m_testing) << '\t'
      		<< pearson(testing.y, m_testing) << endl;
    	cout << "Vld\t" 
      		<< mse(validating.y, m_validating) << '\t'
      		<< mae(validating.y, m_validating) << '\t'
      		<< pearson(validating.y, m_validating) << endl;
	}
	
	if(commandLine.find("-w") != commandLine.end())
    {
        training.y = m_training;
        encoder.decode(training.X);
        training.write(commandLine.arg(0));
        
        testing.y = m_testing;
        encoder.decode(testing.X);
        testing.write(commandLine.arg(1));
        
        validating.y = m_validating;
        encoder.decode(validating.X);
        validating.write(commandLine.arg(2));
    }
	
    return 0;
}

void usage()
{
    std::cerr << "Usage: grnn <training> <testing> <validating> <summary>\n";
}



