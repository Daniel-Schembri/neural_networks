#include "competitiveNeuron.hpp"

competitiveNeuron::competitiveNeuron(unsigned numOutputs, unsigned myIndex, double eta):
    Neuron(eta, myIndex)
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

competitiveNeuron::~competitiveNeuron()
{
}

void competitiveNeuron::feedForward(const std::vector<Layer> &layers, const unsigned &myLayer)
{
    double sum = 0.0;

    unsigned nbNeuronsInLayer = layers[myLayer - 1].size();
   
    // If there's a bias (odd number of neurons in layer), add it first
    if(0 != nbNeuronsInLayer % 2)
        sum += layers[myLayer - 1][nbNeuronsInLayer - 1]->getOutputVal() *
               layers[myLayer - 1][nbNeuronsInLayer - 1]->m_outputWeights[m_myIndex].weight;
    
    for (unsigned n = 0; n < nbNeuronsInLayer; ++n) 
    {
        sum += layers[myLayer - 1][n]->getOutputVal() *
               layers[myLayer - 1][n]->m_outputWeights[m_myIndex].weight;
    }

    m_outputVal = transferFunction(sum);
}

void competitiveNeuron::updateInputWeights(const std::vector<Layer> &layers, const unsigned &myLayer)
{
    // The weights to be updated are in the Connection container
    // in the neurons in the preceding layer

    for (unsigned n = 0; n < layers[myLayer - 1].size(); ++n) 
    {
        Neuron *neuron = layers[myLayer - 1][n];

        //the old weight from it to us
        double oldDeltaWeight = neuron->m_outputWeights[m_myIndex].deltaWeight;

        double newDeltaWeight =
                m_eta *
                (neuron->getOutputVal()
                - oldDeltaWeight);

        neuron->m_outputWeights[m_myIndex].deltaWeight = newDeltaWeight;
        neuron->m_outputWeights[m_myIndex].weight += newDeltaWeight;
    }
}
