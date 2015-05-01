#ifndef COMMONDEFS_HPP
#define COMMONDEFS_HPP 

#include<vector>

#define LEARNING_RATE (0.15)
#define MOMENTUM (0.5)

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

#endif /* COMMONDEFS_HPP */
