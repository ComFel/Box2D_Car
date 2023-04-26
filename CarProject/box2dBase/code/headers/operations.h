// 
// Basado en un código de dominio público de:
// angel.rodriguez@esne.edu
// 2021.03+
// 
// Modificaiones del mismo por:
// felipevm07@gmail.com
// 2022.03
//
// Controles:
//
//   


#include <ciso646>
#include <memory>
#include <vector>
#include <Box2D/Box2D.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;


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

// ------------------------------------------------------------------------------------------ //

b2Body* Create_Circle(b2World& physics_world, b2BodyType body_type, float x, float y, float radius)
{
    // Se crea el body:

    b2BodyDef body_definition;

    body_definition.type = body_type;
    body_definition.position.Set(x, y);                            // Posición inicial absoluta

    b2Body* body = physics_world.CreateBody(&body_definition);

    // Se añande una fixture:

    b2CircleShape body_shape;

    body_shape.m_radius = radius;

    b2FixtureDef body_fixture;

    body_fixture.shape = &body_shape;
    body_fixture.density = 1.00f;
    body_fixture.restitution = 0.75f;
    body_fixture.friction = 0.50f;

    body->CreateFixture(&body_fixture);

    return body;
}

// ------------------------------------------------------------------------------------------ //

b2Body* Create_Box(b2World& physics_world, b2BodyType body_type, float x, float y, float width, float height)
{
    // Se crea el body:

    b2BodyDef body_definition;

    body_definition.type = body_type;
    body_definition.position.Set(x, y);                            // Posición inicial absoluta

    b2Body* body = physics_world.CreateBody(&body_definition);

    // Se añande una fixture:

    b2PolygonShape body_shape;

    body_shape.SetAsBox(width, height);

    b2FixtureDef body_fixture;

    body_fixture.shape = &body_shape;
    body_fixture.density = 1.00f;
    body_fixture.restitution = 0.50f;
    body_fixture.friction = 0.50f;

    body->CreateFixture(&body_fixture);

    return body;
}

// ------------------------------------------------------------------------------------------ //

b2Body* Create_Edge(b2World& physics_world, b2BodyType body_type, float x0, float y0, float x1, float y1)
{
    // Se crea el body:

    b2BodyDef body_definition;

    body_definition.type = body_type;
    body_definition.position.Set(0.f, 0.f);                        // Posición inicial absoluta

    b2Body* body = physics_world.CreateBody(&body_definition);

    // Se añande una fixture:

    b2EdgeShape body_shape;

    body_shape.SetTwoSided(b2Vec2(x0, y0), b2Vec2(x1, y1));        // Coordenadas locales respecto al centro del body

    b2FixtureDef body_fixture;

    body_fixture.shape = &body_shape;

    body->CreateFixture(&body_fixture);

    return body;
}

// ------------------------------------------------------------------------------------------ //

b2RevoluteJoint* Create_Revolute_Joint(b2World& physics_world, b2Body* body_a, b2Body* body_b, bool enable_motor = false)
{
    b2RevoluteJointDef joint_def;

    joint_def.bodyA = body_a;
    joint_def.bodyB = body_b;
    joint_def.enableMotor = enable_motor;
    joint_def.collideConnected = false;
        
    //joint_def.userData.pointer = 1; //false;

    return dynamic_cast<b2RevoluteJoint*>(physics_world.CreateJoint(&joint_def));
}

// Función para los anclajes de la ruedas, donde se le pasa la posición del anclaje. AnchorAy es recomendable que sea la Y / altura / radio de la rueda
b2RevoluteJoint* Create_rev_joint(b2World& physics_world, b2Body* body_a, b2Body* body_b, float anchorAx, float anchorAy, bool enable_motor = false)
{
    b2RevoluteJointDef joint_def;

    joint_def.bodyA = body_a;
    joint_def.bodyB = body_b;
    joint_def.enableMotor = enable_motor;
    joint_def.collideConnected = false;

    joint_def.localAnchorA.Set(anchorAx, anchorAy);
    joint_def.localAnchorB.Set(0, 0);

    //joint_def.userData.pointer = 1; //false;

    return dynamic_cast<b2RevoluteJoint*>(physics_world.CreateJoint(&joint_def));
}

