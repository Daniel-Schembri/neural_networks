//------------------------------------------------------
//Author             : Jonathan Schwarz
//University         : Pforzheim University
//Date of last edit  : Mon, 15 Sep 2014 16:32:09 +0200
//Filename           : feedForwardNet.cpp
//------------------------------------------------------

#include "feedForwardNet.hpp"

FeedForwardNet::FeedForwardNet(const std::vector<unsigned> &topology, const bool &bias)
{
    m_bias = bias;
    unsigned numLayers = topology.size();

    m_recentAverageSmoothingFactor = 100.0; // Number of training samples to average over

    for (unsigned nbLayer = 0; nbLayer < numLayers; ++nbLayer) {
        m_layers.push_back(Layer());
        unsigned numOutputs;
        unsigned neuronNum = 0;

        if(topology.size() -1 == nbLayer)
           // Output neurons don't have outputs
           numOutputs = 0;
        else
           // The number of outputs is equal to the number of neurons in the next layer
           numOutputs = topology[nbLayer + 1];

        // We have a new layer, now fill it with neurons
        for (neuronNum = 0; neuronNum < topology[nbLayer]; ++neuronNum) 
            m_layers.back().push_back(new ffNeuron(numOutputs, neuronNum, MOMENTUM, LEARNING_RATE));

        // Create bias neurons if requested
        if(false != bias)
        {
            m_layers.back().push_back(new ffNeuron(numOutputs, ++neuronNum, MOMENTUM, LEARNING_RATE));
            // Force the bias node's output to 1.0 (it was the last recent neuron pushed in this layer):
            m_layers.back().back()->setOutputVal(1.0);
        }
    }
}

FeedForwardNet::~FeedForwardNet()
{
    for (unsigned i = 0; i < m_layers.size(); ++i)
        for (unsigned j = 0; j < m_layers[i].size(); ++j)
            delete m_layers[i][j];
}

double FeedForwardNet::getRecentAverageError() const 
{ 
    return m_recentAverageError; 
}

void FeedForwardNet::learn(const std::vector<double> &targetVals)
{
    // Calculate overall net error (RMS of output neuron errors)

    Layer &outputLayer = m_layers.back();
    m_error = 0.0;
    unsigned nbOutputNeurons;

    nbOutputNeurons = outputLayer.size() - (1*m_bias); 

    for (unsigned n = 0; n < nbOutputNeurons; ++n) 
    {
        double delta = targetVals[n] - outputLayer[n]->getOutputVal();
        m_error += delta * delta;
    }

    m_error /= nbOutputNeurons; // get average error squared
    m_error = sqrt(m_error); // RMS

    // Implement a recent average measurement

    m_recentAverageError =
            (m_recentAverageError * m_recentAverageSmoothingFactor + m_error)
            / (m_recentAverageSmoothingFactor + 1.0);

    // Calculate output layer gradients
    for (unsigned n = 0; n < nbOutputNeurons; ++n) 
    {
        (dynamic_cast<ffNeuron*>(outputLayer[n]))->calcOutputGradients(targetVals[n]);    
    }

    // Calculate hidden layer gradients, start at rightmost hidden layer
    for (unsigned nbLayer = m_layers.size() - 2; nbLayer > 0; --nbLayer) 
    {
        Layer &hiddenLayer = m_layers[nbLayer];
        Layer &nextLayer = m_layers[nbLayer + 1];

        for (unsigned n = 0; n < hiddenLayer.size(); ++n) 
            (dynamic_cast<ffNeuron*>(hiddenLayer[n]))->calcHiddenGradients(nextLayer, m_bias);
    }

    // For all layers from outputs to first hidden layer,
    // update connection weights
    for (unsigned nbLayer = m_layers.size() - 1; nbLayer > 0; --nbLayer)
    {
        unsigned nbNeuronsToUpdate = m_layers[nbLayer].size() - (m_bias*1);

        for (unsigned n = 0; n < nbNeuronsToUpdate; ++n) 
            (dynamic_cast<ffNeuron*>(m_layers[nbLayer][n]))->updateInputWeights(m_layers, nbLayer);            
    }
}
