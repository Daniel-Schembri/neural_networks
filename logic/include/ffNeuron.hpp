#ifndef FFNEURON_HPP
#define FFNEURON_HPP 

#include "commonDefs.hpp"
#include "neuron.hpp"

class ffNeuron : public Neuron
{
public:
    ffNeuron(unsigned numOutputs, unsigned myIndex, double alpha, double eta);
    virtual ~ffNeuron();

    void calcHiddenGradients(const Layer &nextLayer, const bool &bias);
    void calcOutputGradients(double targetVal);
    virtual void feedForward(const std::vector<Layer> &layers, const unsigned &myLayer);
    inline double getGradient() const;
    virtual void updateInputWeights(const std::vector<Layer> &layers, const unsigned &myLayer);

protected:
    double m_alpha; // [0.0..n] multiplier of last weight change (momentum)
    double m_gradient;
    double m_recentAverageError;
    double m_recentAverageErrorSmoothingFactor;

    double transferFunctionDerivative(double x) const;
    double sumDOW(const Layer &nextLayer, const bool &bias) const;
};

#endif /* FFNEURON_HPP */
