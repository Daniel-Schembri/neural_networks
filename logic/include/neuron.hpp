#ifndef NEURON_HPP
#define NEURON_HPP 

#include <vector>
#include <iostream>
#include <cstdlib>
#include <cmath>

#include "commonDefs.hpp"

//Abstract class
class Neuron
{
public:
    Neuron(double eta, unsigned myIndex);
    virtual ~Neuron();

    void setOutputVal(const double val) {m_outputVal = val;}
    double getOutputVal() const {return m_outputVal;}

    virtual void feedForward(const std::vector<Layer> &layers, const unsigned &myLayer) = 0;
    virtual void updateInputWeights(const std::vector<Layer> &layers, const unsigned &myLayer) = 0;

    std::vector<Connection> m_outputWeights;

protected:
    double m_eta;   // [0.0..1.0] overall net training rate
    unsigned m_myIndex;
    double m_outputVal;

    static double randomWeight() {return rand() / double(RAND_MAX);}
    static double transferFunction(double x);
};

#endif /* NEURON_HPP */
