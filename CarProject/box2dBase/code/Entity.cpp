// 
// Basado en un código de dominio público de:
// angel.rodriguez@esne.edu
// 2021.03+
// 
// Modificaiones del mismo por:
// felipevm07@gmail.com
// 2022.06

#pragma once

#include <box2d/box2d.h>
#include <memory>
#include "headers/Entity.h"

using namespace sf;
using namespace std;

namespace operations
{
    shared_ptr<Entity> Create_Edge(Scene* scene, b2BodyType body_type, float x0, float y0, float x1, float y1, bool isSensor)
    {
        shared_ptr < Entity > body(new Entity());

        // Se crea el body:
        b2BodyDef body_definition;

        body_definition.type = body_type;
        body_definition.position.Set(0.f, 0.f); 

        body->rigidBody = scene->get_world().CreateBody(&body_definition);

        // Se añande una fixture:
        b2EdgeShape body_shape;

        // Coordenadas locales respecto al centro del body
        body_shape.SetTwoSided(b2Vec2(x0, y0), b2Vec2(x1, y1)); 

        b2FixtureDef body_fixture;

        body_fixture.shape = &body_shape;
        body_fixture.density = 5.00f;
        body_fixture.restitution = 0.05f;
        body_fixture.friction = 0.5f;
        body_fixture.isSensor = isSensor;

        body->rigidBody->CreateFixture(&body_fixture);

        return body;
    }    
    
    shared_ptr<Entity> Create_Circle(Scene* scene, b2BodyType body_type, float x, float y, float radius, bool isSensor)
    {
        shared_ptr < Entity > body(new Entity());

        // Se crea el body:
        b2BodyDef body_definition;

        body_definition.type = body_type;
        body_definition.position.Set(x, y); 

        body->rigidBody = scene->get_world().CreateBody(&body_definition);

        // Se añande una fixture:
        b2CircleShape body_shape;

        body_shape.m_radius = radius;

        b2FixtureDef body_fixture;

        body_fixture.shape = &body_shape;
        body_fixture.density = 0.20f;
        body_fixture.restitution = 0.05f;
        body_fixture.friction = .25f;

        body->rigidBody->CreateFixture(&body_fixture);

        return body;
    }
        
    shared_ptr<Entity> Create_Box(Scene* scene, b2BodyType body_type, float x, float y, float width, float height, float radians, bool isSensor)
    {
        shared_ptr < Entity > body(new Entity());

        // Se crea el body:
        b2BodyDef body_definition;

        body_definition.type = body_type;
        body_definition.position.Set(x, y); 

        body->rigidBody = scene->get_world().CreateBody(&body_definition);

        b2Vec2 vec2 = b2Vec2(x, y);
        body->rigidBody->SetTransform(vec2, radians);

        // Se añande una fixture:
        b2PolygonShape body_shape;

        body_shape.SetAsBox(width, height);

        b2FixtureDef body_fixture;

        body_fixture.shape = &body_shape;
        body_fixture.density = 5.00f;
        body_fixture.restitution = 0.05f;
        body_fixture.friction = 0.5f;
        body_fixture.isSensor = isSensor;

        body->rigidBody->CreateFixture(&body_fixture);

        return body;
    }

    b2RevoluteJoint* Create_Revolute_Joint(Scene* scene, b2Body* bodyA, b2Body* bodyB, bool enabledMotor)
    {
        b2RevoluteJointDef revolut;

        revolut.Initialize(bodyA, bodyB, bodyB->GetPosition() - b2Vec2(0.f, 0.f));
        revolut.enableLimit = true;
        revolut.lowerAngle = 0.f;
        revolut.upperAngle = 0.f;
        revolut.enableMotor = enabledMotor;

        return dynamic_cast<b2RevoluteJoint*>(scene->get_world().CreateJoint(&revolut));
    }
    
    b2WheelJoint* Create_Wheel_Joint(Scene* scene, b2Body* bodyA, b2Body* bodyB, float motorSpeed, float torque, bool isEnabled)
    {
        b2WheelJointDef wheel;

        b2Vec2 axis(0.1f, 0.9f);
        wheel.Initialize(bodyA, bodyB, bodyB->GetPosition(), axis);

        wheel.motorSpeed = motorSpeed;
        wheel.maxMotorTorque = torque;
        wheel.enableMotor = isEnabled;
        wheel.enableLimit = true;
        wheel.collideConnected = true;

        return dynamic_cast<b2WheelJoint*>(scene->get_world().CreateJoint(&wheel));
    }

    shared_ptr<Entity> Create_CarBaseShape(Scene* scene, b2BodyType body_type, float x, float y)
    {
        shared_ptr < Entity > body(new Entity());

        // Se crea el body:
        b2BodyDef body_definition;

        body_definition.type = body_type;
        body_definition.position.Set(x, y); 

        body->rigidBody = scene->get_world().CreateBody(&body_definition);

        b2Vec2 vertices[6];
        vertices[0].Set(0.0f, 0.05f);
        vertices[1].Set(1.0f, 0.05f);
        vertices[2].Set(0.1f, 0.5f);
        vertices[3].Set(-0.03f, 0.35f);
        vertices[4].Set(0.55f, 0.5f);
        vertices[5].Set(1.05f, 0.2f);
        int32 count = 6;

        // Se añande una fixture:
        b2PolygonShape body_shape;

        body_shape.Set(vertices, count);

        b2FixtureDef body_fixture;

        body_fixture.shape = &body_shape;
        body_fixture.density = 2.00f;
        body_fixture.restitution = 0.05f;
        body_fixture.friction = 0.5f;

        body->rigidBody->CreateFixture(&body_fixture);

        return body;
    }
}