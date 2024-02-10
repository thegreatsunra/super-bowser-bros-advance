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


}
