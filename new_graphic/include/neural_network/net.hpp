//------------------------------------------------------
//Author             : Jonathan Schwarz
//University         : Pforzheim University
//Date of last edit  : Mon, 15 Sep 2014 16:09:44 +0200
//Filename           : net.hpp
//------------------------------------------------------


#ifndef NET_HPP
#define NET_HPP

#include <vector>

#include "commonDefs.hpp"
#include "neuron.hpp"

class Net
{
public:
    Net();
    Net(const std::vector<unsigned> &topology, const bool &bias);
    ~Net();

    void getResults(std::vector<double> &resultVals) const;
    void randomize_net();
    std::vector<double> get_neuron_weights(unsigned int layernr, unsigned int neuronnr);
    void set_weights(unsigned int layernr, unsigned int neuronnr, std::vector<double> weights);
    std::vector<unsigned int> sizeof_layers();
    void feedForward(const std::vector<double> &inputVals);

    std::vector<Layer> m_layers;
protected:

    bool m_bias;
};

#endif /* NET_HPP */
