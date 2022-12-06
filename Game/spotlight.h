#ifndef GAME_SPOTLIGHT_H
#define GAME_SPOTLIGHT_H


#include "light.h"

class spotlight : public light{
public:
    ray r;
    float cut_off_angle;
    spotlight(){}
    spotlight(glm::vec3 origin, glm::vec3 dir, float cut_off_angle, glm::vec3 i);
    virtual glm::vec3 get_ray (glm::vec3 point) const override;
    virtual float get_t (glm::vec3 point) const override;
};


#endif //GAME_SPOTLIGHT_H
