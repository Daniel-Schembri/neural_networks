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

using namespace std;

// This is used to test sensor shapes.
class NeuralWorld : public Test
{
public:
//Timemeasurement
unsigned long long iterationsteps;
time_t t_start, t_end;
double time_diff;
//Neural Net, Sensor-Input, Output
vector<unsigned> topologynr1;
Net* mynet;
vector<double> inputVals;
vector<double> outputvals;
//Fitness function
double mutation_rate;
double annealing_rate;
double max_delta;
double revert_hillclimber[100][100][100];
int last_fitness;
int fitness;
int best_fitness;
//
	bool net_initialised;
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

	b2Body* food[AMOUNT_OF_FOOD];	
	b2Fixture* fixture_food[AMOUNT_OF_FOOD];
	b2FixtureDef fixturedef_food[AMOUNT_OF_FOOD];
	bool food_active[AMOUNT_OF_FOOD];

	//walls
	b2Body* wall[4];
	b2Fixture* fixture_wall[4];

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
	for(int i=0; i<AMOUNT_OF_FOOD;i++)
	{
	food_active[i] = true;
	}
	net_initialised = false;
	fitness = 0;
	last_fitness = 0;
        best_fitness = 0;
	mutation_rate = 0.1f;
        annealing_rate = 100;
	max_delta = 0.5f;
	for(int i=0;i<100;i++)
	{
	  for(int j=0;j<100;j++)
	  { 
	    for(int k=0;k<100;k++)
	    {
	       revert_hillclimber[i][j][k] = 0;
            }
          }
        }
//Time
time_diff = 0;
iterationsteps = 0;
//Inputvalues
inputVals.push_back(0.0f);inputVals.push_back(0.0f);inputVals.push_back(0.0f);
	//Init End
		{
			b2BodyDef bd;
			for (int32 i = 0; i < 4; i++)
			{
				wall[i] = m_world->CreateBody(&bd);
			}
			{
				b2EdgeShape shape[4];
				shape[0].Set(b2Vec2(-100.0f, 0.0f), b2Vec2(0.0f, 0.0f));
				fixture_wall[0] = wall[0]->CreateFixture(&shape[0], 0.0f);
				fixture_wall[0]->SetUserData(touching + 50);

				shape[1].Set(b2Vec2(-100.0f, 0.0f), b2Vec2(-100.0f, 100.0f));
				fixture_wall[1] = wall[1]->CreateFixture(&shape[1], 0.0f);
				fixture_wall[1]->SetUserData(touching + 51);

				shape[2].Set(b2Vec2(0.0f, 0.0f), b2Vec2(0.0f, 100.0f));
				fixture_wall[2] = wall[2]->CreateFixture(&shape[2], 0.0f);
				fixture_wall[2]->SetUserData(touching + 52);				

				shape[3].Set(b2Vec2(-100.0f, 100.0f), b2Vec2(0.0f, 100.0f));
				fixture_wall[3] = wall[3]->CreateFixture(&shape[3], 0.0f);
				fixture_wall[3]->SetUserData(touching + 53);
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
			for (int32 i = 0; i < AMOUNT_OF_FOOD; i++)
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
	 time (&t_start);
	}

	// Implement contact listener.
	void BeginContact(b2Contact* contact)
	{
		b2Fixture* fixtureA = contact->GetFixtureA();
		b2Fixture* fixtureB = contact->GetFixtureB();
		
		if (fixtureA == m_fixture_sensor)
		{
		   
			for (int32 i = 0; i < AMOUNT_OF_FOOD; i++)
			{
			    if(food_active[i])
			    {
				if (fixtureB == fixture_food[i])
				{
					touch[2][100+i] = true;
				}
			    }
			}
			for (int32 i = 0; i < 4; i++)
			{
				if (fixtureB == fixture_wall[i])
				{
					touch[2][50 + i] = true;
				}
			}
		}
		if (fixtureA == Fixtureplayer)
		{
			for (int32 i = 0; i < AMOUNT_OF_FOOD; i++)
			{
			    if(food_active[i])
			    {
				if (fixtureB == fixture_food[i])
				{
				    fitness++;
				    touch[2][100+i] = false;
				    food_active[i] = false;
				}
			    }
			}
		}
		if (fixtureB == Fixtureplayer)
		{
			for (int32 i = 0; i < AMOUNT_OF_FOOD; i++)
			{
			    if(food_active[i])
			    {
				if (fixtureA == fixture_food[i])
				{
				    fitness++;
				    touch[2][100+i] = false;
				    food_active[i] = false;
				}
			    }
			}
		}
		if (fixtureB == m_fixture_sensor)
		{
			for (int32 i = 0; i < AMOUNT_OF_FOOD; i++)
			{
			    if(food_active[i])
			    {
				if (fixtureA == fixture_food[i])
				{
					touch[2][100 + i] = true;
				}
			    }
			}
			for (int32 i = 0; i < 4; i++)
			{
				if (fixtureA == fixture_wall[i])
				{
					touch[2][50 + i] = true;
				}
			}
		}
	}

	// Implement contact listener.
	void EndContact(b2Contact* contact)
	{
		b2Fixture* fixtureA = contact->GetFixtureA();
		b2Fixture* fixtureB = contact->GetFixtureB();
	
		if (fixtureA == m_fixture_sensor)
		{
			for (int32 i = 0; i < AMOUNT_OF_FOOD; i++)
			{
				if (fixtureB == fixture_food[i])
				{
					touch[2][100 + i] = false;
				}
			}
			for (int32 i = 0; i < 4; i++)
			{
				if (fixtureB == fixture_wall[i])
				{
					touch[2][50 + i] = false;
				}
			}
		}

		if (fixtureB == m_fixture_sensor)
		{
			for (int32 i = 0; i < AMOUNT_OF_FOOD; i++)
			{
				if (fixtureA == fixture_food[i])
				{
					touch[2][100 + i] = false;
				}
			}
			for (int32 i = 0; i < 4; i++)
			{
				if (fixtureA == fixture_wall[i])
				{
					touch[2][50 + i] = false;
				}
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



   b2Vec2* get_FoodVectors()
   {
    b2Vec2 vectorlist[10];
    //Todo Malloc vectorlist
	for (int32 i = 0; i < AMOUNT_OF_FOOD; i++)
	{
  	   if (touch[2][100 + i])
	   {
           vectorlist[i] = food[i]->GetPosition();
           }
        }

    return vectorlist;
   }


//The 2 Output Neurons call this function
void control_creature(float32 speed, float32 angle)
{
if (speed > 0.075f) speed = 0.075f;
if (speed < -0.075f) speed = -0.075f;
if (angle > 0.075f) speed = 0.075f;
if (angle < 0.075f) speed = -0.075f;

player->ApplyForce(b2Vec2(-sin(player->GetAngle())*speed*500, cos(player->GetAngle())*speed*500), player->GetWorldCenter(), true);
player->SetTransform(player->GetPosition(), player->GetAngle()+angle);
}

void correct_angle()
{
//Correction of PlayerAngle. //TODO: Performance at high Angle Value => Needs more loops to correct angle!
    while ( player_rotation < -180 * DEGTORAD) player_rotation += 360*DEGTORAD;
    while ( player_rotation >  180 * DEGTORAD) player_rotation -= 360*DEGTORAD;	
}

void ki_helper()
{

    for (int32 i = 0; i < 10; i++)
    {
      if (touch[2][100 + i])
      { 
        b2Vec2 toTarget =  food[i]->GetPosition() - player->GetPosition();
        float32 desiredangle = atan2f(-toTarget.x, toTarget.y);
    	//touches++; //Todo
    	desiredangle = atan2f(-toTarget.x, toTarget.y);
    	player_rotation = player->GetAngle();
      }
    }
}


    void simpleKI(int32 touches)  //touches is the amount of food detected by sensor
    {
    //Correction of PlayerAngle. //TODO: Performance at high Angle Value => Needs more loops to correct angle!
    while ( player_rotation < -180 * DEGTORAD) player_rotation += 360*DEGTORAD;
    while ( player_rotation >  180 * DEGTORAD) player_rotation -= 360*DEGTORAD;	
    for (int32 i = 0; i < AMOUNT_OF_FOOD; i++)
    {
      if (touch[2][100 + i])
      { 
        b2Vec2 toTarget =  food[i]->GetPosition() - player->GetPosition();
        float32 desiredangle = atan2f(-toTarget.x, toTarget.y);
    	//touches++; //Todo
    	desiredangle = atan2f(-toTarget.x, toTarget.y);
    	player_rotation = player->GetAngle();

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

  //No touch with food?
  if (0 == touches) //Then drive randomly
  {
    for (int32 i = 0; i < 4; i++)
    {
      if (touch[2][50 + i])
      {
        b2Vec2 toWall =  wall[i]->GetPosition() - player->GetPosition();
        float32 wallangle = atan2f(-toWall.x, toWall.y);
        if( (wallangle - player_rotation > 0) && (wallangle - player_rotation < M_PI))
        {
        player->ApplyForce(b2Vec2(sin(player->GetAngle())*0.075f*500, -cos(player->GetAngle())*0.075f*500), player->GetWorldCenter(), true);
        }
      }

    }
  }
 else
      {
      player->ApplyForce(b2Vec2(-sin(player->GetAngle())*0.075f*500, cos(player->GetAngle())*0.075f*500), player->GetWorldCenter(), true);
      player->SetTransform(player->GetPosition(), player->GetAngle()+0.015f); //Rotate Left
      }

}

//Evolutionary Algorithms
void hillclimber_algorithm(bool revert)
{
double delta = 0.0f;
double randomval = 0.0f;

  for (unsigned int i=0; i<topologynr1.size();i++)  //Amount of Layers
  {
     for(unsigned int j=0;j<topologynr1[i];j++)    //Amount of Neurons
     {
       for(unsigned int w=0;w<mynet->m_layers[i][j]->m_outputWeights.size();w++)
       {
         delta = ((((double) rand() / double(RAND_MAX))-0.5f)*(int) 1000) / (1000.0f);  //Range -0.5f to +0.5f //resolution 0.001f
         randomval = (double) rand() / double(RAND_MAX); //
         if( randomval < mutation_rate )
         {
          if (revert)
          {
             mynet->m_layers[i][j]->m_outputWeights[w].weight -= revert_hillclimber[i][j][w];
          }
          else
          {
             mynet->m_layers[i][j]->m_outputWeights[w].weight += delta;
             revert_hillclimber[i][j][w] = delta;
          }
         }
       }
     }
  }
//get_neuron_weights(
}

void simulatedannealing_algorithm()
{
double delta = 0.0f;
double randomval = 0.0f;

  for (unsigned int i=0; i<topologynr1.size();i++)  //Amount of Layers
  {
     for(unsigned int j=0;j<topologynr1[i];j++)    //Amount of Neurons
     {
       for(unsigned int w=0;w<mynet->m_layers[i][j]->m_outputWeights.size();w++)
       {
         delta = ((((double) rand() / double(RAND_MAX))-0.5f)*(int) 1000) / (1000.0f);  //Range -0.5f to +0.5f //resolution 0.001f
         randomval = (double) rand() / double(RAND_MAX); //
         if( randomval < mutation_rate * annealing_rate )
         {
           mynet->m_layers[i][j]->m_outputWeights[w].weight += delta;
           revert_hillclimber[i][j][w] = delta;
         }
       }
     }
  }
if (annealing_rate > 1)
   annealing_rate--;
}
void fitness_function()
{
time (&t_end);
time_diff = difftime(t_end, t_start);
 m_debugDraw.DrawString(0, 400, "Timer: %f", time_diff);
 m_debugDraw.DrawString(0, 300, "Last_Fitness: %d ", last_fitness);
 m_debugDraw.DrawString(0, 350, "Fitness: %d ", fitness);
 m_debugDraw.DrawString(0, 450, "best_fitness: %d ", best_fitness);
 m_debugDraw.DrawString(0, 500, "iterationsteps: %d ", iterationsteps);
//Fitness function
if (time_diff >= 10)
  {
    //Reset food
    for(int i=0; i<AMOUNT_OF_FOOD;i++)
    {
      food_active[i] = true;
    }

    time (&t_start);
simulatedannealing_algorithm();
/*
    if(last_fitness > fitness)
      hillclimber_algorithm(true);

    hillclimber_algorithm(false);
*/ 
    if (fitness > best_fitness)
      best_fitness = fitness;
    last_fitness = fitness; 
    fitness = 0;
    iterationsteps = 0;


  }

}

void neuralnet_init()
{
topologynr1.push_back(3); //Input-layer
topologynr1.push_back(6); //Hidden-layer
topologynr1.push_back(2); //Output-layer
mynet = new Net(topologynr1, true);
mynet->randomize_layer();
net_initialised = true;
}

void neuralnet()
{
mynet->feedForward(inputVals);
mynet->getResults(outputvals);
player->SetTransform(player->GetPosition(), player->GetAngle()+0.075f*outputvals[0]);
player->ApplyForce(b2Vec2(-sin(player->GetAngle())*0.075f*500*outputvals[1], cos(player->GetAngle())*0.075f*500*outputvals[1]), player->GetWorldCenter(), true);
}


	void Step(Settings* settings)
	{
		iterationsteps++;
		Test::Step(settings);
		check_force();
		if (!net_initialised) neuralnet_init();
		int32 touches = 0;
		b2Color distance_color(255,0,0);
		
		inputVals[0] = 0.0f; inputVals[1] = 0.0f; // if no food detected x and y = 0
		for (int32 i = 0; i < AMOUNT_OF_FOOD; i++)
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
		
			inputVals[0] = a; inputVals[1] = b; //The difference vector of the last detected food

//player->SetTransform(player->GetPosition(), desiredangle);



			}
			correct_angle();
			inputVals[2] =  player_rotation; //player_angle
			if (i == 9)
			{
				m_debugDraw.DrawString(5, m_textLine, "Sensor touches %d Foods", touches);
				m_debugDraw.DrawString(0, 100, "Player Angle: %f", player->GetAngle());


				touches = 0;
			}
		}
	        //simpleKI(touches);
		neuralnet(); //inputvals is given globally
		fitness_function();		

	}


	static Test* Create()
	{
		return new NeuralWorld;
	}
};
#endif
