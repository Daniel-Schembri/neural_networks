#ifndef NEURAL_WORLD_H
#define NEURAL_WORLD_H

#include <time.h>		//For initialising the generator und for timemeasurement

#define FORCE_FORWARD   0
#define FORCE_BACKWARD  1
#define FORCE_ROT_LEFT  2
#define FORCE_ROT_RIGHT 3

#define M_PI 3.14159265359
#define DEGTORAD (M_PI/180)




// This is used to test sensor shapes.
class NeuralWorld : public Test
{
public:
	float32 player_rotation;
	bool touch[200][200];
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
	for (int i = 0; i < 200; i++)
	{
		for (int j = 0; j < 200; j++)
			touch[i][j] = false;
	}
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
	}

	// Implement contact listener.
	void BeginContact(b2Contact* contact)
	{
		b2Fixture* fixtureA = contact->GetFixtureA();
		b2Fixture* fixtureB = contact->GetFixtureB();
		
		if (fixtureA == m_fixture_sensor)
		{
			for (int32 i = 0; i < 10; i++)
			{
				if (fixtureB == fixture_food[i])
				{
					touch[2][100+i] = true;
				}
			}
		}

		if (fixtureB == m_fixture_sensor)
		{
			for (int32 i = 0; i < 10; i++)
			{
				if (fixtureA == fixture_food[i])
				{
					touch[2][100 + i] = true;
				}
			}
		}
		/*
		{
			void* userData = fixtureB->GetUserData(); //->GetBody()->GetUserData();
			if (userData)
			{
				bool* touching = (bool*)userData;
				*touching = true;
			}
		}
		*/
		/*
	//	if (fixtureB == m_sensor)
		{
			void* userData = fixtureA->GetUserData(); //->GetBody()->GetUserData();
			if (userData)
			{
				bool* touching = (bool*)userData;
				*touching = true;
			}
		}
	*/	
	}

	// Implement contact listener.
	void EndContact(b2Contact* contact)
	{
		b2Fixture* fixtureA = contact->GetFixtureA();
		b2Fixture* fixtureB = contact->GetFixtureB();
	
		if (fixtureA == m_fixture_sensor)
		{
			for (int32 i = 0; i < 10; i++)
			{
				if (fixtureB == fixture_food[i])
				{
					touch[2][100 + i] = false;
				}
			}
		}

		if (fixtureB == m_fixture_sensor)
		{
			for (int32 i = 0; i < 10; i++)
			{
				if (fixtureA == fixture_food[i])
				{
					touch[2][100 + i] = false;
				}
			}
		}

		/*
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
	*/
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



   b2Vec2* get_FoodVectors()
   {
    b2Vec2 vectorlist[10];
    //Todo Malloc vectorlist
	for (int32 i = 0; i < 10; i++)
	{
  	   if (touch[2][100 + i])
	   {
           vectorlist[i] = food[i]->GetPosition();
           }
        }

    return vectorlist;
   }



    void simpleKI(int32 touches)  //touches is the amount of food detected by sensor
    {
 	//touches = 0; //Todo
	for (int32 i = 0; i < 10; i++)
	{
		if (touch[2][100 + i])
		{
    		b2Vec2 toTarget =  food[i]->GetPosition() - player->GetPosition();
	        float32 desiredangle = atan2f(-toTarget.x, toTarget.y);
    		touches++; //Todo
    		desiredangle = atan2f(-toTarget.x, toTarget.y);
    		player_rotation = player->GetAngle();
	

	//Correction of PlayerAngle. //TODO: Performance at high Angle Value => Needs more loops to correct angle!
	while ( player_rotation < -180 * DEGTORAD) player_rotation += 360*DEGTORAD;
	while ( player_rotation >  180 * DEGTORAD) player_rotation -= 360*DEGTORAD;

    if( (desiredangle - player_rotation > 0) && (desiredangle - player_rotation < M_PI))
    {
player->SetTransform(player->GetPosition(), player->GetAngle()+0.075f); //Rotate Left
    }
    else if((desiredangle - player_rotation < 0) && (desiredangle - player_rotation > -M_PI))
    {
player->SetTransform(player->GetPosition(), player->GetAngle()-0.075f); //Rotate Right
    }

player->ApplyForce(b2Vec2(-sin(player->GetAngle())*0.075f*500, cos(player->GetAngle())*0.075f*500), player->GetWorldCenter(), true);

m_debugDraw.DrawString(0, 200, " Player: %f", player_rotation);
//m_debugDraw.DrawString(0, 200, "Difference Playerangle to Foodangle: %f", ((player->GetAngle()-angle)+(M_PI/2)));
m_debugDraw.DrawString(0, 250, "Desired Angle: %f", (desiredangle));
		}
	}
if (0 == touches) //Drive randomly
{
 player->ApplyForce(b2Vec2(-sin(player->GetAngle())*0.075f*500, cos(player->GetAngle())*0.075f*500), player->GetWorldCenter(), true);
player->SetTransform(player->GetPosition(), player->GetAngle()+0.075f); //Rotate Left
}
    }

	void Step(Settings* settings)
	{
		Test::Step(settings);
		check_force();
		int32 touches = 0;
		b2Color distance_color(255,0,0);
		for (int32 i = 0; i < 10; i++)
		{
			if (touch[2][100 + i])
			{
				touches++;
	//			m_debugDraw.DrawLine(&b2Vec2(0, 0), &b2Vec2(50, 50), distance_color);
				//player->GetAngle()
				m_debugDraw.DrawLine(&player->GetPosition(), &food[i]->GetPosition(), distance_color);
float32 angle;
float32 b = fabs(player->GetPosition().y - food[i]->GetPosition().y); //Fabs ist Betrag
float32 a = fabs(player->GetPosition().x - food[i]->GetPosition().x);
float32 c = sqrt((a*a) + (b*b));
angle = sinh(b/c);



//player->SetTransform(player->GetPosition(), desiredangle);



			}

			if (i == 9)
			{
				m_debugDraw.DrawString(5, m_textLine, "Sensor touches %d Foods", touches);
				m_debugDraw.DrawString(0, 100, "Player Angle: %f", player->GetAngle());
				touches = 0;
			}
		}
	simpleKI(touches);

	}

	static Test* Create()
	{
		return new NeuralWorld;
	}
};
#endif
