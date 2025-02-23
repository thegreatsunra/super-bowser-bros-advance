#include "bn_camera_actions.h"
#include "bn_core.h"
#include "bn_sprite_ptr.h"

#include "bn_sprite_items_bowser_sprite.h"

#include "sbb_stage.hpp"

int main() {
	bn::core::init();
	bn::sprite_ptr bowser_sprite = bn::sprite_items::bowser_sprite.create_sprite(0, 0);
	bowser_sprite.set_visible(false);
	sbb::Player player = sbb::Player(bowser_sprite);

	while (true) {
		sbb::Stage stage = sbb::Stage(player);
		stage.execute(bn::fixed_point(64, 968));
		player.t_delete_data();
		player.t_hide();
		bn::core::update();
	}
}
