#include <cassert>
#include <iostream>
#include <random>

#include "srn.hpp"
#include "srnNeuron.hpp"

SRN::SRN(const std::vector<unsigned> &topology, const bool &bias)
{
    unsigned numLayers = topology.size();

    m_bias                         = bias;
    m_recentAverageSmoothingFactor = 100.0; // Number of training samples to average over
    m_initialY                     = 0.0; 
    m_inputNeurons                 = topology[0];
    m_iterations                   = 0;
    m_recentAverageError           = 0.0;
    m_recentAverageSmoothingFactor = 0.0;
    m_timeHorizon                  = 0;

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
            m_layers.back().push_back(std::unique_ptr<SRNNeuron>(new SRNNeuron(numOutputs, neuronNum, MOMENTUM, LEARNING_RATE)));

        // Create bias neurons if requested
        if(false != bias)
        {
            m_layers.back().push_back(std::unique_ptr<SRNNeuron>(new SRNNeuron(numOutputs, ++neuronNum, MOMENTUM, LEARNING_RATE)));
            // Force the bias node's output to 1.0 (it was the last recent neuron pushed in this layer):
            m_layers.back().back()->setOutputVal(1.0);
        }
    }
}

SRN::SRN(const std::vector<unsigned> &topology, const size_t &timeHorizon, const bool &bias):
    m_timeHorizon(timeHorizon)
{
    unsigned numLayers = topology.size();

    m_bias                         = bias;
    m_recentAverageSmoothingFactor = 100.0; // Number of training samples to average over
    m_initialY                     = 0.0; 
    m_inputNeurons                 = topology[0];
    m_iterations                   = 0;
    m_recentAverageError           = 0.0;
    m_recentAverageSmoothingFactor = 0.0;

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
            m_layers.back().push_back(std::unique_ptr<SRNNeuron>(new SRNNeuron(numOutputs, neuronNum, MOMENTUM, LEARNING_RATE)));
            if((topology.size() - 1 != nbLayer) && (0 != nbLayer))
            {
                // Hidden layer, create input queues for function unrolling
                m_recentInputs.push_back(std::queue<double>());

                // Create random initialized recurrent weights
                m_recurrentWeights.push_back(rand() / double(RAND_MAX));
                m_recurrentDeltaWeights.push_back(0.0);
            }
        }

        // Create bias neurons if requested
        if(false != bias)
        {
            m_layers.back().push_back(std::unique_ptr<SRNNeuron>(new SRNNeuron(numOutputs, ++neuronNum, MOMENTUM, LEARNING_RATE)));
            // Force the bias node's output to 1.0 (it was the last recent neuron pushed in this layer):
            m_layers.back().back()->setOutputVal(1.0);
        }
    }
}

SRN::~SRN()
{
}

// Overwrite base class method to implement function unrolling
void SRN::feedForward(const std::vector<double> &inputVals)
{
    // Assign (latch) the input values into the input neurons
    for (unsigned i = 0; i < inputVals.size(); ++i) 
        m_layers[0][i]->setOutputVal(inputVals[i]);

    // Save current input of each hidden layer
    for (unsigned nbLayer = 1; nbLayer < m_layers.size()-1; ++nbLayer) 
    {
        unsigned nbNeuronsInLayer = m_layers[nbLayer].size() - (1*m_bias);
        double   currentNeuronInput = 0.0;
        for (unsigned n = 0; n < nbNeuronsInLayer; ++n) 
        {
            currentNeuronInput = dynamic_cast<SRNNeuron*>(m_layers[nbLayer][n].get())->getInput(m_layers, nbLayer);
            m_recentInputs[n].push(currentNeuronInput);
        }
    }

    // Before unrolling
//    //std::cerr << "Before unrolling: " << std::endl;
    for (unsigned nbLayer = 0; nbLayer < m_layers.size(); ++nbLayer) 
    {
        unsigned nbNeuronsInLayer = m_layers[nbLayer].size();
        //std::cerr << "Layer: " << nbLayer << ", #Neurons in Layer: " << nbNeuronsInLayer << std::endl;
    }

    // Unroll the network 
    unroll(m_iterations);

    // After unrolling
    //std::cerr << "After unrolling: " << std::endl;
    for (unsigned nbLayer = 0; nbLayer < m_layers.size(); ++nbLayer) 
    {
        unsigned nbNeuronsInLayer = m_layers[nbLayer].size();
        //std::cerr << "Layer: " << nbLayer << ", #Neurons in Layer: " << nbNeuronsInLayer << std::endl;
    }

    // forward propagate
    for (unsigned nbLayer = 1; nbLayer < m_layers.size(); ++nbLayer) 
    {
        unsigned nbNeuronsInLayer = m_layers[nbLayer].size() - (1*m_bias);
        for (unsigned n = 0; n < nbNeuronsInLayer; ++n) 
        {
//            //std::cerr << "Neuron output #" << n << ": " << m_layers[nbLayer][n]->m_outputVal << std::endl;
            m_layers[nbLayer][n]->feedForward(m_layers, nbLayer);
        }
    }
}

