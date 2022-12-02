//
// Created by ilay on 02/12/2022.
//

#ifndef GAME_HITTABLE_LIST_H
#define GAME_HITTABLE_LIST_H


#include "hittable.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable {
public:
    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) { add(object); }

    void clear() { objects.clear(); }
    void add(shared_ptr<hittable> object) { objects.push_back(object); }

    virtual bool hit(const ray &r, float t_min, float t_max, hit_record& rec) const override;

public:
    std::vector<shared_ptr<hittable>> objects;
};


#endif //GAME_HITTABLE_LIST_H
