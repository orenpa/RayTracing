//
// Created by ilay on 02/12/2022.
//

#ifndef GAME_HITTABLE_H
#define GAME_HITTABLE_H

#include "ray.h"

struct material{
    material()= default;;
    explicit material(glm::vec3 color, float r, float transparency, float shininess) {
        base_color = color * 256.f;
        reflective = r;
        transperancy = transparency;
        Kd = 0.7f;
        this->shininess = shininess;
    }
//    explicit material(glm::vec3 color, float r, float transparency) {
//        base_color = color;
//        reflective = r;
//        transperancy = transparency;
//        Kd = 0.7f;
//        this->shininess = 3.f;
//    }
    float reflective;
    float transperancy;
    glm::vec3 base_color;
    float Kd;
    float shininess;
};

struct hit_record {
    glm::vec3 point;
    glm::vec3 normal;
    float t;
    material mat;
};

class hittable {
public:
    material mat;
    hittable()= default;
    virtual bool hit (const ray &r, float t_min, float t_max, hit_record& rec) const = 0;
};

#endif //GAME_HITTABLE_H
