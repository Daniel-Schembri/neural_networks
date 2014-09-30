/*
Box2D-Class for physics-simulation.
Daniel Schembri 2014-09-14
*/
//#include <stdarg.h>
#include "physics.h"

// This is a simple example of building and running a simulation
// using Box2D. Here we create a large ground box and a small dynamic
// box.
// There are no graphics for this example. Box2D is meant to be used
// with your rendering eongine in your game engine.

	void MyContactListener::BeginContact(b2Contact* contact)
	{ /* handle begin event */
//	b2WorldManifold worldmanifold;
	b2Fixture* fixtureA = contact->GetFixtureA(); b2Fixture* fixtureB = contact->GetFixtureB();
	b2Body* bodyA = fixtureA->GetBody(); b2Body* bodyB = fixtureB->GetBody();
	Creature* A; 
	Creature* B;
	A = (Creature*)bodyA->GetUserData(); B = (Creature*)bodyB->GetUserData();

	if (!fixtureA->IsSensor())
	{
		if (1 == (A->get_ID())) //Player-ID?
		{
			if (200 <= (B->get_ID())) //collision with food?
			{
				A->incfoodpoints();
				B->set_inactive();	//Delete Food
				return;
			}
			else
			{
				return;
			}
		}
	}
	else
	{
		if (1 == (A->get_ID())) //Player-ID?
		{
			if (200 <= (B->get_ID())) // Sensor detected food?
			{
		//		contact->GetWorldManifold(&worldManifold);
				//	A->incfoodpoints();
				//	B->set_inactive();	//Delete Food
				return;
			}
			else  //Sensor detected something else
			{
				return;
			}
		}
	}
		if (!fixtureB->IsSensor())
		{
			if (1 == (B->get_ID())) //Player-ID?
			{
				if (200 <= (A->get_ID())) //collision with food?
				{
					B->incfoodpoints();
					A->set_inactive();	//Delete Food
					return;
				}
				else
				{
					return;
				}
			}
		}
		else
		{
			if (1 == (B->get_ID())) //Player-ID?
			{
				if (200 <= (A->get_ID())) //sensor detected food?
				{

					return;
				}
				else  //Sensor detected something else
				{
					return;
				}
			}
		}
	}

	void MyContactListener::EndContact(b2Contact* contact)
	{ /* handle end event */

	}

	void MyContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
	{ /* handle pre-solve event */

	}

	void MyContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
	{ /* handle post-solve event */

	}

	int World::init()
	{
		// Prepare for simulation. Typically we use a time step of 1/60 of a
		// second (60Hz) and 10 iterations. This provides a high quality simulation
		// in most game scenarios.
		timeStep = 1.0f / 60.0f;
		velocityIterations = 6;
		positionIterations = 2;
		return 0;
	}

    World::World()
	{
		gravity = b2Vec2(0.0f, 0.0f);
		// Construct a world object, which will hold and simulate the rigid bodies.
		world = new b2World(b2Vec2(gravity));
		contactlistener = new MyContactListener();
		world->SetContactListener(contactlistener);
		init();
	}

	World::~World()
	{
		delete world;
		delete contactlistener;
	}
	
	b2World* World::getWorld()
	{
		return world;
	}

	int World::prepare_simulation()
	{
		//nu?
		return 0;
	}

	int World::simulate()
	{
		// Instruct the world to perform a single step of simulation.
		// It is generally best to keep the time step and iterations fixed.
		world->Step(timeStep, velocityIterations, positionIterations);

		return 0;
	}

	int Object::fixture_Object()
	{
		//The dynamic body fixture
		m_fixtureDef.shape = &m_dynamicShape;
		// Set the box density to be non-zero, so it will be dynamic.
		m_fixtureDef.density = 1.0f;
		// Override the default friction.
		m_fixtureDef.friction = 0.3f;
		// Add the shape to the body.
		m_fixture = m_body->CreateFixture(&m_fixtureDef);
		return 0;
	}

	Object::Object(b2World* p_world, int32 p_id, b2Vec2 p_position, b2Vec2 *p_shape, int p_shapepoints, float32 p_angle, int p_objecttype, b2BodyType btype)
	{
		m_id = p_id;
		m_bodyDef.type = btype;
		m_bodyDef.position.Set(p_position.x, p_position.y);
		m_bodyDef.linearDamping = 0.07f;
		m_world = p_world;
		m_body = m_world->CreateBody(&m_bodyDef); //
		m_body->SetUserData(this);  //Set a reference to Object_ID to differentiate objects in the contactlistener
		m_objecttype = p_objecttype;
		set_Polygonvectors(p_shape, p_shapepoints);
		shape_Object();
	}
	
	int Object::get_ID()
	{
		return m_id;
	}

	bool Object::is_awake()
	{
		return m_body->IsAwake();
	}

	void Object::set_inactive()
	{
		m_body->SetAwake(false);
		//m_body->SetActive(false);
	}

	void Object::set_Polygonvectors(b2Vec2* b1, int p_size)
	{
		m_polygonpoints = p_size;
		for (int i = 0; i < m_polygonpoints; i++)
		{
			m_polygonVectors[i] = b1[i];
		}
	}

	int Object::get_Polygonsize()
	{
		return m_polygonpoints;
	}

	void Object::copy_Polygonvectors(b2Vec2* b1)
	{
		for (int i = 0; i < m_polygonpoints; i++)
		{
			b1[i] = m_polygonVectors[i];
		}
	}

	b2Vec2* Object::get_Polygonvectors()
	{
		return m_polygonVectors;
	}

	int Object::shape_Object(float32 p_X, float32 p_Y)
	{
		//Errorcheck?
		m_dynamicShape.SetAsBox(p_X, p_Y);
		fixture_Object();
		return 0;
	}

	int Object::shape_Object()
	{
		if (m_polygonpoints > 8 || m_polygonpoints <= 2) //Error, too few or too much points
			return 1;
		//m_polygonVectors = p_polygonVectors;
		//m_polygonpoints = p_polygonpoints;
		m_dynamicShape.Set(m_polygonVectors, m_polygonpoints);
		fixture_Object();
		return 0;
	}

	int Object::make_Sensor()
	{
		m_fixture->SetSensor(true);
		return 0;
	}
	b2Vec2 Object::getPosition()
	{
		return m_body->GetPosition();
	}

	float32 Object::getAngle()
	{
		return m_body->GetAngle();
	}
	int Object::get_Objecttype()
	{
		return m_objecttype;
	}

	//Todo: Function for neural network

	void Creature::check_maxvalue(b2Vec2 &value, b2Vec2 max_posval, b2Vec2 max_negval) //nu
	{
		//Positive velocity
		if (value.x > max_posval.x)
			value.x = max_posval.x;
		if (value.y > max_posval.y)
			value.y = max_posval.y;
		//Negative velocity
		if (value.x < max_negval.x)
			value.x = max_negval.x;
		if (value.y < max_negval.y)
			value.y = max_negval.y;
	}

	int Creature::update_velocity() //nu
	{
		m_velocity += m_acceleration;
		check_maxvalue(m_velocity, max_positive_velocity, max_negative_velocity);
		return 0; //Maybe return check_maxvelocity?
	}

	Creature::Creature(b2World* p_world, int32 p_id, b2Vec2 p_position, b2Vec2 *p_shape, int p_shapepoints, float32 p_angle, int32 p_hp, int p_objecttype) : Object(p_world, p_id, p_position, p_shape, p_shapepoints, p_angle, p_objecttype, b2_dynamicBody)
	{
		m_hp = p_hp;
		init_sensor();
		//m_body->SetAwake(true);
		//m_body->SetSleepingAllowed(false);
	}

	int Creature::init_sensor()
	{
		//add semicircle radar sensor to tower
		float radius = 25;
		//b2Vec2 vertices[8];
		m_sensor_vertices[0].Set(0, 0);
		for (int i = 0; i < 7; i++) {
			float angle = i / 6.0 * 180 * M_PI / 180;
			m_sensor_vertices[i + 1].Set(radius * cosf(angle), radius * sinf(angle));
		}
		m_sensorShape.Set(m_sensor_vertices, 8);
		m_fixturedef_sensor.shape = &m_sensorShape;
		m_fixture_sensor = m_body->CreateFixture(&m_fixturedef_sensor);
		m_fixture_sensor->SetSensor(true);
		return 0;
	}

	void Creature::setSpeed(float32 speedx, float32 speedy)
	{
		//m_body->SetLinearVelocity(b2Vec2(speedx, speedy));
		m_body->ApplyForce(b2Vec2(speedx, speedy), m_body->GetWorldCenter(), true);
	}

	void Creature::accelerate(b2Vec2 acceleration) //nu
	{
		//m_body->SetLinearVelocity()
		m_acceleration = acceleration;
		check_maxvalue(m_acceleration, max_positive_acceleration, max_negative_acceleration);
	}

	b2Vec2* Creature::get_Sensorvectors()
	{
		return m_sensor_vertices;
	}

	void Creature::changeAngle(float32 delta_angle)
	{
		m_body->SetTransform(m_body->GetPosition(), m_body->GetAngle()+delta_angle);
	}
	void Creature::incfoodpoints()
	{
		m_foodpoints++;
	}
