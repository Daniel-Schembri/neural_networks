#ifndef COMMONDEFS_HPP
#define COMMONDEFS_HPP 

#include<vector>

#define LEARNING_RATE (0.15)
#define MOMENTUM (0.5)
#define SIGHT_RADIUS (20.0)

class Neuron;

typedef std::vector<Neuron*> Layer;

//connection between neurons
struct Connection
{
    double weight;
    double deltaWeight;
};

typedef std::vector<std::vector<std::vector<Connection>>> WeightMatrix; 

template <typename T> int sgn(T val)
{
        return (T(0) < val) - (val < T(0));
}

#endif /* COMMONDEFS_HPP */
