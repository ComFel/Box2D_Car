
// Modificaiones del mismo por:
// felipevm07@gmail.com
// 2022.03
//
// Controles:
//
//   

#pragma once

#include <ciso646>
#include <memory>
#include <vector>
#include <Box2D/Box2D.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "headers/Enviroment.h"
#include "headers/Vehicle.h"
#include "headers/operations.h"


using namespace std;
using namespace sf;


#pragma once

namespace operations 
{   
   
    Vehicle::Vehicle(b2World& physics_world)
    {          

        //Creacion cesta del coche
        b2Body* baseCest = create_box(physics_world, b2_kinematicBody, 1, 3.4f, 0.5f, 0.11f);

        b2Body* leftWall = CreateRotatedBox(physics_world, b2_kinematicBody, 0.5f, 3.6f, 0.1f, 0.3f, 0.f);
        auto* joint3 = Create_rev_joint(physics_world, baseCest, leftWall, -0.5f, 0.25f, false);
        b2Body* rightWall = CreateRotatedBox(physics_world, b2_kinematicBody, 1.5f, 3.6f, 0.1f, 0.3f, 0.f);
        auto* joint4 = Create_rev_joint(physics_world, baseCest, rightWall, 1.5f, 0.25f, false);

        b2Body* circleRot = create_circle(physics_world, b2_kinematicBody, 0.1f, 3.1f, 0.1f);
        auto* joint5 = Create_rev_joint(physics_world, baseCest, circleRot, 0.1f, 3.1f, false);
    }

    //Inicio de un intento de crear una salida doble, es decir, de devolver una referencia a ambas ruedas de la misma función
        /* std::make_pair<b2Body,b2Body> CreateBaseCar(b2World& physics_world) // float x, float y, float radius)
        {

            auto* baseCar = create_box(physics_world, b2_dynamicBody, 1, 3, 0.5f, 0.2f);

            auto* wheel1 = CreateWheel(physics_world, 0.5f, 3, 0.2f);
            auto* joint1 = Create_rev_joint(physics_world, baseCar, wheel1, -0.5f, -0.25f, true);
            auto* wheel2 = CreateWheel(physics_world, 1.5f, 3, 0.2f);
            auto* joint2 = Create_rev_joint(physics_world, baseCar, wheel2, 0.5f, -0.25f, true);

            return std::make_pair(wheel1, wheel2);
        }
        */


    //WIP
    void CreateCarCest(b2World& physics_world, float x, float y) //b2Body*
    {
        auto* baseCest = create_box(physics_world, b2_kinematicBody, x, y, 0.5f, 0.11f);

        b2Body* leftWall = CreateRotatedBox(physics_world, b2_kinematicBody, x - 0.5f, y + 0.2f, 0.1f, 0.3f, 0.f);
        auto* joint1 = Create_rev_joint(physics_world, baseCest, leftWall, -0.5f, 0.25f, true);

        b2Body* rightWall = CreateRotatedBox(physics_world, b2_kinematicBody, x + 0.5f, y + 0.2f, 0.1f, 0.3f, 0.f);
        auto* joint2 = Create_rev_joint(physics_world, baseCest, rightWall, 1.5f, 0.25f, true);

        auto* circleRot = create_circle(physics_world, b2_kinematicBody, x - 0.5f, y - 0.2f, 0.1f);


        /*
        b2DistanceJointDef distanceCardef;

        distanceCardef.bodyA = CreateRotatedBox(physics_world, b2_kinematicBody, x + 0.7f, y, 0.05f, 0.5f, 0.5f);
        distanceCardef.bodyB = create_box(physics_world, b2_kinematicBody, x + 0.7f, y, 0.05f, 0.5f);
        distanceCardef.collideConnected = false;

        b2DistanceJoint* joint = (b2DistanceJoint*)(physics_world.CreateJoint(&distanceCardef));


        b2DistanceJointDef distanceCardef2;

        distanceCardef.bodyA = CreateRotatedBox(physics_world, b2_kinematicBody, x + 0.7f, y, 0.05f, 0.5f, 0.5f);
        distanceCardef.bodyB = create_box(physics_world, b2_staticBody, x, y, 0.7f, 0.1f);
        distanceCardef.collideConnected = false;

        b2DistanceJoint* joint = (b2DistanceJoint*)(physics_world.CreateJoint(&distanceCardef2));

        create_box(physics_world, b2_staticBody, x + 0.7f, y, 0.05f, 0.5f);

        b2PolygonShape body_shape;

        body_shape.SetAsBox(0, 0);
        b2FixtureDef body_fixture;

        body_fixture.shape = &body_shape;

        b2Body* body = physics_world.CreateBody(&body_fixture);

        body->CreateFixture(&body_fixture);

        return body;


        b2DistanceJointDef distanceDef;


        distanceDef.bodyA = create_box(physics_world, b2_staticBody, x + 0.7f, y, 0.05f, 0.5f);
        ditanceDef.bodyB = (b2Body)joint;
        distanceDef.collideConnected = false;

        b2DistanceJoint* jointT = (b2DistanceJoint*)(physics_world.CreateJoint(&distanceDef));


        //b2GearJoint joint1 = create_box(physics_world,)*/
    }

    //Creamos un rueda
    b2Body* CreateWheel(b2World& physics_world, float x, float y, float radius)
    {
        return create_circle(physics_world, b2_dynamicBody, x, y, radius);
    }

   

}


