//
// Created by banan on 03/12/2022.
//

#include "light_list.h"

glm::vec3 light_list::get_illumination(ray camera, hit_record start, hittable& world) const {
    hit_record rec;
    glm::vec3 illumination = glm::vec3(0,0,0);
    for (const auto& light_source : light_sources){
        ray r = ray(start.point, light_source->get_ray(start.point));
        if(world.hit(r, 0, light_source->get_t(start.point), rec)){
            continue;
        } else illumination += light_source->get_illumination(camera, start);
    }
    return illumination;
}
