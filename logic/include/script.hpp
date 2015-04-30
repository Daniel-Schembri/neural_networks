#ifndef SCRIPT_H 
#define SCRIPT_H

#include <vector>
#include <cmath>
#include <stdlib.h>
#include <iostream>

using std::vector;

class Script
{

    public:
        Script();
        ~Script();

        vector<double> process(std::vector<double> inputvals);
};

#endif //SCRIPT_H 

