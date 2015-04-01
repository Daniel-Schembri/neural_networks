#include "NeuralWorld.h"


    NeuralWorld::NeuralWorld()
    {

    }


	NeuralWorld::NeuralWorld(std::vector<Creature *> *ppopulation, int pamount_food, int pamount_obstacles, int pfield_size)
	{
//    Test();

	population = ppopulation;
    amount_food = pamount_food; amount_obstacles = pamount_obstacles;
	field_size = pfield_size;
	//Init
	srand((unsigned)time(NULL)); // Zufallsgenerator initialisieren.
    //Animals

	b2FixtureDef fixturedef_filler;
	b2BodyDef    bodydef_filler;

	for (unsigned int i = 0; i < population->size(); i++)
	{
		Animal.push_back(NULL);
		AnimalFixtureDef.push_back(fixturedef_filler);
		AnimalFixture.push_back(NULL);
		AnimalBodydef.push_back(bodydef_filler);

		SensorFixtureDef.push_back(fixturedef_filler);
		SensorFixture.push_back(NULL);
	}

	for (unsigned int i = 0; i < amount_food; i++)
	{
		Food.push_back(NULL);
		FoodFixtureDef.push_back(fixturedef_filler);
		FoodFixture.push_back(NULL);
		FoodBodydef.push_back(bodydef_filler);
	}
	
	for (unsigned int i = 0; i < amount_obstacles; i++)
	{
		Obstacle.push_back(NULL);
		ObstacleFixtureDef.push_back(fixturedef_filler);
		ObstacleFixture.push_back(NULL);
		ObstacleBodydef.push_back(bodydef_filler);
	}

	amount_of_all_objects = population->size()*2 + amount_food + amount_obstacles;
	touches = new std::vector< std::vector<bool> >(amount_of_all_objects);
	std::vector<bool> touch;
	for (unsigned int i = 0; i < amount_of_all_objects; i++)
	{
		active.push_back(true);
		for (unsigned int j = 0; j < amount_of_all_objects; j++)
		{
			touch.push_back(false);
		}
		(*touches)[i] = touch;
		touch.clear();
	}

    Init();
	}

