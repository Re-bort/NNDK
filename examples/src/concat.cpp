#include <iostream>
#include <fstream>
#include <data/Dataset.h>
#include <util/Command.h>

using namespace std;

int main(int argc, char ** argv)
{
    Command command(argc, argv);
    
    unsigned arg = 0;
    bool columnwise = false;
    if(command.arg(arg) == "-c")
    {
        columnwise = true;
        ++arg;
    }
    
    string filename = command.arg(arg);
    ++arg;
   
    if(arg == command.size())
    {
        cerr << "concatc: insufficient arguments" << endl;
        return 0;
    }
   
    table<double> T;
    T.read(command.arg(arg));
    ++arg;
    for(; arg < command.size(); ++arg)
    {
        table<double> U;
        U.read(command.arg(arg));
        if(columnwise)   
            T.concatc(U);
        else
            T.concat(U);
    }

    T.write(command.arg(0));

    return 0;    
}
