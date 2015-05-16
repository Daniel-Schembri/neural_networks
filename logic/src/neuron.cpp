#include "neuron.hpp"
#include <math.h>

Neuron::Neuron(double eta, unsigned myIndex):
    m_eta(eta),
    m_myIndex(myIndex)
{
}

Neuron::Neuron(const Neuron& rhs)
{
    *this = rhs;
}

Neuron::~Neuron()
{
}

double Neuron::transferFunction(double x)
{
    // tanh - output range [-1.0..1.0]
    return tanh(x);
}
