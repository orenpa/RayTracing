#include "spotlight.h"

spotlight::spotlight(glm::vec3 origin, glm::vec3 dir, float cut_off_angle) {
    this->r = ray(origin, dir);
    this->cut_off_angle = cut_off_angle;
}

float spotlight::get_t(glm::vec3 point) const {
    return 0;
}

glm::vec3 spotlight::get_ray(glm::vec3 point) const {
    return glm::vec3();
}
