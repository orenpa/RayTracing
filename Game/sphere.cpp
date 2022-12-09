//
// Created by ilay on 02/12/2022.
//

#include <iostream>
#include "sphere.h"
#include "glm/detail/func_geometric.hpp"

float get_norm(glm::vec3 e){
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
}

bool sphere::hit(const ray &r, float t_min, float t_max, hit_record& rec) const {
    glm::vec3 sphere_center = r.origin() - center;
    float a = get_norm(r.direction());
    auto half_b = glm::dot(sphere_center, r.direction());
    float c = glm::dot(sphere_center, sphere_center) - radius * radius;
    float discriminant = half_b * half_b - a * c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.point = r.at(rec.t);
    rec.normal = (rec.point - center) / radius;
//    glm::vec3 point = rec.t;

//    std::cout << "r: \n";
//    std::cout << r.direction().x;
//    std::cout << "\n";
//    std::cout << r.direction().y;
//    std::cout << "\n";
//    std::cout << r.direction().z;
//    std::cout << "\n";
    rec.mat = mat;

    return true;
}