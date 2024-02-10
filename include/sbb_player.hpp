#ifndef SBB_PLAYER_H
#define SBB_PLAYER_H

#include "bn_affine_bg_ptr.h"
#include "bn_fixed_point.h"
#include "bn_sprite_ptr.h"
#include "bn_camera_ptr.h"
#include "bn_span.h"
#include "bn_affine_bg_map_cell.h"

namespace sbb
{
    class Player
    {
        private:
            bn::sprite_ptr _sprite;

            bn::fixed _dx;
            bn::fixed _dy;
            bn::fixed_point _pos;
            bn::optional<bn::camera_ptr> _camera;
            bn::optional<bn::span<const bn::affine_bg_map_cell>> _map_cells;
            bn::optional<bn::affine_bg_ptr> _map;
        public:
            Player(bn::sprite_ptr sprite);

            void hide();
            void spawn(bn::fixed_point pos, bn::camera_ptr camera, bn::affine_bg_ptr map);
    };
}

#endif
