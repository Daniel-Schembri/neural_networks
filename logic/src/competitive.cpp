//------------------------------------------------------
//Author             : Jonathan Schwarz
//University         : Pforzheim University
//Date of last edit  : Mon, 15 Sep 2014 16:32:09 +0200
//Filename           : competitive.cpp
//------------------------------------------------------

#include "competitive.hpp"
#include "competitiveNeuron.hpp"

Competitive::Competitive(const std::vector<unsigned> &topology, const bool &bias)
{
    m_bias = bias;
    unsigned numLayers = topology.size();

    if(3 != numLayers)
    {
        std::cerr << "Competitive networks only have two layers.\n";
        exit(EXIT_FAILURE);
    }

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
            m_layers.back().push_back(new competitiveNeuron(numOutputs, neuronNum, LEARNING_RATE));

        // Create bias neurons if requested
        if(false != bias)
        {
            m_layers.back().push_back(new competitiveNeuron(numOutputs, ++neuronNum, LEARNING_RATE));
            // Force the bias node's output to 1.0 (it was the last recent neuron pushed in this layer):
            m_layers.back().back()->setOutputVal(1.0);
        }
    }
}

Competitive::~Competitive()
{
}

// Winner takes it all
void Competitive::learn(const std::vector<double> &targetVals)
{
    double greatestOutput = 0;
    unsigned winner = 0;

    // Find neuron with the greatest output
    for (unsigned n = 0; n < m_layers[1].size() - (m_bias*1); ++n) 
        if(greatestOutput < m_layers[1][n]->getOutputVal())
        {
            greatestOutput = m_layers[1][n]->getOutputVal();
            winner = n;
        }

    // Update winner neuron
    m_layers[1][winner]->updateInputWeights(m_layers, 1);
}
