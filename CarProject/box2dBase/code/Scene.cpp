// angel.rodriguez@esne.edu
// 2021.03+
// 
// Modificaiones del mismo por:
// felipevm07@gmail.com
// 2023.06

#pragma once

#include "headers/Scene.h"

using namespace sf;
using namespace std;

namespace miCarBox
{
	// Constructor de la escena, se encarga de cargar todas las entidades(objetos) que se renderizan y usan en el juego
	Scene::Scene(b2Vec2 gravity, RenderWindow* window)
	{
		world.reset(new b2World(gravity));

		// Limites de la escena 
		createSceneBounds();

		// Rampas
		createRamp();

		// Contenedor bolas
		entityMap["containerJoint"] = Entity::Create_Entity_Circle(this, b2_kinematicBody, 12, 3.f, .15f, false);
		entityMap["ballsColumn"] = Entity::Create_Entity_Box(this, b2_staticBody, 12.f, 1.5f, 0.1f, 1.5f, 0.f, true);
		entityMap["rotatedBox_r"] = Entity::Create_Entity_Box(this, b2_dynamicBody, 12.55f, 3.5f, 0.8f, 0.1f, 0.7854f, false);
		entityMap["rotatedBox_l"] = Entity::Create_Entity_Box(this, b2_dynamicBody, 11.45f, 3.5f, 0.8f, 0.1f, -0.7854f, false);

		// Uniones del contenedor de las bolas
		Entity::Create_Entity_Revolute_Joint(this, entityMap["containerJoint"]->getBody(), entityMap["rotatedBox_r"]->getBody(), true);
		Entity::Create_Entity_Revolute_Joint(this, entityMap["containerJoint"]->getBody(), entityMap["rotatedBox_l"]->getBody(), true);

		// Interactuables bolas (Ultimo parametro, numero de bolas)
		Entity::create_balls(this, b2_dynamicBody, 12, 4, 0.15f, false, 7);

		// Ascensor        
		entityMap["elevator"] = Entity::Create_Entity_Box(this, b2_kinematicBody, 14.f, 1.f, 1, .12f, 0.f, false);

		// Coche
		createCar();
	}

	void Scene::render(b2World& physics_world, RenderWindow& window, float scale)
	{
		// Se cachea el alto de la ventana en una variable local:
		float window_height = (float)window.getSize().y;

		// Se recorre toda la lista de bodies del world:
		for (b2Body* body = physics_world.GetBodyList(); body != nullptr; body = body->GetNext())
		{
			// Se obtiene el transform del body:
			const b2Transform& body_transform = body->GetTransform();

			// Se recorre la lista de fixtures del body:
			for (b2Fixture* fixture = body->GetFixtureList(); fixture != nullptr; fixture = fixture->GetNext())
			{
				// Se obtiene el tipo de forma de la fixture:
				b2Shape::Type shape_type = fixture->GetShape()->GetType();

				if (shape_type == b2Shape::e_circle)
				{
					// Se crea un CircleShape a partir de los atributos de la forma de la fixture y del body:
					// En SFML el centro de un círculo no está en su position. Su position es la esquina superior izquierda
					// del cuadrado en el que está inscrito. Por eso a position se le resta el radio tanto en X como en Y.

					b2CircleShape* circle = dynamic_cast<b2CircleShape*>(fixture->GetShape());

					float  radius = circle->m_radius * scale;
					b2Vec2 center = circle->m_p;

					CircleShape shape;

					shape.setPosition(box2d_position_to_sfml_position(b2Mul(body_transform, center), window_height, scale) - Vector2f(radius, radius));
					shape.setFillColor(Color::Red);
					shape.setRadius(radius);

					window.draw(shape);

				}

				if (shape_type == b2Shape::e_edge)
				{
					// Se toman los dos vértices del segmento y se ajusta su posición usando el transform del body.
					// Los vértices resultantes se convierten y se ponen en un array para finalmente dibujar el segmento
					// que los une usando la sobrecarga del método draw() que permite dibujar primitivas de OpenGL a
					// partir de datos de vértices.

					b2EdgeShape* edge = dynamic_cast<b2EdgeShape*>(fixture->GetShape());

					b2Vec2 start = b2Mul(body_transform, edge->m_vertex1);
					b2Vec2 end = b2Mul(body_transform, edge->m_vertex2);

					Vertex line[] =
					{
						Vertex(box2d_position_to_sfml_position(start, window_height, scale), Color::Green),
						Vertex(box2d_position_to_sfml_position(end, window_height, scale), Color::Green),
					};

					window.draw(line, 2, Lines);
				}
				else if (shape_type == b2Shape::e_polygon)
				{
					// Se toma la forma poligonal de Box2D (siempre es convexa) y se crea a partir de sus vértices un
					// ConvexShape de SFML. Cada vértice de Box2D hay que transformarlo usando el transform del body.

					b2PolygonShape* box2d_polygon = dynamic_cast<b2PolygonShape*>(fixture->GetShape());
					ConvexShape       sfml_polygon;

					int number_of_vertices = box2d_polygon->m_count;

					sfml_polygon.setPointCount(number_of_vertices);
					sfml_polygon.setFillColor(Color::Green);

					for (int index = 0; index < number_of_vertices; index++)
					{
						sfml_polygon.setPoint
						(
							index,
							box2d_position_to_sfml_position(b2Mul(body_transform, box2d_polygon->m_vertices[index]), window_height, scale)
						);
					}

					window.draw(sfml_polygon);
				}
			}
		}
	}

