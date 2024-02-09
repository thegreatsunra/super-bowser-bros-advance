#include "bn_core.h"
#include "bn_sprite_ptr.h"
#include "bn_camera_actions.h"

#include "bn_sprite_items_bowser_sprite.h"

#include "sbb_path.h"

int main() {
    bn::core::init();
    bn::sprite_ptr bowser_sprite = bn::sprite_items::bowser_sprite.create_sprite(0, 0);
    bowser_sprite.set_visible(true);
    sbb::Player player = sbb::Player(bowser_sprite);

    while (true) {
        sbb::Path path = sbb::Path(player);
        // path.execute(bn::fixed_point(0, 0));
        bn::core::update();
    }
}
