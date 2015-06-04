/*
* Copyright (c) 2015 Daniel Schembri https://github.com/Daniel-Schembri/neural_networks
*
* neuralworld.h
* This header-file uses Box2D by Erin Catto 
*
* new_graphic
* This is a graphical user interface to evolve object collecting agents 
* The agent have two neural networks for steering angle and velocity.
*
* This project includes the neural net framework, called logic,
* of Jonathan Schwarz https://github.com/jonathan-schwarz/neural_networks
*
* new_graphic also uses the Box2D-library by Erin Catto
*
* At following the license for Box2D:
*
* Copyright (c) 2006-2007 Erin Catto http://www.box2d.org
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#ifndef NEURAL_WORLD_H
#define NEURAL_WORLD_H

#include <math.h>		//For initialising the generator und for timemeasurement
#include <time.h>		//For initialising the generator und for timemeasurement
#include "feedForwardNet.hpp"

#define FORCE_FORWARD   0
#define FORCE_BACKWARD  1
#define FORCE_ROT_LEFT  2
#define FORCE_ROT_RIGHT 3

#define MODE_SINGLEPLAYER 0
#define MODE_EVOLUTION 1
#define MODE_BESTAgentS 2

#define DEGTORAD (M_PI/180)
#define AMOUNT_OF_Object 40

#define TYPE_Agent 0
#define TYPE_Object 1
#define TYPE_OBSTACLE 2

#include "test.h"
#include "agent.hpp"

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
