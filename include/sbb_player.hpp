#ifndef SBB_PLAYER_H
#define SBB_PLAYER_H

#include "bn_affine_bg_map_cell.h"
#include "bn_affine_bg_ptr.h"
#include "bn_camera_ptr.h"
#include "bn_fixed_point.h"
#include "bn_span.h"
#include "bn_sprite_animate_actions.h"
#include "bn_sprite_ptr.h"

#include "sbb_enemy.hpp"
#include "sbb_hitbox.hpp"
#include "sbb_level.hpp"

#include "bn_sprite_items_bowser_sprite.h"

namespace sbb
{
    class Player
    {
        private:
            bn::optional<bn::camera_ptr> m_camera;
            bn::fixed m_dx;
            bn::fixed m_dy;
            bn::optional<bn::vector<Enemy, 16>*> m_enemies;
            bool m_is_jumping = false;
            bool m_is_falling = false;
            bool m_is_running = false;
            bool m_is_grounded = false;
            bool m_is_sliding = false;
            bn::optional<bn::affine_bg_ptr> m_map;
            bn::optional<bn::span<const bn::affine_bg_map_cell>> m_map_cells;
            bn::fixed_point m_pos;
            bn::sprite_ptr m_sprite;
            void m_update_camera(int lerp);
            // for m_hitbox_fall Hitbox(-15, 0, 44, 64) works but I don't know why
            // for m_hitbox_fall Hitbox(0, 0, 10, 64) also works but I don't know why 
            sbb::Hitbox m_hitbox_fall = Hitbox(20, 0, 44, 64);
            sbb::Hitbox m_hitbox_left = Hitbox(20, 0, 44, 64);
            sbb::Hitbox m_hitbox_right = Hitbox(20, 0, 44, 64);
            sbb::Hitbox m_hitbox_jump = Hitbox(12, 0, 0, 64);
            bn::sprite_animate_action<4> m_action = bn::create_sprite_animate_action_forever(
                    m_sprite, 30, bn::sprite_items::bowser_sprite.tiles_item(), 0, 0, 0, 0);

        public:
            Player(bn::sprite_ptr m_sprite);
            void t_apply_animation_state();
            void t_collide_with_enemies();
            void t_collide_with_objects(
                sbb::Level level,
                bn::affine_bg_ptr map
            );
            void t_delete_data();
            void t_hide();
            void t_jump();
            bool t_is_right();
            void t_move_left();
            void t_move_right();
            [[nodiscard]] bn::fixed_point t_pos();
            void t_reset();
            void t_spawn(
                bn::camera_ptr m_camera,
                bn::vector<Enemy, 16> &m_enemies,
                bn::affine_bg_ptr m_map,
                bn::fixed_point m_pos
            );
            void t_update_position(bn::affine_bg_ptr map, sbb::Level level);
    };
}

#endif
