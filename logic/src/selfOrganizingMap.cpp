//------------------------------------------------------
//Author             : Jonathan Schwarz
//University         : Pforzheim University
//Date of last edit  : Wed, 24 Sep 2014 08:04:44 +0200
//Filename           : selfOrganizingMap.cpp
//------------------------------------------------------

#include "selfOrganizingMap.hpp"
#include "somNeuron.hpp"

SelfOrganizingMap::SelfOrganizingMap(const std::vector<unsigned> &topology, const bool &bias, const unsigned &maxSteps):
    m_maxSteps(maxSteps)
{
    m_bias = bias;
    unsigned numLayers = topology.size();

    m_currentStep = 0;

    if(3 != numLayers)
    {
        std::cerr << "Self organizing maps only have two layers.\n";
        exit(EXIT_FAILURE);
    }

    double fracPart, intPart;
    fracPart = modf((double) topology[2], &intPart);

    if((0.0 != fracPart) && (2 != topology[2]))
    {
        std::cerr << "Output units are organized in a n*n matrix.\n";
        exit(EXIT_FAILURE);
    }

    for(unsigned nbLayer = 0; nbLayer < numLayers; ++nbLayer) {
        m_layers.push_back(Layer());
        unsigned numOutputs;
        unsigned neuronNum = 0;

        if(topology.size() -1 == nbLayer)
            // Output neurons don't have outputs
            numOutputs = 0;
        else
            // The number of outputs is equal to the number of neurons in the next layer
            numOutputs = topology[nbLayer + 1];

        /* The n*n matrix is alligned in the following way (numbers are the index of the neurons in the
         * ouput layer vector 
         *
         * 1  2  3 
         * 4  5  6 
         * 7  8  9 
         */

        // We have a new layer, now fill it with neurons
        for (neuronNum = 0; neuronNum < topology[nbLayer]; ++neuronNum) 
            m_layers.back().push_back(new somNeuron(numOutputs, neuronNum, LEARNING_RATE));

        // Create bias neurons if requested
        if(false != bias)
        {
            m_layers.back().push_back(new somNeuron(numOutputs, ++neuronNum, LEARNING_RATE));
            // Force the bias node's output to 1.0 (it was the last recent neuron pushed in this layer):
            m_layers.back().back()->setOutputVal(1.0);
        }
    }
}

SelfOrganizingMap::~SelfOrganizingMap()
{
}


inline double SelfOrganizingMap::getDistance(const double &x, const double &y) const
{
    return sqrt(pow(x-y,2.0));
}

std::vector<unsigned> SelfOrganizingMap::getNeighbors(const unsigned &reference)
{
    /* Neighbors must be in direct vertical or horizontal neighborhood 
     *
     * e.g.:
     *
     * o  o  o     o  nei  o
     * o ref o -> nei ref nei
     * o  o  o     o  nei  o 
     */

    std::vector<unsigned> result; 
    unsigned maxIndex = m_layers[1].size()-1;
    unsigned nbRowsColumns = sqrt(maxIndex+1);

    if(maxIndex >= (reference + nbRowsColumns))
        result.push_back(reference + nbRowsColumns);
    if(maxIndex >= (reference + 1))
        result.push_back(reference + 1);
    if(0 >= (reference - nbRowsColumns))
        result.push_back(reference - nbRowsColumns);
    if(0 >= (reference - 1))
        result.push_back(reference - 1);

    return result;
}

void SelfOrganizingMap::learn(const std::vector<double> &targetVals)
{
    if(m_currentStep < m_maxSteps)
        m_currentStep++;
    else
    {
        std::cout << "Max. number of iterations (" << m_maxSteps << ") reached.\n";
        return;
    }

    // The similarity between the connection weigth vector to one neuron and the input vector
    std::vector<double> similarities;
    double distance = 0.0; 

    // For all neurons
    for(unsigned m = 0; m < m_layers[1].size() - (m_bias*1); ++m) 
    {
        distance = 0.0; 

        // Compute the euclidean distance between the input neurons and the connection weights 
        // between them and this neuron
        for(unsigned n = 0; n < m_layers[0].size(); ++n)
            distance += getDistance(m_layers[0][n]->m_outputWeights[m].weight, m_layers[0][n]->getOutputVal());
        
        similarities.push_back(distance);
    }

    // Index of the the best matching unit
    unsigned bmu = 0;
    // Summed distance of the bmu
    double minDistance = similarities[0];

    for(unsigned i = 1;i < similarities.size();++i)
    {
        if(minDistance > similarities[i])
        {
            minDistance = similarities[i];
            bmu = i;
        }
    }

    std::vector<unsigned> neighbors = getNeighbors(bmu);

    // Update bmu 
    m_layers[1][bmu]->updateInputWeights(m_layers, 1);
    // neighbors 
    for(unsigned i = 0; i < neighbors.size(); ++i)
    {
        m_layers[1][neighbors[i]]->updateInputWeights(m_layers, 1);
    }
}

