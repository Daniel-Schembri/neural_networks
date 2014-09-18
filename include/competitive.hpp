//------------------------------------------------------
//Author             : Jonathan Schwarz
//University         : Pforzheim University
//Date of last edit  : Mon, 15 Sep 2014 16:29:12 +0200
//Filename           : competitive.hpp
//------------------------------------------------------

#ifndef COMPETITIVE_HPP
#define COMPETITIVE_HPP

#include "net.hpp"
#include "commonDefs.hpp"
#include "neuron.hpp"

struct neuronIndex
{
    unsigned layer;
    unsigned neuron;
};

// Abstract class
class Competitive : public Net
{
public:
    Competitive(const std::vector<unsigned> &topology, const bool &bias);
    virtual ~Competitive();

    virtual void learn(const std::vector<double> &targetVals);
};

#endif /* COMPETITIVE_HPP */
