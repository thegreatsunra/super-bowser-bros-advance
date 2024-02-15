#include "bn_affine_bg_map_ptr.h"
#include "bn_display.h"
#include "bn_keypad.h"
#include "bn_log.h"
#include "bn_math.h"
#include "bn_optional.h"
#include "bn_random.h"

#include "sbb_collision.hpp"
#include "sbb_enemy_type.hpp"
#include "sbb_enemy.hpp"
#include "sbb_extras.hpp"
#include "sbb_hitbox.hpp"

#include "bn_sprite_items_mario_sprite.h"

namespace sbb
{
    constexpr const bn::fixed gravity = 0.2;
    constexpr const bn::fixed wall_run_speed = 0.25;
    constexpr const bn::fixed jump_power = 4;
    constexpr const bn::fixed acc = 0.1;
    constexpr const bn::fixed max_dy = 6;
    constexpr const bn::fixed friction = 0.85;

    int jump_timer = 0;
    int time_to_jump = 180;

    bn::random random = bn::random();


    Enemy::Enemy(int x, int y, bn::camera_ptr camera, bn::affine_bg_ptr map, ENEMY_TYPE type, int hp) :
        m_camera(camera),
        m_hp(hp),
        m_level(Level(map)),
        m_map(map),
        m_pos(x, y),
        m_type(type)
    {
        m_dir = 1;
        m_map_cells = map.map().cells_ref().value();

        if (m_type == ENEMY_TYPE::MARIO) {
            m_sprite = bn::sprite_items::mario_sprite.create_sprite(m_pos.x(), m_pos.y());
            m_sprite.value().set_camera(m_camera);
            m_sprite.value().set_bg_priority(1);
            m_action = bn::create_sprite_animate_action_forever(
                           m_sprite.value(), 5, bn::sprite_items::mario_sprite.tiles_item(), 0, 1, 2, 1);
        }

        m_sprite.value().set_visible(true);
    }

    bool Enemy::t_damage_from_left(int damage)
    {
        m_dy -= 0.4;
        m_dx -= 1;
        m_dir = 1;
        m_direction_timer = 0;
        m_grounded = false;
        m_sprite.value().set_horizontal_flip(true);
        return m_take_damage(damage);
    }

    bool Enemy::t_damage_from_right(int damage)
    {
        m_dy -= 0.4;
        m_dx += 1;
        m_dir = -1;
        m_direction_timer = 0;
        m_grounded = false;
        m_sprite.value().set_horizontal_flip(false);
        return m_take_damage(damage);
    }

    void Enemy::t_set_visible(bool visiblity)
    {
        m_sprite.value().set_visible(visiblity);
    }

    bool Enemy::m_take_damage(int damage)
    {
        m_hp -= damage;

        if (m_hp <= 0) {
            // bn::sound_items::death.play();
            if (m_type == ENEMY_TYPE::MARIO) {
                m_action = bn::create_sprite_animate_action_once(
                               m_sprite.value(), 5, bn::sprite_items::mario_sprite.tiles_item(), 0, 1, 2, 1);
            }

            return true;
        }

        return false;
    }

    bool Enemy::t_is_hit(Hitbox attack)
    {
        if (!m_dead) {
            // fuck check_collisions_bb() is a mess
            // return check_collisions_bb(attack, _pos.x(), _pos.y(), 8, 8);
        } else {
            return false;
        }
    }

    bool Enemy::m_will_hit_wall()
    {
        if (sbb::hitbox_collided_with_cell(
                m_pos,
                directions::left,
                Hitbox(-4, 0, 12, 12),
                m_map,
                m_level,
                m_map_cells
            )) {
            return true;
        }

        if (sbb::hitbox_collided_with_cell(
                m_pos,
                directions::right,
                Hitbox(10, 0, 12, 12),
                m_map,
                m_level,
                m_map_cells
            )) {
            return true;
        }

        return false;
    }

    bool Enemy::m_fall_check(bn::fixed x, bn::fixed y)
    {
        if (sbb::hitbox_collided_with_cell(
                bn::fixed_point(x, y),
                directions::down,
                Hitbox(0, 16, 4, 9),
                m_map,
                m_level,
                m_map_cells
            )) {
            BN_LOG("m_fall_check true");
            return true;
        } else {
            BN_LOG("m_fall_check false");
            return false;
        }
    }

