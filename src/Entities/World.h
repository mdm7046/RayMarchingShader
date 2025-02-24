//
// Created by mdm31 on 2/14/2025.
// world represents a scene, which contains objects, a skybox, and lights
//

#ifndef WORLD_H
#define WORLD_H
#include <list>

#include "Object.h"
#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/Shader.hpp"

#define SKY_COLOR glm::vec3(64.f, 160.f, 186.f) / 255.f
#define GROUND_COLOR glm::vec3(75.f, 79.f, 44.f) / 255.f

class World {
private:
    std::pmr::list<Object*> objects;
    std::pmr::list<Light*> lights;

    glm::vec3 sunDirection;
    Light::LightColor sunColor;
    glm::vec3 ambientLight;
public:
    World(const glm::dvec3 &sunDirection, const Light::LightColor &sunColor, const sf::Color ambient)
        : sunDirection(normalize(sunDirection)), sunColor(sunColor) {
        objects = std::pmr::list<Object*>();
        lights = std::pmr::list<Light*>({nullptr}); //sun is represented as nullptr
        ambientLight = glm::vec4(ambient.r, ambient.g, ambient.b, ambient.a) / 255.f;
    }
    ~World();

    void addObject(Object *object) { objects.push_back(object); }
    void addLight(Light *light) { lights.push_back(light); }

    void setShaderObjectsBuffer(sf::Shader &shader) const;
    void getSunInfo(Light::LightColor &sunColor, glm::vec3 &sunDirection, glm::vec3 &ambientLight, glm::vec3 &skyColor, glm::vec3 &groundColor) const {
        sunColor = this->sunColor;
        sunDirection = this->sunDirection;
        ambientLight = this->ambientLight;
        skyColor = SKY_COLOR;
        groundColor = GROUND_COLOR;
    }
};

#endif //WORLD_H
