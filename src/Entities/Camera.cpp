//
// Created by mdm31 on 2/22/2025.
//

#include "Camera.h"

#include "../../cmake-build-debug/_deps/sfml-src/extlibs/headers/glad/include/glad/gl.h"

using namespace glm;

void Camera::calcImagePlaneBases() {
    const double h = nearClipDist * 2 * glm::tan(FOV / 2);
    clipPlaneBottomLeft = {-(h*windowWidth)/(2*windowHeight), -h/2, nearClipDist, 1};
    stepY = {0, h / windowHeight, 0, 0};
    stepX = {stepY.y, 0, 0, 0};

    //Rotate then translate (T * R)
    mat4x4 CamToWorldMatrix = translate(glm::identity<glm::mat4x4>(), Utils::fromHomoCoord(pos));
    CamToWorldMatrix = Utils::rotate(CamToWorldMatrix, rot);
    clipPlaneBottomLeft = CamToWorldMatrix * clipPlaneBottomLeft;
    stepX = CamToWorldMatrix * stepX;
    stepY = CamToWorldMatrix * stepY;
}

void Camera::setShaderUniforms(sf::Shader &shader) const {
    struct shaderstruct_world {
        vec3 sunDir;
        Light::LightColor sunColor;

        vec3 ambientLight;

        vec3 skyColor, groundColor;
    } wrld{};

    world->getSunInfo(wrld.sunColor, wrld.sunDir, wrld.ambientLight, wrld.skyColor, wrld.groundColor);

    //set world uniforms
    Utils::setUnif(shader, "sunDir", wrld.sunDir);
    Utils::setUnif(shader, "ambientLight", wrld.ambientLight);
    Utils::setUnif(shader, "skyColor", wrld.skyColor);
    Utils::setUnif(shader, "groundColor", wrld.groundColor);
    Utils::setUnif(shader, "sunColor.specularEmit", wrld.sunColor.specularColor);
    Utils::setUnif(shader, "sunColor.diffuseEmit", wrld.sunColor.diffuseColor);

    //set camera uniforms
    Utils::setUnif(shader, "imgPlaneBottomLeft", clipPlaneBottomLeft);
    Utils::setUnif(shader, "centerOfProjection", pos);
    Utils::setUnif(shader, "stepX", stepX);
    Utils::setUnif(shader, "stepY", stepY);

    //create buffers
    
    world->setShaderObjectsBuffer(shader);
}
