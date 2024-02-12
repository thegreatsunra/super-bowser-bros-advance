#include "bn_affine_bg_map_ptr.h"
#include "bn_affine_bg_ptr.h"
#include "bn_camera_ptr.h"
#include "bn_fixed_point.h"
#include "bn_keypad.h"
#include "bn_log.h"
#include "bn_math.h"
#include "bn_sprite_ptr.h"

#include "bn_affine_bg_items_path.h"

#include "sbb_extras.hpp"
#include "sbb_hitbox.hpp"
#include "sbb_level.hpp"
#include "sbb_player.hpp"

namespace sbb
{
    enum directions {up, down, left, right};

    [[nodiscard]] int get_map_cell(bn::fixed x, bn::fixed y, bn::affine_bg_ptr &map, bn::span<const bn::affine_bg_map_cell> cells)
    {
        int map_size = map.dimensions().width();
        int cell =  modulo((y.safe_division(8).right_shift_integer() * map_size / 8 + x / 8), map_size * 8).integer();
        return cells.at(cell);
    }

    [[nodiscard]] bool contains_cell(int tile, bn::vector<int, 32> tiles)
    {
        for (int index = 0; index < tiles.size(); ++index) {
            if (tiles.at(index) == tile) {
                return true;
            }
        }

        return false;
    }

    [[nodiscard]] bool check_collisions_map(bn::fixed_point pos, directions direction, Hitbox hitbox, bn::affine_bg_ptr &map, sbb::Level level, bn::span<const bn::affine_bg_map_cell> cells)
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

