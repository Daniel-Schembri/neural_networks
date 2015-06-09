/*
* Copyright (c) 2015 Daniel Schembri https://github.com/Daniel-Schembri/neural_networks
*
* neuralworld.cpp
* This cpp-file uses Box2D by Erin Catto 
*
* new_graphic
* This is a graphical user interface to evolve object collecting agents 
* The agent have two neural networks for steering angle and velocity.
*
* This project includes the neural net framework, called logic,
* of Jonathan Schwarz https://github.com/jonathan-schwarz/neural_networks
*
* new_graphic also uses the Box2D-library by Erin Catto
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

#include "neuralWorld.h"


NeuralWorld::NeuralWorld()
{

}


NeuralWorld::NeuralWorld(std::vector<Agent *> *ppopulation, int pamount_Object, int pfield_size, int pmode)
{

    mode = pmode;
    population = ppopulation;
    amount_Object = pamount_Object;
    field_size = pfield_size;
    //Init
    srand((unsigned)time(NULL)); // Zufallsgenerator initialisieren.
    //Agents

    b2FixtureDef fixturedef_filler;
    b2BodyDef    bodydef_filler;

    for (unsigned int i = 0; i < population->size(); i++)
    {
        Agentbody.push_back(NULL);
        AgentFixtureDef.push_back(fixturedef_filler);
        AgentFixture.push_back(NULL);
        AgentBodydef.push_back(bodydef_filler);

        SensorFixtureDef.push_back(fixturedef_filler);
        SensorFixture.push_back(NULL);
    }

    for (unsigned int i = 0; i < amount_Object; i++)
    {
        Objectbody.push_back(NULL);
        ObjectFixtureDef.push_back(fixturedef_filler);
        ObjectFixture.push_back(NULL);
        ObjectBodydef.push_back(bodydef_filler);
    }

    for (unsigned int i = 0; i < 4; i++)
    {
        Obstaclebody.push_back(NULL);
        ObstacleFixtureDef.push_back(fixturedef_filler);
        ObstacleFixture.push_back(NULL);
        ObstacleBodydef.push_back(bodydef_filler);
    }

    amount_of_all_objects = population->size()*2 + amount_Object;
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


NeuralWorld::~NeuralWorld()
{

    if (NULL != touches)
    {
        delete touches;
        touches = NULL;
    }

}


void NeuralWorld::Init()
{
    force[0] = false; force[1] = false; force[2] = false; force[3] = false;

    m_world->SetAllowSleeping(true);
    m_world->SetWarmStarting(true);
    m_world->SetContinuousPhysics(true);
    m_world->SetSubStepping(false);


    for (unsigned int i=0; i<population->size(); i++)
    {
        // Agent
        {
            b2PolygonShape* poly = new b2PolygonShape();
            poly->Set(triangle, 3);

            AgentFixtureDef[i].shape = poly;
            AgentFixtureDef[i].density = 1;
            AgentFixtureDef[i].isSensor = false;
            // Override the default friction.
            AgentFixtureDef[i].friction = 1.0f;
            AgentBodydef[i].linearDamping = 1.0f;
            AgentBodydef[i].angularDamping = 2.0f;
            AgentBodydef[i].type = b2_dynamicBody;


            b2Vec2 Agentposition; Agentposition.Set((-2 * field_size + 5.0f) + (rand() % (2 * field_size - 10)), (2 * field_size - 5.0f) - (rand() % (2 * field_size - 10)));



            //Position Correction
            for (unsigned int j=0;j<i;j++) //Look if Agentposition are to near to another Agent and correct it
            {
                if( ((AgentBodydef[i].position.x-AgentBodydef[i].position.y)*(AgentBodydef[i].position.x-AgentBodydef[i].position.y)) + (Agentposition.x-Agentposition.y) * (Agentposition.x-Agentposition.y)  < 4.0f )
                {
                    Agentposition; Agentposition.Set((-2 * field_size + 5.0f) + (rand() % (2 * field_size - 10)), (2 * field_size - 5.0f) - (rand() % (2 * field_size - 10)));
                    j = 0; // and restart algorithm
                }
            }

            AgentBodydef[i].position.Set(Agentposition.x, Agentposition.y);
            //Position Correction End


            //b2Body* player;
            Agentbody[i] = m_world->CreateBody(&(AgentBodydef[i]));


            AgentFixture[i] = Agentbody[i]->CreateFixture(&(AgentFixtureDef[i]));
            Agentbody[i]->SetAngularVelocity(0);	
            AgentFixture[i]->SetUserData(touches + i);		

            //add semicircle sensor to Agent

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
            SensorFixture[i] = Agentbody[i]->CreateFixture(&(SensorFixtureDef[i]));
            SensorFixture[i]->SetSensor(true);
            SensorFixture[i]->SetUserData(touches + population->size() + i);
        } //Agent end
    } //For-Loop end

    for (int i=0; i<amount_Object; i++)
    {

        b2PolygonShape* shape = new b2PolygonShape();
        shape->SetAsBox(0.4f, 0.4f);
        ObjectFixtureDef[i].shape = shape;

        ObjectFixtureDef[i].density = 1;
        // Override the default friction.
        ObjectFixtureDef[i].filter.maskBits = 0xFFFF;    //Important for Collisions working!
        ObjectFixtureDef[i].filter.categoryBits = 0x0001; //Important for Collisions working!
        ObjectFixtureDef[i].friction = 0.3f;
        ObjectBodydef[i].linearDamping = 1.0f;
        ObjectBodydef[i].angularDamping = 2.0f;
        ObjectBodydef[i].type = b2_dynamicBody;

        ObjectBodydef[i].position.Set((-2 * field_size + 5.0f) + (rand() % (2 * field_size - 10)), (2 * field_size - 5.0f) - (rand() % (2 * field_size - 10)));

        Objectbody[i] = m_world->CreateBody(&(ObjectBodydef[i]));

        ObjectFixture[i] = Objectbody[i]->CreateFixture(&(ObjectFixtureDef[i]));
        ObjectFixture[i]->SetSensor(true);
        Objectbody[i]->SetAngularVelocity(0);	
        ObjectFixture[i]->SetUserData(touches + population->size()*2 + i); //population->size()*2 because of Agents and their sensors

    }

    //Obstacles are the walls for example  
    b2Vec2 wall_rect_horizontal[2] = {b2Vec2((-50.0f), -5.0f), b2Vec2(50.0f, 5.0f)};
    b2Vec2 wall_rect_vertical[2] = {b2Vec2(-5.0f,(-50.0f)), b2Vec2(5.0f, 50.0f)};

    for (int i=0; i<4; i++)
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

        //Todo: Abstraction: More obstacles, types, etc.
        ObstacleBodydef[0].position.Set(-field_size, 0.0f);  //horizontal
        ObstacleBodydef[1].position.Set(0.0f, field_size); //vertical
        ObstacleBodydef[2].position.Set(-field_size, 2 * field_size); //horizontal
        ObstacleBodydef[3].position.Set(-2 * field_size, field_size); //vertical

        Obstaclebody[i] = m_world->CreateBody(&(ObstacleBodydef[i]));

        ObstacleFixture[i] = Obstaclebody[i]->CreateFixture(&(ObstacleFixtureDef[i]));
        Obstaclebody[i]->SetAngularVelocity(0);	
        ObstacleFixture[i]->SetUserData(touches + population->size()*2 + amount_Object + i);
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

        b2Vec2 Agentposition; Agentposition.Set((-2 * field_size + 5.0f) + (rand() % (2 * field_size - 10)), (2 * field_size - 5.0f) - (rand() % (2 * field_size - 10)));

        //Position Correction
        for (unsigned int j=0;j<i;j++) //Look if Agentposition are to near to another Agent and correct it
        {
            if( ((Agentbody[i]->GetPosition().x-Agentbody[i]->GetPosition().y)*(Agentbody[i]->GetPosition().x-Agentbody[i]->GetPosition().y)) + (Agentposition.x-Agentposition.y) * (Agentposition.x-Agentposition.y)  < 4.0f )
            {
                Agentposition; Agentposition.Set((-2 * field_size + 5.0f) + (rand() % (2 * field_size - 10)), (2 * field_size - 5.0f) - (rand() % (2 * field_size - 10)));
                // If positions are to near, randomly generate new
                j = 0; // and restart algorithm
            }
        }
        Agentbody[i]->SetTransform(b2Vec2(Agentposition.x, Agentposition.y), 0.0f);
        //Reset angular and velocity force
        Agentbody[i]->SetLinearVelocity(b2Vec2(0,0));
        Agentbody[i]->SetAngularVelocity(0);


    }

    for (int i = 0; i < amount_of_all_objects; i++)
    {
        active[i] = true;
        for (int j = 0; j < amount_of_all_objects; j++)
        {
            ((*touches)[i])[j] = false;
        }
    }
    for (unsigned int i = 0; i < amount_Object; i++)
    {
        //Reactivate Object
        active[population->size() * 2 + i] = true;
        Objectbody[i]->SetActive(true);
        Objectbody[i]->SetTransform( b2Vec2( (-2 * field_size + 5.0f) + (rand() % (2 * field_size - 10)), (2 * field_size - 5.0f) - (rand() % (2 * field_size - 10))),0.0f);
        Objectbody[i]->SetLinearVelocity(b2Vec2(0,0));
        Objectbody[i]->SetAngularVelocity(0);
    }

}

Test* NeuralWorld::Create(std::vector<Agent *> *ppopulation, int pamount_Object, int pfield_size, int pmode)
{
    return new NeuralWorld(ppopulation, pamount_Object, pfield_size, pmode);
}


void NeuralWorld::move_bodies(std::vector< std::vector<double> > output_vectors)
{
    for (unsigned int i = 0; i < population->size(); i++)
    {
        //Rotate Agent
        Agentbody[i]->SetTransform(Agentbody[i]->GetPosition(), Agentbody[i]->GetAngle() + 0.075f*(output_vectors[i][1]));
      //  Agentbody[i]->SetTransform(Agentbody[i]->GetPosition(), Agentbody[i]->GetAngle() - (M_PI/2) *(output_vectors[i][1]));
        //Accelerate Agent
        Agentbody[i]->ApplyForce(b2Vec2(-sin(Agentbody[i]->GetAngle())*0.075f * 500 * (output_vectors[i][0]), cos(Agentbody[i]->GetAngle())*0.075f * 500 * (output_vectors[i][0])), Agentbody[i]->GetWorldCenter(), true);
    }
}

double NeuralWorld::correct_angle(double angle)
{
    //Correction of PlayerAngle. //TODO: Performance at high Angle Value => Needs more loops to correct angle!
    while (angle < -180 * DEGTORAD) angle += 360 * DEGTORAD;
    while (angle > 180 * DEGTORAD) angle -= 360 * DEGTORAD;
    return angle;
}

std::vector< std::vector<double> > NeuralWorld::get_sensor_vectors()
{
    std::vector< std::vector<double> > input_vectors;
    std::vector<double> single_vector;
    float angle;
    //Initialization
    for (unsigned int i = 0; i < population->size(); i++)
    {
        //Todo: Get last Object position or get nearest Object position?
        //Agent relative posx and posy to Object. If no Object detected => (0.0f, 0.0f)
        single_vector.push_back(0.0f);
        single_vector.push_back(0.0f);
        //	single_vector.push_back(0.0f);
        input_vectors.push_back(single_vector);
        single_vector.clear();  //TODO: Reallocation problem? (Dont think so cause not using pointer)
    }

    for (unsigned int i = 0; i < population->size(); i++)
    {
        b2Vec2 shortest_vec = b2Vec2(100.0f, 100.0f);
        int nearest_object = -1; //No object detected!

        for (unsigned int j = 0; j < amount_Object; j++)
        {
            if (((*touches)[population->size() + i])[population->size() * 2 + j])       //Sensor touches Object?
            {
                b2Vec2 distance_object = Objectbody[j]->GetPosition() - Agentbody[i]->GetPosition(); 

                if(shortest_vec.Length() > distance_object.Length())
                {
                    shortest_vec = distance_object;
                    nearest_object = j;
                } 
            }
        }
        if(nearest_object >= 0)
        {
            float x_translated = 0;
            float y_translated = 0;
            float x_tmp = 0;
            float y_tmp = 0;
            angle = correct_angle(Agentbody[i]->GetAngle()); 

            //First translation of the x,y coordinates
            x_translated = (Objectbody[nearest_object]->GetPosition().x - Agentbody[i]->GetPosition().x);
            y_translated = (Objectbody[nearest_object]->GetPosition().y - Agentbody[i]->GetPosition().y);
            //Rotation
            x_tmp = (x_translated * cos(angle) + y_translated * sin(angle));
            y_tmp = (-x_translated * sin(angle) + y_translated * cos(angle));

            //negativ y-values arent allowed (out of range of the sensor)
            if(y_tmp < 0.0f)
            {
            single_vector.push_back(0.0f);
            single_vector.push_back(0.0f);
            }
            else
            {
            single_vector.push_back(x_tmp);
            single_vector.push_back(y_tmp);
            }
            //		single_vector.push_back(angle);

            input_vectors[i] = single_vector;
            single_vector.clear();
        }
    }
return input_vectors;
}

void NeuralWorld::Step(Settings* settings)
{
    //DebugDraw Agent Angles
//    for(int i=0; i<population->size();++i)
//    {
//        m_debugDraw.DrawString(20, 100 + 40*i, "Agent %d Angle: %f \n", i, Agentbody[i]->GetAngle());
//    }
    //Deactivate Object if inactive
    for (unsigned int i = 0; i < amount_Object; i++)
    {
        if (!active[population->size() * 2 + i])
        {
            //Objectbody[i]->SetActive(false);
            //Respawn Object
            active[population->size() * 2 + i] = true;
            Objectbody[i]->SetTransform( b2Vec2( (-2 * field_size + 5.0f) + (rand() % (2 * field_size - 10)), (2 * field_size - 5.0f) - (rand() % (2 * field_size - 10))),0.0f);
        }
            
    }


    //TODO: If drawObjectVectors
    //TODO: Own Method
    for (unsigned int j = 0; j < population->size(); j++)
    {
        //Draw all active objects in sensorrange
        for (unsigned int i = 0; i < amount_Object; i++)
        {
            if (((*touches)[population->size() + j])[population->size() * 2 + i])
            {
                m_debugDraw.DrawLine(Agentbody[j]->GetPosition(), Objectbody[i]->GetPosition(), b2Color(0.8f, 0.8f, 0.8f));
            }
        }
    }
    //Draw the shortest Objectvector
    for (unsigned int i = 0; i < population->size(); i++)
    {
        b2Vec2 shortest_vec = b2Vec2(100.0f, 100.0f);
        int nearest_object = -1;

        for (unsigned int j = 0; j < amount_Object; j++)
        {
            if (((*touches)[population->size() + i])[population->size() * 2 + j])       //Sensor touches Object?
            {
                b2Vec2 distance_object = Objectbody[j]->GetPosition() - Agentbody[i]->GetPosition(); 

                if(shortest_vec.Length() > distance_object.Length())
                {
                   shortest_vec = distance_object;
                   nearest_object = j;
                } 
            }
        }
        if(nearest_object >= 0)
        {
            m_debugDraw.DrawLine(Agentbody[i]->GetPosition(), Objectbody[nearest_object]->GetPosition(), b2Color(1.0f, 0.0f, 0.0f));
        } 
    }
    Test::Step(settings);
}



/*
   void NeuralWorld::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) //TODO: Necessary?
   {
   contact->ResetFriction();

   }
   */

