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

    double getDistance(const double &x, const double &y) const;
};

#endif /* SELF_ORGANIZING_MAP_HPP */
