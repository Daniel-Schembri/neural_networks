//------------------------------------------------------
//Author             : Jonathan Schwarz
//University         : Pforzheim University
//Date of last edit  : Mon, 15 Sep 2014 16:31:35 +0200
//Filename           : srn.cpp
//------------------------------------------------------

#include "srn.hpp"
#include "srnNeuron.hpp"

srn::srn(const std::vector<unsigned> &topology, const bool &bias)
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
        {
                m_layers.back().push_back(new srnNeuron(numOutputs, neuronNum, MOMENTUM, LEARNING_RATE));
        }

        // Create bias neurons if requested
        if(false != bias)
        {
            m_layers.back().push_back(new srnNeuron(numOutputs, ++neuronNum, MOMENTUM, LEARNING_RATE));
            // Force the bias node's output to 1.0 (it was the last recent neuron pushed in this layer):
            m_layers.back().back()->setOutputVal(1.0);
        }
    }
}

srn::~srn()
{
    for (unsigned i = 0; i < m_layers.size(); ++i)
        for (unsigned j = 0; j < m_layers[i].size(); ++j)
            delete m_layers[i][j];
}

// Overwrite base class method to implement function unrolling
void srn::feedForward(const std::vector<double> &inputVals)
{
    // Assign (latch) the input values into the input neurons
    for (unsigned i = 0; i < inputVals.size(); ++i) 
        m_layers[0][i]->setOutputVal(inputVals[i]);

    // forward propagate
    for (unsigned nbLayer = 1; nbLayer < m_layers.size(); ++nbLayer) 
    {
        unsigned nbNeuronsInLayer = m_layers[nbLayer].size() - (1*m_bias);

        for (unsigned n = 0; n < nbNeuronsInLayer; ++n) 
            m_layers[nbLayer][n]->feedForward(m_layers, nbLayer);
    }

    unroll();
}

void srn::learn(const std::vector<double> &targetVals)
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
        (dynamic_cast<srnNeuron*>(outputLayer[n]))->calcOutputGradients(targetVals[n]);    
    }

    // Calculate hidden layer gradients, start at rightmost hidden layer
    for (unsigned nbLayer = m_layers.size() - 2; nbLayer > 0; --nbLayer) 
    {
        Layer &hiddenLayer = m_layers[nbLayer];
        Layer &nextLayer = m_layers[nbLayer + 1];

        for (unsigned n = 0; n < hiddenLayer.size(); ++n) 
        {
            (dynamic_cast<srnNeuron*>(hiddenLayer[n]))->calcHiddenGradients(nextLayer, m_bias);    
        }
    }

    // For all layers from outputs to first hidden layer,
    // update connection weights

    for (unsigned nbLayer = m_layers.size() - 1; nbLayer > 0; --nbLayer)
    {
        unsigned nbNeuronsInLayer = m_layers[nbLayer].size() - (m_bias*1);

        for (unsigned n = 0; n < nbNeuronsInLayer; ++n) 
            m_layers[nbLayer][n]->updateInputWeights(m_layers, nbLayer);
    }
}

void srn::unroll()
{
    std::vector<Layer>::iterator it = (m_layers.end())-1;
    std::vector<Layer>::iterator itMinus = it - 1;

    m_layers.insert(it, Layer());

    unsigned numOutputs = m_layers.end()->size();
    unsigned neuronNum = 0;

    for (; neuronNum < it->size(); ++neuronNum) 
        it->push_back(new srnNeuron(numOutputs, neuronNum, MOMENTUM, LEARNING_RATE));

    // Copy bias neuron for output layer to rightmost hidden layer
    it->push_back(new srnNeuron(*dynamic_cast<srnNeuron*>(itMinus->back())));
    delete itMinus->back();

    // Save current input x_T of preceeding hidden layer by creating constant neurons in the same layer.
    // Those neurons have only one link
    neuronNum = itMinus->size()-1;
    unsigned neuronsToAdd = itMinus->size();

    for (unsigned n = 0; n < neuronsToAdd; ++n, ++neuronNum)
    {
        itMinus->push_back(new srnNeuron(1, neuronNum, MOMENTUM, LEARNING_RATE));
        double x_T = dynamic_cast<srnNeuron*>((*itMinus)[n])->getInput();
        itMinus->back()->setOutputVal(x_T);
        dynamic_cast<srnNeuron*>(itMinus->back())->singleConnection(n, 1.0);
    }
}
