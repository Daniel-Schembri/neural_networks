#ifndef SRN_HPP
#define SRN_HPP

#include "net.hpp"
#include "commonDefs.hpp"
#include "neuron.hpp"

#include <queue>
#include <vector>

// Abstract class
class SRN : public Net
{
public:
    SRN(const std::vector<unsigned> &topology, const bool &bias);
    SRN(const std::vector<unsigned> &topology, const size_t &timeHorizon, const bool &bias);
    virtual ~SRN();

    double getRecentAverageError() const { return m_recentAverageError; }

    void feedForward(const std::vector<double> &inputVals);
    virtual void learn(const std::vector<double> &targetVals);
    void unroll(const size_t &times); // Function unrolling required by the BTT algorithm
    void rollup(const size_t &times);

protected:
    double m_error;
    double m_initialY; 
    unsigned m_inputNeurons; 
    size_t m_iterations; // Keeps track of the learning iteration,
                         // slightly different treatment for m_iterations < m_timeHorizon;
    double m_recentAverageError;
    double m_recentAverageSmoothingFactor;
    std::vector<std::queue<double>> m_recentInputs;
    std::vector<double> m_recurrentWeights;
    std::vector<double> m_recurrentDeltaWeights;
    size_t m_timeHorizon;
};

#endif /* SRN_HPP */
