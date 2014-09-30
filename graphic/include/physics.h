/*
Box2D-Class for physics-simulation.
Daniel Schembri 2014-09-14
*/
#ifndef PHYSICS_H
#define PHYSICS_H

#include <Box2D/Box2D.h>
#include <stdio.h> //nu??

#define M_PI           3.14159265358979323846
#define OBJ_TRIANGLE 0
#define OBJ_RECT 1
#define OBJ_POLYGON 2
#define OBJ_CIRCLE 3

// This is a simple example of building and running a simulation
// using Box2D. Here we create a large ground box and a small dynamic
// box.
// There are no graphics for this example. Box2D is meant to be used
// with your rendering engine in your game engine.



class MyContactListener : public b2ContactListener
{
public:
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

};


class World
{
private:
	b2Vec2 gravity;
	b2World* world; 
	//b2Body* groundBody;
	//b2BodyDef groundBodyDef;
	b2PolygonShape groundBox;
	float32 timeStep;
	int32 velocityIterations;
	int32 positionIterations;
	MyContactListener* contactlistener;
	//b2ContactListener* contactlistener;

	int init();

public:
	World();
	~World();

	int prepare_simulation();
	int simulate();
	b2World* getWorld();

};

class Object
{
protected:
	int32 m_id;
	b2World* m_world;
	b2Body* m_body;
	b2BodyDef m_bodyDef;
	b2PolygonShape m_dynamicShape;
	b2Vec2 m_polygonVectors[8];
	int32 m_polygonpoints;
	b2Vec2 m_acceleration, m_velocity;
	b2FixtureDef m_fixtureDef;
	b2Fixture* m_fixture;

	int m_objecttype;  //objecttype means triangle, rect, polygon..

	int fixture_Object();

public:
	Object();
	Object(b2World* p_world, int32 p_id, b2Vec2 p_position, b2Vec2 *p_shape, int p_shapepoints, float32 p_angle, int p_objecttype, b2BodyType btype = b2_staticBody);
	int shape_Object(float32 p_X, float32 p_Y);
	int shape_Object();
	int get_Objecttype();
	int get_Polygonsize();
	int get_ID();
	bool is_awake();

	void set_inactive();
	void set_Polygonvectors(b2Vec2* b1, int p_size);
	void copy_Polygonvectors(b2Vec2* b1);
	b2Vec2* get_Polygonvectors();

	int make_Sensor();
	b2Vec2 getPosition();
	float32 getAngle();
};
#define MAX_AMOUNT_OF_SENSORS 10
class Creature : public Object
{
private:
	const b2Vec2 max_positive_velocity = b2Vec2(0.01f, 0.01f);
	const b2Vec2 max_negative_velocity = b2Vec2(0.005f, -0.005f);
	const b2Vec2 max_positive_acceleration = b2Vec2(0.0002f, 0.0002f);
	const b2Vec2 max_negative_acceleration = b2Vec2(-0.0002f, -0.0002f);
	//sensors
	b2FixtureDef m_fixture_sensors[MAX_AMOUNT_OF_SENSORS]; //nu
	int32 nr_of_sensors; //nu
	b2Fixture* m_fixture_sensor;
	b2PolygonShape m_sensorShape;
	b2FixtureDef m_fixturedef_sensor;
	int32 m_hp;
	int32 m_foodpoints;  //Our First Fitness-Score
	//Todo: neural network
	void check_maxvalue(b2Vec2 &value, b2Vec2 max_posval, b2Vec2 max_negval);
	int update_velocity();
	int init_sensor();
	b2Vec2 m_sensor_vertices[8];

public:
	Creature(b2World* p_world, int32 p_id, b2Vec2 p_position, b2Vec2 *p_shape, int p_shapepoints, float32 p_angle, int32 p_hp, int p_objecttype);
	void accelerate(b2Vec2 acceleration);
	void changeAngle(float32 delta_angle);
	void setSpeed(float32 speedx, float32 speedy);
	void incfoodpoints();
	b2Vec2* get_Sensorvectors();
};
#endif