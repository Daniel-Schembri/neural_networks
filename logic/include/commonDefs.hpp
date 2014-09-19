#ifndef COMMONDEFS_HPP
#define COMMONDEFS_HPP 

class Neuron;

typedef std::vector<Neuron> Layer;

//connection between neurons
struct Connection
{
    double weight;
    double deltaWeight;
};

#endif /* COMMONDEFS_HPP */
