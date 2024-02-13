#include "bn_affine_bg_map_cell.h"
#include "bn_affine_bg_map_ptr.h"
#include "bn_affine_bg_ptr.h"
#include "bn_camera_ptr.h"
#include "bn_core.h"
#include "bn_math.h"
#include "bn_regular_bg_ptr.h"

#include "bn_affine_bg_items_stage.h"
#include "bn_regular_bg_items_background.h"

#include "sbb_enemy_type.hpp"
#include "sbb_enemy.hpp"
#include "sbb_player.hpp"
#include "sbb_scene.hpp"
#include "sbb_stage.hpp"

namespace sbb
{
    Stage::Stage(Player &player)
        : m_player(&player) {}

    Scene Stage::execute(bn::fixed_point spawn_location)
    {
        bn::camera_ptr camera = bn::camera_ptr::create(spawn_location.x(), spawn_location.y());
        bn::regular_bg_ptr map_bg = bn::regular_bg_items::background.create_bg(512, 512);
        bn::affine_bg_ptr map = bn::affine_bg_items::stage.create_bg(512, 512);
        map_bg.set_priority(2);
        map.set_priority(1);
        // we'll need this eventually for player:set_position()
        sbb::Level level = sbb::Level(map);
        map.set_camera(camera);
        map_bg.set_camera(camera);
        bn::vector<Enemy, 16> enemies = {};
        enemies.push_back(Enemy(128, 900, camera, map, ENEMY_TYPE::MARIO, 2));
        enemies.push_back(Enemy(176, 900, camera, map, ENEMY_TYPE::MARIO, 2));
        enemies.push_back(Enemy(200, 900, camera, map, ENEMY_TYPE::MARIO, 2));
        enemies.push_back(Enemy(228, 900, camera, map, ENEMY_TYPE::MARIO, 2));
        enemies.push_back(Enemy(256, 900, camera, map, ENEMY_TYPE::MARIO, 2));
        m_player->t_spawn(camera, enemies, map, spawn_location);

        while (true) {
            m_player->t_update_position(map, level);
            m_player->t_apply_animation_state();

            for (Enemy &enemy : enemies) {
                if (bn::abs(enemy.t_pos().x() - camera.x()) < 200 && bn::abs(enemy.t_pos().y() - camera.y()) < 100) {
                    enemy.t_update(m_player->t_pos());
                } else {
                    enemy.t_set_visible(false);
                }
            }

            bn::core::update();
        }
    }
}
