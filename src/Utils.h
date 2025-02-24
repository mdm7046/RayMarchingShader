//
// Created by mdm31 on 2/15/2025.
//

#ifndef UTILS_H
#define UTILS_H

#include "../glm/vec3.hpp"
#include "../glm/vec4.hpp"
#include "../glm/ext/matrix_transform.hpp"
#include "SFML/Graphics/Shader.hpp"

class Utils {
public:
    static glm::vec3 fromHomoCoord(const glm::vec4 &vec) {
        return glm::vec3(vec);
    }

    static glm::vec4 toHomoCoord(const glm::vec3 &vec) {
        return glm::vec4(vec, 1.);
    }

    //calculate full rotation matrix around all three axis, and multiply it by base
    static glm::mat4x4 rotate(glm::mat4x4 base, const glm::vec3 &rot) {
        base = glm::rotate(base, rot.z, {0, 0, 1});
        base = glm::rotate(base, rot.y, {0, 1, 0});
        return glm::rotate(base, rot.x, {1, 0, 0});
    }

    //calculate inverse of full rotation matrix around all three axis, and multiply it by base
    static glm::mat4x4 rotateInverse(glm::mat4x4 base, const glm::vec3 &rot) {
        base = glm::rotate(base, -rot.x, {1, 0, 0});
        base = glm::rotate(base, -rot.y, {0, 1, 0});
        return glm::rotate(base, -rot.z, {0, 0, 1});
    }

    static void setUnif(sf::Shader &shader, const std::string &name, const glm::vec4 &vec) {
        shader.setUniform(name, sf::Glsl::Vec4(vec.x, vec.y, vec.z, vec.w));
    }

    static void setUnif(sf::Shader &shader, const std::string &name, const glm::vec3 &vec) {
        shader.setUniform(name, sf::Glsl::Vec3(vec.x, vec.y, vec.z));
    }
};

#endif //UTILS_H