void NeuralWorld::Init()
{

  for (unsigned int i=0; i<population->size(); i++)
  {
    // Animal
	{
	b2PolygonShape* poly = new b2PolygonShape();
	poly->Set(triangle, 3);
	
	AnimalFixtureDef[i].shape = poly;
    AnimalFixtureDef[i].density = 1;
    AnimalFixtureDef[i].isSensor = false;
	// Override the default friction.
    AnimalFixtureDef[i].friction = 1.0f;
     AnimalFixtureDef[i].filter.maskBits = 0xFFFF;    //Important for Collisions working!
     AnimalFixtureDef[i].filter.categoryBits = 0x0001; //Important for Collisions working!
	AnimalBodydef[i].linearDamping = 1.0f;
	AnimalBodydef[i].angularDamping = 2.0f;
	AnimalBodydef[i].type = b2_dynamicBody;
    AnimalBodydef[i].active = true;    //Important that body is active!!
	AnimalBodydef[i].allowSleep = true;
//    AnimalBodydef[i].awake = true;
//    AnimalBodydef[i].bullet = true;

	b2Vec2 Animalposition; Animalposition.Set((-2 * field_size + 5.0f) + (rand() % (2 * field_size - 10)), (2 * field_size - 5.0f) - (rand() % (2 * field_size - 10)));
	

	
    //Position Correction
    for (unsigned int j=0;j<i;j++) //Look if Animalposition are to near to another Animal and correct it
      {
        if( ((AnimalBodydef[i].position.x-AnimalBodydef[i].position.y)*(AnimalBodydef[i].position.x-AnimalBodydef[i].position.y)) + (Animalposition.x-Animalposition.y) * (Animalposition.x-Animalposition.y)  < 4.0f )
        {
		  Animalposition; Animalposition.Set((-2 * field_size + 5.0f) + (rand() % (2 * field_size - 10)), (2 * field_size - 5.0f) - (rand() % (2 * field_size - 10)));
          j = 0; // and restart algorithm
        }
      }
	  
    AnimalBodydef[i].position.Set(Animalposition.x, Animalposition.y);
    //Position Correction End

   
	//b2Body* player;
	Animal[i] = m_world->CreateBody(&(AnimalBodydef[i]));


	AnimalFixture[i] = Animal[i]->CreateFixture(&(AnimalFixtureDef[i]));
//    AnimalFixture[i]->SetSensor(false);
	Animal[i]->SetAngularVelocity(0);	
	AnimalFixture[i]->SetUserData(touches + i);		
	//Set damping, so that animals dont float away
	//Animal[i]->SetLinearDamping(1.0f);
	//Animal[i]->SetAngularDamping(2.0f);

    //add semicircle sensor to Animal

	float radius = 13;
	b2Vec2 m_sensor_vertices[8];
	b2PolygonShape* m_sensorShape = new b2PolygonShape();
	m_sensor_vertices[0].Set(0, 0);
	for (unsigned int i = 0; i < 7; i++)
    {
	  float angle = i / 6.0 * 180 * M_PI / 180;
	  m_sensor_vertices[i + 1].Set(radius * cosf(angle), radius * sinf(angle));
	}

    m_sensorShape->Set(m_sensor_vertices, 8);
	SensorFixtureDef[i].shape = m_sensorShape;
    SensorFixtureDef[i].filter.maskBits = 0xFFFF;    //Important for Collisions working!
    SensorFixtureDef[i].filter.categoryBits = 0x0001; //Important for Collisions working!
	SensorFixture[i] = Animal[i]->CreateFixture(&(SensorFixtureDef[i]));
	SensorFixture[i]->SetSensor(true);
	SensorFixture[i]->SetUserData(touches + population->size() + i);
	} //Animal end
  } //For-Loop end

  for (int i=0; i<amount_food; i++)
  {
    
	b2PolygonShape* shape = new b2PolygonShape();
	shape->SetAsBox(0.4f, 0.4f);
    FoodFixtureDef[i].shape = shape;

    FoodFixtureDef[i].density = 1;
	// Override the default friction.
    FoodFixtureDef[i].filter.maskBits = 0xFFFF;    //Important for Collisions working!
    FoodFixtureDef[i].filter.categoryBits = 0x0001; //Important for Collisions working!
    FoodFixtureDef[i].friction = 0.3f;
	FoodBodydef[i].linearDamping = 1.0f;
	FoodBodydef[i].angularDamping = 2.0f;
	FoodBodydef[i].type = b2_dynamicBody;
    FoodBodydef[i].active = true;    //Important that body is active!!
	FoodBodydef[i].allowSleep = true;

	FoodBodydef[i].position.Set((-2 * field_size + 5.0f) + (rand() % (2 * field_size - 10)), (2 * field_size - 5.0f) - (rand() % (2 * field_size - 10)));

	Food[i] = m_world->CreateBody(&(FoodBodydef[i]));

	FoodFixture[i] = Food[i]->CreateFixture(&(FoodFixtureDef[i]));
    FoodFixture[i]->SetSensor(true);
	Food[i]->SetAngularVelocity(0);	
	FoodFixture[i]->SetUserData(touches + population->size()*2 + i); //population->size()*2 because of Animals and their sensors

	//Food[i]->SetLinearDamping(1.0f);
	//Food[i]->SetAngularDamping(2.0f);

  }

 //Obstacles are the walls for example  
  b2Vec2 wall_rect_horizontal[2] = {b2Vec2((-50.0f), -5.0f), b2Vec2(50.0f, 5.0f)};
   b2Vec2 wall_rect_vertical[2] = {b2Vec2(-5.0f,(-50.0f)), b2Vec2(5.0f, 50.0f)};
  //b2Vec2 wall_rect_horizontal[2] = {b2Vec2((-(sizex/2), 0.0f), b2Vec2((sizex/2), 1.0f)};
  //b2Vec2 wall_rect_vertical[2] = {b2Vec2(0.0f,(-(sizey/2)), b2Vec2(1.0f, (sizey/2))};

  for (int i=0; i<amount_obstacles; i++)
  {
	b2PolygonShape* shape_horizontal = new b2PolygonShape();
	b2PolygonShape* shape_vertical = new b2PolygonShape();
	shape_horizontal->SetAsBox(field_size, 1.0f);
	shape_vertical->SetAsBox(1.0f, field_size);
	if (0 == i % 2)
	{
		ObstacleFixtureDef[i].shape = shape_horizontal;
	}
 
	else
	{
		ObstacleFixtureDef[i].shape = shape_vertical;
	}

    ObstacleFixtureDef[i].density = 1;
	// Override the default friction.
    ObstacleFixtureDef[i].friction = 0.3f;
	ObstacleBodydef[i].linearDamping = 0.07f;
	ObstacleBodydef[i].angularDamping = 0.1f;
	ObstacleBodydef[i].type = b2_staticBody;
	ObstacleBodydef[i].active = true; //Important that body is active!!

    //Todo: Abstraction: More obstacles, types, etc.
	ObstacleBodydef[0].position.Set(-field_size, 0.0f);  //horizontal
	ObstacleBodydef[1].position.Set(0.0f, field_size); //vertical
	ObstacleBodydef[2].position.Set(-field_size, 2 * field_size); //horizontal
	ObstacleBodydef[3].position.Set(-2 * field_size, field_size); //vertical

	Obstacle[i] = m_world->CreateBody(&(ObstacleBodydef[i]));

	ObstacleFixture[i] = Obstacle[i]->CreateFixture(&(ObstacleFixtureDef[i]));
	Obstacle[i]->SetAngularVelocity(0);	
	ObstacleFixture[i]->SetUserData(touches + population->size()*2 + amount_food + i);
  }

  for (int i=0; i<amount_of_all_objects; i++)
  {
    active[i] = true;
    for (int j=0; j<amount_of_all_objects; j++)
    {
      ((*touches)[i])[j] = false;
    }
  }

}

