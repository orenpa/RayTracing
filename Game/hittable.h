//
// Created by ilay on 02/12/2022.
//

#ifndef GAME_HITTABLE_H
#define GAME_HITTABLE_H

#include "ray.h"

struct hit_record {
    glm::vec3 p;
    glm::vec3 normal;
    float t;
};

class hittable {
public:
    virtual bool hit (const ray &r, float t_min, float t_max, hit_record& rec) const = 0;
};

#endif //GAME_HITTABLE_H
