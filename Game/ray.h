//
// Created by orenp on 11/30/2022.
//

#ifndef GAME_RAY_H
#define GAME_RAY_H

#include "glfw/deps/linmath.h"
#include "glm/vec4.hpp"

class ray {
public:
    ray() {}
    ray(const glm::vec4& origin, const glm::vec4& direction)
            : orig(origin), dir(direction)
    {}

    glm::vec4 origin() const  { return orig; }
    glm::vec4 direction() const { return dir; }

    glm::vec4 at(float t) const {
        return orig + t*dir;
    }

public:
    glm::vec4 orig;
    glm::vec4 dir;
};

#endif //GAME_RAY_H
