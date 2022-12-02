//
// Created by ilay on 02/12/2022.
//

#ifndef GAME_EYE_H
#define GAME_EYE_H


#include "ray.h"

class eye {
public:
    glm::vec3 origin;
    eye() {
        auto viewport_height = 2.0f;
        auto viewport_width =  viewport_height;
        auto focal_length = 4.0f;

        origin = glm::vec3(0, 0, 0);
        horizontal = glm::vec3(viewport_width, 0.0f, 0.0f);
        vertical = glm::vec3(0.0f, viewport_height, 0.0f);
        lower_left_corner = origin - horizontal/2.0f - vertical/2.0f - glm::vec3(0.0f, 0.0f, focal_length);
    }

    ray get_ray(float u, float v) const {
        return ray(origin, lower_left_corner + u*horizontal + v*vertical - origin);
    }

private:
    glm::vec3 lower_left_corner;
    glm::vec3 horizontal;
    glm::vec3 vertical;
};
#endif //GAME_RAY_H
