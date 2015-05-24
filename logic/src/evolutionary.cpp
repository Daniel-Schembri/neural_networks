#include <cassert>

#include "evolutionary.hpp"
#include "commonDefs.hpp"
#include "math.h"

evolutionary::evolutionary()
{
    
}

evolutionary::evolutionary(struct parameter psim_parameter, std::vector<unsigned> ptopology)
{
    srand((unsigned)time(NULL));
    sim_parameter = psim_parameter;
    evolvesteps = sim_parameter.evolvetime * 60; //60 Steps per Second (60Hz)
    iterationsteps = 0; generations = 0;

	//Necessary for writing the in and ouputs of the agent in a file
	datasetwritten = false;
    //Supervised learning
    net_learned = false;

    sim_parameter.topology = ptopology;
    sim_parameter.amount_of_weights = 0;

    for(unsigned i=0; i < sim_parameter.topology.size()-1; i++)
    {
        sim_parameter.amount_of_weights += sim_parameter.topology[i] * sim_parameter.topology[i+1];
    }

    if (MODE_SINGLEPLAYER == sim_parameter.mode)
    {
        sim_parameter.population_size = 1;
    }

    for (unsigned i = 0; i < sim_parameter.population_size; i++)
    {
        population.push_back(new Agent (rand() % 90 - 90, rand() % 80 + 10, i, ptopology, sim_parameter.nettype));
    }

    best_fitnesses.push_back(0);
    average_fitnesses.push_back(0.0f);

    best_Agents.push_back(population[0]);  //Just to keep one field to save later best Agent

    for (unsigned int i = 0; i < 100; i++)  
    {
        for (unsigned int j = 0; j < 100; j++)    
        {
            for (unsigned int w = 0; w < 100; w++)
            {
                revert_agent_a[i][j][w] = 0;
                revert_agent_v[i][j][w] = 0;
            }
        }
    }

}

evolutionary::~evolutionary()
{
    /*
    if(!best_Agents.empty())
    {
        for(unsigned i=0; i<best_Agents.size();++i)
        {
            if(NULL != best_Agents[i])
            {
                delete best_Agents[i];
                best_Agents[i] = NULL;
            }
        }
        best_Agents.clear();
    }
    if(!population.empty())
    {
        for(unsigned i=0; i<population.size();++i)
        {
            if(NULL != population[i])
            {
                delete population[i];
                population[i] = NULL;
            }
        }
        population.clear();
    }
    if(!newPopulation.empty())
    {
        for(unsigned i=0; i<newPopulation.size();++i)
        {
            if(NULL != newPopulation[i])
            {
                delete newPopulation[i];
                newPopulation[i] = NULL;
            }
        }
        newPopulation.clear();
    }
    */
}


int evolutionary::evolve(int id_algo)
{
    int reset_sim = 0;
    switch (id_algo)
    {
        case 0:
            reset_sim = evolve_hillclimber();
            return reset_sim;
            break;
        case 1:
            reset_sim = evolve_simulatedannealing();
            return reset_sim;
            break;
        case 2:
            reset_sim = evolve_learn();
            return reset_sim;
            break;
        case 3:
            reset_sim = evolve_crossover();
            return reset_sim;
            break;
        default:
            return reset_sim;
            break;
    }

}



int evolutionary::evolve_hillclimber()
{

    iterationsteps++;

    if (iterationsteps >= evolvesteps) // 3000 = 1Min
    {

        iterationsteps = 0;
        save_bestAgent();
        //if fitness < lastfitness -> revert the last hillclimber-changes
        for (unsigned i = 0; i<sim_parameter.population_size; i++)
        {
            if(population[i]->lastfitness > population[i]->fitness)
            {
                hillclimber(population[i], true);  //Revert Hillclimber
            }
            population[i]->lastfitness = population[i]->fitness; //Save Last fitness to revert hillclimber if worse fitness
            hillclimber(population[i],false);
        }
        generations++;
        return 1;  //Signal that Simulation must be Reset
    }
    return 0;
}



