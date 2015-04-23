#ifndef EVOLUTIONARY_H
#define EVOLUTIONARY_H

#include <time.h>		//For initialising the random_generator
#include <vector>
#include "agent.h"

#define MODE_SINGLEPLAYER 0
#define MODE_EVOLUTION 1
#define MODE_BESTAgentS 2

struct parameter
{
	unsigned int population_size;
	unsigned int amount_of_Object;
	unsigned int field_size;
	unsigned int evolvetime;
	int evolve_algorithm;
	int mode;
	int nettype;
	int random;
	float mutation_rate;
	int annealing_rate;
};

class evolutionary
{
//Attributes

public:   // PUBLIC

	struct parameter sim_parameter;
	unsigned int iterationsteps;
	unsigned int evolvesteps;
	unsigned int generations;

	std::vector< std::vector<double> > inputvals_vector;

	//The Fitnesses over generations
	std::vector<int> best_fitnesses;
	std::vector<float> average_fitnesses;

	std::vector<Agent *> population;
	std::vector<Agent *> best_Agents;

private:    //PRIVATE

	//double max_delta; 
	std::vector< std::vector<float> > trainingdata;
	double revert_agent[100][100][100];

	//Methods

public:		//PUBLIC

	evolutionary();
	evolutionary(struct parameter psim_parameter, std::vector<unsigned> ptopology);

	int evolve(int id_algo);


	std::vector< std::vector<double> > process(std::vector< std::vector<double> > inputvals_vector);

	void save_bestFitness();
	void save_averageFitness();

	void save_bestAgent();

	int get_bestFitness_overall();

	void set_trainingdata(std::vector< std::vector<float> > ptrainingdata);


private:	//PRIVATE

	void randomize_population();

	
	int evolve_hillclimber();
	int evolve_simulatedannealing();
	int evolve_learn(); //Supervised Learning
	//Todo: Jonathan: Add recombination-method and maybe add roulettewheel-method

	void hillclimber(Agent *Agent, bool revert);
	void simulated_annealing(Agent *Agent, bool revert);
	void learn(int plearn_cycles);

	//void mutation();
	//void crossover();


};
#endif 
