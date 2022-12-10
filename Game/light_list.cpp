#include <iostream>
#include "light_list.h"

glm::vec3 light_list::get_illumination(ray camera, hit_record start, hittable& world) const {
    hit_record rec;
    camera.dir = glm::normalize(camera.dir);
    glm::vec3 illumination = glm::vec3(0.0f,0.0f,0.0f);
    for (const auto& light_source : light_sources){
        ray r = ray(start.point, light_source->get_ray(start.point));
        if(world.hit(r, 0.001f, light_source->get_t(start.point), rec)){
            continue;
        } else illumination += light_source->get_illumination(camera, start);
    }
    return illumination + Ia * start.mat.Kd;
}
