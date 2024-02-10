#include "bn_affine_bg_map_ptr.h"
#include "bn_affine_bg_ptr.h"
#include "bn_camera_ptr.h"
#include "bn_fixed_point.h"
#include "bn_keypad.h"
#include "bn_math.h"
#include "bn_sprite_ptr.h"

#include "bn_affine_bg_items_path.h"

#include "sbb_level.hpp"
#include "sbb_player.hpp"

namespace sbb
{
    enum directions {up, down, left, right};

    Player::Player(bn::sprite_ptr sprite) :
        m_sprite(sprite),
        m_camera(bn::camera_ptr::create(0, 0)),

        m_map(bn::affine_bg_items::path.create_bg(0, 0))
    {
        m_map.value().set_visible(false);
        m_sprite.put_above();
        m_sprite.set_visible(true);
    }

    void Player::t_spawn(bn::fixed_point pos, bn::camera_ptr camera, bn::affine_bg_ptr map)
    {
        m_pos = pos;
        m_camera = camera;
        m_map = map;
        m_map_cells = map.map().cells_ref().value();
        m_map.value().set_visible(true);
        m_sprite.set_visible(true);
        m_sprite.set_x(m_pos.x());
        m_sprite.set_y(m_pos.y());
        m_sprite.put_above();
    }

    void Player::t_delete_data()
    {
        m_camera.reset();
        m_map.reset();
        m_map_cells.reset();
    }

    void Player::t_reset()
    {
        m_sprite.set_camera(m_camera);
        m_sprite.set_bg_priority(1);
        m_sprite.put_above();
        m_update_camera(1);
        m_sprite.set_horizontal_flip(false);
        m_dy = 0;
        m_dx = 0;
    }

    void Player::t_hide()
    {
        m_sprite.set_visible(false);
    }

    constexpr const bn::fixed GRAVITY = 0.2;
    constexpr const bn::fixed FRICTION = 0.85;
    constexpr const bn::fixed ACC = 0.4;

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
        }

        // update position
        m_pos.set_x(m_pos.x() + m_dx);
        m_pos.set_y(m_pos.y() + m_dy);

        // lock player position to map limits x
        if (m_pos.x() > 1016) {
            m_pos.set_x(1016);
        } else if (m_pos.x() < 4) {
            m_pos.set_x(4);
        }

        // teleport player to "start" if they fall in a pit
        // i.e. they "fall" above y=600
        if (m_pos.y() > 600) {
            m_pos.set_y(540);
            m_pos.set_x(100);
        }

        // update sprite position
        m_sprite.set_x(m_pos.x());
        m_sprite.set_y(m_pos.y());
    }

    void Player::t_move_right()
    {
        m_sprite.set_horizontal_flip(false);
        m_dx += ACC;
    }

    void Player::t_move_left()
    {
        m_sprite.set_horizontal_flip(true);
        m_dx -= ACC;
    }

    void Player::m_update_camera(int lerp)
    {
        // update camera
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
}