int evolutionary::evolve_simulatedannealing()
{
    double T;
    double randomval = 0.0f;
    double r, p;
    r = 0.0f; p = 0.0f; T = sim_parameter.annealing_rate;
    iterationsteps++;

    if (iterationsteps >= evolvesteps) // 3000 = 1Min
    {

        iterationsteps = 0;
        save_bestAgent();

        for (unsigned i = 0; i < sim_parameter.population_size; i++)
        {
            r = (population[i]->fitness - population[i]->lastfitness);
            p = 1 / (1 + exp(-r / T));
            if(randomval > p) 	//with probality p, keep the changes
            {
                hillclimber(population[i], true); //Otherwise revert simulated annealing step
            }
            population[i]->lastfitness = population[i]->fitness; //Save Last fitness to revert if worse fitness
            hillclimber(population[i], false);
        }
        generations++;
        // Cool down annealing rate
        if (sim_parameter.annealing_rate > 1)
            sim_parameter.annealing_rate--;
        return 1;  //Signal that Simulation must be Reset
    }
    return 0;
}

int evolutionary::evolve_learn()  //
{
    if(!net_learned)
    {
        learn();
        net_learned = true;
    }
    return 0;
}

void evolutionary::do_or_save_revert(Agent *agent, bool revert)
{
//Do or save Angle Net
    for (unsigned int i=0; i<agent->angle_net->m_layers.size();i++)  //Amount of Layers
    {
        for(unsigned int j=0;j<agent->angle_net->m_layers[i].size();j++)    //Amount of Neurons
        {
            if (revert)
            {
                for (unsigned int w = 0; w < agent->angle_net->m_layers[i][j]->m_outputWeights.size(); w++)
                {
                    agent->angle_net->m_layers[i][j]->m_outputWeights[w].weight = revert_agent_a[i][j][w];
                }
            }
            else
            {
                for (unsigned int w = 0; w < agent->angle_net->m_layers[i][j]->m_outputWeights.size(); w++)
                {
                    revert_agent_a[i][j][w] = agent->angle_net->m_layers[i][j]->m_outputWeights[w].weight;
                }
            }
        }
    }

//Do or save Velocity Net
    for (unsigned int i=0; i<agent->velocity_net->m_layers.size();i++)  //Amount of Layers
    {
        for(unsigned int j=0;j<agent->velocity_net->m_layers[i].size();j++)    //Amount of Neurons
        {
            if (revert)
            {
                for (unsigned int w = 0; w < agent->velocity_net->m_layers[i][j]->m_outputWeights.size(); w++)
                {
                    agent->velocity_net->m_layers[i][j]->m_outputWeights[w].weight = revert_agent_v[i][j][w];
                }
            }
            else
            {
                for (unsigned int w = 0; w < agent->velocity_net->m_layers[i][j]->m_outputWeights.size(); w++)
                {
                    revert_agent_v[i][j][w] = agent->velocity_net->m_layers[i][j]->m_outputWeights[w].weight;
                }
            }
        }
    }


}



void evolutionary::proove_net_maxvals(Net *net)
{
    for (unsigned int i=0; i<net->m_layers.size();i++)  //Amount of Layers
    {
        for(unsigned int j=0;j<net->m_layers[i].size();j++)    //Amount of Neurons
        {
            for(unsigned int w=0;w<net->m_layers[i][j]->m_outputWeights.size();w++)
            {
                if (net->m_layers[i][j]->m_outputWeights[w].weight > 1.0f)
                {
                    net->m_layers[i][j]->m_outputWeights[w].weight = 1.0f;
                }
                if (net->m_layers[i][j]->m_outputWeights[w].weight < -1.0f)
                {
                    net->m_layers[i][j]->m_outputWeights[w].weight = -1.0f;
                }
            }
        }
    }
}

