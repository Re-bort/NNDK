#include <iostream>
#include <algorithm>
#include <fstream>
#include <util/Command.h>
#include <kde/Pdf.h>
#include <kde/Regression.h>
#include <data/PmisFilter.h>
#include <data/Standardiser.h>
#include <util/matio.h>
#include <data/Graph.h>

using namespace std;

// --- Data tables for MCS-derived critical values of MI

unsigned int mcs_n[] =  {50,		60,		70,
			80,		90,		100,
			120,	140,	160,
			180,	200,	220,
			240,	260,	280,
			300,	400,	500,
			600,	700,	800,
			900,	1000,	2000,
			3000,	4000,	5000};
	
double mcs_i90[] = {0.198995,	0.182501,	0.169432,
			0.15921,	0.150562,	0.14285,
			0.130937,	0.121069,	0.113837,
			0.107224,	0.101882,	0.0974926,
			0.0931639,	0.0894051,	0.0861965,
			0.0834157,	0.072391,	0.0645754,
			0.0589085,	0.054412,	0.0509089,
			0.0479442,	0.0455432,	0.0317976,
			0.0257284,	0.0220921,	0.0196341 };
		
double mcs_i95[] = {0.222379, 	0.20309, 	0.187934,
			0.175586, 	0.165689, 	0.157189,
			0.143381, 	0.132078, 	0.123731,
			0.11655, 	0.110336, 	0.105498,
			0.100506, 	0.0964719, 	0.0928288,
			0.0896203,	0.0774698, 	0.0688698,
			0.0627355, 	0.0577663, 	0.0538687,
			0.0507436, 	0.0481287, 	0.033295,
			0.0267935, 	0.0229649, 	0.0203884};
		
		
double mcs_i99[] = {0.270473, 	0.245216, 	0.225438,
			0.209143, 	0.197278, 	0.185844,
			0.16882, 	0.154589, 	0.144381,
			0.135554, 	0.127576, 	0.121535,
			0.115796, 	0.110845, 	0.106182,
			0.102211, 	0.0875737, 	0.0774688,
			0.0701649, 	0.0644399, 	0.0597487,
			0.0562651, 	0.0531293, 	0.0361493,
			0.0288702, 	0.024716, 	0.0218283 };



// --- Sub-function declarations
void writeCandidateScores(PmisFilter& filter, ostream& os);

void writeInputScores(PmisFilter& filter,
	vector<double>& bsi95,
	vector<double>& bsi99,	 
	vector<double>& mci95,
	vector<double>& mci99,	 
	vector<double>& aic_k,
	vector<double>& aic_p,
	vector<double>& hampel, 
	ostream& os);
	
void writeSelection(PmisFilter& filter, ostream& os);
            
// --- Main function          
int main(int argc, char ** argv)
{
    Command command(argc, argv);
    
    table<double> candidates;
    ifstream fin(command.arg(0).c_str(), ios::in);
    fin >> candidates;
    fin.close();
    
    Standardiser encoder(candidates);
    encoder.encode(candidates);
    
    series<double> output(candidates.backc());
    candidates.pop_backc();
    
    unsigned k = candidates.columns();
    command.get("-k", k);
    
    unsigned bootstrapSize = 0;
    if(command.find("-b") != command.end())
    {
    	bootstrapSize = 100;
	   	command.get("-b", bootstrapSize);
    }
           
    PmisFilter filter(candidates, output);
    vector<double> aic_k;
    vector<double> aic_p;
    vector<double> hampel;
    vector<double> bsi95;
    vector<double> bsi99;
    vector<double> mci95;
    vector<double> mci99;
    
	unsigned n = candidates.rows();
    Graph<unsigned, double> graph_i95(mcs_n, mcs_i95, 27);
    Graph<unsigned, double> graph_i99(mcs_n, mcs_i99, 27);
   
    filter.calculateResiduals();
	for(unsigned j = 1; j <= k && j <= candidates.columns(); ++j)
    {
		filter.calculateScores();
		writeCandidateScores(filter, cout);
		
		hampel.push_back(filter.hampel(filter.s()));
        
        filter.select(filter.s());
        filter.calculateResiduals();
        
        aic_k.push_back(filter.aic());
		aic_p.push_back(filter.aicK());

        if(command.find("-b") != command.end())
        {
        	vector<double> bootstrap = filter.bootstrap(bootstrapSize);
        	sort(bootstrap.begin(), bootstrap.end());
        	bsi95.push_back(percentile(bootstrap, 95));
        	bsi99.push_back(percentile(bootstrap, 99));
        }
        
        mci95.push_back(graph_i95(n));
        mci99.push_back(graph_i99(n));
        
        if(command.find("-v") != command.end())
	        writeInputScores(filter, bsi95, bsi99, mci95, 
	        	mci99, aic_k, aic_p, hampel, cout);
    }
    
    ofstream fout(command.arg(1).c_str(), ios::out);
	writeInputScores(filter, bsi95, bsi99, mci95, mci99, aic_k, aic_p, hampel, fout);
    fout.close();
    
    if(command.find("-v") != command.end())
    {
    	cout << "Summary:\n";
   		writeInputScores(filter, bsi95, bsi99, mci95, mci99, aic_k, aic_p, hampel, cout);	
    }
    
    return 0;
}

void writeInputScores(PmisFilter& filter,
                      vector<double>& bsi95,
                      vector<double>& bsi99,
                      vector<double>& mci95,
                      vector<double>& mci99,
                      vector<double>& aic_k,
                      vector<double>& aic_p,
                      vector<double>& hampel,
                      ostream& os)
{
	os << setw(8) << "Iteration\tVariable\tI(x;y)\t";
    if(bsi95.size() > 0)
    	os << "BS-I*(95)\tBS-I*(99)\t";
    os << "MC-I*(95)\tMC-I*(99)\tAIC(k)\tAIC(p)\tHampel\n";
    
    for(unsigned j = 0; j < filter.inputs().columns(); ++j)
    {
        os << j << '\t' << filter.inputs().header[j] << '\t' 
            << filter.inputScores(j) << '\t';
        if(bsi95.size() > 0)
    		os << bsi95[j] << '\t' << bsi99[j] << '\t';
		os << mci95[j] << '\t' << mci99[j] << '\t';
    	os << aic_k[j] << '\t'<< aic_p[j] << '\t' << hampel[j] << endl;
    }
    os << endl;
}

void writeCandidateScores(PmisFilter& filter, ostream& os)
{
	os << "Iteration " << filter.inputs().columns() + 1 << endl;
    os << "Candidate variable set, C:\nj\tc\tI'(c,y|S)\n";   
    for(unsigned j = 0; j < filter.candidates().columns(); ++j)
        os << j << '\t' << filter.candidates().header[j] << '\t' 
            << filter.candidateScores(j) << endl;    
    os << endl;
}

void writeSelection(PmisFilter& filter, ostream& os)
{
	os << "Best candidate: " << filter.candidates().header[filter.s()] << '\n'
		<< "Score: " << filter.candidateScores(filter.s()) << '\n' << endl;
}
