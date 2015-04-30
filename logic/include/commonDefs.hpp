#ifndef COMMONDEFS_HPP
#define COMMONDEFS_HPP 

#include<vector>

#define LEARNING_RATE (0.15)
#define MOMENTUM (0.5)
#define SIGHT_RADIUS (20.0)

using std::vector;

class Neuron;

typedef vector<Neuron*> Layer;

//connection between neurons
struct Connection
{
    double weight;
    double deltaWeight;
};

typedef vector<vector<vector<Connection>>> WeightMatrix; 

template <typename T> int sgn(T val)
{
        return (T(0) < val) - (val < T(0));
}

#endif /* COMMONDEFS_HPP */
