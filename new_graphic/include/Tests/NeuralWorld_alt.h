#ifndef NEURAL_WORLD_H
#define NEURAL_WORLD_H

#include <time.h>		//For initialising the generator und for timemeasurement

#define FORCE_FORWARD   0
#define FORCE_BACKWARD  1
#define FORCE_ROT_LEFT  2
#define FORCE_ROT_RIGHT 3


// This is used to test sensor shapes.
class NeuralWorld : public Test
{
public:
	bool touching[1000];			//For collision detections
	bool	force[4];			// forces to move the player
	enum
	{
		e_count = 7
	};

	b2Body* player;
	b2FixtureDef PlayerFixtureDef;
	b2Fixture* Fixtureplayer;

	b2Fixture* m_sensor;
	b2Body* m_bodies[e_count];
	bool m_touching[e_count];

	b2Body* food[10];	
	b2Fixture* fixture_food[10];
	b2FixtureDef fixturedef_food[10];

	b2FixtureDef m_fixturedef_sensor;
	b2Fixture* m_fixture_sensor;

	NeuralWorld()
	{
	//Init
	srand((unsigned)time(NULL)); // Zufallsgenerator initialisieren.
	for(int i=0;i<4;i++)
	   force[i] = false;
	for(int i=0;i<1000;i++)
	   touching[i] = false;
	//Init End
		{
			b2BodyDef bd;
			b2Body* walls[4];
			for (int32 i = 0; i < 4; i++)
			{
				walls[i] = m_world->CreateBody(&bd);
			}
			{
				b2EdgeShape shape[4];
				shape[0].Set(b2Vec2(-100.0f, 0.0f), b2Vec2(0.0f, 0.0f));
				walls[0]->CreateFixture(&shape[0], 0.0f);

				shape[1].Set(b2Vec2(-100.0f, 0.0f), b2Vec2(-100.0f, 100.0f));
				walls[1]->CreateFixture(&shape[1], 0.0f);

				shape[2].Set(b2Vec2(0.0f, 0.0f), b2Vec2(0.0f, 100.0f));
				walls[2]->CreateFixture(&shape[2], 0.0f);
				
				shape[3].Set(b2Vec2(-100.0f, 100.0f), b2Vec2(0.0f, 100.0f));
				walls[3]->CreateFixture(&shape[3], 0.0f);
			}
			//Make sensor
			{
			/*
			   	b2FixtureDef fd;
				fd.shape = &shape;
				fd.isSensor = true;
				m_sensor = ground->CreateFixture(&fd);
			*/
			}
			// Player
			{
			b2Vec2 polyvecs[3] = {b2Vec2(0.0f, 2.0f), b2Vec2(-1.0f, -1.0f), b2Vec2(1.0f, -1.0f)};
			b2PolygonShape poly;
			poly.Set(polyvecs, 3);			
			
			PlayerFixtureDef.shape = &poly;
		        PlayerFixtureDef.density = 1;
			// Override the default friction.
		        PlayerFixtureDef.friction = 0.3f;
			b2BodyDef bd_player;
			bd_player.linearDamping = 0.07f;
			bd_player.angularDamping = 0.1f;
			bd_player.type = b2_dynamicBody;
			bd_player.position.Set(-5.0f, 5.0f);
			
			//b2Body* player;
			player = m_world->CreateBody(&bd_player);

			Fixtureplayer = player->CreateFixture(&PlayerFixtureDef);
			player->SetAngularVelocity(0);	
			Fixtureplayer->SetUserData(touching + 1);		
		

			//Player Sensor-Radar
			//add semicircle radar sensor to tower
			float radius = 13;
			b2Vec2 m_sensor_vertices[8];
			b2PolygonShape m_sensorShape;
			m_sensor_vertices[0].Set(0, 0);
			for (int i = 0; i < 7; i++) {
				float angle = i / 6.0 * 180 * M_PI / 180;
				m_sensor_vertices[i + 1].Set(radius * cosf(angle), radius * sinf(angle));
			}
			m_sensorShape.Set(m_sensor_vertices, 8);
			m_fixturedef_sensor.shape = &m_sensorShape;
			m_fixture_sensor = player->CreateFixture(&m_fixturedef_sensor);
			m_fixture_sensor->SetSensor(true);
			m_fixture_sensor->SetUserData(touching + 2);
			}
			//Food
			{
			b2CircleShape shape;
			shape.m_radius = 0.4f;
			for (int32 i = 0; i < 10; i++)
			  {
			  b2BodyDef bdfood;
			  bdfood.type = b2_dynamicBody;
			  bdfood.position.Set(rand() % 90 - 90,rand() % 80 + 10);
			  fixturedef_food[i].shape = &shape;
		          fixturedef_food[i].density = 1;
		 	 // fixturedef_food[i].isSensor = true;
	   		  food[i] = m_world->CreateBody(&bdfood);
			  fixture_food[i] = food[i]->CreateFixture(&fixturedef_food[i]);
			  fixture_food[i]->SetSensor(true);
			  fixture_food[i]->SetUserData(touching + 100 + i);
			 // food[i]->SetAwake(false);
	   		  }
			}

		}
/*
		{
			b2CircleShape shape;
			shape.m_radius = 1.0f;

			for (int32 i = 0; i < e_count; ++i)
			{
				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.position.Set(-10.0f + 3.0f * i, 20.0f);
				bd.userData = m_touching + i;

				m_touching[i] = false;
				m_bodies[i] = m_world->CreateBody(&bd);

				m_bodies[i]->CreateFixture(&shape, 1.0f);
			}
		}
*/
	}

