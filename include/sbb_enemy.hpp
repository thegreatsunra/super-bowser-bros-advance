#ifndef SBB_ENEMY_H
#define SBB_ENEMY_H

#include "bn_affine_bg_map_cell.h"
#include "bn_affine_bg_ptr.h"
#include "bn_camera_ptr.h"
#include "bn_fixed_point.h"
#include "bn_sprite_animate_actions.h"
#include "bn_sprite_ptr.h"

#include "sbb_enemy_type.hpp"
#include "sbb_hitbox.hpp"
#include "sbb_level.hpp"

namespace sbb {
class Enemy {
private:
    bn::optional<bn::sprite_animate_action<4>> m_action;
    bn::camera_ptr m_camera;
    bool m_dead = false;
    int m_dir;
    int m_direction_timer = 0;
    bn::fixed m_dy = 0;
    bn::fixed m_dx = 0;
    bool m_fall_check(bn::fixed x, bn::fixed y);
    bool m_grounded = false;
    int m_hp;
    int m_inv_timer = 0;
    Level m_level;
    bn::affine_bg_ptr m_map;
    bn::span<const bn::affine_bg_map_cell> m_map_cells;
    bn::fixed_point m_pos;
    int m_sound_timer = 0;
    bool m_spotted_player = false;
    bn::optional<bn::sprite_ptr> m_sprite;
    bool m_stunned = false;
    bool m_take_damage(int damage);
    bn::fixed_point m_target = bn::fixed_point(0, 0);
    bool m_target_locked = false;
    ENEMY_TYPE m_type;
    bool m_will_hit_wall();
    bool m_will_fall();

public:
    Enemy(int x, int y, bn::camera_ptr camera, bn::affine_bg_ptr map, ENEMY_TYPE type, int hp);

    bool t_damage_from_left(int damage);
    bool t_damage_from_right(int damage);
    int t_hp();
    bn::fixed_point pos();
    bool t_is_hit(Hitbox attack);
    void t_set_pos(bn::fixed_point pos);
    void t_set_visible(bool visibility);
    bool t_spotted_player();
    ENEMY_TYPE t_type();
    void t_update(bn::fixed_point player_pos);
};
}

#endif
