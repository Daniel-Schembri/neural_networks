#ifndef NET_HPP
#define NET_HPP

#include <vector>
#include <iostream>
#include <cstdlib>
#include <cmath>

#include "commonDefs.hpp"
#include "neuron.hpp"

// Abstract class
class Net
{
public:
    Net();
    virtual ~Net();

    virtual void learn(const std::vector<double> &targetVals) = 0;
    void feedForward(const std::vector<double> &inputVals);

    void getResults(std::vector<double> &resultVals) const;

    WeightMatrix getConnections() const;
    void setConnections(const WeightMatrix &weights);

    std::vector<Layer> m_layers;
protected:
    bool m_bias;
};

#endif /* NET_HPP */