void evolutionary::mutate_net(Net *net)
{
    double delta = 0.0f;
    double randomval = 0.0f;

    for (unsigned int i=0; i<net->m_layers.size();i++)  //Amount of Layers
    {
        for(unsigned int j=0;j<net->m_layers[i].size();j++)    //Amount of Neurons
        {
            for(unsigned int w=0;w<net->m_layers[i][j]->m_outputWeights.size();w++)
            {
                delta = (((((double) rand() / double(RAND_MAX)) / 5.0f) -0.1f)*(int) 1000) / (1000.0f);  //Range -0.1f to +0.1f //resolution 0.001f
                randomval = (double) rand() / double(RAND_MAX); //
                if (randomval < sim_parameter.weight_mutation_rate)
                {
                    net->m_layers[i][j]->m_outputWeights[w].weight += delta;
                }
            }
        }
    }
    proove_net_maxvals(net);
}


//Evolutionary Algorithms
void evolutionary::hillclimber(Agent *Agent, bool revert)
{
    do_or_save_revert(Agent, revert);

    if(!revert)
    {
        mutate_net(Agent->angle_net);
        mutate_net(Agent->velocity_net);
    }
    
}


void evolutionary::learn()
{
    // Teach velocity
    for (unsigned i = 0; i < population.size(); i++)
    {
        //Learn with the trainings set
        std::vector<double> trainingdata_input;
        std::vector<double> trainingdata_output;
        std::vector< std::vector<double> >::iterator row;
        std::vector<double>::iterator col;
        for (row = trainingdataV.begin(); row != trainingdataV.end(); row++)
        {
            col = row->begin();
            // do stuff ...
            trainingdata_input.push_back(col[0]);
            trainingdata_input.push_back(col[1]);
            trainingdata_output.push_back(col[2]);
            trainingdata_output.push_back(col[3]);

            population[i]->velocity_net->feedForward(trainingdata_input);
            population[i]->learnV(trainingdata_output);

            //Clear for new Values
            trainingdata_input.clear();
            trainingdata_output.clear();
        }
    }

    // Teach angle
    for (unsigned i = 0; i < population.size(); i++)
    {
        //Learn with the trainings set
        std::vector<double> trainingdata_input;
        std::vector<double> trainingdata_output;
        std::vector< std::vector<double> >::iterator row;
        std::vector<double>::iterator col;
        for (row = trainingdataA.begin(); row != trainingdataA.end(); row++)
        {
            col = row->begin();
            // do stuff ...
            trainingdata_input.push_back(col[0]);
            trainingdata_input.push_back(col[1]);
            trainingdata_output.push_back(col[2]);
            trainingdata_output.push_back(col[3]);

            population[i]->angle_net->feedForward(trainingdata_input);
            population[i]->learnA(trainingdata_output);

            //Clear for new Values
            trainingdata_input.clear();
            trainingdata_output.clear();
        }
    }
}

