//
// Created by mdm31 on 2/22/2025.
//

#include "World.h"

World::~World() {
    for (const auto &obj : objects) { delete obj; }
    for (const auto &light : lights) { delete light; }
}

void World::setShaderObjectsBuffer(sf::Shader &shader) const {

}
