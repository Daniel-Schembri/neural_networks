#ifndef COMPETITIVE_HPP
#define COMPETITIVE_HPP

#include "net.hpp"
#include "commonDefs.hpp"
#include "neuron.hpp"

// Abstract class
class Competitive : public Net
{
public:
    Competitive(const std::vector<unsigned> &topology, const bool &bias);
    virtual ~Competitive();

    virtual void learn(const std::vector<double> &targetVals);
};

#endif /* COMPETITIVE_HPP */
