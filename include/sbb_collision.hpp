#ifndef SBB_COLLISION_H
#define SBB_COLLISION_H

#include "bn_fixed_point.h"

#include "sbb_hitbox.hpp"

namespace sbb
{
    bool check_collisions(Hitbox boxA, Hitbox boxB);

    bool check_collisions(Hitbox boxA, bn::fixed x, bn::fixed y, bn::fixed w, bn::fixed h);
}

#endif
