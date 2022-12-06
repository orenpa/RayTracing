#include "light.h"

//// implemented according to https://en.wikipedia.org/wiki/Phong_reflection_model
glm::vec3 light::get_illumination(ray camera, hit_record obj) const {
    glm::vec3 L = get_ray(obj.point) * (-1.0f);
    glm::vec3 V = camera.dir * (-1.0f);
    glm::vec3 N = obj.normal;
    glm::vec3 R = 2.0f * (N * L) * N - L; //https://www.fabrizioduroni.it/2017/08/25/how-to-calculate-reflection-vector/
    return (Kd * glm::dot(L, N) + Ks * std::fmax(0.0f, ((float)std::pow(glm::dot(R, V),1)))) * base_illumination;
}

