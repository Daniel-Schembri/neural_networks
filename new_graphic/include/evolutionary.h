#include <time.h>		//For initialising the random_generator
#include <vector>
#include "Creature.h"

class evolutionary
{

private:
void randomize_population();

public:

std::vector< std::vector<double> > inputvals_vector;

std::vector<Creature *> population;
std::vector<Creature *> best_creatures;

//bool fixed_topology;

int population_size;
int amount_of_food;
int amount_of_obstacles;

//The Fitnesses over generations
std::vector<int> best_fitnesses;
std::vector<float> average_fitnesses;

int last_fitness;
int fitness;
int best_fitness;
//Fitness function
unsigned int iterationsteps;
unsigned int evolvesteps;
unsigned int generations;
double mutation_rate;
double annealing_rate;
double max_delta;
double revert_hillclimber[100][100][100];


evolutionary();
evolutionary(int ppopulation_size, int pamount_of_food, int pamount_of_obstacles, float pmutation_rate, unsigned int pevolvetime, std::vector<unsigned> ptopology); //, bool pfixed_topology);

void hillclimber(Creature *creature, bool revert);
void simulated_annealing(Creature *creature, bool revert);
void mutation();
void crossover();

int evolve_hillclimber();
int evolve_simulatedannealing();
int evolve(int id_algo);

std::vector< std::vector<double> > process(std::vector< std::vector<double> > inputvals_vector);

void save_bestFitness();
void save_averageFitness();

void save_bestCreature();

int get_bestFitness_overall();

//std::vector<int> get_Fitnesses();

};
