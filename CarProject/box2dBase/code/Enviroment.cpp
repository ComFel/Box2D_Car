



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
#include "headers/operations.h"


using namespace std;
using namespace sf;
using namespace miCarBox;

   
    //Constructor de algunos elementos de la escena
    Enviroment::Enviroment(b2World& physics_world)
    {
        CreateMargins(physics_world);
        CreateStaticBox(physics_world, 0, 2.5f, 0.1f, 0.1f, 3);
        CreateStaticBox(physics_world, 12, 1.f, 0.1f, 0.1f, 5);
        CreateCurve(physics_world);
        CreateBalleReciever(physics_world, b2_staticBody, 5, 5.5f, 2.5f);
    };

    //Crear margenes de la escena
    void CreateMargins(b2World& physics_world)
    {
        constexpr float left = 0.01f;
        constexpr float right = 15.00f;
        constexpr float top = 7.50f;
        constexpr float bottom = 0.01f;

        Create_Edge(physics_world, b2_staticBody, left, bottom, right, bottom);
        Create_Edge(physics_world, b2_staticBody, left, bottom, left, top);
        Create_Edge(physics_world, b2_staticBody, left, top, right, top);
        Create_Edge(physics_world, b2_staticBody, right, bottom, right, top);
    };

    //Crear un box estático, ademas, con el parámetro lenght podemos de una realizar varias iteraciones, como por ejemplo para crear el suelo plano en ambos extremos.
    void CreateStaticBox(b2World& physics_world, float xStart, float yStart, float xEnd, float yEnd, int length)
    {
        for (size_t i = 0; i < length; i++)
        {
            Create_Box(physics_world, b2_staticBody, xStart, yStart, xEnd + i, yEnd);
        }

    };       

    //Con la anterior función, se usa para crear la curva del inicio de la escena                                               ----------- try chain shape
    void CreateCurve(b2World& physics_world)//, float xStart, float yStart, float xEnd, float yEnd,int length, float angle)
    {
        CreateRotatedBox(physics_world, b2_staticBody, 2.32f, 2.41f, 0.3f, 0.1f, -0.32f);
        CreateRotatedBox(physics_world, b2_staticBody, 2.72f, 2.15f, 0.3f, 0.1f, -0.78f);
        CreateRotatedBox(physics_world, b2_staticBody, 3.1f, 1.72f, 0.3f, 0.1f, 2.25f);
        CreateRotatedBox(physics_world, b2_staticBody, 3.5f, 1.3f, 0.3f, 0.1f, 2.45f);

        CreateRotatedBox(physics_world, b2_staticBody, 3.95f, 1.f, 0.3f, 0.1f, 2.7f);
        CreateRotatedBox(physics_world, b2_staticBody, 4.4f, 0.85f, 0.3f, 0.1f, -0.3f);

        CreateRotatedBox(physics_world, b2_staticBody, 4.9f, 0.76f, 0.3f, 0.1f, -0.1f);
        CreateRotatedBox(physics_world, b2_staticBody, 5.5f, 0.75f, 0.3f, 0.1f, 0.12f);
        CreateRotatedBox(physics_world, b2_staticBody, 6.f, 0.9f, 0.3f, 0.1f, 0.4f);
        CreateRotatedBox(physics_world, b2_staticBody, 6.55f, 1.15f, 0.3f, 0.1f, 0.5f);

    };

    //Deprecated
    void CreateRevJoint(b2World& physics_world, float x, float y, b2Body* platform)
    {
        auto* box_anchor = Create_Box(physics_world, b2_staticBody, x, y, 0.1f, 0.1f);
        //auto* box_platform = Create_Box(physics_world, b2_dynamicBody, x, y, 1.4f, 0.1f);
        auto* boxes_joint = Create_Revolute_Joint(physics_world, box_anchor, platform, true);

        if (boxes_joint->GetUserData().pointer == 1)
            printf("Move"); //... move elevator
    };

    //Estructura que sujeta las bolas a recoger,así como la ref para dejarlas caer
    b2Body* CreateBallContainer(b2World& physics_world, b2BodyType bodytype, float xStart, float yStart, float angle, int length)
    {
        //Creamos la joint aqui            
        auto* verticalLeftBox = CreateRotatedBox(physics_world, b2_kinematicBody, xStart, yStart, 0.1f, 1.f, -angle);
        auto* verticalRightBox = CreateRotatedBox(physics_world, b2_kinematicBody, xStart + 0.9f, yStart, 0.1f, 1.f, angle);

        auto* joint1 = Create_rev_joint(physics_world, verticalRightBox, verticalLeftBox, -0.5f, -1.f, true);


        //Crear las bolas a recoger / soltar 
        for (size_t i = 0; i < length; i++)
        {
            Create_Circle(physics_world, b2_dynamicBody, xStart, yStart + 0.3f, 0.15f);
        }

        return verticalRightBox;
    }

    //Estructura que recoge las bolas a despues de que el coche las suelte
    void CreateBalleReciever(b2World& physics_world, b2BodyType bodytype, float xStart, float yStart, float angle)
    {
        CreateRotatedBox(physics_world, b2_kinematicBody, xStart, yStart, 0.1f, 0.8f, -angle);
        CreateRotatedBox(physics_world, b2_kinematicBody, xStart + 0.9f, yStart, 0.1f, 0.8f, angle);

        Create_Box(physics_world, b2_kinematicBody, xStart + 1.9f, yStart + 0.6f, 0.6f, 0.1f);

    };
    