std::vector< std::vector<double> > evolutionary::process(std::vector< std::vector<double> > inputvals_vector)
{
    std::vector< std::vector<double> > result_vectors;
    for (unsigned int i = 0; i<population.size(); i++)
    {
        std::vector<double> agent_vals;

        //TODO: hand over correct values
        double x_value = inputvals_vector[i][0] / SIGHT_RADIUS;
        double y_value = inputvals_vector[i][1] / SIGHT_RADIUS;

        // Returns veloctiy in [0.0,1.0], no need for scalling
        agent_vals.push_back(population[i]->processV(inputvals_vector[i]));

        // Return angle between vector and x-axis. Angle is element of [0.0,1.0], where 1.0 = pi/2
        // Unscaled angle = pi/2 * Network output
        // The control value for a agent is calculated using the angle between vector and y-axis
        // Angle(vector, y-axis) = pi/2 - (Networkout * pi/2)
        // The control value for a agent is +pi/2...0...-pi/2.
        // The neural net is not aware of negative values -> sign(x) must be used to correct value
        // Complete formula:
        //                   steering = sign(x) * -(pi/2-(Network output - pi/2))
        double angle_x_v = M_PI/2.0 * population[i]->processA(inputvals_vector[i]);
        double angle_y_v = M_PI/2.0 - angle_x_v;
        
        if (0 == inputvals_vector[i][0] && 0 == inputvals_vector[i][1] && angle_x_v == M_PI/2.0 * 1.03f)
        {
            x_value = 1;
        }

        double steering_angle = sgn(x_value) * -angle_y_v;

        agent_vals.push_back(steering_angle);

    //    agent_vals[0] = 0;
   //     agent_vals[1] = 0;

        result_vectors.push_back(agent_vals);

        std::cout << "Input: (" << inputvals_vector[i][0] << "," << inputvals_vector[i][1] << 
                     ") Output:" << result_vectors[i][0] << "," << result_vectors[i][1] << ")\n";
        //std::cout << "angle_x_v :" << angle_x_v << ", angle_y_v: " << angle_y_v << ", steering_angle: " << steering_angle << "\n"; 

    }

    if (!datasetwritten)
    {
        save_vals(inputvals_vector, result_vectors);
        datasetwritten = true;
    }

    return result_vectors;

}

//Save actual in- and outputs of the agents in a file
void evolutionary::save_vals(std::vector< std::vector<double> > inputvals_vector, std::vector< std::vector<double> > results_vector)
{
	std::vector<double> inputvals(2);
	std::vector<double> resultvals(2);

	std::string s = "";

	std::ofstream outfile("agent_vals.txt", std::ofstream::out);
	if (outfile.is_open())
	{
		for (unsigned i = 0; i < inputvals_vector.size(); i++)
		{
			inputvals = inputvals_vector[i];
			resultvals = results_vector[i];

			s += "Agent: " + std::to_string(i) + " \nInputvals       Outputvals \n";

			//to convert float to String

			std::ostringstream floatstring;

			floatstring.clear();

			floatstring << inputvals[0] << ", " << inputvals[1] << " | " << resultvals[0] << ", " << resultvals[1] << "\n";
			s += floatstring.str();
			floatstring.clear();

			s += "\n";
		}
	// write to outfile
	outfile.write(s.c_str(), s.length());

	outfile.close();
	}
	else
	{
		//	std::cerr << "Unable to open file to write stats!";
	}
}

void evolutionary::save_bestFitness()
{
    int best_Fitness = 0;
    for(unsigned int i=0;i<population.size();i++)
    {    
        if (population[i]->fitness > best_Fitness)
        {
            best_Fitness = population[i]->fitness;
        }
    }
    best_fitnesses.push_back(best_Fitness);
}

void evolutionary::save_averageFitness()  
{
    float average_Fitness = 0.0f;
    for(unsigned int i=0;i<population.size();i++)
    {    
        average_Fitness += population[i]->fitness;
    }
    average_Fitness = average_Fitness / (float) population.size();

    average_fitnesses.push_back(average_Fitness);
}


int evolutionary::get_bestFitness_overall()
{
    int best_fitness = 0;

    for(unsigned int i=0;i<best_fitnesses.size();i++)
    {    
        if (best_fitnesses[i] > best_fitness)
        {
            best_fitness = best_fitnesses[i];
        }
    }
    return best_fitness;
}

float evolutionary::get_best_average_Fitness_overall()
{
    float best_average_fitness = 0.0f;

    for(unsigned int i=0;i<average_fitnesses.size();i++)
    {    
        if (average_fitnesses[i] > best_average_fitness)
        {
            best_average_fitness = average_fitnesses[i];
        }
    }
    return best_average_fitness;
}

void evolutionary::save_bestAgent()
{
    int best_fitness = 0;

    for(unsigned int i=0;i<population.size();i++)
    {    
        if (population[i]->fitness > best_fitness)
        {
            best_Agents[0] = population[i];
        }
    }

}

