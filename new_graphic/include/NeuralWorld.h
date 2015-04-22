#ifndef NEURAL_WORLD_H
#define NEURAL_WORLD_H

#include <time.h>		//For initialising the generator und for timemeasurement
#include "feedForwardNet.hpp"

#define FORCE_FORWARD   0
#define FORCE_BACKWARD  1
#define FORCE_ROT_LEFT  2
#define FORCE_ROT_RIGHT 3

#define MODE_SINGLEPLAYER 0
#define MODE_EVOLUTION 1
#define MODE_BESTAgentS 2

#define M_PI 3.14159265359
#define DEGTORAD (M_PI/180)
#define AMOUNT_OF_Object 40

#define TYPE_Agent 0
#define TYPE_Object 1
#define TYPE_OBSTACLE 2

#include "Test.h"
#include "Agent.h"

const b2Vec2 triangle[3] = {b2Vec2(0.0f, 2.0f), b2Vec2(-1.0f, -1.0f), b2Vec2(1.0f, -1.0f)}; // Triangle-shape

// This is used to test sensor shapes.
class NeuralWorld : public Test
{

public:

private:
	bool	keys[256];			// Array Used For The Keyboard Routine
	unsigned int amount_Object;
	unsigned int amount_of_all_objects;

	int field_size;

	int mode;
	bool force[4];

	std::vector<Agent *> *population;

	//Agent
	std::vector<b2Body *> Agentbody;
	std::vector<b2FixtureDef> AgentFixtureDef;
	std::vector<b2Fixture *> AgentFixture;
	std::vector<b2BodyDef> AgentBodydef;
	//Agent Sensor
	std::vector<b2FixtureDef> SensorFixtureDef;
	std::vector<b2Fixture *> SensorFixture;
	//Object
	std::vector<b2Body *> Objectbody;
	std::vector<b2FixtureDef> ObjectFixtureDef;
	std::vector<b2Fixture *> ObjectFixture;
	std::vector<b2BodyDef> ObjectBodydef;
	//Obstacles
	std::vector<b2Body *> Obstaclebody;
	std::vector<b2FixtureDef> ObstacleFixtureDef;
	std::vector<b2Fixture *> ObstacleFixture;
	std::vector<b2BodyDef> ObstacleBodydef;

	std::vector< std::vector<bool> > *touches;
	std::vector<bool> active;

//Methods
public:
	NeuralWorld();
	NeuralWorld(std::vector<Agent *> *ppopulation, int pamount_Object, int field_size, int pmode);
	static Test* Create(std::vector<Agent *> *ppopulation, int pamount_Object, int pfield_size, int pmode);
	void Step(Settings* settings);
	std::vector< std::vector<double> > get_sensor_vectors();
	void move_bodies(std::vector< std::vector<double> > output_vectors);
	std::vector<double> move_player();

	void Reset();

	void Keyboard(unsigned char key);
	void KeyboardUp(unsigned char key);

private:
	void Init();

	//void PreSolve(b2Contact* contact, const b2Manifold* oldManifold); //necessary?
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);

	//Helpermethods
	double correct_angle(double angle);

};

#endif