    bool Enemy::m_will_fall()
    {
        if (m_dx < 0) { // left
            if (!sbb::hitbox_collided_with_cell(
                    m_pos,
                    directions::down,
                    Hitbox(-4, 32, 4, 8),
                    m_map,
                    m_level,
                    m_map_cells
                )) {
                BN_LOG("m_will_fall true (left)");
                return true;
            }
        } else { //right
            if (!sbb::hitbox_collided_with_cell(
                    m_pos,
                    directions::down,
                    Hitbox(4, 32, 4, 8),
                    m_map,
                    m_level,
                    m_map_cells
                )) {
                BN_LOG("m_will_fall true (right)");
                return true;
            }
        }

        return false;
    }

    bn::fixed_point Enemy::t_pos()
    {
        return m_pos;
    }

    int Enemy::t_hp()
    {
        return m_hp;
    }

    void Enemy::t_set_pos(bn::fixed_point pos)
    {
        m_pos = pos;
    }

    bool Enemy::t_spotted_player()
    {
        return m_spotted_player;
    }

    ENEMY_TYPE Enemy::t_type()
    {
        return m_type;
    }

    void Enemy::t_update(bn::fixed_point player_pos)
    {
        if (!m_dead) {
            if (!m_sprite.value().visible()) {
                m_sprite.value().set_visible(true);
            }

            // dead check
            if (m_action.has_value() && m_action.value().done()) {
                m_sprite.value().set_visible(false);
                m_dead = true;
            }

            m_dy += gravity;

            if (m_type == ENEMY_TYPE::MARIO) {
                if (m_grounded && m_direction_timer > 30) {
                    if (m_will_fall() || m_will_hit_wall()) {
                        m_dx = 0;
                        m_dir = -m_dir;
                        m_direction_timer = 0;
                        m_sprite.value().set_horizontal_flip(!m_sprite.value().horizontal_flip());
                    }
                }

                if (m_type == ENEMY_TYPE::MARIO) {
                    if ((m_action.value().current_index() == 1 || m_action.value().current_index() == 3)  && m_grounded) {
                        if (m_type == ENEMY_TYPE::MARIO) {
                            m_dx += m_dir * acc;
                        } else {
                            m_dx += m_dir * acc * 1.5;
                        }

                        // 10 (or 20) is a magic number
                        // maybe bind to animate_forever value
                        // if (_sound_timer > 10) {
                        //     bn::sound_items::steps.play(0.3);
                        //     _sound_timer = 0;
                        // } else {
                        //     ++_sound_timer;
                        // }
                    }
                }
            }

            m_dx = m_dx * friction;

            if (m_dx > 0) {
                m_sprite.value().set_horizontal_flip(false);
            } else if (m_dx < 0) {
                m_sprite.value().set_horizontal_flip(true);
            }

            //fall
            if (m_dy > 0) {
                if (sbb::hitbox_collided_with_cell(
                        m_pos,
                        directions::down,
                        Hitbox(0, 16, 32, 0),
                        m_map,
                        m_level,
                        m_map_cells
                    )) {
                    m_dy = 0;
                    // BN_LOG(bn::to_string<32>(_pos.x())+" " + bn::to_string<32>(_pos.y()));
                    m_pos.set_y(m_pos.y() - modulo(m_pos.y(), 8));
                    m_grounded = true;
                } else {
                    m_grounded = false;
                }
            }

            //bounce?
            // if (bn::abs(m_dx) > 0) {
            //     if (sbb::hitbox_collided_with_cell(
            //             m_pos,
            //             directions::left,
            //             Hitbox(0, 0, 4, 8),
            //             m_map,
            //             m_level,
            //             m_map_cells
            //         )) {
            //         m_dx = -m_dx;
            //         // _direction_timer = 0;
            //     }
            // }

            //max
            if (m_dy > max_dy) {
                m_dy = max_dy;
            }

            if (m_hp >= 0) {
                m_pos.set_x(m_pos.x() + m_dx);
                m_pos.set_y(m_pos.y() + m_dy);
                m_sprite.value().set_position(m_pos);
                m_sprite.value().set_y(m_sprite.value().y() - 1);
            }

            if (m_action.has_value() && !m_action.value().done()) {
                m_action.value().update();
            }

            if (m_direction_timer < 121) {
                m_direction_timer += 1;
            }
        }
    }
}
