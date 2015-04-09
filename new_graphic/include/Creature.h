#ifndef CREATURE_H
#define CREATURE_H

#define M_PI 3.14159265359
#define DEGTORAD (M_PI/180)

#include "FeedForwardNet.hpp"

// This is used to test sensor shapes.
class Creature
{
public:
int health;
float posx, posy, angle;
int id;
int fitness, lastfitness, bestfitness;

//Processing (Neural Net)
std::vector<unsigned> topology;
Net* mynet;
//Output
std::vector<double> outputvals;

Creature();
//~Creature();
//Creature(int phealth, float pposx, float pposy);
Creature(int phealth, float pposx, float pposy, int pid, std::vector<unsigned> ptopology);

void randomize_net();
void setNet();
void setNet_zero();
void initNet();
std::vector<double> process(std::vector<double> inputVals);  //Give input in the neural network and get output
void addLayer(int amountneurons); //Add a layer to the NN-topology
void correct_angle();

void learn(std::vector<double> ptrainingdata_output);

};
#endif
