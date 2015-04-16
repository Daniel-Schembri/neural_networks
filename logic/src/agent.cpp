#include "Agent.h"


Agent::Agent()
{
fitness = 0; lastfitness = 0;
}

Agent::Agent(int phealth, float pposx, float pposy, int pid, std::vector<unsigned> ptopology)
{
fitness = 0; lastfitness = 0; id=pid;

health = phealth; posx = pposx; posy = pposy;
topology = ptopology;
mynet = new FeedForwardNet(topology, true);
}

/*
Agent::~Agent()
{
delete health; delete posx; delete posy; delete angle;
}
*/

//Neural Net methods

void Agent::randomize_net()
{
   mynet->randomize_net();
}

void Agent::setNet_zero()
{
	mynet->set_net_zero();
}


void Agent::learn(std::vector<double> ptrainingdata_output)
{
	mynet->learn(ptrainingdata_output);

}

std::vector<double> Agent::process(std::vector<double> inputvals)
{
   std::vector<double> resultvals;

   mynet->feedForward(inputvals);
   mynet->getResults(resultvals);

   return resultvals;

}

/*
float32 a = fabs(player->GetPosition().x - Object[i]->GetPosition().x);
float32 b = fabs(player->GetPosition().y - Object[i]->GetPosition().y); //Fabs ist Betrag
*/
