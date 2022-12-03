//
// Created by banan on 03/12/2022.
//

#ifndef GAME_LIGHT_LIST_H
#define GAME_LIGHT_LIST_H


#include <memory>
#include <vector>
using std::shared_ptr;
using std::make_shared;

#include "light.h"
#include "hittable_list.h"

class light_list {
public:
    void add(shared_ptr<light> light_source) { light_sources.push_back(light_source); }
    void clear() { light_sources.clear(); }
    glm::vec3 get_illumination(ray camera, hit_record start, hittable& world) const;
private:
    std::vector<shared_ptr<light>> light_sources;
};


#endif //GAME_LIGHT_LIST_H
