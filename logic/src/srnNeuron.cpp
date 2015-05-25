#include "srnNeuron.hpp"

SRNNeuron::SRNNeuron(unsigned numOutputs, unsigned myIndex, double alpha, double eta):
    Neuron(eta, myIndex),
    m_alpha(alpha)
{
    m_outputVal = 0.0;
    // Output neurons don't have connections
    if(0 != numOutputs)
    {
        for(unsigned c = 0; c < numOutputs; ++c) 
        {
            Connection conn;
            conn.weight      = randomWeight();
            conn.deltaWeight = 0.0;
            m_outputWeights.push_back(conn);
        }
    }
}

SRNNeuron::SRNNeuron(const SRNNeuron &rhs):
    Neuron(rhs)
{
    *this = rhs;
}

SRNNeuron::~SRNNeuron()
{
}

void SRNNeuron::calcHiddenGradients(const Layer &nextLayer, const bool &bias)
{
    double dow = sumDOW(nextLayer, bias);
    m_gradient = dow * SRNNeuron::transferFunctionDerivative(m_outputVal);
}

void SRNNeuron::calcOutputGradients(double targetVal)
{
    double delta = targetVal - m_outputVal;
    m_gradient = delta * SRNNeuron::transferFunctionDerivative(m_outputVal);
}

double SRNNeuron::getInput(const std::vector<Layer> &layers, const unsigned &myLayer)
{
    double sum = 0.0;

    unsigned nbNeuronsInLayer = layers[myLayer - 1].size();
    
    // Sums up everything including bias, if existent
    for (unsigned n = 0; n < nbNeuronsInLayer; ++n) 
    {
        // Check if there's a connection to us
        if (m_myIndex < layers[myLayer - 1][n]->m_outputWeights.size())
            sum += layers[myLayer - 1][n]->getOutputVal() *
                   layers[myLayer - 1][n]->m_outputWeights[m_myIndex].weight;
    }

    return sum;
}

void SRNNeuron::feedForward(const std::vector<Layer> &layers, const unsigned &myLayer)
{
    double sum = 0.0;

    unsigned nbNeuronsInLayer = layers[myLayer - 1].size();
   
    // Sums up everything including bias, if existent
    for (unsigned n = 0; n < nbNeuronsInLayer; ++n) 
    {
        // Check if there's a connection to us
        if (m_myIndex < layers[myLayer - 1][n]->m_outputWeights.size())
            sum += layers[myLayer - 1][n]->getOutputVal() *
                   layers[myLayer - 1][n]->m_outputWeights[m_myIndex].weight;
        
//        std::cerr << "My layer: " << myLayer << " My Index: " << m_myIndex << std::endl;
        size_t size = layers[myLayer - 1][n]->m_outputWeights.size();
        for (size_t i = 0; i < size; i++) 
        {
//            std::cerr << "Weights of preceding layer. Index #" << i << ": " << layers[myLayer - 1][n]->m_outputWeights[i].weight << "/" << layers[myLayer - 1][n]->m_outputWeights[i].deltaWeight << std::endl;
        }

    }
    
//    std::cerr << "=============================" << std::endl;

    m_input = sum;
    m_outputVal = transferFunction(sum);
}

double SRNNeuron::getGradient() const
{
    return m_gradient;
}

double SRNNeuron::sumDOW(const Layer &nextLayer, const bool &bias) const
{
    double sum = 0.0;

    unsigned nbNeuronsInLayer = nextLayer.size() - (1*bias);

    // Sum our contributions of the errors at the nodes we feed.
    for (unsigned n = 0; n < nbNeuronsInLayer; ++n) 
        sum += m_outputWeights[n].weight * dynamic_cast<SRNNeuron*>(nextLayer[n].get())->m_gradient;

    return sum;
}

double SRNNeuron::transferFunctionDerivative(double x)
{
    // tanh derivative approximation. Actual derivative is 1 - tanh^2(x) 
    return 1.0 - x * x;
}

void SRNNeuron::updateInputWeights(const std::vector<Layer> &layers, const unsigned &myLayer)
{
    // The weights to be updated are in the Connection container
    // in the neurons in the preceding layer
    for (unsigned n = 0; n < layers[myLayer - 1].size(); ++n) 
    {
        // Check if there's a connection to us
        if (m_myIndex < layers[myLayer - 1][n]->m_outputWeights.size())
        {
            Neuron *neuron = layers[myLayer - 1][n].get();

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
}
//
//void SRNNeuron::singleConnection(const unsigned &index, const double &weight, const double &deltaWeight) 
//{
//    unsigned size = m_outputWeights.size();
//
//    while(index > --size)
//    {
//        // Create dummy connections. They won't contribute to the next neuron's input
//        m_outputWeights.push_back(Connection());
//        m_outputWeights.back().weight = 0.0;
//    }
//
//    for(unsigned i=0; i<m_outputWeights.size(); ++i)
//    {
//        if(index != i)
//        {
//            m_outputWeights[i].weight = 0.0;
//            m_outputWeights[i].deltaWeight = 0.0;
//        }
//        else
//        {
//            m_outputWeights[i].weight = weight;
//            m_outputWeights[i].deltaWeight = deltaWeight;
//        }
//    }
//}
