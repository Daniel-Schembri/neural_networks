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

    for (unsigned n = 0; n < m_layers.back().size() - (m_bias*1); ++n) 
        resultVals.push_back(m_layers.back()[n]->getOutputVal());
}

void Net::feedForward(const std::vector<double> &inputVals)
{
    // Assign (latch) the input values into the input neurons
    for (unsigned i = 0; i < inputVals.size(); ++i) 
        m_layers[0][i]->setOutputVal(inputVals[i]);

    // forward propagate
    for (unsigned nbLayer = 1; nbLayer < m_layers.size(); ++nbLayer) 
    {
        unsigned nbNeuronsInLayer = m_layers[nbLayer].size() - (1*m_bias);

        for (unsigned n = 0; n < nbNeuronsInLayer; ++n) 
            m_layers[nbLayer][n]->feedForward(m_layers,nbLayer);
    }
}
