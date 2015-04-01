#ifndef NEURAL_WORLD_H
#define NEURAL_WORLD_H

#include <time.h>		//For initialising the generator und for timemeasurement


//#include "neural_network/commonDefs.hpp"
#include "neural_network/net.hpp"
//#include "neural_network/neuron.hpp"

#define FORCE_FORWARD   0
#define FORCE_BACKWARD  1
#define FORCE_ROT_LEFT  2
#define FORCE_ROT_RIGHT 3

#define M_PI 3.14159265359
#define DEGTORAD (M_PI/180)
#define AMOUNT_OF_FOOD 40

#include "Test.h"
#include "Creature.h"

const b2Vec2 triangle[3] = {b2Vec2(0.0f, 2.0f), b2Vec2(-1.0f, -1.0f), b2Vec2(1.0f, -1.0f)}; // Triangle-shape
//const b2Vec2 food_rect[2] = {b2Vec2(0.0f, 0.0f), b2Vec2(0.4f, 0.4f)};
//const b2Vec2 wall_rect_horizontal[2] = {b2Vec2((-50.0f), 0.0f), b2Vec2(50.0f, 1.0f)};
//const b2Vec2 wall_rect_vertical[2] = {b2Vec2(0.0f,(-50.0f)), b2Vec2(1.0f, 50.0f)};

// This is used to test sensor shapes.
class NeuralWorld : public Test
{

private:
bool	keys[256];			// Array Used For The Keyboard Routine
int amount_food;
int amount_obstacles;
int amount_of_all_objects;

int field_size;

void Init();
double correct_angle(double angle);

public:

std::vector<Creature *> *population;

//Creature
std::vector<b2Body *> Animal;
std::vector<b2FixtureDef> AnimalFixtureDef;
std::vector<b2Fixture *> AnimalFixture;
std::vector<b2BodyDef> AnimalBodydef;
//Creature Sensor
std::vector<b2FixtureDef> SensorFixtureDef;
std::vector<b2Fixture *> SensorFixture;
//Food
std::vector<b2Body *> Food;
std::vector<b2FixtureDef> FoodFixtureDef;
std::vector<b2Fixture *> FoodFixture;
std::vector<b2BodyDef> FoodBodydef;
//Obstacles

std::vector<b2Body *> Obstacle;
std::vector<b2FixtureDef> ObstacleFixtureDef;
std::vector<b2Fixture *> ObstacleFixture;
std::vector<b2BodyDef> ObstacleBodydef;


/*
b2Body** Animal;
b2FixtureDef** AnimalFixtureDef;
b2Fixture** AnimalFixture;
b2BodyDef** AnimalBodydef;
//Creature Sensor
b2Fixture** SensorFixture;
b2FixtureDef** SensorFixtureDef;

b2Body** Food;
b2FixtureDef** FoodFixtureDef;
b2Fixture** FoodFixture;
b2BodyDef** FoodBodydef;


b2Body** Obstacle;
b2FixtureDef** ObstacleFixtureDef;
b2Fixture** ObstacleFixture;
b2BodyDef** ObstacleBodydef;
*/
std::vector< std::vector<bool> > *touches;
std::vector<bool> active;

//bool** touches;
//bool* active;

NeuralWorld();
NeuralWorld(std::vector<Creature *> *ppopulation, int pamount_food, int pamount_obstacles, int field_size);
void Step(Settings* settings);
std::vector< std::vector<double> > get_sensor_vectors();
void move_bodies(std::vector< std::vector<double> > output_vectors);

//void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
void EndContact(b2Contact* contact);
void BeginContact(b2Contact* contact);

void Reset();

static Test* Create(std::vector<Creature *> *ppopulation, int pamount_food, int pamount_obstacles, int pfield_size);
/*
NeuralWorld(std::vector<Creature *> *ppopulation, int pamount_food, int pamount_obstacles)
static Test* Create(std::vector<Creature *> *ppopulation)

*/
};

#endif
