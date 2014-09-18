//------------------------------------------------------
//Author             : Jonathan Schwarz
//University         : Pforzheim University
//Date of last edit  : Mon, 15 Sep 2014 16:29:12 +0200
//Filename           : srn.hpp
//------------------------------------------------------

#ifndef SRN_HPP
#define SRN_HPP

#include "net.hpp"
#include "commonDefs.hpp"
#include "neuron.hpp"

// Abstract class
class srn : public Net
{
public:
    srn(const std::vector<unsigned> &topology, const bool &bias);
    virtual ~srn();

    virtual void learn(const std::vector<double> &targetVals);
};

#endif /* SRN_HPP */
