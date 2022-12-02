//
// Created by ilay on 02/12/2022.
//

#ifndef GAME_PLANE_H
#define GAME_PLANE_H


#include "glm/vec4.hpp"
#include "ray.h"
#include "hittable.h"

class plane:hittable{
    plane() {}
    plane(glm::vec4 eq) : equation(eq) {};

    virtual bool hit(const ray &r, float t_min, float t_max, hit_record& rec) const override;

public:
    glm::vec4 equation;
};


#endif //GAME_PLANE_H
