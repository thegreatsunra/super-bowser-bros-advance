#ifndef SBB_PLAYER_H
#define SBB_PLAYER_H

#include "bn_affine_bg_map_cell.h"
#include "bn_affine_bg_ptr.h"
#include "bn_camera_ptr.h"
#include "bn_fixed_point.h"
#include "bn_span.h"
#include "bn_sprite_ptr.h"

#include "sbb_hitbox.hpp"
#include "sbb_level.hpp"

namespace sbb
{
    class Player
    {
        private:
            bn::optional<bn::camera_ptr> m_camera;
            bn::fixed m_dx;
            bn::fixed m_dy;
            //used for state management
            bool m_is_jumping = false;
            bool m_is_falling = false;
            bool m_is_running = false;
            bool m_is_grounded = false;
            bool m_is_sliding = false;
            bool m_is_already_running = false;
            bn::optional<bn::affine_bg_ptr> m_map;
            bn::optional<bn::span<const bn::affine_bg_map_cell>> m_map_cells;
            bn::fixed_point m_pos;
            bn::sprite_ptr m_sprite;
            void m_update_camera(int lerp);
            sbb::Hitbox m_hitbox_fall = Hitbox(0, 0, 64, 64);
            sbb::Hitbox m_hitbox_left = Hitbox(0, 0, 64, 64);
            sbb::Hitbox m_hitbox_right = Hitbox(0, 0, 64, 64);
            sbb::Hitbox m_hitbox_jump = Hitbox(0, 0, 64, 64);

        public:
            Player(bn::sprite_ptr m_sprite);
            void t_collide_with_objects(bn::affine_bg_ptr map, sbb::Level level);
            void t_delete_data();
            void t_hide();
            void t_jump();
            bool t_is_right();
            void t_move_left();
            void t_move_right();
            [[nodiscard]] bn::fixed_point t_pos();
            void t_reset();
            void t_spawn(bn::fixed_point m_pos, bn::camera_ptr m_camera, bn::affine_bg_ptr m_map);
            void t_update_position(bn::affine_bg_ptr map, sbb::Level level);
    };
}

#endif
