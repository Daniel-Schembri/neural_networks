#ifndef __EVOLUTONARY_HPP__
#define __EVOLUTONARY_HPP__



#include <stdlib.h>
#include <time.h>		//For initialising the random_generator
#include <vector>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <string>
#include <stdexcept>

#include "agent.hpp"
#include "commonDefs.hpp"

#define MODE_SINGLEPLAYER 0
#define MODE_EVOLUTION 1
#define MODE_BESTAgentS 2

struct parameter
{
    unsigned int amount_of_Object;
    unsigned int amount_of_elite_copies;
    unsigned int evolvetime;
    unsigned int field_size;
    unsigned int population_size;
    int annealing_rate;
    int evolve_algorithm;
    int mode;
    int nettype;    
    std::vector<unsigned> topology;
    int amount_of_weights;

    //Range [0.0, 1.0]
    float crossover_rate;  
    // Probability threshold of selecting an animal for mutation
    float mutation_rate;
    // Mutation probability threshold for each weight
    float weight_mutation_rate;
};

class evolutionary
{
    //Attributes
    public:

        struct parameter sim_parameter;
        unsigned int evolvesteps;
        unsigned int generations;
        unsigned int iterationsteps;

        std::vector< std::vector<double> > inputvals_vector;

        //The Fitnesses over generations
        std::vector<int>   best_fitnesses;
        std::vector<float> average_fitnesses;

        std::vector<Agent *> best_Agents;

        std::vector<Agent *> newPopulation;
        std::vector<Agent *> population;

    private:
		
		bool datasetwritten;
        //double max_delta; 
        std::vector< std::vector<double> > trainingdataV;
        std::vector< std::vector<double> > trainingdataA;

        double revert_agent[100][100][100];

        //Methods

    public:

        evolutionary();
        evolutionary(struct parameter psim_parameter, std::vector<unsigned> ptopology);

        int evolve(int id_algo);


        std::vector< std::vector<double> > process(std::vector< std::vector<double> > inputvals_vector);

        void save_bestFitness();
        void save_averageFitness();

        void save_bestAgent();

        int get_bestFitness_overall();

        void set_trainingdataV(std::vector< std::vector<double> > ptrainingdata);
        void set_trainingdataA(std::vector< std::vector<double> > ptrainingdata);

    private:

        int evolve_crossover();
        int evolve_hillclimber();
        int evolve_learn();
        int evolve_simulatedannealing();

        vector<Agent*> crossover(Agent& mum, Agent& dad);
        void hillclimber(Agent *Agent, bool revert);
        void learn();
        void simulated_annealing(Agent *Agent, bool revert);

		void save_vals(std::vector< std::vector<double> > inputvals_vector, std::vector< std::vector<double> > results_vector);
};

#endif /* __EVOLUTONARY_HPP__ */
