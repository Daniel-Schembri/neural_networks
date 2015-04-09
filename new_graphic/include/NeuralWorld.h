#ifndef NEURAL_WORLD_H
#define NEURAL_WORLD_H

#include <time.h>		//For initialising the generator und for timemeasurement
#include "FeedForwardNet.hpp"

#define FORCE_FORWARD   0
#define FORCE_BACKWARD  1
#define FORCE_ROT_LEFT  2
#define FORCE_ROT_RIGHT 3

#define MODE_SINGLEPLAYER 0
#define MODE_EVOLUTION 1
#define MODE_BESTCREATURES 2

#define M_PI 3.14159265359
#define DEGTORAD (M_PI/180)
#define AMOUNT_OF_FOOD 40

#define TYPE_CREATURE 0
#define TYPE_FOOD 1
#define TYPE_OBSTACLE 2

#include "Test.h"
#include "Creature.h"

const b2Vec2 triangle[3] = {b2Vec2(0.0f, 2.0f), b2Vec2(-1.0f, -1.0f), b2Vec2(1.0f, -1.0f)}; // Triangle-shape

// This is used to test sensor shapes.
class NeuralWorld : public Test
{

private:
bool	keys[256];			// Array Used For The Keyboard Routine
int amount_food;
int amount_of_all_objects;

int field_size;

int mode;
bool force[4];

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

std::vector< std::vector<bool> > *touches;
std::vector<bool> active;

NeuralWorld();
NeuralWorld(std::vector<Creature *> *ppopulation, int pamount_food, int field_size, int pmode);
void Step(Settings* settings);
std::vector< std::vector<double> > get_sensor_vectors();
void move_bodies(std::vector< std::vector<double> > output_vectors);

//void PreSolve(b2Contact* contact, const b2Manifold* oldManifold); //necessary?
void EndContact(b2Contact* contact);
void BeginContact(b2Contact* contact);

void Keyboard(unsigned char key);
void KeyboardUp(unsigned char key);

std::vector<double> move_player();

void Reset();

static Test* Create(std::vector<Creature *> *ppopulation, int pamount_food, int pfield_size, int pmode);

};

#endif
