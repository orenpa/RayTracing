//
// Created by ilay on 02/12/2022.
//

#include "sphere.h"
#include "glm/detail/func_geometric.hpp"

bool sphere::hit(const ray &r, float t_min, float t_max, hit_record& rec) const {
    glm::vec3 sphere_center = r.origin() - center;
    float a = glm::dot(r.direction(), r.direction());
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
    rec.mat = mat;

    return true;
}