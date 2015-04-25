#include "evolutionary.hpp"
#include <stdlib.h>

evolutionary::evolutionary()
{
}

evolutionary::evolutionary(struct parameter psim_parameter, std::vector<unsigned> ptopology)
{
    srand((unsigned)time(NULL));
    sim_parameter = psim_parameter;
    evolvesteps = sim_parameter.evolvetime * 60; //60 Steps per Second (60Hz)
    iterationsteps = 0; generations = 0;

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
        population.push_back(new Agent (100, rand() % 90 - 90, rand() % 80 + 10, i, ptopology));
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
                revert_agent[i][j][w] = 0;
            }
        }
    }

}


int evolutionary::evolve(int id_algo)
{
    int reset_sim = -1;
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
                simulated_annealing(population[i], true); //Otherwise revert simulated annealing step
            }
            population[i]->lastfitness = population[i]->fitness; //Save Last fitness to revert if worse fitness
            simulated_annealing(population[i], false);
        }
        generations++;
        // Cool down annealing rate
        if (sim_parameter.annealing_rate > 1)
            sim_parameter.annealing_rate--;
        return 1;  //Signal that Simulation must be Reset
    }
    return 0;
}

int evolutionary::evolve_learn()  //TODO How manyLearningCycles?
{

    iterationsteps++;

    if (iterationsteps >= evolvesteps) // 3000 = 1Min
    {
        iterationsteps = 0;
        save_bestAgent();
        learn(10);
        generations++;

        return 1;  //Signal that Simulation must be Reset
    }
    return 0;
}

//Evolutionary Algorithms
void evolutionary::hillclimber(Agent *Agent, bool revert)
{
    double delta = 0.0f;
    double randomval = 0.0f;


    for (unsigned int i=0; i<Agent->mynet->m_layers.size();i++)  //Amount of Layers
    {
        for(unsigned int j=0;j<Agent->mynet->m_layers[i].size();j++)    //Amount of Neurons
        {
            if (!revert)
            {
                for (unsigned int w = 0; w < Agent->mynet->m_layers[i][j]->m_outputWeights.size(); w++)
                {
                    revert_agent[i][j][w] = Agent->mynet->m_layers[i][j]->m_outputWeights[w].weight;
                }
            }

            for(unsigned int w=0;w<Agent->mynet->m_layers[i][j]->m_outputWeights.size();w++)
            {

                delta = (((((double) rand() / double(RAND_MAX)) / 5.0f) -0.1f)*(int) 1000) / (1000.0f);  //Range -0.1f to +0.1f //resolution 0.001f
                randomval = (double) rand() / double(RAND_MAX); //
                if (revert)
                {       
                    Agent->mynet->m_layers[i][j]->m_outputWeights[w].weight = revert_agent[i][j][w];
                }
                else
                {
                    if (randomval < sim_parameter.weight_mutation_rate)
                    {
                        Agent->mynet->m_layers[i][j]->m_outputWeights[w].weight += delta;
                    }
                }
                if (Agent->mynet->m_layers[i][j]->m_outputWeights[w].weight > 1.0f)
                {
                    Agent->mynet->m_layers[i][j]->m_outputWeights[w].weight = 1.0f;
                }
                if (Agent->mynet->m_layers[i][j]->m_outputWeights[w].weight < -1.0f)
                {
                    Agent->mynet->m_layers[i][j]->m_outputWeights[w].weight = -1.0f;
                }
            }
        }
    }
}

void evolutionary::simulated_annealing(Agent * Agent, bool revert)
{
    double delta = 0.0f;
    double randomval = 0.0f;

    for (unsigned int i=0; i<Agent->mynet->m_layers.size();i++)  //Amount of Layers
    {
        for(unsigned int j=0;j<Agent->mynet->m_layers[i].size();j++)    //Amount of Neurons
        {
            if (!revert)
            {
                for (unsigned int w = 0; w < Agent->mynet->m_layers[i][j]->m_outputWeights.size(); w++)
                {
                    revert_agent[i][j][w] = Agent->mynet->m_layers[i][j]->m_outputWeights[w].weight;
                }
            }

            for(unsigned int w=0;w<Agent->mynet->m_layers[i][j]->m_outputWeights.size();w++)
            {
                delta = (((((double) rand() / double(RAND_MAX)) / 5.0f)-0.1f)*(int) 1000) / (1000.0f);  //Range -0.1f to +0.1f //resolution 0.001f
                if (delta > 1.0f) delta = 1.0f;
                if (delta < -1.0f) delta = -1.0f;
                randomval = (double) rand() / double(RAND_MAX); //
                if (randomval < sim_parameter.weight_mutation_rate)
                {
                    Agent->mynet->m_layers[i][j]->m_outputWeights[w].weight += delta;
                }

                if (Agent->mynet->m_layers[i][j]->m_outputWeights[w].weight > 1.0f)
                {
                    Agent->mynet->m_layers[i][j]->m_outputWeights[w].weight = 1.0f;
                }
                if (Agent->mynet->m_layers[i][j]->m_outputWeights[w].weight < -1.0f)
                {
                    Agent->mynet->m_layers[i][j]->m_outputWeights[w].weight = -1.0f;
                }
            }
        }
    }
}

