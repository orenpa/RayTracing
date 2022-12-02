//
// Created by ilay on 02/12/2022.
//

#ifndef GAME_SPHERE_H
#define GAME_SPHERE_H



#include <utility>

#include "hittable.h"

class sphere : public hittable {
public:
    sphere() {}
    sphere(glm::vec3 cen, float r) : center(cen), radius(r) { this->mat = material(glm::vec3(80.0f,186.0f,186.0f), 0.1f);};
    sphere(glm::vec3 cen, float r, material m) : center(cen), radius(r){ mat = std::move(m);};

    virtual bool hit(const ray &r, float t_min, float t_max, hit_record& rec) const override;

public:
    glm::vec3 center;
    float radius;
};

#endif //GAME_SPHERE_H
