//------------------------------------------------------
//Author             : Jonathan Schwarz
//University         : Pforzheim University
//Date of last edit  : Mon, 15 Sep 2014 16:09:44 +0200
//Filename           : net.hpp
//------------------------------------------------------


#ifndef NET_HPP
#define NET_HPP

#include <vector>
#include <iostream>
#include <cstdlib>
//#include <cassert>
#include <cmath>
//#include <fstream>
//#include <sstream>

#include "commonDefs.hpp"
#include "neuron.hpp"

// Abstract class
class Net
{
public:
    Net();
    virtual ~Net();
    void feedForward(const std::vector<double> &inputVals);

    virtual void learn(const std::vector<double> &targetVals) = 0;

    void getResults(std::vector<double> &resultVals) const;

protected:
    std::vector<Layer> m_layers;
    bool m_bias;
};

#endif /* NET_HPP */
