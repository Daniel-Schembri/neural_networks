//------------------------------------------------------
//Author             : Jonathan Schwarz
//University         : Pforzheim University
//Date of last edit  : Mon, 15 Sep 2014 16:32:09 +0200
//Filename           : competitive.cpp
//------------------------------------------------------

#include "competitive.hpp"

Competitive::Competitive(const std::vector<unsigned> &topology, const bool &bias)
{
    m_bias = bias;
    unsigned numLayers = topology.size();

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
            m_layers.back().push_back(Neuron(numOutputs, neuronNum, false));

        // Create bias neurons if requested
        if(false != bias)
        {
            m_layers.back().push_back(Neuron(numOutputs, ++neuronNum, false));
            // Force the bias node's output to 1.0 (it was the last recent neuron pushed in this layer):
            m_layers.back().back().setOutputVal(1.0);
        }
    }
}

Competitive::~Competitive()
{
}

// Winner takes it all
void Competitive::learn(const std::vector<double> &targetVals)
{
    // Calculate overall net error (RMS of output neuron errors)
    Layer &outputLayer = m_layers.back();
    m_error = 0.0;
    unsigned nbOutputNeurons;

    nbOutputNeurons = outputLayer.size() - (1*m_bias); 

    for (unsigned n = 0; n < nbOutputNeurons; ++n) 
    {
        double delta = targetVals[n] - outputLayer[n].getOutputVal();
        m_error += delta * delta;
    }

    m_error /= nbOutputNeurons; // get average error squared
    m_error = sqrt(m_error); // RMS

    // Implement a recent average measurement
    m_recentAverageError =
            (m_recentAverageError * m_recentAverageSmoothingFactor + m_error)
            / (m_recentAverageSmoothingFactor + 1.0);

    double greatestOutput = 0;
    neuronIndex winner = {0,0}

    // Find neuron with the greatest output
    for (unsigned m = 0; n < m_layers.size(); ++m) 
        for (unsigned n = 0; n < m_layers[m].size() - (m_bias*1); ++n) 
            if(greatestOutput < m_layers[m][n].getOutputVal())
            {
                greatestOutput = m_layers[m][n].getOutputVal();
                winner.layer = m;
                winner.layer = n;
            }

    // Update winner neuron
    m_layers[m][n].updateInputWeights(m_layers[m-1], true);
}