b2Body* create_VshapePolygon(b2World& physics_world, b2BodyType body_type, float x, float y)
{
    // Se crea el body:

    b2BodyDef body_definition;
    body_definition.type = body_type;
    body_definition.position.Set(x, y);

    b2Body* body = physics_world.CreateBody(&body_definition);

    // This defines the polygon V
    b2Vec2 vertices[8];
    vertices[0].Set(0.f  , 0.f);
    vertices[1].Set(1.0f , 0.f);
    vertices[2].Set(1.5f , 2.5f);
    vertices[3].Set(1.2f , 2.5f);

    vertices[4].Set(0.f   , 0.f);
    vertices[5].Set(-1.5f , 2.5f);
    vertices[6].Set(-1.2f , 2.5f);
    vertices[7].Set(0.2f  , 0.3f);               
        
    int32 count = 8;

    b2PolygonShape body_shape;

    body_shape.Set(vertices, count);

    b2FixtureDef body_fixture;

    body_fixture.shape = &body_shape;
    body_fixture.density = 0.50f;
    body_fixture.restitution = 0.05f;
    body_fixture.friction = 0.5f;

    body->CreateFixture(&body_fixture);

    return body;
}

// ------------------------------------------------------------------------------------------ //

void render_circle
(
    b2Vec2              center,
    float               radius,
    const b2Transform& transform,
    RenderWindow& renderer,
    float               window_height,
    float               scale
)
{
    CircleShape shape;

    radius *= scale;

    shape.setPosition(box2d_position_to_sfml_position(b2Mul(transform, center), window_height, scale) - Vector2f(radius, radius));
    shape.setRadius(radius);
    shape.setFillColor(Color::Color(144, 10, 52, 255));

    renderer.draw(shape);
}

// ------------------------------------------------------------------------------------------ //

void render_segment
(
    const Vector2f& start,
    const Vector2f& end,
    const Color& color,
    RenderWindow& renderer
)
{
    Vertex line[] =
    {
        Vertex(start, color),
        Vertex(end, color),
    };

    renderer.draw(line, 2, Lines);
}

// ------------------------------------------------------------------------------------------ //

void render_segment
(
    b2Vec2              start,
    b2Vec2              end,
    const b2Transform& transform,
    RenderWindow& renderer,
    float               window_height,
    float               scale
)
{
    start = b2Mul(transform, start);
    end = b2Mul(transform, end);

    render_segment
    (
        box2d_position_to_sfml_position(start, window_height, scale),
        box2d_position_to_sfml_position(end, window_height, scale),
        Color::Green,
        renderer
    );
}

// ------------------------------------------------------------------------------------------ //

void render_polygon
(
    const b2Vec2* vertices,
    int                 number_of_vertices,
    const b2Transform& transform,
    RenderWindow& renderer,
    float               window_height,
    float               scale
)
{
    ConvexShape sfml_polygon;

    sfml_polygon.setPointCount(number_of_vertices);
    sfml_polygon.setFillColor(Color::Color(75,150,177,255));

    for (int index = 0; index < number_of_vertices; index++)
    {
        sfml_polygon.setPoint
        (
            index,
            box2d_position_to_sfml_position(b2Mul(transform, vertices[index]), window_height, scale)
        );
    }

    renderer.draw(sfml_polygon);
}

// ------------------------------------------------------------------------------------------ //

