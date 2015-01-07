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

    double getRecentAverageError() const { return m_recentAverageError; }

    void feedForward(const std::vector<double> &inputVals);
    virtual void learn(const std::vector<double> &targetVals);
    void unroll(); // Does the function unrolling required by the BTT algorithm

protected:
    double m_error;
    double m_recurrentWeight;
    double m_recentAverageError;
    double m_recentAverageSmoothingFactor;
};
#endif /* SRN_HPP */