	// Implement contact listener.
	void BeginContact(b2Contact* contact)
	{
		b2Fixture* fixtureA = contact->GetFixtureA();
		b2Fixture* fixtureB = contact->GetFixtureB();
		
	//	bool* A = ((bool*)fixtureA->GetUserData());
	//	bool* B = ((bool*)fixtureB->GetUserData());
	//	*A = true; *B = true;

	//	if ( ((A == 2) && (B >= 100)) || ((B == 2) && (A >= 100)) )
	//	{
	//	m_debugDraw.DrawString(5, m_textLine, "Sensor touches Food");
	//	}
		

		//Todo Error: If 2 Food are touched the same and one gets out of reach, sensor dont detect the other one anymore
	//	if (fixtureA == m_sensor)
		{
			void* userData = fixtureB->GetUserData(); //->GetBody()->GetUserData();
			if (userData)
			{
				bool* touching = (bool*)userData;
				*touching = true;
			}
		}

	//	if (fixtureB == m_sensor)
		{
			void* userData = fixtureA->GetUserData(); //->GetBody()->GetUserData();
			if (userData)
			{
				bool* touching = (bool*)userData;
				*touching = true;
			}
		}
	
	}

	// Implement contact listener.
	void EndContact(b2Contact* contact)
	{
	
		b2Fixture* fixtureA = contact->GetFixtureA();
		b2Fixture* fixtureB = contact->GetFixtureB();

	//	if (fixtureA == m_sensor)
		{
			void* userData = fixtureB->GetUserData(); //->GetBody()->GetUserData();
			if (userData)
			{
				bool* touching = (bool*)userData;
				*touching = false;
			}
		}

	//	if (fixtureB == m_sensor)
		{
			void* userData = fixtureA->GetUserData(); //->GetBody()->GetUserData();
			if (userData)
			{
				bool* touching = (bool*)userData;
				*touching = false;
			}
		}
	
	}

void Keyboard(unsigned char key)
{
	switch (key)
	{
	case 'w':
		force[FORCE_FORWARD] = true;
		break;

	case 's':
		force[FORCE_BACKWARD] = true;
		break;

	case 'a':
		force[FORCE_ROT_LEFT] = true;
		break;

	case 'd':
		force[FORCE_ROT_RIGHT] = true;
		break;
	default: break;
	}
}

void KeyboardUp(unsigned char key) 
{
	switch (key)
	{
	case 'w':
		force[FORCE_FORWARD] = false;
		break;

	case 's':
		force[FORCE_BACKWARD] = false;
		break;

	case 'a':
		force[FORCE_ROT_LEFT] = false;
		break;

	case 'd':
		force[FORCE_ROT_RIGHT] = false;
		break;
	default: break;
	}
}
bool	keys[256];			// Array Used For The Keyboard Routine

void check_force()
{
//player->ApplyForce( b2Vec2(0,50), player->GetWorldCenter(), true);
if(force[FORCE_FORWARD])
player->ApplyForce(b2Vec2(-sin(player->GetAngle())*0.075f*500, cos(player->GetAngle())*0.075f*500), player->GetWorldCenter(), true);
if(force[FORCE_BACKWARD])
player->ApplyForce(b2Vec2(sin(player->GetAngle())*0.075f*500, -cos(player->GetAngle())*0.075f*500), player->GetWorldCenter(), true);
if(force[FORCE_ROT_LEFT])
player->SetTransform(player->GetPosition(), player->GetAngle()+0.075f);
//player->ApplyAngularImpulse(0.075f, true);
if(force[FORCE_ROT_RIGHT])
player->SetTransform(player->GetPosition(), player->GetAngle()-0.075f);
}

	void Step(Settings* settings)
	{
		Test::Step(settings);
		check_force();
		
		if (touching[2])
	{
		m_debugDraw.DrawString(3, m_textLine, "Sensor true");
		for (int32 i = 100; i < 110; i++) //Geht so nicht! Denkfehler wenn Sensor etwas anderes berührt und Food auch was anderes berührt
		{
			if (touching[i] == false)
			{
				continue;
			}
		m_debugDraw.DrawString(5, m_textLine, "Sensor touches Food");

		}
	}
/*	
		// Traverse the contact results. Apply a force on shapes
		// that overlap the sensor.
		for (int32 i = 0; i < e_count; ++i)
		{
			if (m_touching[i] == false)
			{
				continue;
			}

			b2Body* body = m_bodies[i];
			b2Body* ground = m_sensor->GetBody();

			b2CircleShape* circle = (b2CircleShape*)m_sensor->GetShape();
			b2Vec2 center = ground->GetWorldPoint(circle->m_p);

			b2Vec2 position = body->GetPosition();

			b2Vec2 d = center - position;
			if (d.LengthSquared() < FLT_EPSILON * FLT_EPSILON)
			{
				continue;
			}

			d.Normalize();
			b2Vec2 F = 100.0f * d;
			body->ApplyForce(F, position, false);
		}
*/	
	}

	static Test* Create()
	{
		return new NeuralWorld;
	}


};
#endif
