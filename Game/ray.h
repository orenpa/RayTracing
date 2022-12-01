//
// Created by orenp on 11/30/2022.
//

#ifndef GAME_RAY_H
#define GAME_RAY_H

#include "glfw/deps/linmath.h"
#include "glm/vec3.hpp"

class ray {
public:
    ray() {}
    ray(const glm::vec3& origin, const glm::vec3& direction)
            : orig(origin), dir(direction)
    {}

    glm::vec3 origin() const  { return orig; }
    glm::vec3 direction() const { return dir; }

    glm::vec3 at(float t) const {
        return orig + t*dir;
    }

public:
    glm::vec3 orig;
    glm::vec3 dir;
};

#endif //GAME_RAY_H
