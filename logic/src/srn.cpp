//------------------------------------------------------
//Author             : Jonathan Schwarz
//University         : Pforzheim University
//Date of last edit  : Mon, 15 Sep 2014 16:31:35 +0200
//Filename           : srn.cpp
//------------------------------------------------------

#include <random.h>

#include "srn.hpp"
#include "srnNeuron.hpp"

srn::srn(const std::vector<unsigned> &topology, const bool &bias)
{
    m_bias = bias;
    unsigned numLayers = topology.size();

    m_recentAverageSmoothingFactor = 100.0; // Number of training samples to average over
    m_initialX                     = 0.0; 
    m_iterations                   = 0;


    for (unsigned nbLayer = 0; nbLayer < numLayers; ++nbLayer) 
    {
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
            m_layers.back().push_back(new srnNeuron(numOutputs, neuronNum, MOMENTUM, LEARNING_RATE));

        // Create bias neurons if requested
        if(false != bias)
        {
            m_layers.back().push_back(new srnNeuron(numOutputs, ++neuronNum, MOMENTUM, LEARNING_RATE));
            // Force the bias node's output to 1.0 (it was the last recent neuron pushed in this layer):
            m_layers.back().back()->setOutputVal(1.0);
        }
    }
}

srn(const std::vector<unsigned> &topology, const size_t &timeHorizon, const bool &bias)
{
    m_bias = bias;
    m_timeHorizon = m_timeHorizon;

    unsigned numLayers = topology.size();

    m_recentAverageSmoothingFactor = 100.0; // Number of training samples to average over
    m_initialX   = 0.0; 
    m_iterations = 0;

    for (unsigned nbLayer = 0; nbLayer < numLayers; ++nbLayer) 
    {
        m_layers.push_back(Layer());
        unsigned numOutputs;
        unsigned neuronNum = 0;

        if(topology.size() - 1 == nbLayer)
           // Output neurons don't have outputs
           numOutputs = 0;
        else
           // The number of outputs is equal to the number of neurons in the next layer
           numOutputs = topology[nbLayer + 1];


        // We have a new layer, now fill it with neurons
        for (neuronNum = 0; neuronNum < topology[nbLayer]; ++neuronNum) 
        {
            m_layers.back().push_back(new srnNeuron(numOutputs, neuronNum, MOMENTUM, LEARNING_RATE));
            if((topology.size() - 1 != nbLayer) && (0 != nbLayer))
            {
                // Hidden layer, create input queues for function unrolling
                m_recurrentWeights.push_back(rand() / double(RAND_MAX));
                m_recurrentDeltaWeights.push_back(0.0);
            }
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

    // Save current input for the hidden layer
    for (unsigned nbLayer = 1; nbLayer < m_layers.size()-1; ++nbLayer) 
    {
        unsigned nbNeuronsInLayer = m_layers[nbLayer].size() - (1*m_bias);
        double   currentNeuronInput = 0.0;
        for (unsigned n = 0; n < nbNeuronsInLayer; ++n) 
        {
            currentNeuronInput = m_layers[nbLayer][n]->getInput(m_layers, nbLayer);
            m_recentInputs[0].push(currentNeuronInput);
        }
    }

    m_iterations++;
    if (m_timeHorizon > m_iterations) 
        unroll(m_iterations);
    else
        unroll(m_timeHorizon);

    // forward propagate
    for (unsigned nbLayer = 1; nbLayer < m_layers.size(); ++nbLayer) 
    {
        unsigned nbNeuronsInLayer = m_layers[nbLayer].size() - (1*m_bias);
        for (unsigned n = 0; n < nbNeuronsInLayer; ++n) 
            m_layers[nbLayer][n]->feedForward(m_layers, nbLayer);
    }
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

void srn::unroll(const size_t &times)
{
    std::vector<Layer>::iterator it = (m_layers.end()) - 1;

    // Unfolding does not include the bias neuron
    unsigned neuronsToAdd = it.size() - (1*m_bias);;

    for (int i = 0; i < times; ++i)
    {
        // Create a new layer for the respective time step, set iterator to newly created layer.
        m_layers.insert(it--, Layer());
        std::vector<Layer>::iterator itMinus = it - 1;

        unsigned neuronNum = 0;

        for (; neuronNum < neuronsToAdd; ++neuronNum) 
        {
            // Neurons created during unfolding only have one connection to themselves 
            // in the next time step
            it->push_back(new srnNeuron(0, neuronNum, MOMENTUM, LEARNING_RATE));

            // Weight of the feedback connection
            it->back().m_outputWeights[0].weight      = m_recurrentWeights[neuronNum];
            it->back().m_outputWeights[0].deltaWeight = m_recurrentDeltaWeights[neuronNum];
        }

        // Input x(t) will be added by creating a constant neuron in the preceding layer.
        // Those neurons have only one link
        for (unsigned n = 0; n < neuronsToAdd; ++n, ++neuronNum)
        {
            it->push_back(new srnNeuron(1, neuronNum, MOMENTUM, LEARNING_RATE));

            std::queue<double> newQueue;
            
            if ((0 != n) || (times != m_timeHorizon)) 
            {
                double x_T = m_recentInputs[0].pop();
                newQueue.push(x_T);
            }
            else
                double x_T = m_recentInputs[0].pop();

            it->back()->setOutputVal(x_T);

            // w value
            dynamic_cast<srnNeuron*>(it->back())->singleConnection(n, 1.0, 0.0);

            m_recentInputs[0] = newQueue;
        }

    }
}

void srn::rollup(const size_t &times)
{
}