void NeuralWorld::Reset()
{
  for(unsigned int i=0; i<population->size(); i++)
  {
    (*population)[i]->fitness = 0;

	b2Vec2 Animalposition; Animalposition.Set((-2 * field_size + 5.0f) + (rand() % (2 * field_size - 10)), (2 * field_size - 5.0f) - (rand() % (2 * field_size - 10)));

    //Position Correction
    for (unsigned int j=0;j<i;j++) //Look if Animalposition are to near to another Animal and correct it
      {
        if( ((Animal[i]->GetPosition().x-Animal[i]->GetPosition().y)*(Animal[i]->GetPosition().x-Animal[i]->GetPosition().y)) + (Animalposition.x-Animalposition.y) * (Animalposition.x-Animalposition.y)  < 4.0f )
        {
		 Animalposition; Animalposition.Set((-2 * field_size + 5.0f) + (rand() % (2 * field_size - 10)), (2 * field_size - 5.0f) - (rand() % (2 * field_size - 10)));
			// If positions are to near, randomly generate new
          j = 0; // and restart algorithm
        }
      }
    Animal[i]->SetTransform(b2Vec2(Animalposition.x, Animalposition.y), 0.0f);
  }

for (int i = 0; i < amount_of_all_objects; i++)
{
	active[i] = true;
	for (int j = 0; j < amount_of_all_objects; j++)
	{
		((*touches)[i])[j] = false;
	}
}
for (unsigned int i = 0; i < amount_food; i++)
{
	Food[i]->SetTransform( b2Vec2( (-2 * field_size + 5.0f) + (rand() % (2 * field_size - 10)), (2 * field_size - 5.0f) - (rand() % (2 * field_size - 10))),0.0f);
}

//  Init();

}

Test* NeuralWorld::Create(std::vector<Creature *> *ppopulation, int pamount_food, int pamount_obstacles, int pfield_size)
{
	return new NeuralWorld(ppopulation, pamount_food, pamount_obstacles, pfield_size);
}


void NeuralWorld::move_bodies(std::vector< std::vector<double> > output_vectors)
{
	for (unsigned int i = 0; i < population->size(); i++)
	{
		//Accelerate Animal
		Animal[i]->ApplyForce(b2Vec2(-sin(Animal[i]->GetAngle())*0.075f * 500 * (output_vectors[i][0]), cos(Animal[i]->GetAngle())*0.075f * 500 * (output_vectors[i][0])), Animal[i]->GetWorldCenter(), true);
		//Rotate Animal
		Animal[i]->SetTransform(Animal[i]->GetPosition(), Animal[i]->GetAngle() + 0.075f*(output_vectors[i][1]));
	}
}

