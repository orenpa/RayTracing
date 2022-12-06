#include "plane.h"
#include "glm/detail/func_geometric.hpp"

bool plane::hit(const ray &r, float t_min, float t_max, hit_record &rec) const {
    float denom  = glm::dot(normal,r.direction());
    if(abs(denom) > 0.000001f) {
        float t = (-(glm::dot(r.origin(), normal) + scalar)) / denom;
        rec.t = t;
        rec.point = r.origin()+ t*r.direction();
        rec.normal = normal;
        rec.mat = mat;
        return t >= 0;
    }
    return false;
    }

