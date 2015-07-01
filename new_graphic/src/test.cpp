/*
 * Copyright (c) 2015 Daniel Schembri https://github.com/Daniel-Schembri/neural_networks
 *
 * test.cpp
 * This is a modified version of test.cpp of Erin Catto's Box2D Testbed 
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

#include "test.h"
#include <stdio.h>

void DestructionListener::SayGoodbye(b2Joint* joint)
{
    if (test->m_mouseJoint == joint)
    {
        test->m_mouseJoint = NULL;
    }
    else
    {
        test->JointDestroyed(joint);
    }
}

Test::Test()
{
    b2Vec2 gravity;
    gravity.Set(0.0f, 0.0f);
    m_world = new b2World(gravity);
    m_textLine = 30;
    m_mouseJoint = NULL;
    m_pointCount = 0;

    m_destructionListener.test = this;
    m_world->SetDestructionListener(&m_destructionListener);
    m_world->SetContactListener(this);
    m_world->SetDebugDraw(&m_debugDraw);


    m_stepCount = 0;

    b2BodyDef bodyDef;
    m_groundBody = m_world->CreateBody(&bodyDef);

}

Test::~Test()
{
    // By deleting the world, we delete the bomb, mouse joint, etc.
    delete m_world;
    m_world = NULL;
}

void Test::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
    const b2Manifold* manifold = contact->GetManifold();

    if (manifold->pointCount == 0)
    {
        return;
    }

    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();

    b2PointState state1[b2_maxManifoldPoints], state2[b2_maxManifoldPoints];
    b2GetPointStates(state1, state2, oldManifold, manifold);

    b2WorldManifold worldManifold;
    contact->GetWorldManifold(&worldManifold);

    for (int32 i = 0; i < manifold->pointCount && m_pointCount < k_maxContactPoints; ++i)
    {
        ContactPoint* cp = m_points + m_pointCount;
        cp->fixtureA = fixtureA;
        cp->fixtureB = fixtureB;
        cp->position = worldManifold.points[i];
        cp->normal = worldManifold.normal;
        cp->state = state2[i];
        cp->normalImpulse = manifold->points[i].normalImpulse;
        cp->tangentImpulse = manifold->points[i].tangentImpulse;
        cp->separation = worldManifold.separations[i];
        ++m_pointCount;
    }
}

class QueryCallback : public b2QueryCallback
{
    public:
        QueryCallback(const b2Vec2& point)
        {
            m_point = point;
            m_fixture = NULL;
        }

        bool ReportFixture(b2Fixture* fixture)
        {
            b2Body* body = fixture->GetBody();
            if (body->GetType() == b2_dynamicBody)
            {
                bool inside = fixture->TestPoint(m_point);
                if (inside)
                {
                    m_fixture = fixture;

                    // We are done, terminate the query.
                    return false;
                }
            }

            // Continue the query.
            return true;
        }

        b2Vec2 m_point;
        b2Fixture* m_fixture;
};

void Test::MouseDown(const b2Vec2& p)
{
    m_mouseWorld = p;

    if (m_mouseJoint != NULL)
    {
        return;
    }

    // Make a small box.
    b2AABB aabb;
    b2Vec2 d;
    d.Set(0.001f, 0.001f);
    aabb.lowerBound = p - d;
    aabb.upperBound = p + d;

    // Query the world for overlapping shapes.
    QueryCallback callback(p);
    m_world->QueryAABB(&callback, aabb);

    if (callback.m_fixture)
    {
        b2Body* body = callback.m_fixture->GetBody();
        b2MouseJointDef md;
        md.bodyA = m_groundBody;
        md.bodyB = body;
        md.target = p;
        md.maxForce = 1000.0f * body->GetMass();
        m_mouseJoint = (b2MouseJoint*)m_world->CreateJoint(&md);
        body->SetAwake(true);
    }
}

void Test::ShiftMouseDown(const b2Vec2& p)
{
    m_mouseWorld = p;

    if (m_mouseJoint != NULL)
    {
        return;
    }

}

void Test::MouseUp(const b2Vec2& p)
{
    if (m_mouseJoint)
    {
        m_world->DestroyJoint(m_mouseJoint);
        m_mouseJoint = NULL;
    }

}

void Test::MouseMove(const b2Vec2& p)
{
    m_mouseWorld = p;

    if (m_mouseJoint)
    {
        m_mouseJoint->SetTarget(p);
    }
}

void Test::Step(Settings* settings)
{
    float32 timeStep = settings->hz > 0.0f ? 1.0f / settings->hz : float32(0.0f);

    if (settings->pause)
    {
        if (settings->singleStep)
        {
            settings->singleStep = 0;
        }
        else
        {
            timeStep = 0.0f;
        }
    }

    m_pointCount = 0;

    m_world->Step(timeStep, settings->velocityIterations, settings->positionIterations);


    if (timeStep > 0.0f)
    {
        ++m_stepCount;
    }

    //ds Drawpart separated
    if(!settings->turbo)
    {
        uint32 flags = 0;
        flags += b2Draw::e_shapeBit;
        m_debugDraw.SetFlags(flags);

        m_world->DrawDebugData();

        if (m_mouseJoint)
        {
            b2Vec2 p1 = m_mouseJoint->GetAnchorB();
            b2Vec2 p2 = m_mouseJoint->GetTarget();

            b2Color c;
            c.Set(0.0f, 1.0f, 0.0f);
            m_debugDraw.DrawPoint(p1, 4.0f, c);
            m_debugDraw.DrawPoint(p2, 4.0f, c);

            c.Set(0.8f, 0.8f, 0.8f);
            m_debugDraw.DrawSegment(p1, p2, c);
        }

    }
}

void Test::ShiftOrigin(const b2Vec2& newOrigin)
{
    m_world->ShiftOrigin(newOrigin);
}
