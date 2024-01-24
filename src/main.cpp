#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_regular_bg_ptr.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_actions.h"

#include "bn_regular_bg_items_background.h"
#include "bn_sprite_items_bowser.h"
#include "bn_sprite_items_mario.h"

namespace
{
    void default_scene()
    {
        // (520, -48) are magic numbers to align the 1280x256 .bmp image with the lower-left corner of the display
        bn::regular_bg_ptr regular_bg = bn::regular_bg_items::background.create_bg(520, -48);
        bn::fixed move_amplitude = 30;

        // (0, 40) is a magic number to make the sprite appear on the ground
        bn::sprite_ptr mario = bn::sprite_items::mario.create_sprite(0, 40);
        // (32, 32) is a magic number to make the sprite appear on the ground
        bn::sprite_ptr bowser = bn::sprite_items::bowser.create_sprite(32, 32);

        bn::sprite_move_loop_action action(mario, 120, move_amplitude, move_amplitude);
        while(true)
            {
            action.update();
            if(bn::keypad::left_held())
            {
                bowser.set_x(bowser.x() - 1);
            }
            else if(bn::keypad::right_held())
            {
                bowser.set_x(bowser.x() + 1);
            }
            if(bn::keypad::up_held())
            {
                bowser.set_y(bowser.y() - 1);
            }
            else if(bn::keypad::down_held())
            {
                bowser.set_y(bowser.y() + 1);
            }
            bn::core::update();
        }
    }
}

int main()
{
    bn::core::init();

    while(true)
    {
        default_scene();
    }
}
