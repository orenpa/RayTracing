
#ifndef GAME_SPHERE_H
#define GAME_SPHERE_H



#include <utility>

#include "hittable.h"
#include <iostream>
class sphere : public hittable {
public:
    sphere() {}
    sphere(glm::vec3 cen, float r, material m) : center(cen), radius(r){
        mat = std::move(m);
        std::cout << cen.x << " " << cen.y << " " << cen.z << "\n";
        std::cout << r << "\n";
    };

    virtual bool hit(const ray &r, float t_min, float t_max, hit_record& rec) const override;

public:
    glm::vec3 center;
    float radius;
};

#endif //GAME_SPHERE_H