        if (contains_cell(get_map_cell(l, u, map, cells), tiles) ||
            contains_cell(get_map_cell(l, d, map, cells), tiles) ||
            contains_cell(get_map_cell(r, u, map, cells), tiles) ||
            contains_cell(get_map_cell(l, d, map, cells), tiles)) {
            return true;
        } else {
            return false;
        }
    }

    constexpr const bn::fixed ACC = 0.4;
    constexpr const bn::fixed FRICTION = 0.85;
    constexpr const bn::fixed GRAVITY = 0.2;
    constexpr const bn::fixed JUMP_POWER = 4;
    constexpr const bn::fixed MAX_DY = 6;
    constexpr const bn::fixed MAX_DY = 4;

    Player::Player(bn::sprite_ptr sprite) :
        m_camera(bn::camera_ptr::create(0, 0)),
        m_map(bn::affine_bg_items::path.create_bg(0, 0)),
        m_sprite(sprite)
    {
        m_map.value().set_visible(false);
        m_sprite.put_above();
        m_sprite.set_visible(true);
    }

    void Player::t_apply_animation_state()
    {
        if (m_is_jumping) {
            m_action = bn::create_sprite_animate_action_forever(
                           m_sprite, 30, bn::sprite_items::bowser_sprite.tiles_item(), 4, 4, 4, 4);
        } else if (m_is_falling) {
            m_action = bn::create_sprite_animate_action_forever(
                           m_sprite, 30, bn::sprite_items::bowser_sprite.tiles_item(), 3, 3, 3, 3);
        } else if (m_is_sliding) {
            m_action = bn::create_sprite_animate_action_forever(
                           m_sprite, 30, bn::sprite_items::bowser_sprite.tiles_item(), 4, 4, 4, 4);
        } else if (m_is_running) {
            // checks if animation state is already "running" and if so, avoids "thrashing" the m_action value
            if (m_action.graphics_indexes().front() != 1) {
                m_action = bn::create_sprite_animate_action_forever(
                               m_sprite, 3, bn::sprite_items::bowser_sprite.tiles_item(), 1, 0, 2, 0);
            }
        } else {
            // checks if animation state is already "standing" and if so, avoids "thrashing" the m_action value
            if (m_action.graphics_indexes().front() != 0) {
                m_action = bn::create_sprite_animate_action_forever(
                               m_sprite, 30, bn::sprite_items::bowser_sprite.tiles_item(), 0, 0, 0, 0);
            }
        }

        m_action.update();
    }

    void Player::t_collide_with_objects(bn::affine_bg_ptr map, sbb::Level level)
    {
        // if falling
        if (m_dy > 0) {
            m_is_falling = true;
            m_is_grounded = false;
            m_is_jumping = false;

            // clamp max fall speed
            if (m_dy > MAX_DY) {
                m_dy = MAX_DY;
            }

            if (check_collisions_map(m_pos, down, m_hitbox_fall, map, level, m_map_cells.value())) {
                m_is_grounded = true;
                m_is_falling = false;
                m_dy = 0;
                m_pos.set_y(m_pos.y() - modulo(m_pos.y(), 8));
                //todo if they pressed jump a few milliseconds before hitting the ground then jump now
            }
        } else if (m_dy < 0) { // jumping
            m_is_jumping = true;
            m_is_falling = false;

            if (check_collisions_map(m_pos, up, m_hitbox_jump, map, level, m_map_cells.value())) {
                m_dy = 0;
            }
        }

        if (m_dx > 0) { // moving right
            if (check_collisions_map(m_pos, right, m_hitbox_right, map, level, m_map_cells.value())) {
                m_dx = 0;
            }
        } else if (m_dx < 0) { // moving left
            if (check_collisions_map(m_pos, left, m_hitbox_fall, map, level, m_map_cells.value())) {
                m_dx = 0;
            }
        }
    }

    void Player::t_delete_data()
    {
        m_camera.reset();
        m_map.reset();
        m_map_cells.reset();
    }

    void Player::t_hide()
    {
        m_sprite.set_visible(false);
    }

    void Player::t_jump()
    {
        if (m_is_grounded) {
            m_dy -= JUMP_POWER;
            m_is_grounded = false;
        }
    }

    void Player::t_move_left()
    {
        m_sprite.set_horizontal_flip(true);
        m_dx -= ACC;
        m_is_running = true;
        m_is_sliding = false;
    }
    void Player::t_move_right()
    {
        m_sprite.set_horizontal_flip(false);
        m_dx += ACC;
        m_is_running = true;
        m_is_sliding = false;
    }

    void Player::t_reset()
    {
        m_dx = 0;
        m_dy = 0;
        m_is_already_running = false;
        m_is_falling = false;
        m_is_grounded = false;
        m_is_jumping = false;
        m_is_running = false;
        m_sprite.put_above();
        m_sprite.set_bg_priority(1);
        m_sprite.set_camera(m_camera);
        m_sprite.set_horizontal_flip(false);
        m_update_camera(1);
    }

    void Player::t_spawn(bn::fixed_point pos, bn::camera_ptr camera, bn::affine_bg_ptr map)
    {
        m_camera = camera;
        m_map = map;
        m_map_cells = map.map().cells_ref().value();
        m_map.value().set_visible(true);
        m_pos = pos;
        m_sprite.set_visible(true);
        t_reset();
    }

    void Player::m_update_camera(int lerp)
    {
        if (m_pos.x() < 122 + 30) {
            m_camera.value().set_x(m_camera.value().x() + (122 - m_camera.value().x()) / lerp);
        } else if (m_pos.x() > 922 - 30) {
            m_camera.value().set_x(m_camera.value().x() + (922 - 20 - m_camera.value().x()) / lerp);
        } else {
            if (m_sprite.horizontal_flip()) {
                m_camera.value().set_x(m_camera.value().x() + (m_pos.x() - 30 - m_camera.value().x() + m_dx * 8) / lerp);
            } else {
                m_camera.value().set_x(m_camera.value().x() + (m_pos.x() + 30 - m_camera.value().x() + m_dx * 8) / lerp);
            }
        }

        if (m_pos.y() > 942) {
            m_camera.value().set_y(m_camera.value().y() + (942 - m_camera.value().y()) / lerp);
        } else if (m_pos.y() < 90) {
            m_camera.value().set_y(m_camera.value().y() + (90 - m_camera.value().y()) / lerp);
        } else {
            m_camera.value().set_y(m_camera.value().y() + (m_pos.y() - 10 - m_camera.value().y()) / lerp);
        }
    }

    void Player::t_update_position(bn::affine_bg_ptr map, sbb::Level level)
    {
        m_update_camera(30 - bn::abs(m_dx.integer()) * 5);
        m_dx = m_dx * FRICTION;
        m_dy += GRAVITY;

        // take input
        if (bn::keypad::left_held()) {
            t_move_left();
        } else if (bn::keypad::right_held()) {
            t_move_right();
        } else if (m_is_running) { //slide to a stop
            if (!m_is_falling & !m_is_jumping) {
                m_is_sliding = true;
                m_is_running = false;
            }
        } else if (m_is_sliding) { //stop sliding
            if (bn::abs(m_dx) < 0.1 || m_is_running) {
                m_is_sliding = false;
            }
        }

        if (bn::keypad::a_pressed()) {
            t_jump();
        }

        // collide
        t_collide_with_objects(map, level);
        // update position
        m_pos.set_x(m_pos.x() + m_dx);
        m_pos.set_y(m_pos.y() + m_dy);

        // lock player position to map limits x
        if (m_pos.x() > 1016) {
            m_pos.set_x(1016);
        } else if (m_pos.x() < 4) {
            m_pos.set_x(4);
        }

        // update sprite position
        m_sprite.set_x(m_pos.x());
        m_sprite.set_y(m_pos.y());
    }
}
