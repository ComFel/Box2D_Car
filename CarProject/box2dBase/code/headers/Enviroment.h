
// Modificaiones del mismo por:
// felipevm07@gmail.com
// 2023.06
//
//   

#pragma once

#include <ciso646>
#include <memory>
#include <vector>
#include <Box2D/Box2D.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Vehicle.h"
#include "operations.h"


using namespace std;
using namespace sf;


namespace miCarBox 
{
    class Enviroment
    {
    public:
        //Constructor por defecto
        //Enviroment() = default;

        //Constructor de algunos elementos de la escena
        Enviroment(b2World& physics_world);

        //Crear margenes de la escena
        void CreateMargins(b2World& physics_world);

        //Crear un box est�tico, ademas, con el par�metro lenght podemos de una realizar varias iteraciones, como por ejemplo para crear el suelo plano en ambos extremos.
        void CreateStaticBox(b2World& physics_world, float xStart, float yStart, float xEnd, float yEnd, int length);

        //Con la anterior funci�n, se usa para crear la curva del inicio de la escena             
        void CreateCurve(b2World& physics_world);//, float xStart, float yStart, float xEnd, float yEnd,int length, float angle)

        //Deprecated
        void CreateRevJoint(b2World& physics_world, float x, float y, b2Body* platform);

        //Estructura que sujeta las bolas a recoger,as� como la ref para dejarlas caer
        /**
         *
         *
         * \param physics_world
         * \param bodytype
         * \param xStart
         * \param yStart
         * \param angle
         * \param length
         * \return
         */
        b2Body* CreateBallContainer(b2World& physics_world, b2BodyType bodytype, float xStart, float yStart, float angle, int length);

        //Estructura que recoge las bolas a despues de que el coche las suelte
        /**
         *
         * \param physics_world
         * \param bodytype
         * \param xStart
         * \param yStart
         * \param angle
         */
        void CreateBalleReciever(b2World& physics_world, b2BodyType bodytype, float xStart, float yStart, float angle);
    };


}