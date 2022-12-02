//
// Created by banan on 02/12/2022.
//

#ifndef GAME_DIRECTIONAL_LIGHT_H
#define GAME_DIRECTIONAL_LIGHT_H


#include "light.h"
#include "glm/detail/func_geometric.hpp"

class directional_light :  public light{
public:
    glm::vec3 direction;
    directional_light() {}
    explicit directional_light(const glm::vec3& dir, glm::vec3 i) {
        direction = glm::normalize(dir);
        base_illumination = i;
    };

    virtual glm::vec3 get_ray (glm::vec3 point) const override;

};


#endif //GAME_DIRECTIONAL_LIGHT_H
