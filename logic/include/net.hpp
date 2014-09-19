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
#include <cassert>
#include <cmath>
#include <fstream>
#include <sstream>

#include "commonDefs.hpp"
#include "neuron.hpp"

// Abstract class
class Net
{
public:
    Net();
    virtual ~Net();
    void feedForward(const std::vector<double> &inputVals);

    // Pure virtual function
    virtual void learn(const std::vector<double> &targetVals) = 0;

    void getResults(std::vector<double> &resultVals) const;
    double getRecentAverageError(void) const { return m_recentAverageError; }

protected:
    std::vector<Layer> m_layers; // m_layers[layerNum][neuronNum]
    double m_error;
    double m_recentAverageError;
    bool m_bias;
    static double m_recentAverageSmoothingFactor;
};

#endif /* NET_HPP */