double NeuralWorld::correct_angle(double angle)
{
	//Correction of PlayerAngle. //TODO: Performance at high Angle Value => Needs more loops to correct angle!
	while (angle < -180 * DEGTORAD) angle += 360 * DEGTORAD;
	while (angle > 180 * DEGTORAD) angle -= 360 * DEGTORAD;
	return angle;
}

//TODO: Rename Vector< <Vector> > to Matrix
std::vector< std::vector<double> > NeuralWorld::get_sensor_vectors()
{
	std::vector< std::vector<double> > input_vectors;
	std::vector<double> single_vector;
	float angle;
	//Initialization
	for (unsigned int i = 0; i < population->size(); i++)
	{
		//Todo: Get last food position or get nearest food position?
		//Animal relative posx and posy to food. If no food detected => (0.0f, 0.0f)
		single_vector.push_back(0.0f);
		single_vector.push_back(0.0f);
		//Todo Optimization speed correction of angle
		angle = correct_angle(Animal[i]->GetAngle());
		single_vector.push_back(angle);

		input_vectors.push_back(single_vector);
		single_vector.clear();  //TODO: Reallocation problem? (Dont think so cause not using pointer)
	}

	for (unsigned int i = 0; i < population->size(); i++)
	{
		for (unsigned int j = 0; j < amount_food; j++)
		{
			if (((*touches)[population->size() + i])[population->size() * 2 + j])       //Sensor touches food?
			{
				single_vector.push_back(Animal[i]->GetPosition().x);
				single_vector.push_back(Animal[i]->GetPosition().y);
				angle = correct_angle(Animal[i]->GetAngle());
				single_vector.push_back(angle);
				input_vectors[i] = single_vector;
				//m_debugDraw.DrawString(5, 250, "Sensor Touches food!");
				single_vector.clear();
				break;   //The first detected food is the input
			}
		}
	}
	return input_vectors;
}

void NeuralWorld::Step(Settings* settings)
{
	//TODO: If drawFoodVectors
	//TODO: Own Method
	unsigned int last_detected = 0;
	for (unsigned int j = 0; j < population->size(); j++)
	{
		for (unsigned int i = 0; i < amount_food; i++)
		{
			if (((*touches)[population->size() + j])[population->size() * 2 + i])
			{
				m_debugDraw.DrawLine(Animal[j]->GetPosition(), Food[i]->GetPosition(), b2Color(0.8f, 0.8f, 0.8f));
				last_detected = i;
			}
		}
		if (((*touches)[population->size() + j])[population->size() * 2 + last_detected])
		 {
			 m_debugDraw.DrawLine(Animal[j]->GetPosition(), Food[last_detected]->GetPosition(), b2Color(1.0f, 0.0f, 0.0f));
		 }

	}
		Test::Step(settings);
}
		
/* Not here!
		inputVals[0] = 0.0f; inputVals[1] = 0.0f; // if no food detected x and y = 0
	    inputVals[0] = a; inputVals[1] = b; //The difference vector of the last detected food
		correct_angle();
		inputVals[2] =  player_rotation; //player_angle
*/

//	}

