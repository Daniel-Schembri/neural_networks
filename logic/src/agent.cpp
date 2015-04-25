#include "agent.hpp"

Agent::Agent()
{
    fitness = 0; lastfitness = 0;
}

Agent::Agent(float pposx, float pposy, int pid, std::vector<unsigned> ptopology, int pnet_type)
{
    fitness = 0; lastfitness = 0; id=pid;

    posx = pposx; posy = pposy;
    topology = ptopology;

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

Agent::Agent(float pposx, float pposy, int pid, std::vector<unsigned> ptopology, int pnet_type,
             vector<vector<vector<Connection> > > pweights)
{
    fitness = 0; lastfitness = 0; id=pid;

    health = phealth; posx = pposx; posy = pposy;
    topology = ptopology;

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

    mynet->setConnections(pweights);
}

Agent::~Agent()
{
    delete mynet;
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
