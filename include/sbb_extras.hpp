#ifndef SBB_EXTRAS_H
#define SBB_EXTRAS_H

#include "bn_affine_bg_map_cell.h"
#include "bn_affine_bg_map_ptr.h"
#include "bn_affine_bg_ptr.h"
#include "bn_fixed.h"
#include "bn_fixed_point.h"
#include "bn_span.h"

#include "sbb_hitbox.hpp"
#include "sbb_level.hpp"

namespace sbb
{
    bool contains_cell(int tile, bn::vector<int, 32> tiles);
    enum directions {up, down, left, right};
    int get_map_cell(bn::fixed x, bn::fixed y, bn::affine_bg_ptr &map, bn::span<const bn::affine_bg_map_cell> cells);
    bool hitbox_collided_with_cell(bn::fixed_point pos, directions direction, Hitbox hitbox, bn::affine_bg_ptr &map, Level level, bn::span<const bn::affine_bg_map_cell> cells);
    constexpr bn::fixed modulo(bn::fixed a, bn::fixed m)
    {
        return a - m * ((a / m).integer());
    }
}

#endif
