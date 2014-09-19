//------------------------------------------------------
//Author             : Jonathan Schwarz
//University         : Pforzheim University
//Date of last edit  : Mon, 15 Sep 2014 16:25:34 +0200
//Filename           : feedForwardNet.hpp
//------------------------------------------------------

#ifndef FEED_FORWARD_NET_HPP
#define FEED_FORWARD_NET_HPP

#include "net.hpp"
#include "commonDefs.hpp"
#include "neuron.hpp"

// Abstract class
class FeedForwardNet : public Net
{
public:
    FeedForwardNet(const std::vector<unsigned> &topology, const bool &bias);
    virtual ~FeedForwardNet();

    virtual void learn(const std::vector<double> &targetVals);
};

#endif /* FEED_FORWARD_NET_HPP */
