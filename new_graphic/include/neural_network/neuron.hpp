#ifndef NEURON_HPP
#define NEURON_HPP 

#include <vector>
#include <cmath>
#include <cstdlib>

#include "commonDefs.hpp"

class Neuron
{
public:
    Neuron(double eta, unsigned myIndex, unsigned numOutputs);
    ~Neuron();

    void setOutputVal(const double val) {m_outputVal = val;}
    double getOutputVal() const {return m_outputVal;}

    void feedForward(const Layer &previousLayer);

    std::vector<Connection> m_outputWeights;

protected:
    double   m_eta;   // [0.0..1.0] overall net training rate
    unsigned m_myIndex;
    double   m_outputVal;

    static double randomWeight() {return rand() / double(RAND_MAX);}
    static double transferFunction(double x);
};

#endif /* NEURON_HPP */
