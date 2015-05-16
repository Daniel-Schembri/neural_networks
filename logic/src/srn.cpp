#include <cassert>
#include <random>

#include "srn.hpp"
#include "srnNeuron.hpp"

srn::srn(const std::vector<unsigned> &topology, const bool &bias)
{
    unsigned numLayers = topology.size();

    m_bias                         = bias;
    m_recentAverageSmoothingFactor = 100.0; // Number of training samples to average over
    m_initialX                     = 0.0; 
    m_inputNeurons                 = topology[0];
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

srn::srn(const std::vector<unsigned> &topology, const size_t &timeHorizon, const bool &bias):
    m_timeHorizon(timeHorizon)
{
    unsigned numLayers = topology.size();

    m_bias                         = bias;
    m_recentAverageSmoothingFactor = 100.0; // Number of training samples to average over
    m_initialX                     = 0.0; 
    m_inputNeurons                 = topology[0];
    m_iterations                   = 0;

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
                m_recentInputs.push_back(std::queue<double>());

                // Create random initialized recurrent weights
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

    assert(m_layers.size() <= 3);

    // Save current input of each hidden layer
    for (unsigned nbLayer = 1; nbLayer < m_layers.size()-1; ++nbLayer) 
    {
        unsigned nbNeuronsInLayer = m_layers[nbLayer].size() - (1*m_bias);
        double   currentNeuronInput = 0.0;
        for (unsigned n = 0; n < nbNeuronsInLayer; ++n) 
        {
            currentNeuronInput = dynamic_cast<srnNeuron*>(m_layers[nbLayer][n])->getInput(m_layers, nbLayer);
            m_recentInputs[n].push(currentNeuronInput);
        }
    }

    if (m_timeHorizon > m_iterations + 1) 
        unroll(++m_iterations);
    else
        unroll(m_iterations);

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

    // Rollup the network
    rollup(m_iterations);
}

void srn::unroll(const size_t &times)
{
    std::vector<Layer>::iterator it = (m_layers.end()) - 1;

    // Unfolding does not include the bias neuron
    unsigned neuronsToAdd = it->size() - (1*m_bias);;

    for (unsigned i = 0; i < times; ++i)
    {
        // Create a new layer for the respective time step, set iterator to newly created layer.
        m_layers.insert(it--, Layer());
        std::vector<Layer>::iterator itMinus = it - 1;

        unsigned neuronNum = 0;

        for (; neuronNum < neuronsToAdd; ++neuronNum) 
        {
            // Neurons created during unfolding only have one connection to themselves 
            // in the next time step
            it->push_back(new srnNeuron(1, neuronNum, MOMENTUM, LEARNING_RATE));

            // g value = weight of the feedback connection
            dynamic_cast<srnNeuron*>(it->back())->singleConnection(neuronNum, m_recurrentWeights[neuronNum],  m_recurrentDeltaWeights[neuronNum]);
        }

        // Input x(t) will be added by creating a constant neuron in the preceding layer.
        // Those neurons have only one link
        for (unsigned n = 0; n < neuronsToAdd; ++n, ++neuronNum)
        {
            it->push_back(new srnNeuron(1, neuronNum, MOMENTUM, LEARNING_RATE));

            std::queue<double> newQueue;
            
            if ((0 != n) || (times != m_timeHorizon)) 
            {
                // In case of an unfilled queue or time step < time horizon, we want to keep the input
                double x_T = m_recentInputs[n].front();
                m_recentInputs[n].pop()
                newQueue.push(x_T);

                it->back()->setOutputVal(x_T);
            }
            else
            {
                // Input of the least recent time step is not used anymore if queue has reached its
                // max. size
                double x_T = m_recentInputs[n].front()
                m_recentInputs[n].pop()
                it->back()->setOutputVal(x_T);
            }

            // w value
            dynamic_cast<srnNeuron*>(it->back())->singleConnection(n, 1.0, 0.0);

            m_recentInputs[n] = newQueue;
        }
    }

    it = m_layers.begin();
    unsigned neuronNum  = it.size();
    it++;
    unsigned numOutputs = it.size()/2;
    it--;
    
    // Add initial y(0) in input layer, provide input for all unfolded neurons, ignore
    // constant neurons which provide respective x(t) (i.e. size/2)
    it.push_back(new srnNeuron(numOutputs, neuronNum, MOMENTUM, LEARNING_RATE));

    // Set y(0), zero magnitude vector in the first iteration
    unsigned connections = it.back().m_outputWeights.size();
    for (int c = 0; c < connections ; ++c) 
    {
        it.back().m_outputWeights[c].weight = m_initialX;
        it.back().m_outputWeights[c].deltaWeight = 0.0;
    }
}

void srn::rollup(const size_t &times)
{
    std::vector<Layer>::iterator it = (m_layers.begin()) + 1;

    // n doubles with value 0.0
    std::vector<double> weightSum(times, 0.0);
    std::vector<double> deltaWeightsSum(times, 0.0);

    // Get all weights
    for (int i = 0; i < times; ++i)
    {
        unsigned copiedNeurons = it->size()/2;
        for (int j = 0; j < copiedNeurons; ++j)
        {
            // TODO: true?
            assert(0.0 != it[j].m_outputweights[0].weight);
            weightSum[j] += it[j].m_outputweights[0].weight;
            deltaWeightSum[j] += it[j].m_outputweights[0].deltaWeight;
        }

        it++;
    }

    // Clean up input layer 
    std::vector<Layer>::iterator input_it = m_layers.begin();
    assert(0 != m_inputNeurons);

    // Number of neurons in the input which serve as initial y(0)
    unsigned nbYZero = it.size()/2;

    for (int i = 0; i < nbYerzo; ++i) 
        delete input_it[m_inputNeurons + i];

    assert(input_it.size() == m_inputNeurons);

    // Clean up unfolded layers (all layers before the rightmost hidden layer)
    while(it != m_layers.end - 2)
    {
        for (int i = 0; i < c_it->size(); ++i)
        {
            // Delete all the neurons in the unfolded layers
            delete it[i];
        }

        // Delete the unfolded layer itself
        m_layers.erase(it);
    }

    assert(m_layers.size() == 3);

    // Calculate average weight
    unsigned size = weightSum.size();
    for (int i = 0; i < size; ++i) 
    {
        weightSum[i] /= times;
        deltaWeightSum[i] /= times;

        m_reccurentWeights[i] = weightSum[i];
        m_reccurentDeltaWeights[i] = deltaWeightSum[i];
    }
}
