
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


using namespace std;
using namespace sf;


class Enviroment
{   
public:
    //Constructor por defecto
    //Enviroment() = default;

    //Constructor de algunos elementos de la escena
    Enviroment(b2World& physics_world);

    //Crear margenes de la escena
    void CreateMargins(b2World& physics_world);

    //Crear un box estático, ademas, con el parámetro lenght podemos de una realizar varias iteraciones, como por ejemplo para crear el suelo plano en ambos extremos.
    void CreateStaticBox(b2World& physics_world, float xStart, float yStart, float xEnd, float yEnd, int length);

    //Con la anterior función, se usa para crear la curva del inicio de la escena                                               ----------- try chain shape
    void CreateCurve(b2World& physics_world);//, float xStart, float yStart, float xEnd, float yEnd,int length, float angle)

    //Deprecated
    void CreateRevJoint(b2World& physics_world, float x, float y, b2Body* platform);

    //Estructura que sujeta las bolas a recoger,así como la ref para dejarlas caer
    b2Body* CreateBallContainer(b2World& physics_world, b2BodyType bodytype, float xStart, float yStart, float angle, int length);

    //Estructura que recoge las bolas a despues de que el coche las suelte
    void CreateBalleReciever(b2World& physics_world, b2BodyType bodytype, float xStart, float yStart, float angle);
};