// Implement contact listener.
void NeuralWorld::BeginContact(b2Contact* contact)
{
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();

    for(unsigned int j=0; j<population->size(); j++)
    {
        if (fixtureA == SensorFixture[j])
        {
            for (int32 i = 0; i < amount_Object; i++)
            {
                if(active[population->size()*2+i])  //Object active?
                {
                    if (fixtureB == ObjectFixture[i])
                    {
                        ((*touches)[population->size() + j])[population->size() * 2 + i] = true;
                    }
                }
            }

        }
        if (fixtureA == AgentFixture[j])
        {
            for (int32 i = 0; i < amount_Object; i++)
            {
                if(active[population->size()*2+i])
                {
                    if (fixtureB == ObjectFixture[i])
                    {
                        (*population)[j]->fitness++;
                        ((*touches)[j])[population->size() * 2 + i] = false; //Because Object then dont need to be detected if inactive
                        active[population->size()*2+i] = false;

                    }
                }
            }
        }
        if (fixtureB == AgentFixture[j])
        {
            for (int32 i = 0; i < amount_Object; i++)
            {
                if(active[population->size()*2+i])
                {
                    if (fixtureA == ObjectFixture[i])
                    {
                        (*population)[j]->fitness++;
                        ((*touches)[j])[population->size() * 2 + i] = false; //Because Object then dont need to be detected if inactive
                        active[population->size()*2+i] = false;
                    }
                }
            }
        }
        if (fixtureB == SensorFixture[j])
        {
            for (int32 i = 0; i < amount_Object; i++)
            {
                if(active[population->size()*2+i])
                {
                    if (fixtureA == ObjectFixture[i])
                    {
                        ((*touches)[population->size() + j])[population->size() * 2 + i] = true;
                    }
                }
            }
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
            for (int32 i = 0; i < amount_Object; i++)
            {
                if (fixtureB == ObjectFixture[i])
                {
                    ((*touches)[population->size() + j])[population->size() * 2 + i] = false;
                }
            }
        }

        if (fixtureB == SensorFixture[j])
        {
            for (int32 i = 0; i < amount_Object; i++)
            {
                if (fixtureA == ObjectFixture[i])
                {
                    ((*touches)[population->size() + j])[population->size() * 2 + i] = false;
                }
            }
        }
    }
}


