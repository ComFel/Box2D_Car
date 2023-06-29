// 
// Basado en un c�digo de dominio p�blico de:
// angel.rodriguez@esne.edu
// 2021.03+
// 
// Modificaiones del mismo por:
// felipevm07@gmail.com
// 2022.06

#pragma once

#include <box2d/box2d.h>
#include <memory>
#include <map>
#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "operations.h"


using namespace sf;
using namespace std;

namespace miCarBox 
{
	class Entity;

	class Scene
	{
	public:
		// Referencia al mundo creado por b2World
		unique_ptr < b2World > world;

		// Organizar las entidades por nombre (string)
		map< string, shared_ptr< Entity > > entityMap;

		// Variables para las fases de los eventos
		bool doItOnce = false;
		bool elevetarSeen = false;
		bool elevetarActive = false;

		Scene() = default;

		// Constructor de la escena con las entidades necesarias
		Scene(b2Vec2 gravity, RenderWindow* window);

		void render(b2World& physics_world, RenderWindow& renderer, float scale);

		void update(b2World& physics_world, RenderWindow& window, float scale, float delta_time);

		// Referencia la rueda izquierda del coche		
		b2WheelJoint* leftWheelCarJoint = nullptr;

		// Referencia hacia la rueda derecha del coche		
		b2WheelJoint* rightWheelJoint = nullptr;

		//Funci�n que referencia la clase b2World		
		b2World& get_world() const { return *world; }

		// Funcion para crear los limites de la escena (ToDo: par�metros)
		void createSceneBounds();

		// Crear la rampa de la escena por separado
		void createRamp();

		// Creacion del coche
		void createCar();

		// Funci�n para aplicar fuerza a las ruedas
		void setCarVelocity(float speed);

		// Funci�n que sirve para reiniciar la posici�n inicial del coche
		void resetCarPosition();
	};


}