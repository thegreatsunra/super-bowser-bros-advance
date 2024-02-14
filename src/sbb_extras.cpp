#include "bn_affine_bg_map_cell.h"
#include "bn_affine_bg_ptr.h"
#include "bn_fixed.h"
#include "bn_fixed_point.h"
#include "bn_size.h"
#include "bn_span.h"
#include "bn_vector.h"

#include "sbb_hitbox.hpp"
#include "sbb_level.hpp"
#include "sbb_extras.hpp"

namespace sbb
{
    int get_map_cell(bn::fixed x, bn::fixed y, bn::affine_bg_ptr &map, bn::span<const bn::affine_bg_map_cell> cells)
    {
        int map_size = map.dimensions().width();
        int cell = modulo((y.safe_division(8).right_shift_integer() * map_size / 8 + x / 8), map_size * 8).integer();
        return cells.at(cell);
    }

    bool contains_cell(int tile, bn::vector<int, 32> tiles)
    {
        for (int index = 0; index < tiles.size(); ++index) {
            if (tiles.at(index) == tile) {
                return true;
            }
        }

        return false;
    }

    bool hitbox_collided_with_cell(bn::fixed_point pos, directions direction, Hitbox hitbox, bn::affine_bg_ptr &map, sbb::Level level, bn::span<const bn::affine_bg_map_cell> cells)
    {
        bn::fixed l = pos.x() - (hitbox.t_width() / 2 - hitbox.t_x());
        bn::fixed r = pos.x() + (hitbox.t_width() / 2 - hitbox.t_x());
        bn::fixed u = pos.y() - (hitbox.t_height() / 2 - hitbox.t_y());
        bn::fixed d = pos.y() + (hitbox.t_height() / 2 - hitbox.t_y());
        bn::vector<int, 32> tiles;

        if (direction == down) {
            tiles = level.t_floor_tiles();
        } else if (direction == left || direction == right) {
            tiles = level.t_wall_tiles();
        } else if (direction == up) {
            tiles = level.t_ceil_tiles();
        }

        if (contains_cell(get_map_cell(l, u, map, cells), tiles) ||
            contains_cell(get_map_cell(l, d, map, cells), tiles) ||
            contains_cell(get_map_cell(r, u, map, cells), tiles) ||
            contains_cell(get_map_cell(r, d, map, cells), tiles)) {
            return true;
        } else {
            return false;
        }
    }
}
