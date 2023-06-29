//
// Basado en un código de dominio público de:
// angel.rodriguez@esne.edu
// 2021.03+
// 
// Modificaiones del mismo por:
// felipevm07@gmail.com
// 2023.06
// 
// 
// Controles:
// Flechas direccion derecha e izquierda
// R para reiniciar posicion
// Espacio para frenar


#include "headers/Scene.h"
#include "headers/operations.h"

using namespace sf;
using namespace std;
using namespace miCarBox;


int main ()
{
    constexpr auto window_width  = 1500u;
    constexpr auto window_height = 750u;

    RenderWindow window(VideoMode(window_width, window_height), "Practica_1_Anim_3D : Coche", Style::Titlebar | Style::Close, ContextSettings(32));

    window.setVerticalSyncEnabled (true);

    // Creacion de la escena con el constructor
    Scene* scene = new Scene(b2Vec2(0.f, -10.f), &window);

    // Escala para pasar de unidades de física a unidades de gráficos
    const float physics_to_graphics_scale = 100.f;      

    // Variables para los eventos de input
    float speed = 100.f;
    float stopCar = 0.f;

    // Se ejecuta el bucle principal de la animación:
    miCarBox::Input_Status status;
    
    const float target_fps  = 60.f;                     // Cuántos fotogramas por segundo se busca conseguir
    const float target_time = 1.f / target_fps;         // Duración en segundos de un fotograma a la tasa deseada

    float delta_time = target_time;                     // Previsión de la duración del fotograma actual
    bool  exit       = false;

    Clock timer;

    do
    {
        timer.restart ();

        // Controlador de los eventos de pulsacion
        Event event;

        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case Event::KeyPressed:
                {
                    switch (event.key.code)
                    {
                        // Velocidad coche, usamos el negativo para la direccion
                        case Keyboard::Right:
                        {
                            scene->setCarVelocity(speed);   
                            break;
                        }
                        case Keyboard::Left:
                        {
                            scene->setCarVelocity(-speed); 
                            break;
                        }
                        // Parada
                        case Keyboard::Space:
                        {
                            scene->setCarVelocity(stopCar);    
                            break;
                        }
                        // Reset de la posición
                        case Keyboard::R:
                        {
                            scene->resetCarPosition();         
                            break;
                        }
                    }

                    break;
                }

                case Event::Closed:
                {
                    return true;
                }
            }
        }


        // Render:
        scene->get_world().Step(delta_time, 8, 4);

        window.clear ();

        //render (physics_world, window, physics_to_graphics_scale);
        scene->update(scene->get_world(), window, physics_to_graphics_scale, delta_time);
        
        window.display ();

        // Si resulta necesario se detiene la ejecución unos instantes para no exceder la tasa de
        // fotogramas por segundo deseada:

        float elapsed = timer.getElapsedTime ().asSeconds ();

        if (elapsed < target_time)
        {
            sleep (seconds (target_time - elapsed));
        }

        // Se restablece la estimación de la duración del siguiente fotograma:

        delta_time = timer.getElapsedTime ().asSeconds ();
    }
    while (not exit);

    return 0;
}
