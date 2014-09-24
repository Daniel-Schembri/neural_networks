#ifndef NEURON_HPP
#define NEURON_HPP 

#include <vector>
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <fstream>
#include <sstream>

#include "commonDefs.hpp"

class Neuron
{
public:
    Neuron(unsigned numOutputs, unsigned myIndex, const bool &connectsToContext);
    void setOutputVal(double val) { m_outputVal = val; }
    double getOutputVal(void) const { return m_outputVal; }
    void feedForward(const Layer &prevLayer, const Layer &currentLayer);
    void calcOutputGradients(double targetVal);
    void calcHiddenGradients(const Layer &nextLayer, const bool &bias);
    void updateInputWeights(Layer &prevLayer, const bool &competitive = false);

    std::vector<Connection> m_outputWeights;

protected:
    static double eta;   // [0.0..1.0] overall net training rate
    static double alpha; // [0.0..n] multiplier of last weight change (momentum)
    static double transferFunction(double x);
    static double transferFunctionDerivative(double x);
    static double randomWeight(void) { return rand() / double(RAND_MAX); }
    double sumDOW(const Layer &nextLayer, const bool &bias) const;
    double m_outputVal;
    unsigned m_myIndex;
    double m_gradient;
    bool m_connectsToContext;
};

#endif /* NEURON_HPP */
