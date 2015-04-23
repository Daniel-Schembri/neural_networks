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

using std::vector;

// Abstract class
class Net
{
public:
    Net();
    virtual ~Net();

    virtual void learn(const vector<double> &targetVals) = 0;
    void feedForward(const vector<double> &inputVals);

    void getResults(vector<double> &resultVals) const;
    void getConnections(vector<vector<vector<Connection> > > &results) const;

    void setConnections(vector<vector<vector<Connection> > > &connections);

protected:
    vector<Layer> m_layers;
    bool m_bias;
};

#endif /* NET_HPP */
