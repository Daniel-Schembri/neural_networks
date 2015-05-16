#ifndef SRNNEURON_HPP
#define SRNNEURON_HPP 

#include "commonDefs.hpp"
#include "neuron.hpp"

class srnNeuron : public Neuron
{
public:
    srnNeuron(unsigned numOutputs, unsigned myIndex,double alpha, double eta);
    virtual ~srnNeuron();

    double getGradient() const;
    double getInput() const {return m_input;}

    virtual void feedForward(const std::vector<Layer> &layers, const unsigned &myLayer);
    double getInput(const std::vector<Layer> &layers, const unsigned &myLayer);
    void calcHiddenGradients(const Layer &nextLayer, const bool &bias);
    void calcOutputGradients(double targetVal);
    // Ensures that there is only one connection to the specified index
    void singleConnection(const unsigned &index, const double &weight = 1.0, const double &deltaWeight = 1.0); 
    virtual void updateInputWeights(const std::vector<Layer> &layers, const unsigned &myLayer);

protected:
    double m_alpha; // [0.0..n] multiplier of last weight change (momentum)
    double m_gradient;
    double m_input; // Last input of the Neuron, needed for function unrolling

    double sumDOW(const Layer &nextLayer, const bool &bias) const;
    static double transferFunctionDerivative(double x);
};

#endif /* SRNNEURON_HPP */
