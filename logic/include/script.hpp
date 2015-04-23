#ifndef SCRIPT_H 
#define SCRIPT_H

#include <vector>
#include <cmath>
#include <stdlib.h>
#include <iostream>

class Script
{

    public:
        Script();
        std::vector<double> process(std::vector<double> inputvals);
         
    private:
        void function();

};

#endif //SCRIPT_H 

