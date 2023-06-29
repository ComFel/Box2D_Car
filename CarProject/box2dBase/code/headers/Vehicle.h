
// Modificaiones del mismo por:
// felipevm07@gmail.com
// 2022.06
//
//   

#pragma once

#include <ciso646>
#include <memory>
#include <vector>
#include <Box2D/Box2D.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "operations.h"

using namespace std;
using namespace sf;

#pragma once

namespace miCarBox 
{
    class Vehicle
    {
    public:
        //Constructor base del veh�culo
        Vehicle(b2World& physics_world);

        //WIP
        void CreateCarCest(b2World& physics_world, float x, float y); //b2Body*

        //Creamos un rueda
        b2Body* CreateWheel(b2World& physics_world, float x, float y, float radius);

    };
}