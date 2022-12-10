#include "directional_light.h"
const float infinity = std::numeric_limits<float>::infinity();
glm::vec3 directional_light::get_ray(glm::vec3 point) const {
        return glm::normalize(direction) * -1.f;
}

float directional_light::get_t(glm::vec3 point) const {
    return infinity;
}
