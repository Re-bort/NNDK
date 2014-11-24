#include <iostream>
#include <fstream>
#include <util/Command.h>
#include <data/Standardiser.h>
#include <data/Scaler.h>
#include <table>

using namespace std;

int main(int argc, char ** argv)
{
	// Get the command arguments
    Command command(argc, argv);

	// Check no. of mandatory arguments (2)
    if(command.size() < 2)
    {
        cerr << "sample: Must specify 2 filenames: data, scaled data" 
             << endl;
        return 1;
    }
    
	table<double> data;
	ifstream fin(command.arg(0).c_str(), ios::in);
	fin >> data;
	fin.close();
	 
	Encoder * encoder;
	
	if(command.find("-s") != command.end())
	{
		double a = 0.0;
		if(command.find("-a") != command.end())
			command.get("-a", a);
		
		double b = 0.0;
		if(command.find("-b") != command.end())
			command.get("-b", b);
		
		encoder = new Scaler(data, a, b);
	}
	else 
	{
		encoder = new Standardiser(data);
	}
	
	encoder->encode(data);
	
	ofstream fout(command.arg(1).c_str(), ios::out);
	fout << data << endl;
	fout.close();
	
 	return 0;   
}
