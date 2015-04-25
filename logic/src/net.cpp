//------------------------------------------------------
//Author             : Jonathan Schwarz
//University         : Pforzheim University
//Date of last edit  : Mon, 08 Sep 2014 11:10:32 +0200
//Filename           : net.cpp
//------------------------------------------------------

#include "net.hpp"

Net::Net()
{
}

Net::~Net()
{
}

void Net::getResults(std::vector<double> &resultVals) const
{
    resultVals.clear();

    // Return the net's results
    for (unsigned n = 0; n < m_layers.back().size() - (m_bias*1); ++n) 
        resultVals.push_back(m_layers.back()[n]->getOutputVal());
}

void Net::feedForward(const std::vector<double> &inputVals)
{
    // Assign (latch) the input values into the input neurons
    for (unsigned i = 0; i < inputVals.size(); ++i) 
        m_layers[0][i]->setOutputVal(inputVals[i]);

    // Forward propagate
    for (unsigned nbLayer = 1; nbLayer < m_layers.size(); ++nbLayer) 
    {
        unsigned nbNeuronsInLayer = m_layers[nbLayer].size() - (1*m_bias);

        for (unsigned n = 0; n < nbNeuronsInLayer; ++n) 
            m_layers[nbLayer][n]->feedForward(m_layers, nbLayer);
    }
}


vector<vector<vector<Connection> > > Net::getConnections() const
{
    vector<vector<vector<Connection> > > results;

    // Loop through the net
    unsigned nbLayersInNet = m_layers.size();
    for (unsigned nbLayer = 0; nbLayer < nbLayersInNet; ++nbLayer) 
    {
        // All Connections of the respective layer
        vector<vector<Connection> > layerConnections;
        results.push_back(layerConnections);

        unsigned nbNeuronsInLayer = m_layers[nbLayer].size();
        for (unsigned nbNeuron = 0; nbNeuron < nbNeuronsInLayer; ++nbNeuron) 
        {
            // All Connections of the respective neuron
            vector<Connection> neuronConnections = m_layers[nbLayer][nbNeuron]->m_outputWeights;
            results[nbLayer].push_back(neuronConnections);
        }
    }

    return results;
}

void Net::setConnections(const vector<vector<vector<Connection> > > &connections)
{
    // Loop through the net
    unsigned nbLayersInNet = m_layers.size();
    for (unsigned nbLayer = 0; nbLayer < nbLayersInNet; ++nbLayer) 
    {
        unsigned nbNeuronsInLayer = m_layers[nbLayer].size();
        for (unsigned nbNeuron = 0; nbNeuron < nbNeuronsInLayer; ++nbNeuron) 
        {
            // Set the connections of the respective neuron
            m_layers[nbLayer][nbNeuron]->m_outputWeights = connections[nbLayer][nbNeuron];
        }
    }
}
