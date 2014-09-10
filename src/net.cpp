//------------------------------------------------------
//Author             : Jonathan Schwarz
//University         : Pforzheim University
//Date of last edit  : Mon, 08 Sep 2014 11:10:32 +0200
//Filename           : net.cpp
//------------------------------------------------------

#include "net.hpp"

double Net::m_recentAverageSmoothingFactor = 100.0; // Number of training samples to average over

void Net::getResults(std::vector<double> &resultVals) const
{
    resultVals.clear();

    for (unsigned n = 0; n < m_layers.back().size() - (m_bias*1); ++n) 
        resultVals.push_back(m_layers.back()[n].getOutputVal());
}

void Net::backProp(const std::vector<double> &targetVals)
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

    // Calculate output layer gradients
    for (unsigned n = 0; n < nbOutputNeurons; ++n) 
    {
        outputLayer[n].calcOutputGradients(targetVals[n]);
    }

    // Calculate hidden layer gradients, start at rightmost hidden layer
    for (unsigned nbLayer = m_layers.size() - 2; nbLayer > 0; --nbLayer) 
    {
        Layer &hiddenLayer = m_layers[nbLayer];
        Layer &nextLayer = m_layers[nbLayer + 1];

        for (unsigned n = 0; n < hiddenLayer.size(); ++n) 
        {
            hiddenLayer[n].calcHiddenGradients(nextLayer, m_bias);
        }
    }

    // For all layers from outputs to first hidden layer,
    // update connection weights

    for (unsigned nbLayer = m_layers.size() - 1; nbLayer > 0; --nbLayer)
    {
        Layer &layer = m_layers[nbLayer];
        Layer &prevLayer = m_layers[nbLayer - 1];

        unsigned nbNeuronsToUpdate = layer.size() - (m_bias*1);

        if(FEED_FORWARD == m_netType)
        {
            for (unsigned n = 0; n < nbNeuronsToUpdate; ++n) 
                layer[n].updateInputWeights(prevLayer);
        }
        else if(SIMPLE_RECURRENT == m_netType)
        {
            //context neurons only exist in hidden layers
            if(m_layers.size() -1 != nbLayer)
                nbNeuronsToUpdate = layer.size() / 2;

            //skip context neurons, as their input weights are constant
            for (unsigned n = 0; n < nbNeuronsToUpdate; ++n) 
                layer[n].updateInputWeights(prevLayer);
        }
    }
}

void Net::feedForward(const std::vector<double> &inputVals)
{
    // Assign (latch) the input values into the input neurons
    for (unsigned i = 0; i < inputVals.size(); ++i) 
        m_layers[0][i].setOutputVal(inputVals[i]);

    // forward propagate
    for (unsigned nbLayer = 1; nbLayer < m_layers.size(); ++nbLayer) 
    {
        Layer &currentLayer = m_layers[nbLayer];
        Layer &prevLayer = m_layers[nbLayer - 1];

        unsigned nbNeuronsInLayer = m_layers[nbLayer].size() - (1*m_bias);
        for (unsigned n = 0; n < nbNeuronsInLayer; ++n) 
            m_layers[nbLayer][n].feedForward(prevLayer, currentLayer);
    }
}

Net::Net(const std::vector<unsigned> &topology, const bool &bias, const NET_TYPE &netType):
    m_netType(netType),
    m_bias(bias)
{
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
        {
            if((SIMPLE_RECURRENT == netType) && (0 < nbLayer) && (numLayers -1 != nbLayer))
                // Hidden layer within a SRN, the neurons connect to an additional context neuron
                m_layers.back().push_back(Neuron(numOutputs + 1, neuronNum, true));
            else
                m_layers.back().push_back(Neuron(numOutputs, neuronNum, false));
        }

        // Create context neurons if requested
        if((SIMPLE_RECURRENT == netType) && (0 < nbLayer) && (numLayers -1 != nbLayer))
        {
            // A context neurons feeds all non-context neurons in its layer
            numOutputs = topology[nbLayer];

            for (unsigned nbContextNeurons = 0; nbContextNeurons < topology[nbLayer]; ++nbContextNeurons) 
                m_layers.back().push_back(Neuron(numOutputs, ++neuronNum, false));
        }

        // Create bias neurons if requested
        if(false != bias)
        {
            m_layers.back().push_back(Neuron(numOutputs, ++neuronNum, false));
            // Force the bias node's output to 1.0 (it was the last recent neuron pushed in this layer):
            m_layers.back().back().setOutputVal(1.0);
        }
    }
}
