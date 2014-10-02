#include "srnNeuron.hpp"

srnNeuron::srnNeuron(unsigned numOutputs, unsigned myIndex, double alpha, double eta):
    Neuron(eta,myIndex),
    m_alpha(alpha)
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

srnNeuron::~srnNeuron()
{
}

void srnNeuron::calcHiddenGradients(const Layer &nextLayer, const bool &bias)
{
    double dow = sumDOW(nextLayer, bias);
    m_gradient = dow * srnNeuron::transferFunctionDerivative(m_outputVal);
}

void srnNeuron::calcOutputGradients(double targetVal)
{
    double delta = targetVal - m_outputVal;
    m_gradient = delta * srnNeuron::transferFunctionDerivative(m_outputVal);
}

void srnNeuron::feedForward(const std::vector<Layer> &layers, const unsigned &myLayer)
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

double srnNeuron::getGradient() const
{
    return m_gradient;
}

double srnNeuron::sumDOW(const Layer &nextLayer, const bool &bias) const
{
    double sum = 0.0;

    unsigned nbNeuronsInLayer = nextLayer.size() - (1*bias);

    // Sum our contributions of the errors at the nodes we feed.
    for (unsigned n = 0; n < nbNeuronsInLayer; ++n) 
        sum += m_outputWeights[n].weight * dynamic_cast<srnNeuron*>(nextLayer[n])->m_gradient;

    return sum;
}

double srnNeuron::transferFunctionDerivative(double x)
{
    // tanh derivative approximation. Actual derivative is 1 - tanh^2(x) 
    return 1.0 - x * x;
}

void srnNeuron::updateInputWeights(const std::vector<Layer> &layers, const unsigned &myLayer)
{
    // The weights to be updated are in the Connection container
    // in the neurons in the preceding layer
    
    for (unsigned n = 0; n < layers[myLayer - 1].size(); ++n) 
    {
        Neuron *neuron = layers[myLayer - 1][n];

        //the old weight from it to us
        double oldDeltaWeight = neuron->m_outputWeights[m_myIndex].deltaWeight;

        double newDeltaWeight =
                // Individual input, magnified by the gradient and train rate:
                m_eta
                * neuron->getOutputVal()
                * m_gradient
                // Also add momentum (a fraction of the previous delta weight);
                + m_alpha
                * oldDeltaWeight;

        neuron->m_outputWeights[m_myIndex].deltaWeight = newDeltaWeight;
        neuron->m_outputWeights[m_myIndex].weight += newDeltaWeight;
    }
}
