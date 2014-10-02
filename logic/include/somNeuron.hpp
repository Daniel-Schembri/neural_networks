#ifndef SOMNEURON_HPP
#define SOMNEURON_HPP 

#include "commonDefs.hpp"
#include "neuron.hpp"

class somNeuron : public Neuron
{
public:
    somNeuron(unsigned numOutputs, unsigned myIndex, double eta);
    virtual ~somNeuron();

    inline double getEta() const;
    inline void setEta(double newEta);

    virtual void feedForward(const std::vector<Layer> &layers, const unsigned &myLayer);
    virtual void updateInputWeights(const std::vector<Layer> &layers, const unsigned &myLayer);

protected:
    double getDistance(const double &x, const double &y);
    std::vector<unsigned> getNeighbors(const unsigned &reference);
};

#endif /* SOMNEURON_HPP */