void evolutionary::set_trainingdataV(std::vector< std::vector<double> > ptrainingdata)
{
    trainingdataV = ptrainingdata;
}

void evolutionary::set_trainingdataA(std::vector< std::vector<double> > ptrainingdata)
{
    trainingdataA = ptrainingdata;
}

vector<Agent*> evolutionary::crossover(Agent& mum, Agent& dad)
{
    // Crossover will only be done with a certain probability 
    float randval = (rand())/(RAND_MAX+1.0);
    assert(randval >= 0.0 && randval <= 1.0);

    vector<Agent*> kids; 

    if ((randval > sim_parameter.crossover_rate) || (mum == dad)) 
    {
        // Simply return mum and dad, they will be copied into the next population
        kids.push_back(&mum);
        kids.push_back(&dad);

        return kids;
    }

    unsigned crossover_point = rand() % sim_parameter.amount_of_weights + 1;  //Range between 1 and amount_of_weights

    assert(crossover_point >= 1 && crossover_point <= sim_parameter.amount_of_weights);

    WeightMatrix mum_weights = mum.angle_net->getConnections();
    WeightMatrix dad_weights = dad.angle_net->getConnections();

    WeightMatrix kid1_weights;
    WeightMatrix kid2_weights;
 
    // To keep track of how many connections have been added
    unsigned gene_count = 0;

    // Child 1: mum|dad, child 2: dad|mum
    unsigned nbLayersInNet = mum_weights.size();
    for(unsigned nbLayer = 0; nbLayer < nbLayersInNet ; ++nbLayer)
    {
        // All Connections of the respective layer
        vector<vector<Connection> > kid1_layerConnections;
        kid1_weights.push_back(kid1_layerConnections);

        vector<vector<Connection> > kid2_layerConnections;
        kid2_weights.push_back(kid2_layerConnections);

        unsigned nbNeuronsInLayer = dad_weights[nbLayer].size();
        for (unsigned nbNeuron = 0; nbNeuron < nbNeuronsInLayer; ++nbNeuron) 
        {
            vector<Connection> kid1_neuronConnections;
            vector<Connection> kid2_neuronConnections;

            unsigned nbConnectionsOfNeuron = dad_weights[nbLayer][nbNeuron].size();
            for (unsigned nbConnection = 0; nbConnection < nbConnectionsOfNeuron ; ++nbConnection) 
            {
                // Determine whether this connection should be cloned from mum or dad
                if(gene_count < crossover_point)
                {
                    // Mum genes
                    kid1_neuronConnections.push_back(mum_weights[nbLayer][nbNeuron][nbConnection]);
                    // Dad genes
                    kid2_neuronConnections.push_back(dad_weights[nbLayer][nbNeuron][nbConnection]);
                }
                else
                {
                    // Dad genes
                    kid1_neuronConnections.push_back(dad_weights[nbLayer][nbNeuron][nbConnection]);
                    // Mum genes
                    kid2_neuronConnections.push_back(mum_weights[nbLayer][nbNeuron][nbConnection]);
                }
                ++gene_count;
            }
        }
    }

    kids.push_back(new Agent (rand() % 90 - 90, rand() % 80 + 10, 0, sim_parameter.topology, sim_parameter.nettype, kid1_weights));
    kids.push_back(new Agent (rand() % 90 - 90, rand() % 80 + 10, 0, sim_parameter.topology, sim_parameter.nettype, kid2_weights));




	//Second crossover_point_V for velocity_net
    unsigned crossover_point_V = rand() % sim_parameter.amount_of_weights + 1;  //Range between 1 and amount_of_weights

    assert(crossover_point_V >= 1 && crossover_point_V <= sim_parameter.amount_of_weights);

    WeightMatrix mum_weights_V = mum.velocity_net->getConnections();
    WeightMatrix dad_weights_V = dad.velocity_net->getConnections();

    WeightMatrix kid1_weights_V;
    WeightMatrix kid2_weights_V;
 
    // To keep track of how many connections have been added
    unsigned gene_count_V = 0;

    // Child 1: mum|dad, child 2: dad|mum
    unsigned nbLayersInNet_V = mum_weights_V.size();
    for(unsigned nbLayer_V = 0; nbLayer_V < nbLayersInNet_V ; ++nbLayer_V)
    {
        // All Connections of the respective layer
        vector<vector<Connection> > kid1_layerConnections_V;
        kid1_weights_V.push_back(kid1_layerConnections_V);

        vector<vector<Connection> > kid2_layerConnections_V;
        kid2_weights_V.push_back(kid2_layerConnections_V);

        unsigned nbNeuronsInLayer_V = dad_weights_V[nbLayer_V].size();
        for (unsigned nbNeuron_V = 0; nbNeuron_V < nbNeuronsInLayer_V; ++nbNeuron_V) 
        {
            vector<Connection> kid1_neuronConnections_V;
            vector<Connection> kid2_neuronConnections_V;

            unsigned nbConnectionsOfNeuron_V = dad_weights_V[nbLayer_V][nbNeuron_V].size();
            for (unsigned nbConnection_V = 0; nbConnection_V < nbConnectionsOfNeuron_V ; ++nbConnection_V) 
            {
                // Determine whether this connection should be cloned from mum or dad
                if(gene_count_V < crossover_point_V)
                {
                    // Mum genes
                    kid1_neuronConnections_V.push_back(mum_weights_V[nbLayer_V][nbNeuron_V][nbConnection_V]);
                    // Dad genes
                    kid2_neuronConnections_V.push_back(dad_weights_V[nbLayer_V][nbNeuron_V][nbConnection_V]);
                }
                else
                {
                    // Dad genes
                    kid1_neuronConnections_V.push_back(dad_weights_V[nbLayer_V][nbNeuron_V][nbConnection_V]);
                    // Mum genes
                    kid2_neuronConnections_V.push_back(mum_weights_V[nbLayer_V][nbNeuron_V][nbConnection_V]);
                }
                ++gene_count_V;
            }
        }
	}

	kids[0]->velocity_net->setConnection(kid1_weights_V);
    kids[1]->velocity_net->setConnection(kid2_weights_V);

    return kids;
}

