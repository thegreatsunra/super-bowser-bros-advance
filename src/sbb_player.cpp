#include "bn_affine_bg_map_ptr.h"
#include "bn_affine_bg_ptr.h"
#include "bn_camera_ptr.h"
#include "bn_fixed_point.h"
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


    void Player::update_position(bn::affine_bg_ptr map, fe::Level level)
    {
        _update_camera(30 - bn::abs(_dx.integer()) * 5);
        // apply friction
        _dx = _dx * friction;
        //apply gravity
        _dy += gravity;

        // take input
        if (bn::keypad::left_held() && !_listening) {
            move_left();
        } else if (bn::keypad::right_held() && !_listening) {
            move_right();
        } else if (_running) { //slide to a stop
            if (!_falling & !_jumping) {
                _sliding = true;
                _running = false;
            }
        } else if (_sliding) { //stop sliding
            if (bn::abs(_dx) < 0.1 || _running) {
                _sliding = false;
            }
        }

        // jump
        if (bn::keypad::a_pressed()) {
            jump();
        }

        // attack
        if (bn::keypad::b_pressed()) {
            attack();
        }

        // collide with enemies
        // ouch
        if (_invulnerable) {
            ++_inv_timer;

            if (modulo(_inv_timer / 5, 2) == 0) {
                _sprite.set_visible(true);
            } else {
                _sprite.set_visible(false);
            }

            if (_inv_timer > 120) {
                _invulnerable = false;
                _inv_timer = 0;
                _sprite.set_visible(true);
            }
        }

        // update position
        _pos.set_x(_pos.x() + _dx);
        _pos.set_y(_pos.y() + _dy);

        // lock player position to map limits x
        if (_pos.x() > 1016) {
            _pos.set_x(1016);
        } else if (_pos.x() < 4) {
            _pos.set_x(4);
        }

        // teleport player to "start" if they fall in a pit
        // i.e. they "fall" above y=600
        if (_pos.y() > 600) {
            _pos.set_y(540);
            _pos.set_x(100);
        }

        // update sprite position
        _sprite.set_x(_pos.x());
        _sprite.set_y(_pos.y());
    }

}
