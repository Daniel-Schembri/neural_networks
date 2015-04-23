#ifndef Agent_H
#define Agent_H

#define M_PI 3.14159265359
#define DEGTORAD (M_PI/180)

#include "commonDefs.hpp"
#include "feedForwardNet.hpp"
#include "srn.hpp"

// This is used to test sensor shapes.
class Agent
{
    //Attributes
    public:
        Net* mynet;  //The Neural Net
        int fitness, lastfitness; // bestfitness;

        //Output
        //std::vector<double> outputvals;

    private:

        std::vector<unsigned> topology;

        int id; int nettype; 
        float posx, posy, angle;  //NU

        //Methods
    public:

        Agent();
        ~Agent();
        Agent(float pposx, float pposy, int pid, std::vector<unsigned> ptopology, int pnet_type);
        Agent(float pposx, float pposy, int pid, std::vector<unsigned> ptopology,  int pnet_type,
              vector<vector<vector<Connection> > > pweights);

        std::vector<double> process(std::vector<double> inputVals);  //Give input in the neural network and get output

        void learn(std::vector<double> ptrainingdata_output);
};
#endif