/*
void NeuralWorld::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) //TODO: Needed?
{
    contact->ResetFriction();

}
*/
//TODO: also set the other field of touches[][] see first "IF" below

	// Implement contact listener.
	void NeuralWorld::BeginContact(b2Contact* contact)
	{
		b2Fixture* fixtureA = contact->GetFixtureA();
		b2Fixture* fixtureB = contact->GetFixtureB();
		
    for(unsigned int j=0; j<population->size(); j++)
      {
		if (fixtureA == SensorFixture[j])
		{
			for (int32 i = 0; i < amount_food; i++)
			{
			    if(active[population->size()*2+i])  //Food active?
			    {
				if (fixtureB == FoodFixture[i])
				{
					((*touches)[population->size() + j])[population->size() * 2 + i] = true;
                  //Todo:	touches[population->size()*2+i][population->size()+j] = true;  //Example
				}
			    }
			}
        /*
			for (int32 i = 0; i < 4; i++)
			{
				if (fixtureB == fixture_wall[i])
				{
					touches[2][50 + i] = true;
				}
			}
        */
		}
		if (fixtureA == AnimalFixture[j])
		{
			for (int32 i = 0; i < amount_food; i++)
			{
			    if(active[population->size()*2+i])
			    {
				if (fixtureB == FoodFixture[i])
				{
                    (*population)[j]->fitness++;
					((*touches)[j])[population->size() * 2 + i] = false; //Because Food then dont need to be detected if inactive
				    active[population->size()*2+i] = false;
                 //TODO: Deactivate Food
					Food[i]->SetAwake(false);
					//Food[i]->SetActive(false);
				}
			    }
			}
		}
		if (fixtureB == AnimalFixture[j])
		{
			for (int32 i = 0; i < amount_food; i++)
			{
			    if(active[population->size()*2+i])
			    {
				if (fixtureA == FoodFixture[i])
				{
                    (*population)[j]->fitness++;
					((*touches)[j])[population->size() * 2 + i] = false; //Because Food then dont need to be detected if inactive
				    active[population->size()*2+i] = false;
                   //TODO: Deactivate Food
					Food[i]->SetAwake(false);
					//Food[i]->SetActive(false);
				}
			    }
			}
		}
		if (fixtureB == SensorFixture[j])
		{
			for (int32 i = 0; i < amount_food; i++)
			{
			    if(active[population->size()*2+i])
			    {
				if (fixtureA == FoodFixture[i])
				{
					((*touches)[population->size() + j])[population->size() * 2 + i] = true;
				}
			    }
			}
    /*
			for (int32 i = 0; i < 4; i++)
			{
				if (fixtureA == fixture_wall[i])
				{
					touches[2][50 + i] = true;
				}
			}
    */
		}
      }
    }

	// Implement contact listener.
	void NeuralWorld::EndContact(b2Contact* contact)
	{
		b2Fixture* fixtureA = contact->GetFixtureA();
		b2Fixture* fixtureB = contact->GetFixtureB();
	 for(unsigned int j=0; j<population->size(); j++)
     {
        if (fixtureA == SensorFixture[j])
		{
			for (int32 i = 0; i < amount_food; i++)
			{
				if (fixtureB == FoodFixture[i])
				{
					((*touches)[population->size() + j])[population->size() * 2 + i] = false;
				}
			}
        /*
			for (int32 i = 0; i < 4; i++)
			{
				if (fixtureB == fixture_wall[i])
				{
					touches[2][50 + i] = false;
				}
			}
        */
		}

		if (fixtureB == SensorFixture[j])
		{
			for (int32 i = 0; i < amount_food; i++)
			{
				if (fixtureA == FoodFixture[i])
				{
					((*touches)[population->size() + j])[population->size() * 2 + i] = false;
				}
			}
        /*
			for (int32 i = 0; i < 4; i++)
			{
				if (fixtureA == fixture_wall[i])
				{
					touches[2][50 + i] = false;
				}
			}
        */
		}
      }
	}

/*
void Keyboard(unsigned char key)
{
	switch (key)
	{
	case 'w':
		//force[FORCE_FORWARD] = true;
		break;

	case 's':
		//force[FORCE_BACKWARD] = true;
		break;

	case 'a':
		//force[FORCE_ROT_LEFT] = true;
		break;

	case 'd':
		//force[FORCE_ROT_RIGHT] = true;
		break;
	default: break;
	}
}

void KeyboardUp(unsigned char key) 
{
	switch (key)
	{
	case 'w':
	//	force[FORCE_FORWARD] = false;
		break;

	case 's':
	//	force[FORCE_BACKWARD] = false;
		break;

	case 'a':
	//	force[FORCE_ROT_LEFT] = false;
		break;

	case 'd':
	//	force[FORCE_ROT_RIGHT] = false;
		break;
	default: break;
	}
}
*/

/*
	void setPopulation(std::vector<Creature *> *ppopulation)
	{
	  population = ppopulation;
	}

*/

