//------------------------------------------------------
//Author             : Jonathan Schwarz
//University         : Pforzheim University
//Date of last edit  : Wed, 24 Sep 2014 07:59:00 +0200
//Filename           : selfOrganizingMap.hpp
//------------------------------------------------------

#ifndef SELF_ORGANIZING_MAP_HPP
#define SELF_ORGANIZING_MAP_HPP

#include "net.hpp"
#include "commonDefs.hpp"
#include "neuron.hpp"

class SelfOrganizingMap : public Net
{
public:
    SelfOrganizingMap(const std::vector<unsigned> &topology, const bool &bias, const unsigned &maxSteps);
    virtual ~SelfOrganizingMap();

    virtual void learn(const std::vector<double> &targetVals);

private:
    std::vector<unsigned> getNeighbors(const unsigned &reference);
    unsigned m_maxSteps;
    unsigned m_currentStep;

    inline double getDistance(const double &x, const double &y) const;
};

#endif /* SELF_ORGANIZING_MAP_HPP */
