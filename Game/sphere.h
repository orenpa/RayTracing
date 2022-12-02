//
// Created by ilay on 02/12/2022.
//

#ifndef GAME_SPHERE_H
#define GAME_SPHERE_H



#include "hittable.h"

class sphere : public hittable {
public:
    sphere() {}
    sphere(glm::vec3 cen, double r) : center(cen), radius(r) {};

    virtual bool hit(const ray &r, float t_min, float t_max, hit_record& rec) const override;

public:
    glm::vec3 center;
    float radius;
};

#endif //GAME_SPHERE_H
