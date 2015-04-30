#include "agent.hpp"

Agent::Agent()
{
    fitness = 0; lastfitness = 0;
}

Agent::Agent(float pposx, float pposy, int pid, std::vector<unsigned> ptopology, int pnet_type)
{
    fitness = 0; lastfitness = 0; id=pid; nettype = pnet_type;

    posx = pposx; posy = pposy;
    topology = ptopology;

    mynet = NULL;
    myscript = NULL;

	switch(nettype)
	{
	case NET_FEEDFORWARD:
		mynet = new FeedForwardNet(topology, true);
		break;
	case NET_SRN:
		mynet = new srn(topology, true);
		break;
	case NET_SCRIPT:
        myscript = new Script();
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

    posx = pposx; posy = pposy;
    topology = ptopology;
	
    mynet = NULL;
    myscript = NULL;

	switch(nettype)
	{
	case NET_FEEDFORWARD:
		mynet = new FeedForwardNet(topology, true);
		break;
	case NET_SRN:
		mynet = new srn(topology, true);
		break;
	case NET_SCRIPT:
        myscript = new Script();
		break;
	default:
		mynet = new FeedForwardNet(topology, true);
		break;
    };

    if(NULL != mynet)
    {
        mynet->setConnections(pweights);
    }
}

Agent::~Agent()
{
    if(NULL != mynet)
    {
        delete mynet;
        mynet = NULL;
    }

    if(NULL != myscript)
    {
        delete myscript;
        myscript = NULL;
    }
}

void Agent::learn(std::vector<double> ptrainingdata_output)
{
    mynet->learn(ptrainingdata_output);
}

std::vector<double> Agent::process(std::vector<double> inputvals)
{
    std::vector<double> resultvals;

    if (nettype < 2)
    {
        mynet->feedForward(inputvals);
        mynet->getResults(resultvals);
    }
    else
    {
      resultvals = myscript->process(inputvals);
    }

    return resultvals;
}

/*
float32 a = fabs(player->GetPosition().x - Object[i]->GetPosition().x);
float32 b = fabs(player->GetPosition().y - Object[i]->GetPosition().y); //Fabs ist Betrag
*/