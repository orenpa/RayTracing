//
// Created by ilay on 02/12/2022.
//

#ifndef GAME_PLANE_H
#define GAME_PLANE_H

#include <utility>
#include "glm/vec4.hpp"
#include "ray.h"
#include "hittable.h"

class plane : public hittable {
public:
    plane() {}

//    plane(glm::vec4 eq) : equation(eq) {};

    plane(glm::vec3 normal, float scalar, material m) : normal(normal), scalar(scalar) {
        mat = std::move(m);
    };


    virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const override;

public:
//    glm::vec4 equation;
    glm::vec3 normal;
    float scalar;
};


#endif //GAME_PLANE_H
