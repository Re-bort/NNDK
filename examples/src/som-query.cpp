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

    // Read dataset from file
   	Dataset dataset;
	dataset.read(command.arg(0));

    // Add output variable if to be included in analysis.
    matrix<double> X(dataset.X);
    if(command.find("-y") != command.end())
    {
    	X.push_backc(dataset.y);
    }

    // Read codebook from file
    matrix<double> codebook;
    ifstream ifs(command.arg(1).c_str(), ios::in);
    ifs >> codebook;
    ifs.close();
    
    unsigned m, n;    
    if(command.find("-m") != command.end() && 
    	command.find("-n") != command.end())
    {
    	command.get("-m", m);
    	command.get("-n", n);
    }
    else
    {
    	m = 1; 
    	n = codebook.rows();
    }
   		
    Som som(X, m, n, codebook);
    som.query(X);
    
    if(command.find("-v") != command.end())
	    cout << command.arg(0) << endl;
    
    if(command.find("-H") != command.end())
        cout << som.histogram() << endl;
    else if(command.find("-N") != command.end())
    	cout << som.neyman() << endl;
    else if(command.find("-D") != command.end())
    	cout << som.stdevs() << endl;
    else if(command.find("-M") != command.end())
    	cout << som.membership() << endl;
    else if(command.find("-S") != command.end())
	{	
    	unsigned s = 8;
    	command.get("-S", s);
    	cout << som.sd_histogram(s) << endl;
	}
    else
    	cout << som << endl;

    return 0;
}




