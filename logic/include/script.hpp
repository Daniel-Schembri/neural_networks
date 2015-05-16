#ifndef SCRIPT_HPP
#define SCRIPT_HPP

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

        double processV(std::vector<double> inputvals);
        double processA(std::vector<double> inputvals);

    private:
        double   current_angle_;
        unsigned idleness_count_;
};

#endif /* SCRIPT_HPP */

