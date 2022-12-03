#ifndef GAME_LIGHT_H
#define GAME_LIGHT_H

#include "ray.h"
#include "eye.h"
#include "hittable.h"
#include "glm/detail/func_geometric.hpp"
static float Kd = 0.7f;
static float Ks = 0.3f;
static float Ka = 0.7f; // ambient coefficient

class light {
public:
    light() = default;;
    // returns the ray that comes out of the point toward the light source
    virtual glm::vec3 get_ray (glm::vec3 point) const = 0;
    // like t in hit record, to tell if the light is visible
    virtual float get_t (glm::vec3 point) const = 0;

    // calculates phong's reflection
    virtual glm::vec3 get_illumination(ray camera, hit_record obj) const;

    glm::vec3 base_illumination; // has rgb values
};

#endif //GAME_LIGHT_H
