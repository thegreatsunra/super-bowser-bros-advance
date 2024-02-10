#ifndef SBB_PLAYER_H
#define SBB_PLAYER_H

#include "bn_affine_bg_map_cell.h"
#include "bn_affine_bg_ptr.h"
#include "bn_camera_ptr.h"
#include "bn_fixed_point.h"
#include "bn_span.h"
#include "bn_sprite_ptr.h"

#include "sbb_level.hpp"

namespace sbb
{
    class Player
    {
        private:
            bn::sprite_ptr m_sprite;
            bn::fixed m_dx;
            bn::fixed m_dy;
            bn::fixed_point m_pos;
            bn::optional<bn::camera_ptr> m_camera;
            bn::optional<bn::span<const bn::affine_bg_map_cell>> m_map_cells;
            bn::optional<bn::affine_bg_ptr> m_map;
            void m_update_camera(int lerp);
        public:
            Player(bn::sprite_ptr m_sprite);
    void t_move_right();
    void t_move_left();

            void t_delete_data();
            void t_hide();
            void t_reset();
            void t_spawn(bn::fixed_point m_pos, bn::camera_ptr m_camera, bn::affine_bg_ptr m_map);
            void t_update_position(bn::affine_bg_ptr map, sbb::Level level);
    };
}

#endif
