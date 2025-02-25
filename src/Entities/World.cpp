//
// Created by mdm31 on 2/22/2025.
//

#include "World.h"

#include "../../cmake-build-debug/_deps/sfml-src/extlibs/headers/glad/include/glad/gl.h"

World::~World() {
    for (const auto &obj : objects) { delete obj; }
    for (const auto &light : lights) { delete light; }
}

void World::setShaderObjectsBuffer(sf::Shader &shader) const {
    assert(objects.size() < 10 && lights.size() < 10);
    shader.setUniform("objectBuffer.numObjects", static_cast<int>(objects.size()));

    std::string locs[] = {
        "objectBuffer.objects[0].material.specularCoeffs",
        "objectBuffer.objects[0].material.diffuseCoeffs",
        "objectBuffer.objects[0].material.ambientCoeffs",
        "objectBuffer.objects[0].material.reflectionCoeffs",
        "objectBuffer.objects[0].material.shininess",
        "objectBuffer.objects[0].WorldToObjTransRot",
        "objectBuffer.objects[0].ObjToWorldRot",
        "objectBuffer.objects[0].scaleFactors",
        "objectBuffer.objects[0].objType"
    };
    int n = 0;
    for (const auto &obj : objects) {
        struct shader_object {
            Object::Material mat;
            glm::mat4x4 WorldToObjMat;
            glm::mat3x3 ObjToWorldRot;
            glm::vec3 scaleFactors;
            int type;
        } focus{};

        focus.type = obj->getType();
        focus.mat = obj->objMaterial();

        obj->getMatrices(focus.WorldToObjMat, focus.ObjToWorldRot, focus.scaleFactors);

        for (auto & loc : locs)
            loc.replace(21, 1, std::to_string(n));

        Utils::setUnif(shader, locs[0], focus.mat.specularCoeffs);
        Utils::setUnif(shader, locs[1], focus.mat.diffuseCoeffs);
        Utils::setUnif(shader, locs[2], focus.mat.ambientCoeffs);
        Utils::setUnif(shader, locs[3], focus.mat.reflectionCoeffs);
        shader.setUniform(locs[4], focus.mat.shininess);

        Utils::setUnif(shader, locs[5], focus.WorldToObjMat);
        Utils::setUnif(shader, locs[6], focus.ObjToWorldRot);
        Utils::setUnif(shader, locs[7], focus.scaleFactors);
        shader.setUniform(locs[8], focus.type);
        n++;
    }

    std::string lightlocs[] = {
        "lightsBuffer.lights[0].col.specularEmit",
        "lightsBuffer.lights[0].col.diffuseEmit",
        "lightsBuffer.lights[0].pos"};

    shader.setUniform("lightsBuffer.numLights", static_cast<int>(lights.size()));
    n = 0;
    for (auto & light : lights) {
        for (auto & loc : lightlocs)
            loc.replace(21, 1, std::to_string(n));

        Utils::setUnif(shader, lightlocs[0], light->getColor().specularColor);
        Utils::setUnif(shader, lightlocs[1], light->getColor().diffuseColor);
        Utils::setUnif(shader, lightlocs[2], Utils::fromHomoCoord(light->getPos()));
        n++;
    }
}
