#ifndef SRNNEURON_HPP
#define SRNNEURON_HPP 

#include "commonDefs.hpp"
#include "neuron.hpp"

class srnNeuron : public Neuron
{
public:
    srnNeuron(unsigned numOutputs, unsigned myIndex,double alpha, double eta);
    virtual ~srnNeuron();

    void calcHiddenGradients(const Layer &nextLayer, const bool &bias);
    void calcOutputGradients(double targetVal);
    virtual void feedForward(const std::vector<Layer> &layers, const unsigned &myLayer);
    inline double getGradient() const;
    virtual void updateInputWeights(const std::vector<Layer> &layers, const unsigned &myLayer);

protected:
    double m_alpha; // [0.0..n] multiplier of last weight change (momentum)
    double m_gradient;

    double sumDOW(const Layer &nextLayer, const bool &bias) const;
    static double transferFunctionDerivative(double x);
};

#endif /* SRNNEURON_HPP */
