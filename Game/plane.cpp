
#include <algorithm>
#include "plane.h"
#include "glm/detail/func_geometric.hpp"

bool plane::hit(const ray &r, float t_min, float t_max, hit_record &rec) const {
    float denom = glm::dot(normal, r.direction());
    if (abs(denom) > 0.000001f) {
        float t = (-(glm::dot(r.origin(), normal) + scalar)) / denom;
        rec.t = t;
        rec.point = r.origin() + (t - 0.000001f) * r.direction();
        rec.normal = normal * -1.0f;
//        rec.mat = mat;
        glm::vec3 projected_point = rec.point - (glm::dot(rec.normal, rec.point) * rec.normal);
        if (((int)(std::floor(projected_point.x) + std::floor(projected_point.y)) % 2) == 0)
            rec.mat.Kd = 0.5f;
        else
            rec.mat = mat;
        return t >= 0;
    }
    return false;
}

// Define the normal vector of the plane
//Vector3 planeNormal = Vector3(0.0, 0.0, 1.0);

// Define the point that you want to project
//Vector3 point = Vector3(1.0, 1.0, 1.0);

// Calculate the dot product of the normal vector and the point
//float dotProduct = planeNormal.dot(point);

// Subtract the distance from the point to get the projected point
//Vector3 projectedPoint = point - dotProduct * planeNormal;