void evolutionary::learn(int plearn_cycles)
{
    for (int learningcycles = 0; learningcycles < plearn_cycles; learningcycles++)
    {
        for (unsigned i = 0; i < population.size(); i++)
        {
            //Learn the with the training-set
            std::vector<double> trainingdata_input;
            std::vector<double> trainingdata_output;
            std::vector< std::vector<float> >::iterator row;
            std::vector<float>::iterator col;
            for (row = trainingdata.begin(); row != trainingdata.end(); row++)
            {
                col = row->begin();
                // do stuff ...
                trainingdata_input.push_back(col[0]);
                trainingdata_input.push_back(col[1]);
                trainingdata_output.push_back(col[2]);
                trainingdata_output.push_back(col[3]);

                population[i]->mynet->feedForward(trainingdata_input);
                population[i]->learn(trainingdata_output);

                //Clear for new Values
                trainingdata_input.clear();
                trainingdata_output.clear();
            }
        }
    }
}

std::vector< std::vector<double> > evolutionary::process(std::vector< std::vector<double> > inputvals_vector)
{
    std::vector< std::vector<double> > result_vectors;
    for (unsigned int i = 0; i<population.size(); i++)
    {
        result_vectors.push_back(population[i]->process(inputvals_vector[i]));
    }

    return result_vectors;

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

void evolutionary::set_trainingdata(std::vector< std::vector<float> > ptrainingdata)
{
    trainingdata = ptrainingdata;
}

std::vector<Agent*> evolutionary::crossover(Agent* mum, Agent* dad)
{
    std::vector<Agent *> kids;
    float randval = (rand())/(RAND_MAX+1.0);

    if ( (randval > sim_parameter.crossover_rate) || (mum == dad)) 
    {
        kids.push_back(mum);
        kids.push_back(dad);
    }

    unsigned crossover_point = rand() % sim_parameter.amount_of_weights + 1;  //Range between 1 and amount_of_weights

    vector<vector<vector<Connection> > > mum_weights = mum->mynet->getConnections();
    vector<vector<vector<Connection> > > dad_weights = mum->mynet->getConnections();

    vector<vector<vector<Connection> > > kid1_weights;
    vector<vector<vector<Connection> > > kid2_weights;
 
    // To keep track of how many connections have been added
    unsigned gene_count = 0;

    unsigned nbLayersInNet = dad_weights.size();
    // Child 1: mum|dad, child 2: dad|mum
    // For each layer
    for(unsigned nbLayer = 0; nbLayer < nbLayersInNet ; ++nbLayer)
    {
        // All Connections of the respective layer
        vector<vector<Connection> > kid1_layerConnections;
        kid1_weights.push_back(kid1_layerConnections);

        vector<vector<Connection> > kid2_layerConnections;
        kid2_weights.push_back(kid2_layerConnections);

        unsigned nbNeuronsInLayer = dad_weights[nbLayer].size();
        // For each neuron in the layer
        for (unsigned nbNeuron = 0; nbNeuron < nbNeuronsInLayer; ++nbNeuron) 
        {
            vector<Connection> kid1_neuronConnections;
            vector<Connection> kid2_neuronConnections;

            unsigned nbConnectionsOfNeuron = dad_weights[nbLayer][nbNeuron].size();
            // For each connection of the respective neuron
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

    kids.push_back(new Agent (100, rand() % 90 - 90, rand() % 80 + 10, 0, sim_parameter.topology, kid1_weights));
    kids.push_back(new Agent (100, rand() % 90 - 90, rand() % 80 + 10, 0, sim_parameter.topology, kid1_weights));

    return kids;
}

int evolutionary::evolve_crossover()
{
    if (iterationsteps >= evolvesteps) // 3000 = 1Min
    {
        iterationsteps = 0;
        generations++;    
        save_bestAgent();

        std::vector<unsigned> roulette;

        for(unsigned i=0;i<population.size();i++)
            for(unsigned j=0;j<population[j]->fitness;j++)
                roulette.push_back(i);

        // Add some elitism by copying the best agent n times
        for(unsigned i=0;i < sim_parameter.amount_of_elite_copies; ++i)
            newPopulation.push_back(best_Agents[0]);

        while(sim_parameter.population_size != newPopulation.size())
        {
            //TODO: implement roulette
//             Agent* mum = roulette();
//             Agent* dad = roulette();

            std::vector<Agent*> kids;

            // Create offspring
//             kids = crossover(mum, dad);

            // Mutate offspring 
            // TODO: Implement mutate
//             mutate(kids[0]);
//             mutate(kids[1]);

            // Insert into the new population
            newPopulation.push_back(kids[0]);
            newPopulation.push_back(kids[1]);
        }
        return 1;
    }
    return 0;
}