	void Scene::update(b2World& physics_world, RenderWindow& window, float scale, float delta_time)
	{
		this->render(physics_world, window, scale);

		if (doItOnce == false)
		{
			// Si la rueda izquierda pasa la posición de la torreta con las balls,se rota la torreta para que caigan las bolas y sale el ascensor.
			if (entityMap["leftWheelCar"]->getBody()->GetPosition().x > entityMap["ballsColumn"]->getBody()->GetPosition().x)
			{
				entityMap["containerJoint"]->getBody()->SetAngularVelocity(-0.2f);
				elevetarSeen = true;
				doItOnce = true;
			}
		}

		// La torreta para de girar cuando la altura de se lado derecho es menor o igual  a 2.5
		if (entityMap["rotatedBox_r"]->getBody()->GetPosition().y <= 2.5f)
		{
			entityMap["containerJoint"]->getBody()->SetAngularVelocity(0.f);
		}

		// El ascensor sale y se posiciona a la altura del suelo
		if (elevetarSeen == true)
		{
			if (entityMap["elevator"]->getBody()->GetPosition().y < 0.10f)
			{
				entityMap["elevator"]->getBody()->SetLinearVelocity({ 0, +0.5f });
			}
			else
			{
				elevetarSeen = false;
				elevetarActive = true;
				entityMap["elevator"]->getBody()->SetLinearVelocity({ 0, +0.0f });
			}
		}

		// Si se detecta que la reuda derecha del coche subió a la plataforma, esta sube hasta el segundo piso.
		if (elevetarActive == true)
		{
			if ((entityMap["rightWheelCar"]->getBody()->GetPosition().x > 6.00f) && (entityMap["rightWheelCar"]->getBody()->GetPosition().x < 9.00f))
			{
				if (entityMap["elevator"]->getBody()->GetPosition().y < 4)
				{
					entityMap["elevator"]->getBody()->SetLinearVelocity({ 0, +0.5f });
				}
				else
				{
					entityMap["elevator"]->getBody()->SetLinearVelocity({ 0, +0.0f });
					elevetarActive = false;
				}
			}
		}

		// Si el coche se cae por el vacío se resetea su posición
		if (entityMap["carBaseShape"]->getBody()->GetPosition().y < 0.f)
		{
			resetCarPosition();
		}
	}

