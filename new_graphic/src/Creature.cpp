#include "Creature.h"


Creature::Creature()
{
fitness = 0; lastfitness = 0; bestfitness = 0;
}

Creature::Creature(int phealth, float pposx, float pposy, int pid, std::vector<unsigned> ptopology)
{
fitness = 0; lastfitness = 0; bestfitness = 0; id=pid;

health = phealth; posx = pposx; posy = pposy;
topology = ptopology;
mynet = new Net(topology, true);
}

/*
Creature::~Creature()
{
delete health; delete posx; delete posy; delete angle;
}
*/

void Creature::correct_angle()
{
//Correction of PlayerAngle. //TODO: Performance at high Angle Value => Needs more loops to correct angle!
    while ( angle < -180 * DEGTORAD) angle += 360*DEGTORAD;
    while ( angle >  180 * DEGTORAD) angle -= 360*DEGTORAD;	
}

//Neural Net methods
void Creature::addLayer(int amountneurons)
{
  topology.push_back(amountneurons);
}

void Creature::randomize_net()
{
   mynet->randomize_net();
}

std::vector<double> Creature::process(std::vector<double> inputvals)
{
   std::vector<double> resultvals;

   mynet->feedForward(inputvals);
   mynet->getResults(resultvals);

   return resultvals;

}

/*
float32 a = fabs(player->GetPosition().x - food[i]->GetPosition().x);
float32 b = fabs(player->GetPosition().y - food[i]->GetPosition().y); //Fabs ist Betrag
*/
