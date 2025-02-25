//
// Created by mdm31 on 2/14/2025.
// Provides methods for creating a camera, given a World object
//

#ifndef CAMERA_H
#define CAMERA_H

#include "World.h"

#include "../../glm/vec3.hpp"
#include "../../glm/vec4.hpp"

inline extern const int windowWidth = 300;
inline extern const int windowHeight = 300;

class Camera {
private:
    World *world;

    glm::vec4 clipPlaneBottomLeft, stepX, stepY;

    glm::vec4 pos;
    glm::vec3 rot;
    float nearClipDist, FOV;

    // recalculate clipPlaneBottomLeft, stepX, & stepY from current pos, rot, FOV, & dist - must be called after each transform
    void calcImagePlaneBases();
public:
    Camera(const glm::vec3 &pos, const glm::vec3 &rot, float clipPlaneDist, float FOV, World* world)
        : world(world), pos(Utils::toHomoCoord(pos)), rot(rot), nearClipDist(clipPlaneDist), FOV(FOV) { calcImagePlaneBases(); }
    ~Camera() = default;

    void transform(const glm::vec3 &translate, const glm::dvec3 &rotate) {
        pos += glm::vec4(translate, 0.f);
        rot += rotate;
        calcImagePlaneBases();
    }
    void set(const glm::dvec3 &position, const glm::dvec3 &rotation) {
        pos = glm::vec4(position, 0.f);
        rot = rotation;
        calcImagePlaneBases();
    }
    void changeParams(const float &FOV, const float &nearClipPlane) {
        this->FOV = FOV;
        this->nearClipDist = nearClipPlane;
        calcImagePlaneBases();
    }

    void setShaderUniforms(sf::Shader &shader) const;
};

#endif //CAMERA_H
