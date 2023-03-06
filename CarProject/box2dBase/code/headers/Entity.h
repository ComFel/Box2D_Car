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
#include "Scene.h"


using namespace std;

namespace operations 
{
	class Scene;

	class Entity
	{
	public:
		// Constructor por defecto
		Entity() = default;

		// Referencia al cuerpo b2Body de la entidad
		b2Body* rigidBody = nullptr;
		
		// Puntero a la forma del body
		shared_ptr<b2Shape> graphShape;

		// Funci�n que devuelve la referencia al body de la entidad
		b2Body* getBody() { return rigidBody; }

		static shared_ptr < Entity >  create_edge(Scene* scene, b2BodyType body_type, float x0, float y0, float x1, float y1, bool isSensor);

		static shared_ptr < Entity >  create_circle(Scene* scene, b2BodyType body_type, float x, float y, float radius, bool isSensor);
		
		static shared_ptr < Entity >  create_box(Scene* scene, b2BodyType body_type, float x, float y, float width, float height, float radians, bool isSensor);

		static b2RevoluteJoint* create_revolute_joint(Scene* scene, b2Body* bodyA, b2Body* bodyB, bool enabledMotor);
		
		static b2WheelJoint* create_wheel_joint(Scene* scene, b2Body* bodyA, b2Body* bodyB, float motorSpeed, float torque, bool isEnabled);

		static shared_ptr < Entity >  create_carBaseShape(Scene* scene, b2BodyType body_type, float x, float y);


		static void create_balls(Scene* scene, b2BodyType body_type, float x, float y, float radius, bool isSensor, int collectBalls)
		{
			for (size_t i = 0; i < collectBalls; i++)
			{
				Entity::create_circle(scene, body_type, x, y, radius, isSensor);
			}
		}
	};

}