int evolutionary::evolve_crossover()
{
    iterationsteps++;

    if (iterationsteps >= evolvesteps) // 3000 = 1Min
    {
        iterationsteps = 0;
        generations++;    
        save_bestAgent();

        std::vector<unsigned> roulette;

        // Created strcutre: [1][1][1][2][2][3][4]. Random number between 1 and length(array) will
        // more likely choose an agent with a superior fitness
        for(unsigned i=0; i < population.size(); ++i)
            for(int j=0; j < population[j]->fitness; ++j)
                roulette.push_back(i);

        // Add some elitism by copying the best agent n times
        for(unsigned i=0;i < sim_parameter.amount_of_elite_copies; ++i)
        {
            mutate_net(best_Agents[0]->angle_net);
			mutate_net(best_Agents[0]->velocity_net);
            newPopulation.push_back(best_Agents[0]);
        }

        while(sim_parameter.population_size != newPopulation.size())
        {
            int mum_id = rand() % (population.size()-1);
            int dad_id = rand() % (population.size()-1);

            assert(0 <= mum_id && (population.size()-1) >= mum_id);

            Agent& mum = *(population[mum_id]);
            Agent& dad = *(population[dad_id]);

            std::vector<Agent*> kids;

            // Create offspring
            kids = crossover(mum, dad);

            mutate_net(kids[0]->angle_net);
            mutate_net(kids[0]->velocity_net);

			mutate_net(kids[1]->angle_net);
			mutate_net(kids[1]->velocity_net);		
			
            // Insert into the new population
            newPopulation.push_back(kids[0]);
            newPopulation.push_back(kids[1]);
        }
        return 1;
    }
    return 0;
}