	// Creamos los limites de la escena
	void Scene::createSceneBounds()
	{
		//To do: limites como parametros
		constexpr float left = 0.01f;
		constexpr float right = 15.00f;
		constexpr float top = 7.50f;
		constexpr float bottom = 0.01f;

		Entity::Create_Entity_Edge(this, b2_staticBody, left, bottom, right, bottom, false);
		Entity::Create_Entity_Edge(this, b2_staticBody, left, bottom, left, top, false);
		Entity::Create_Entity_Edge(this, b2_staticBody, left, top, right, top, false);
		Entity::Create_Entity_Edge(this, b2_staticBody, right, bottom, right, top, false);
	}

	// Rampas de la escena
	void Scene::createRamp()
	{

		Entity::Create_Entity_Edge(this, b2_staticBody, 2.32f, 2.41f, 0.3f, 0.1f, false);
		Entity::Create_Entity_Edge(this, b2_staticBody, 2.72f, 2.15f, 0.3f, 0.1f, false);
		Entity::Create_Entity_Edge(this, b2_staticBody, 3.1f, 1.72f, 0.3f, 0.1f, false);
		Entity::Create_Entity_Edge(this, b2_staticBody, 3.5f, 1.3f, 0.3f, 0.1f, false);

		Entity::Create_Entity_Edge(this, b2_staticBody, 3.95f, 1.f, 0.3f, 0.1f, false);
		Entity::Create_Entity_Edge(this, b2_staticBody, 4.4f, 0.85f, 0.3f, 0.1f, false);

		Entity::Create_Entity_Edge(this, b2_staticBody, 4.9f, 0.76f, 0.3f, 0.1f, false);
		Entity::Create_Entity_Edge(this, b2_staticBody, 5.5f, 0.75f, 0.3f, 0.1f, false);
		Entity::Create_Entity_Edge(this, b2_staticBody, 6.f, 0.9f, 0.3f, 0.1f, false);
		Entity::Create_Entity_Edge(this, b2_staticBody, 6.55f, 1.15f, 0.3f, 0.1f, false);
	}

	// Coche
	void Scene::createCar()
	{
		// Cuerpo coche
		entityMap["carBaseShape"] = Entity::Create_CarBaseShape(this, b2_dynamicBody, 0.10f, 2.5f);
		entityMap["baseCarBox"] = Entity::Create_Entity_Box(this, b2_dynamicBody, 0.6f, 2.6f, 0.5f, 0.05f, 0.f, false);
		Entity::Create_Entity_Revolute_Joint(this, entityMap["carBaseShape"]->getBody(), entityMap["baseCarBox"]->getBody(), false);

		// Ruedas coche
		entityMap["rightWheelCar"] = Entity::Create_Entity_Circle(this, b2_dynamicBody, 1.f, 2.4f, .15f, false);
		rightWheelJoint = Entity::Create_Wheel_Joint(this, entityMap["baseCarBox"]->getBody(), entityMap["rightWheelCar"]->getBody(), 0.0f, 2000.f, true);
		entityMap["leftWheelCar"] = Entity::Create_Entity_Circle(this, b2_dynamicBody, 0.2f, 2.4f, .15f, false);
		leftWheelCarJoint = Entity::Create_Wheel_Joint(this, entityMap["baseCarBox"]->getBody(), entityMap["leftWheelCar"]->getBody(), 0.0f, 2000.f, true);
	}

	// Modificamos la velocidad de las ruedas acorde al mismo parametro
	void Scene::setCarVelocity(float speed)
	{
		leftWheelCarJoint->SetMotorSpeed(speed);
		rightWheelJoint->SetMotorSpeed(speed);
	}

	// Reseteamos el coche es una posicion
	void Scene::resetCarPosition()
	{
		entityMap["carBaseShape"]->getBody()->SetTransform(b2Vec2(0.10f, 2.5f), 0);
		entityMap["baseCarBox"]->getBody()->SetTransform(b2Vec2(0.6f, 2.6f), 0);
		entityMap["rightWheelCar"]->getBody()->SetTransform(b2Vec2(1.f, 2.4f), 0);
		entityMap["leftWheelCar"]->getBody()->SetTransform(b2Vec2(0.2f, 2.4f), 0);

		rightWheelJoint->SetMotorSpeed(0);
		leftWheelCarJoint->SetMotorSpeed(0);
	}


}