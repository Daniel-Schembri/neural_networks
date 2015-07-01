#ifndef Agent_H
#define Agent_H

#define DEGTORAD (M_PI/180)

#include <math.h>

#include "commonDefs.hpp"
#include "feedForwardNet.hpp"
#include "srn.hpp"
#include "script.hpp"

//Nettypes
#define NET_FEEDFORWARD   0
#define NET_SRN           1 
#define NET_SCRIPT        2

class Agent
{
    //Attributes
    public:
        Net* velocity_net;        //The Neural Net used to compute the agent's velocity
        Net* angle_net;           //The Neural Net used to compute the agent's angle
        Script* myscript;         //Script to directly test functions
        int fitness, lastfitness; // bestfitness;

    private:
        unsigned int idleness_count;
        std::vector<unsigned> topology;

        int id; int nettype; 
        float posx, posy, angle;

        //Methods
    public:

        Agent();
        Agent(const Agent &obj);
        ~Agent();

        Agent(float pposx, float pposy, int pid, 
                std::vector<unsigned> ptopology, int pnet_type);
        Agent(float pposx, float pposy, int pid, 
                std::vector<unsigned> ptopology, int pnet_type, WeightMatrix &pweights);

        int getid() const;
        int getnettype() const;
        std::vector<unsigned> gettopology() const;
        //Obtain the agent's velocity from the current sensor values
        double processV(std::vector<double> inputVals);

        //Obtain the agent's angle from the current sensor values
        double processA(std::vector<double> inputVals);

        // Used to teach angle_net
        void learnA(std::vector<double> ptrainingdata_output);
        // Used to teach velocity_net
        void learnV(std::vector<double> ptrainingdata_output);

        bool operator== (const Agent &other) const;
};

#endif
