#ifndef COMMONDEFS_HPP
#define COMMONDEFS_HPP 

//neural network type
enum NET_TYPE {FEED_FORWARD, SIMPLE_RECURRENT};

class Neuron;

typedef std::vector<Neuron> Layer;

//connection between neurons
struct Connection
{
    double weight;
    double deltaWeight;
};

#endif /* COMMONDEFS_HPP */
