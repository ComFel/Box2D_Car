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
#include <map>
#include <SFML/Graphics.hpp>
#include "operations.h"

using namespace sf;
using namespace std;
using namespace miCarBox;


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

	//Función que referencia la clase b2World		
	b2World& get_world() const { return *world; }

	// Funcion para crear los limites de la escena (ToDo: parámetros)
	void createSceneBounds();

	// Crear la rampa de la escena por separado
	void createRamp();

	// Creacion del coche
	void createCar();

	// Función para aplicar fuerza a las ruedas
	void setCarVelocity(float speed);

	// Función que sirve para reiniciar la posición inicial del coche
	void resetCarPosition();

	/** En Box2D las coordenadas Y crecen hacia arriba y en SFML crecen hacia abajo desde el borde
	* superior. Esta función se encarga de convertir el sistema de coordenadas para que la escena
	* no se vea invertida.
	*/
	inline Vector2f box2d_position_to_sfml_position(const b2Vec2& box2d_position, float window_height, float scale)
	{
		return Vector2f(box2d_position.x * scale, window_height - box2d_position.y * scale);
	}

	// ------------------------------------------------------------------------------------------ //

	inline b2Vec2 sfml_position_to_box2d_position(const Vector2f& sfml_position, float window_height, float scale)
	{
		return b2Vec2(sfml_position.x / scale, (window_height - sfml_position.y) / scale);
	}
};


