#ifndef COMMONDEFS_HPP
#define COMMONDEFS_HPP 

#include<vector>

#define LEARNING_RATE (0.15)
#define MOMENTUM (0.5)

class Neuron;

typedef std::vector<Neuron*> Layer;

//connection between neurons
struct Connection
{
    double weight;
    double deltaWeight;
};

#endif /* COMMONDEFS_HPP */
