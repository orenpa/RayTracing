#include <iostream>
#include "spotlight.h"

spotlight::spotlight(glm::vec3 origin, glm::vec3 dir, float cut_off_angle, glm::vec3 i) {
    this->r = ray(origin, glm::normalize(dir));
    this->cut_off_angle = cut_off_angle;
    this->base_illumination = i;
}

float spotlight::get_t(glm::vec3 point) const {
    glm::vec3 dir_to_point = point - r.origin();
    dir_to_point = glm::normalize(dir_to_point);
    return (r.origin().x - point.x)/dir_to_point.x;
}

glm::vec3 spotlight::get_ray(glm::vec3 point) const {
    glm::vec3 dir_to_point = point - r.origin();
    dir_to_point = glm::normalize(dir_to_point);
    float degree = std::acos(glm::clamp(glm::dot(dir_to_point, r.direction()), -0.999f,.9999f));
    if(degree > cut_off_angle)
        return glm::vec3(0.0f, 0.0f, 0.0f);
    else return dir_to_point * -1.f;
}
