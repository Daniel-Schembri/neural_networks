#ifndef COMPETITIVENEURON_HPP
#define COMPETITIVENEURON_HPP 

#include "commonDefs.hpp"
#include "neuron.hpp"

class competitiveNeuron : public Neuron
{
public:
    competitiveNeuron(unsigned numOutputs, unsigned myIndex, double eta);
    virtual ~competitiveNeuron();

    virtual void feedForward(const std::vector<Layer> &layers, const unsigned &myLayer);
    virtual void updateInputWeights(const std::vector<Layer> &layers, const unsigned &myLayer);
};

#endif /* COMPETITIVENEURON_HPP */
