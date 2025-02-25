//
// Created by mdm31 on 2/15/2025.
//

#ifndef UTILS_H
#define UTILS_H

#include "../cmake-build-debug/_deps/sfml-src/extlibs/headers/glad/include/glad/gl.h"
#include "../glm/vec3.hpp"
#include "../glm/vec4.hpp"
#include "../glm/ext/matrix_transform.hpp"
#include "SFML/Graphics/Shader.hpp"
#include "SFML/Graphics/Transform.hpp"

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

    static void setUnif(sf::Shader &shader, const std::string &name, const glm::mat3x3 &mat) {
        const float stream[] = {mat[0][0], mat[0][1], mat[0][2], mat[1][0], mat[1][1], mat[1][2], mat[2][0], mat[2][1], mat[2][2]};
        shader.setUniform(name, sf::Glsl::Mat3(stream));
    }

    static void setUnif(sf::Shader &shader, const std::string &name, const glm::mat4x4 &mat) {
        const float stream[] = {mat[0][0], mat[0][1], mat[0][2], mat[0][3], mat[1][0], mat[1][1], mat[1][2], mat[1][3], mat[2][0], mat[2][1], mat[2][2], mat[2][3], mat[3][0], mat[3][1], mat[3][2], mat[3][3]};
        shader.setUniform(name, sf::Glsl::Mat4(stream));
    }
};

#endif //UTILS_H
