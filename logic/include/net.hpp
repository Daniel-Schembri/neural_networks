#ifndef NET_HPP
#define NET_HPP

#include <vector>
#include <iostream>
#include <cstdlib>
#include <cmath>

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

    WeightMatrix getConnections() const;
    void setConnections(const WeightMatrix &weights);

    vector<Layer> m_layers;
protected:
    bool m_bias;
};

#endif /* NET_HPP */
