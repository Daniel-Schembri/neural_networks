//------------------------------------------------------
//Author             : Jonathan Schwarz
//University         : Pforzheim University
//Date of last edit  : Mon, 08 Sep 2014 11:10:32 +0200
//Filename           : net.cpp
//------------------------------------------------------

#include "neural_network/net.hpp"

Net::Net(const std::vector<unsigned> &topology, const bool &bias)
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
            m_layers.back().push_back(new Neuron(LEARNING_RATE, neuronNum, numOutputs));

        // Create bias neurons if requested
        if(false != bias)
        {
            m_layers.back().push_back(new Neuron(LEARNING_RATE, ++neuronNum, numOutputs));
            // Force the bias node's output to 1.0 (it was the last recent neuron pushed in this layer):
            m_layers.back().back()->setOutputVal(1.0);
        }
    }
}

Net::~Net()
{
    for (unsigned i = 0; i < m_layers.size(); ++i)
        for (unsigned j = 0; j < m_layers[i].size(); ++j)
            delete m_layers[i][j];
}

void Net::randomize_net()
{
   if (m_layers.empty())
       return;
   
   for(unsigned i = 0; i < m_layers.size(); i++)
   {
     for(unsigned j = 0; j < m_layers[i].size(); j++)
     {
       for(unsigned n = 0; n < m_layers[i][j]->m_outputWeights.size(); n++)
          m_layers[i][j]->m_outputWeights[n].weight =(((((double) rand() / double(RAND_MAX))* 2)-1.0f)*(int) 1000) / (1000.0f); //Random value between -1.0f and 1.0f. Truncate after 3 digits, so we get a resolution of 0.001f
     }
   }
}

std::vector<double> Net::get_neuron_weights(unsigned int layernr, unsigned int neuronnr)
{
  std::vector<double> weights;
  for (int i=0;i<m_layers[layernr][neuronnr]->m_outputWeights.size();i++)
    weights.push_back(m_layers[layernr][neuronnr]->m_outputWeights[i].weight);
 
  return weights;
}

void Net::set_weights(unsigned int layernr, unsigned int neuronnr, std::vector<double> weights)
{ 
  for (unsigned int i=0;i<weights.size();i++)
  {
   if(weights[i] < -1.0f || weights[i] > 1.0f) 
      return;
  }
  for (unsigned int i=0;i<m_layers[layernr][neuronnr]->m_outputWeights.size();i++)
    m_layers[layernr][neuronnr]->m_outputWeights[i].weight = weights[i]; //Random value between -1.0f and 1.0f
}

std::vector<unsigned int> Net::sizeof_layers()
{
  std::vector<unsigned int> size;

  for(unsigned i = 0; i < m_layers.size(); i++)
  {
     size.push_back(m_layers[i].size());
  }
  return size;
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
            m_layers[nbLayer][n]->feedForward(m_layers[nbLayer - 1]);
    }
}
