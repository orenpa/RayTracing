#include <iostream>
#include "light.h"

//// implemented according to https://en.wikipedia.org/wiki/Phong_reflection_model
glm::vec3 light::get_illumination(ray camera, hit_record obj) const {
    glm::vec3 L = glm::normalize(get_ray(obj.point));
    glm::vec3 V = glm::normalize(camera.dir * (-1.0f));
    glm::vec3 N = glm::normalize(obj.normal);
    glm::vec3 R = glm::normalize(2.0f * glm::dot(N, L) * N - L); //https://www.fabrizioduroni.it/2017/08/25/how-to-calculate-reflection-vector/
    return (obj.mat.Kd * glm::dot(L, N) + Ks * std::fmax(0.0f, ((float)std::pow((float)glm::dot(R, V),1)))) * base_illumination;
}