void SRN::learn(const std::vector<double> &targetVals)
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
        (dynamic_cast<SRNNeuron*>(outputLayer[n].get()))->calcOutputGradients(targetVals[n]);    
    }

    // Calculate hidden layer gradients, start at rightmost hidden layer
    for (unsigned nbLayer = m_layers.size() - 2; nbLayer > 0; --nbLayer) 
    {
        Layer &hiddenLayer = m_layers[nbLayer];
        Layer &nextLayer = m_layers[nbLayer + 1];

        for (unsigned n = 0; n < hiddenLayer.size(); ++n) 
        {
            (dynamic_cast<SRNNeuron*>(hiddenLayer[n].get()))->calcHiddenGradients(nextLayer, m_bias);    
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

    // Rollup the network
    rollup(m_iterations);

    if (m_timeHorizon > m_iterations) 
        m_iterations++;
}

void SRN::unroll(const size_t &times)
{
    if (0 == times)
        return;

    // Rightmost hidden layer
    std::vector<Layer>::iterator it = m_layers.end() - 2;

    // Unfolding does not include the bias neuron
    unsigned neuronsToAdd = it->size() - (1*m_bias);;
    assert(3 == neuronsToAdd);

    // Create the unfolded layers
    for (unsigned i = 0; i < times; ++i)
    {
        // Create a new layer for the respective time step, set iterator to newly created layer.
        it = m_layers.insert(it, Layer());

        unsigned neuronNum = 0;
        unsigned numOutputs = 1;
        for (; neuronNum < neuronsToAdd; ++neuronNum) 
        {
            // Neurons created during unfolding only have one connection to themselves 
            // in the next time step
            it->push_back(std::unique_ptr<SRNNeuron>(new SRNNeuron(numOutputs, neuronNum, MOMENTUM, LEARNING_RATE)));

            // g value = weight of the feedback connection
            unsigned connections = it->back()->m_outputWeights.size();
            for (unsigned i = 0; i < connections; ++i)
            {
                if (neuronNum == i)
                {
                    it->back()->m_outputWeights[i].weight      = m_recurrentWeights[neuronNum]; 
                    it->back()->m_outputWeights[i].deltaWeight = m_recurrentDeltaWeights[neuronNum]; 
                }
                else
                {
                    it->back()->m_outputWeights[i].weight      = 0.0;
                    it->back()->m_outputWeights[i].deltaWeight = 0.0;
                }
            }

            numOutputs++; 
        }

        numOutputs = 1;
        // Input x(t) will be added by creating a constant neuron in the preceding layer.
        // Those neurons have only one link
        for (unsigned n = 0; n < neuronsToAdd; ++n, ++neuronNum)
        {
            it->push_back(std::unique_ptr<SRNNeuron>(new SRNNeuron(numOutputs, neuronNum, MOMENTUM, LEARNING_RATE)));

            std::queue<double> newQueue;
            
            if ((0 != n) || (times != m_timeHorizon)) 
            {
                // In case of an unfilled queue or time step < time horizon, we want to keep the input
                double x_T = m_recentInputs[n].front();
                m_recentInputs[n].pop();
                newQueue.push(x_T);

                it->back()->setOutputVal(x_T);
            }
            else
            {
                // Input of the least recent time step is not used anymore if queue has reached its
                // max. size
                double x_T = m_recentInputs[n].front();
                m_recentInputs[n].pop();

                it->back()->setOutputVal(x_T);
            }

            // w value = weight of input onnection
            unsigned connections = it->back()->m_outputWeights.size();
            for (unsigned i = 0; i < connections; ++i)
            {
                if (neuronNum == i)
                {
                    it->back()->m_outputWeights[i].weight      = 1.0;
                    it->back()->m_outputWeights[i].deltaWeight = 0.0;
                }
                else
                {
                    it->back()->m_outputWeights[i].weight      = 0.0;
                    it->back()->m_outputWeights[i].deltaWeight = 0.0;
                }
            }

            m_recentInputs[n] = newQueue;
            numOutputs++;
        }
    }

    it = m_layers.begin();
    unsigned neuronNum  = it->size();
    it++;
    unsigned numOutputs = it->size()/2;
    it--;
    
    // Add initial y(0) in input layer, provide input for all unfolded neurons, ignore
    // constant neurons which provide respective x(t). i.e. only keep weights for first n neurons
    it->push_back(std::unique_ptr<SRNNeuron>(new SRNNeuron(numOutputs, neuronNum, MOMENTUM, LEARNING_RATE)));

    // Set y(0), zero magnitude vector in the first iteration
    // TODO: Set initial_y to last result
    unsigned connections = it->back()->m_outputWeights.size();
    for (unsigned c = 0; c < connections ; ++c) 
    {
        it->back()->m_outputWeights[c].weight = m_initialY;
        it->back()->m_outputWeights[c].deltaWeight = 0.0;
    }
}

void SRN::rollup(const size_t &times)
{
    if (0 == times)
        return;

    // First hidden layer
    std::vector<Layer>::iterator it = m_layers.begin() + 1;

    // n doubles with value 0.0
    std::vector<double> weightSum(times, 0.0);
    std::vector<double> deltaWeightSum(times, 0.0);


    // Get all weights
    for (unsigned i = 0; i < times; ++i)
    {
        unsigned copiedNeurons = it->size()/2;

        assert(3 == copiedNeurons);

        for (unsigned j = 0; j < copiedNeurons; ++j)
        {
            // TODO: true?
            //assert(0.0 != (*it)[j]->m_outputWeights[0].weight);
            weightSum[j]      += (*it)[j]->m_outputWeights[0].weight;
            deltaWeightSum[j] += (*it)[j]->m_outputWeights[0].deltaWeight;
        }
        it++;
    }

    // Iterator now at rightmost hidden layer
    assert(3 == it->size());

    // Clean up input layer 
    std::vector<Layer>::iterator input_it = m_layers.begin();

    // Number of neurons in the input which serve as initial y(0)
    assert(3 == input_it->size());
    input_it->pop_back();
    assert(2 == input_it->size());
    assert(input_it->size() == m_inputNeurons);

    it = input_it + 1;
    // Clean up unfolded layers (all layers before the rightmost hidden layer)
    while(it != m_layers.end() - 2)
    {
        unsigned nbNeuronsInLayer = it->size();
        for (unsigned i = 0; i < nbNeuronsInLayer; ++i)
            // Delete all the neurons in the unfolded layers
            it->pop_back();

        assert(0 == it->size());

        // Delete the unfolded layer itself
        it = m_layers.erase(it);
        //assert(3 == it->size());
    }

    //assert(m_layers.size() == 3);

    // Calculate average weight
    unsigned size = weightSum.size();
    for (unsigned i = 0; i < size; ++i) 
    {
        weightSum[i] /= times;
        deltaWeightSum[i] /= times;

        m_recurrentWeights[i] = weightSum[i];
        m_recurrentDeltaWeights[i] = deltaWeightSum[i];
    }
}
