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
    enum directions {up, down, left, right};

    [[nodiscard]] int _get_map_cell(bn::fixed x, bn::fixed y, bn::affine_bg_ptr &map, bn::span<const bn::affine_bg_map_cell> cells)
    {
        int map_size = map.dimensions().width();
        int cell =  modulo((y.safe_division(8).right_shift_integer() * map_size / 8 + x / 8), map_size * 8).integer();
        return cells.at(cell);
    }

    [[nodiscard]] bool _contains_cell(int tile, bn::vector<int, 32> tiles)
    {
        for (int index = 0; index < tiles.size(); ++index) {
            if (tiles.at(index) == tile) {
                return true;
            }
        }

        return false;
    }

    [[nodiscard]] bool _check_collisions_map(bn::fixed_point pos, Hitbox hitbox, directions direction, bn::affine_bg_ptr &map, sbb::Level level, bn::span<const bn::affine_bg_map_cell> cells)
    {
        bn::fixed l = pos.x() - hitbox.t_width() / 2 + hitbox.t_x();
        bn::fixed r = pos.x() + hitbox.t_width() / 2 + hitbox.t_x();
        bn::fixed u = pos.y() - hitbox.t_height() / 2 + hitbox.t_y();
        bn::fixed d = pos.y() + hitbox.t_height() / 2 + hitbox.t_y();
        bn::vector<int, 32> tiles;

        if (direction == down) {
            tiles = level.t_floor_tiles();
        } else if (direction == left || direction == right) {
            tiles = level.t_wall_tiles();
        } else if (direction == up) {
            tiles = level.t_ceil_tiles();
        }

        if (_contains_cell(_get_map_cell(l, u, map, cells), tiles) ||
            _contains_cell(_get_map_cell(l, d, map, cells), tiles) ||
            _contains_cell(_get_map_cell(r, u, map, cells), tiles) ||
            _contains_cell(_get_map_cell(l, d, map, cells), tiles)) {
            return true;
        } else {
            return false;
        }
    }

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
                           m_sprite.value(), 10, bn::sprite_items::mario_sprite.tiles_item(), 0, 1, 0, 1);
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
}
