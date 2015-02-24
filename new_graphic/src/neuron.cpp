#include "neural_network/neuron.hpp"

Neuron::Neuron(double eta, unsigned myIndex, unsigned numOutputs):
    m_eta(eta),
    m_myIndex(myIndex)
{
    // We don't need connections for the output layer
    if(0 != numOutputs)
    {
        for(unsigned c = 0; c < numOutputs; ++c) 
        {
            m_outputWeights.push_back(Connection());
            m_outputWeights.back().weight = randomWeight();
        }
    }
}

Neuron::~Neuron()
{
}

void Neuron::feedForward(const Layer &previousLayer)
{
    double sum = 0.0;

    unsigned nbNeuronsInLayer = previousLayer.size();
   
    // Sums up everything including bias, if existent
    for (unsigned n = 0; n < nbNeuronsInLayer; ++n) 
        sum += previousLayer[n]->getOutputVal() *
               previousLayer[n]->m_outputWeights[m_myIndex].weight;

    m_outputVal = transferFunction(sum);
}

double Neuron::transferFunction(double x)
{
    // tanh - output range [-1.0..1.0]
    return tanh(x);
}
