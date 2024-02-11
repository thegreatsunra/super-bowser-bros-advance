
#include "bn_affine_bg_map_cell.h"
#include "bn_affine_bg_map_ptr.h"
#include "bn_affine_bg_ptr.h"
#include "bn_camera_ptr.h"
#include "bn_core.h"

#include "bn_affine_bg_items_path.h"
// #include "bn_sprite_items_bowser_sprite.h"

#include "sbb_path.hpp"
#include "sbb_player.hpp"
#include "sbb_scene.hpp"

namespace sbb
{
    Path::Path(Player &player)
        : m_player(&player) {}

    Scene Path::execute(bn::fixed_point spawn_location)
    {
        bn::camera_ptr camera = bn::camera_ptr::create(spawn_location.x(), spawn_location.y());
        bn::affine_bg_ptr map = bn::affine_bg_items::path.create_bg(512, 512);
        map.set_priority(1);
        // we'll need this eventually for player:set_position()
        sbb::Level level = sbb::Level(map);
        map.set_camera(camera);
        m_player->t_spawn(spawn_location, camera, map);

        while (true) {
            m_player->t_update_position(map, level);
            bn::core::update();
        }
    }
}
