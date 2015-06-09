#include "agent.hpp"
#include <assert.h>

Agent::Agent()
{
    fitness = 0; lastfitness = 0;
    idleness_count = 0;
}

Agent::Agent(float pposx, float pposy, int pid, std::vector<unsigned> ptopology, int pnet_type)
{
    fitness = 0; lastfitness = 0; id=pid; nettype = pnet_type;

    idleness_count = 0;

    posx = pposx; posy = pposy;
    topology = ptopology;

    velocity_net = NULL;
    angle_net    = NULL;
    myscript = NULL;

	switch(nettype)
	{
	case NET_FEEDFORWARD:
		velocity_net = new FeedForwardNet(topology, true);
		angle_net    = new FeedForwardNet(topology, true);
		break;
	case NET_SRN:
//		velocity_net = new srn(topology, true);
//		angle_net    = new srn(topology, true);
		break;
	case NET_SCRIPT:
        myscript = new Script();
		break;
	default:
		velocity_net = new FeedForwardNet(topology, true);
		angle_net    = new FeedForwardNet(topology, true);
		break;
	};
}

Agent::Agent(float pposx, float pposy, int pid, std::vector<unsigned> ptopology, int pnet_type,
             WeightMatrix &pweights)
{
    fitness = 0; lastfitness = 0; id=pid;

    posx = pposx; posy = pposy;
    topology = ptopology;
	
    velocity_net = NULL;
    angle_net    = NULL; myscript = NULL; 
	switch(nettype)
	{
	case NET_FEEDFORWARD:
		velocity_net = new FeedForwardNet(topology, true);
		angle_net    = new FeedForwardNet(topology, true);
		break;
	case NET_SRN:
//		velocity_net = new srn(topology, true);
//		angle_net    = new srn(topology, true);
		break;
	case NET_SCRIPT:
        myscript = new Script();
		break;
	default:
		velocity_net = new FeedForwardNet(topology, true);
		angle_net    = new FeedForwardNet(topology, true);
		break;
    };

    if(NULL != velocity_net)
    {
        angle_net->setConnections(pweights);
    }
}

Agent::Agent(const Agent &other)
{
    velocity_net = NULL;
    angle_net = NULL;
    myscript = NULL;

    //private attributes
    idleness_count = 0;
    posx = 0; posy = 0; angle = 0; //not important to get position
    id = other.getid() ; nettype = other.getnettype();
    topology = other.gettopology();

    fitness = other.fitness; lastfitness = other.lastfitness;

    if (NULL != other.velocity_net)
    {
        if (NET_FEEDFORWARD == nettype)
        {
            velocity_net = new FeedForwardNet(topology, true);
        }
        else if (NET_SRN == nettype);
        {
    //        velocity_net = new srn(topology, true);
        }
        velocity_net->setConnections(other.velocity_net->getConnections());
    }

    if (NULL != other.angle_net)
    {
        if (NET_FEEDFORWARD == nettype)
        {
            angle_net = new FeedForwardNet(topology, true);
        }
        else if (NET_SRN == nettype);
        {
      //      angle_net = new srn(topology, true);
        }
        angle_net->setConnections(other.angle_net->getConnections());
    }

    if (NULL != other.myscript)
    {
 //       myscript = new Script();
    }

    
}

Agent::~Agent()
{
    if(NULL != velocity_net)
    {
        delete velocity_net;
        velocity_net = NULL;
    }

    if(NULL != angle_net)
    {
        delete angle_net;
        angle_net = NULL;
    }

    if(NULL != myscript)
    {
        delete myscript;
        myscript = NULL;
    }
}


int Agent::getid() const
{
    return id;
}

int Agent::getnettype() const
{
    return nettype;
}

std::vector<unsigned> Agent::gettopology() const
{
    return topology; 
}

void Agent::learnV(std::vector<double> ptrainingdata_output)
{
    velocity_net->learn(ptrainingdata_output);
}

void Agent::learnA(std::vector<double> ptrainingdata_output)
{
    angle_net->learn(ptrainingdata_output);
}

double Agent::processV(std::vector<double> inputvals)
{
    std::vector<double> result_vals;

    if (nettype < 2)
    {
        velocity_net->feedForward(inputvals);
        velocity_net->getResults(result_vals);
        //For driving the agent if no food detected

        if (idleness_count >= 19)
        {
            result_vals[0] = 0.2f;
        }

    }
    else
    {
        result_vals.push_back(myscript->processV(inputvals));
    }

    assert(1 == result_vals.size());
    return result_vals[0];
}

double Agent::processA(std::vector<double> inputvals)
{
    std::vector<double> result_vals;

    if (nettype < 2)
    {
        angle_net->feedForward(inputvals);
        angle_net->getResults(result_vals);
    
        //For driving the agent if no food detected
        if(0 == inputvals[0] && 0 == inputvals[1])
        {
            idleness_count++;
        }
        else
        {
            idleness_count = 0;
        }

        if (idleness_count >= 20)
        {
            result_vals[0] = 1.03f;
        }
    }
    else
    {
        result_vals.push_back(myscript->processA(inputvals));
    }

    assert(1 == result_vals.size());
    return result_vals[0];
}


bool Agent::operator== (const Agent &other) const
{
    WeightMatrix mine = angle_net->getConnections();
    WeightMatrix his  = other.angle_net->getConnections();

    // Loop through the net
    unsigned nbLayersInNet = mine.size();
    for (unsigned nbLayer = 0; nbLayer < nbLayersInNet; ++nbLayer) 
    {
        unsigned nbNeuronsInLayer = mine[nbLayer].size();
        for (unsigned nbNeuron = 0; nbNeuron < nbNeuronsInLayer; ++nbNeuron) 
        {
            unsigned nbConnectionsOfNeuron = mine[nbLayer][nbNeuron].size();
            for (unsigned nbConnection = 0; nbConnection < nbConnectionsOfNeuron ; ++nbConnectionsOfNeuron) 
            {
                // Are the weights equal?
                if(mine[nbLayer][nbNeuron][nbConnection].weight == his[nbLayer][nbNeuron][nbConnection].weight)
                {
                    return false;
                }
            }
        }
    }

    return true;
}

/*
float32 a = fabs(player->GetPosition().x - Object[i]->GetPosition().x);
float32 b = fabs(player->GetPosition().y - Object[i]->GetPosition().y);
*/
