#include <iostream>
#include <fstream>
#include <sstream>
#include <util/Command.h>
#include <table>

using namespace std;

#define DEFAULT_ORDER 1
#define DEFAULT_HORIZON 0
#define DEFAULT_LAGSUFFIX ".lag"
#define DEFAULT_LEADSUFFIX ".plus"

int main(int argc, char ** argv)
{
	Command command(argc, argv);
	
	// Create instance of a table and read in from file
	table<double> unlagged;
	ifstream fin(command.arg(0).c_str(), ios::in);
	fin >> unlagged;
	fin.close();  
	
	// Get maximum delay specified from command line option -d?
	unsigned d = DEFAULT_ORDER;	
	if(command.find("-d") != command.end())
	{
		command.get("-d", d);
	}
	
	unsigned h = DEFAULT_HORIZON;
	if(command.find("-h") != command.end())
	{
		command.get("-h", h);
	}
	
	string lagSuffix = DEFAULT_LAGSUFFIX;
	if(command.find("-s") != command.end())
	{
		command.get("-s", lagSuffix);
	}
	
	string leadSuffix = DEFAULT_LEADSUFFIX;
	if(command.find("-t") != command.end())
	{
		command.get("-t", leadSuffix);
	}
	
	bool keepOutputColumn = false;
	if(command.find("-y") != command.end())
	{
		keepOutputColumn = true;	
	}
	
	table<double> lagged;
	if(unlagged.rows() - (d + h) > 1)
	{
		lagged.resize(unlagged.rows() - (d + h), unlagged.columns() * (d + 1));
		for(unsigned i = 0; i < lagged.rows(); ++i)
		{
			lagged.index[i] = unlagged.index[i + d];
			for(unsigned j = 0; j < unlagged.columns(); ++j)
				for(unsigned k = 0; k <= d; ++k) 
					lagged[i][((d + 1) * j) + k] = unlagged[i + d - k][j];
		}
		
		// Add labels to columns in lagged table
		lagged.index.label = unlagged.index.label;
		for(unsigned j = 0; j < unlagged.columns(); ++j)
		{
			string label = unlagged.column(j).label;
			lagged.header[(d + 1) * j] = label;
			for(unsigned k = 1; k <= d; ++k)
			{
				stringstream ss;
				ss << label << lagSuffix << k;
				lagged.header[((d + 1) * j) + k] = ss.str();
			}
		}
	}
	
	// Keep last column in unlagged table as last column in lagged table
	if(keepOutputColumn)
	{
		// Get column index in lagged table corresponding to last column in 
		// the unlagged table
		unsigned c = (unlagged.columns() - 1) * d;
		
		// Erase column from lagged table
		lagged.erasec(c);
		
		// Append last column from unlagged table to lagged table
		series<double> output = unlagged.column(unlagged.columns() - 1);
		lagged.push_backc(output);
	}
	
	// Generate forecast series x(t + h) using final column as x(t)
	if(h > 0)
	{
		unsigned c = unlagged.columns() - 1;
		
		stringstream ss;
		ss << unlagged.header[c] << leadSuffix << h;
		
		series<double> forecast(ss.str(), lagged.rows());
		for(unsigned i = 0; i < lagged.rows(); ++i)
			forecast[i] = unlagged[i + d + h][c];
			
		lagged.push_backc(forecast);
	}
	
	
	
	ofstream fout(command.arg(1).c_str(), ios::out);
	fout << lagged << endl;
	fout.close();
		
	return 0;
}
