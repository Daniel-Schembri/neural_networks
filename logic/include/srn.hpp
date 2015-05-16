#ifndef SRN_HPP
#define SRN_HPP

#include "net.hpp"
#include "commonDefs.hpp"
#include "neuron.hpp"

#include <queue>

// Abstract class
class srn : public Net
{
public:
    srn(const std::vector<unsigned> &topology, const bool &bias);
    srn(const std::vector<unsigned> &topology, const size_t &timeHorizon, const bool &bias);
    virtual ~srn();

    double getRecentAverageError() const { return m_recentAverageError; }

    void feedForward(const std::vector<double> &inputVals);
    virtual void learn(const std::vector<double> &targetVals);
    void unroll(const size_t &times); // Function unrolling required by the BTT algorithm
    void rollup(const size_t &times);

protected:
    double m_error;
    double m_initialX; 
    unsigned m_inputNeurons; 
    size_t m_iterations; // Keeps track of the learning iteration,
                        // slightly different treatment for m_iterations < m_timeHorizon;
    double m_recentAverageError;
    double m_recentAverageSmoothingFactor;
    std::vector<std::queue<double> m_recentInputs;
    std::vector<double> m_recurrentWeights;
    std::vector<double> m_recurrentDeltaWeights;
    size_t m_timeHorizon;
};

#endif /* SRN_HPP */
