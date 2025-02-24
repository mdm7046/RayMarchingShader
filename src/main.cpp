#include <iostream>
#include <SFML/Window.hpp>

#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Image.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Shader.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/Texture.hpp"

#include "Entities/Camera.h"

#define PI glm::pi<float>()

#define ROT_SPEED 0.1
#define MOVE_SPEED 0.25

int main() {
    auto window = sf::RenderWindow(sf::VideoMode({windowWidth, windowHeight}), "Ray Marching", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(10);

    sf::Clock clock;
    const auto font = sf::Font(R"(C:\Windows\Fonts\Arial.TTF)");
    auto fpsCounter = sf::Text(font);
    fpsCounter.setScale({0.5, 0.5});

    World world = {{0, -1, 0}, Light::LightColor::DimWhite, {50, 50, 50}};

    // world.addObject(new Ball({0, 1.5, 0}, 1, Object::Material::BluePlastic));
    // world.addObject(new Ball({0, 3, 0}, 0.25, Object::Material::WhitePlastic));
    // Box testBox = {{0, 0, 0}, {}, {10, 1, 10}, Object::Material::Metal};
    // world.addObject(&testBox);

    // world.addObject(new Ball({1.5,0,0}, 1, Object::Material::Metal));
    // world.addObject(new Ball({-1.5,0,0}, 1, Object::Material::BluePlastic));

    // glm::dvec3 p1 = {2.5, 2.5, 2.5};
    // glm::dvec3 p2 = {-2.5, -2.5, 2.5};
    // glm::dvec3 p3 = {2.5, -2.5, 2.5};
    // Plane testPlane = {p1, p2, p3, Object::Material::Metal};
    // world.addObject(&testPlane);
    // world.addObject(new Plane(p1, p2, p4, Object::Material::BluePlastic));

    // world.addLight(new Light({5, 5, 5}, Light::LightColor::Cyan));

    Camera eye = {{0, 0, -5}, {0, 0, 0}, 0.1, 0.5*PI, &world};

    auto cover = sf::RectangleShape(sf::Vector2f(windowWidth, windowHeight));
    auto shader = sf::Shader{};
    if (!shader.loadFromFile("shader.frag", sf::Shader::Type::Fragment))
    {
        std::cerr << "Couldn't load fragment shader\n";
        return -1;
    }

    clock.start();
    while (window.isOpen()) {
        // check all the window's events that were triggered since the last iteration of the loop
        while (const std::optional event = window.pollEvent())
        {
            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>()) {
                window.close();
                return 0;
            }
            if (event->is<sf::Event::KeyPressed>()) {
                switch (event->getIf<sf::Event::KeyPressed>()->code) {
                    case sf::Keyboard::Key::I:
                        eye.transform({}, {ROT_SPEED, 0, 0});
                        break;
                    case sf::Keyboard::Key::J:
                        eye.transform({}, {0, -ROT_SPEED, 0});
                        break;
                    case sf::Keyboard::Key::K:
                        eye.transform({}, {-ROT_SPEED, 0, 0});
                        break;
                    case sf::Keyboard::Key::L:
                        eye.transform({}, {0, ROT_SPEED, 0});
                        break;


                    case sf::Keyboard::Key::W:
                        eye.transform({0, 0, MOVE_SPEED}, {});
                        break;
                    case sf::Keyboard::Key::A:
                        eye.transform({-MOVE_SPEED, 0, 0}, {});
                        break;
                    case sf::Keyboard::Key::S:
                        eye.transform({0, 0, -MOVE_SPEED}, {});
                        break;
                    case sf::Keyboard::Key::D:
                        eye.transform({MOVE_SPEED, 0, 0}, {});
                        break;
                    case sf::Keyboard::Key::Space:
                        eye.transform({0, MOVE_SPEED, 0}, {});
                        break;
                    case sf::Keyboard::Key::LShift:
                        eye.transform({0, -MOVE_SPEED, 0}, {});
                        break;

                    default:
                        break;
                }
            }
        }

        //render frame, then send to GPU
        eye.setShaderUniforms(shader);

        float fps = 1.f / clock.restart().asSeconds();
        fpsCounter.setString("FPS: " + std::to_string(fps));

        //draw frame to screen
        window.draw(cover, &shader);
        window.draw(fpsCounter);
        window.display();

        // testPlane.transform({0, 0, 0}, {0.01, 0, 0}, {1, 1, 1});
    }

    return 0;
}
