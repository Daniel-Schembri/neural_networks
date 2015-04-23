#include "agent.h"


Agent::Agent()
{
fitness = 0; lastfitness = 0;
}

Agent::Agent(int phealth, float pposx, float pposy, int pid, std::vector<unsigned> ptopology, int pnet_type)
{
fitness = 0; lastfitness = 0; id=pid;

health = phealth; posx = pposx; posy = pposy; nettype = pnet_type;
topology = ptopology;

//TODO Jonathan: more cases for different net-types / algorithms
switch(nettype)
{
	case 0:
		mynet = new FeedForwardNet(topology, true);
		break;
	case 1:
		mynet = new srn(topology, true);
		break;

	default:
		mynet = new FeedForwardNet(topology, true);
		break;
};

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
   //TODO Jonathan: Generalize for other types
   mynet->feedForward(inputvals);
   mynet->getResults(resultvals);

   return resultvals;

}

/*
float32 a = fabs(player->GetPosition().x - Object[i]->GetPosition().x);
float32 b = fabs(player->GetPosition().y - Object[i]->GetPosition().y); //Fabs ist Betrag
*/