std::vector<double> NeuralWorld::move_player()
{
    //TODO: correct angle?
    std::vector<double> Outputs;
    Outputs.push_back(0.0f); Outputs.push_back(0.0f);
    if (MODE_SINGLEPLAYER == mode)
    {
        if (force[FORCE_FORWARD])
        {
            Agentbody[0]->ApplyForce(b2Vec2(-sin(Agentbody[0]->GetAngle())*0.075f * 500, cos(Agentbody[0]->GetAngle())*0.075f * 500), Agentbody[0]->GetWorldCenter(), true);
            Outputs[0] = 1.0f;
        }	
        else if (force[FORCE_BACKWARD])
        {
            Agentbody[0]->ApplyForce(b2Vec2(sin(Agentbody[0]->GetAngle())*0.075f * 500, -cos(Agentbody[0]->GetAngle())*0.075f * 500), Agentbody[0]->GetWorldCenter(), true);
            Outputs[0] = -1.0f;
        }
        if (force[FORCE_ROT_LEFT])
        {
            Agentbody[0]->SetTransform(Agentbody[0]->GetPosition(), Agentbody[0]->GetAngle() + 0.075f);
            Outputs[1] = 0.075f;
        }
        else if (force[FORCE_ROT_RIGHT])
        {
            Agentbody[0]->SetTransform(Agentbody[0]->GetPosition(), Agentbody[0]->GetAngle() - 0.075f);
            Outputs[1] = -0.075f;
        }

        return Outputs;
    }
    return Outputs;
}


void NeuralWorld::Keyboard(unsigned char key)
{
    if (MODE_SINGLEPLAYER == mode)
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
}

void NeuralWorld::KeyboardUp(unsigned char key)
{
    if (mode == MODE_SINGLEPLAYER)
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
}


//TODO: Maybe needed for BestAgents-Mode
/*
   void setPopulation(std::vector<Agent *> *ppopulation)
   {
   population = ppopulation;
   }

*/

