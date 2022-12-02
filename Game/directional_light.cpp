#include "directional_light.h"

glm::vec3 directional_light::get_ray(glm::vec3 point) const {
        return (-1.0f) * direction;
}
