#include "neuron.hpp"

double Neuron::eta = 0.15;    // overall net learning rate, [0.0..1.0]
double Neuron::alpha = 0.5;   // momentum, multiplier of last deltaWeight, [0.0..1.0]

void Neuron::updateInputWeights(Layer &prevLayer)
{
    // The weights to be updated are in the Connection container
    // in the neurons in the preceding layer

    for (unsigned n = 0; n < prevLayer.size(); ++n) 
    {
        Neuron &neuron = prevLayer[n];

        //the old weight from it to us
        double oldDeltaWeight = neuron.m_outputWeights[m_myIndex].deltaWeight;

        double newDeltaWeight =
                // Individual input, magnified by the gradient and train rate:
                eta
                * neuron.getOutputVal()
                * m_gradient
                // Also add momentum = a fraction of the previous delta weight;
                + alpha
                * oldDeltaWeight;

        neuron.m_outputWeights[m_myIndex].deltaWeight = newDeltaWeight;
        neuron.m_outputWeights[m_myIndex].weight += newDeltaWeight;
    }
}

double Neuron::sumDOW(const Layer &nextLayer, const bool &bias) const
{
    double sum = 0.0;

    unsigned nbNeuronsInLayer = nextLayer.size() - (1*bias);
    // Sum our contributions of the errors at the nodes we feed.
    for (unsigned n = 0; n < nbNeuronsInLayer; ++n) 
        sum += m_outputWeights[n].weight * nextLayer[n].m_gradient;

    return sum;
}

void Neuron::calcHiddenGradients(const Layer &nextLayer, const bool &bias)
{
    double dow = sumDOW(nextLayer, bias);
    m_gradient = dow * Neuron::transferFunctionDerivative(m_outputVal);
}

void Neuron::calcOutputGradients(double targetVal)
{
    double delta = targetVal - m_outputVal;
    m_gradient = delta * Neuron::transferFunctionDerivative(m_outputVal);
}

double Neuron::transferFunction(double x)
{
    // tanh - output range [-1.0..1.0]

    return tanh(x);
}

double Neuron::transferFunctionDerivative(double x)
{
    // tanh derivative approximation. Actual derivative is 1 - tanh^2(x) 
    return 1.0 - x * x;
}

void Neuron::feedForward(const Layer &prevLayer, const Layer &currentLayer)
{
    double sum = 0.0;
    unsigned nbNeuronsInLayer = prevLayer.size();
   
    if(false != m_connectsToContext)
    {
        // sum up the input from the previous layer
        for (unsigned n = 0; n < nbNeuronsInLayer; ++n) 
        {
            sum += prevLayer[n].getOutputVal() *
                   prevLayer[n].m_outputWeights[m_myIndex].weight;
        }

        nbNeuronsInLayer = currentLayer.size();
        // add the context layer input
        for (unsigned n = currentLayer.size()/2; n < nbNeuronsInLayer; ++n) 
        {
            //TODO: Stimmt der Index?
            sum += currentLayer[n].getOutputVal() *
                   currentLayer[n].m_outputWeights[m_myIndex].weight;
        }
    } 
    else
    {
        // If there's a bias (odd number of neurons in layer), add it first
        if(0 != nbNeuronsInLayer % 2)
            sum += prevLayer[nbNeuronsInLayer - 1].getOutputVal() *
                   prevLayer[nbNeuronsInLayer - 1].m_outputWeights[m_myIndex].weight;

        for (unsigned n = 0; n < nbNeuronsInLayer; ++n) 
        {
            if(prevLayer[n].m_connectsToContext)
            {
                // We only get Input from non-context neurons
                nbNeuronsInLayer /= 2;
                break;
            }
        }
        
        for (unsigned n = 0; n < nbNeuronsInLayer; ++n) 
        {
            sum += prevLayer[n].getOutputVal() *
                   prevLayer[n].m_outputWeights[m_myIndex].weight;
        }
    }

    m_outputVal = Neuron::transferFunction(sum);
}


Neuron::Neuron(unsigned numOutputs, unsigned myIndex, const bool &connectsToContext):
    m_myIndex(myIndex),
    m_connectsToContext(connectsToContext)
{
    // We don't need connections for the output layer
    if(0 != numOutputs)
    {
        for(unsigned c = 0; c < numOutputs-1; ++c) 
        {
            m_outputWeights.push_back(Connection());
            m_outputWeights.back().weight = randomWeight();
        }

        // If we're dealing with a SRN, the last Output connects to a context Neuron, 
        // and is therefore constant 1.0
        if(true == m_connectsToContext)
        {
            m_outputWeights.push_back(Connection());
            m_outputWeights.back().weight = 1.0;
        }
        else
        {
            m_outputWeights.push_back(Connection());
            m_outputWeights.back().weight = randomWeight();
        }
    }
}
