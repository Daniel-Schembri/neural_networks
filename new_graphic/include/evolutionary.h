#include <time.h>		//For initialising the random_generator
#include <vector>
#include "Creature.h"

#define MODE_SINGLEPLAYER 0
#define MODE_EVOLUTION 1
#define MODE_BESTCREATURES 2

struct parameter
{
	unsigned int population_size;
	unsigned int amount_of_food;
	unsigned int field_size;
	unsigned int evolvetime;
	int evolve_algorithm;
	int mode;
	int random;
	float mutation_rate;
	int annealing_rate;
};

class evolutionary
{

private:
void randomize_population();

public:
struct parameter sim_parameter;

std::vector< std::vector<double> > inputvals_vector;

std::vector<Creature *> population;
std::vector<Creature *> best_creatures;

//TrainingData
std::vector< std::vector<float> > trainingdata;

//The Fitnesses over generations
std::vector<int> best_fitnesses;
std::vector<float> average_fitnesses;

//Fitness function
unsigned int iterationsteps;
unsigned int evolvesteps;
unsigned int generations;

double max_delta;
double revert_hillclimber[100][100][100];


evolutionary();
evolutionary(struct parameter psim_parameter, std::vector<unsigned> ptopology);

void hillclimber(Creature *creature, bool revert);
void simulated_annealing(Creature *creature, bool revert);
void mutation();
void crossover();

int evolve(int id_algo);
int evolve_hillclimber();
int evolve_simulatedannealing();
int evolve_learn();

std::vector< std::vector<double> > process(std::vector< std::vector<double> > inputvals_vector);

void save_bestFitness();
void save_averageFitness();

void save_bestCreature();

int get_bestFitness_overall();

void set_trainingdata(std::vector< std::vector<float> > ptrainingdata);

};
