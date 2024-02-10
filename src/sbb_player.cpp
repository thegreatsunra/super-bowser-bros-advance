#include "bn_fixed_point.h"
#include "bn_sprite_ptr.h"
#include "bn_camera_ptr.h"
#include "bn_affine_bg_ptr.h"
#include "bn_affine_bg_map_ptr.h"

#include "sbb_player.hpp"
#include "sbb_level.hpp"

#include "bn_affine_bg_items_path.h"

namespace sbb
{
    enum directions {up, down, left, right};

    Player::Player(bn::sprite_ptr sprite) :
        _sprite(sprite),
        _camera(bn::camera_ptr::create(0, 0)),

        _map(bn::affine_bg_items::path.create_bg(0, 0))
    {
        _map.value().set_visible(false);
        _sprite.put_above();
        _sprite.set_visible(true);
    }

    void Player::spawn(bn::fixed_point pos, bn::camera_ptr camera, bn::affine_bg_ptr map)
    {
        _pos = pos;
        _camera = camera;
        _map = map;
        _map_cells = map.map().cells_ref().value();
        _map.value().set_visible(true);
        _sprite.set_visible(true);
        _sprite.set_x(_pos.x());
        _sprite.set_y(_pos.y());
        _sprite.put_above();
    }
}