/* void Scene::render(b2World& physics_world, RenderWindow& renderer, float scale)
{
    // Se cachea el alto de la ventana en una variable local:

    float window_height = (float)renderer.getSize().y;

    // Se recorre toda la lista de bodies de physics_world:

    for (b2Body* body = physics_world.GetBodyList(); body != nullptr; body = body->GetNext())
    {
        // Se obtiene el transform del body:

        const b2Transform& body_transform = body->GetTransform();

        // Se recorre la lista de fixtures del body:

        for (b2Fixture* fixture = body->GetFixtureList(); fixture != nullptr; fixture = fixture->GetNext())
        {
            // Se obtiene el tipo de forma de la fixture:

            b2Shape::Type shape_type = fixture->GetShape()->GetType();

            switch (shape_type)
            {
            case b2Shape::e_circle:
            {
                // Se crea un CircleShape a partir de los atributos de la forma de la fixture y del body:
                // En SFML el centro de un círculo no está en su position. Su position es la esquina superior izquierda
                // del cuadrado en el que está inscrito. Por eso a position se le resta el radio tanto en X como en Y.

                b2CircleShape* circle = dynamic_cast<b2CircleShape*>(fixture->GetShape());

                render_circle(circle->m_p, circle->m_radius, body_transform, renderer, window_height, scale);

                break;
            }

            case b2Shape::e_edge:
            {
                // Se toman los dos vértices del segmento y se ajusta su posición usando el transform del body.
                // Los vértices resultantes se convierten y se ponen en un array para finalmente dibujar el segmento
                // que los une usando la sobrecarga del método draw() que permite dibujar primitivas de OpenGL a
                // partir de datos de vértices.

                b2EdgeShape* edge = dynamic_cast<b2EdgeShape*>(fixture->GetShape());

                render_segment(edge->m_vertex1, edge->m_vertex2, body_transform, renderer, window_height, scale);

                break;
            }

            case b2Shape::e_polygon:
            {
                // Se toma la forma poligonal de Box2D (siempre es convexa) y se crea a partir de sus vértices un
                // ConvexShape de SFML. Cada vértice de Box2D hay que transformarlo usando el transform del body.

                b2PolygonShape* box2d_polygon = dynamic_cast<b2PolygonShape*>(fixture->GetShape());

                render_polygon(box2d_polygon->m_vertices, box2d_polygon->m_count, body_transform, renderer, window_height, scale);

                break;
            }
            }
        }
    }
} */

// ------------------------------------------------------------------------------------------ //

struct Input_Status
{
    bool  mouse_was_clicked = false;
    float click_start_x = 0;
    float click_start_y = 0;
    float current_mouse_x = 0;
    float current_mouse_y = 0;
};

// ------------------------------------------------------------------------------------------ //

bool process(Window& window, Input_Status& status, b2Body* body_a, b2Body* body_b, float window_height, float scale)
{
    Event event;

    while (window.pollEvent(event))
    {
        switch (event.type)
        {
        case Event::KeyPressed:
        {
            switch (event.key.code)
            {
            case Keyboard::Left:
            {
                if (body_a->GetAngularVelocity() < +10.f) body_a->ApplyTorque(+15, true);
                if (body_b->GetAngularVelocity() < +10.f) body_b->ApplyTorque(+15, true);
                break;
            }
            case Keyboard::Right:
            {
                if (body_a->GetAngularVelocity() > -10.f) body_a->ApplyTorque(-10, true);
                if (body_b->GetAngularVelocity() > -10.f) body_b->ApplyTorque(-10, true);
                break;
            }
            case Keyboard::Space:
            {
                //Rotar la cesta
                //...
                break;
            }
            }

            break;
        }

        case Event::MouseButtonPressed:
        {
            if (event.mouseButton.button == Mouse::Button::Left)
            {
                status.click_start_x = status.current_mouse_x = float(event.mouseButton.x);
                status.click_start_y = status.current_mouse_y = float(event.mouseButton.y);
                status.mouse_was_clicked = true;
            }

            break;
        }

        case Event::Closed:
        {
            return true;
        }
        }
    }

    return false;
}

//Usado para crear las partes de la curva, parecido al anterior, le pasamos un ángulo en radianes para su rotación
b2Body* CreateRotatedBox(b2World& physics_world, b2BodyType bodytype, float xStart, float yStart, float xEnd, float yEnd, float angle)
{
    b2Body* myBody = Create_Box(physics_world, bodytype, xStart, yStart, xEnd, yEnd);
    b2Vec2 vec2 = b2Vec2(xStart, yStart);
    myBody->SetTransform(vec2, angle);

    return myBody;
};







