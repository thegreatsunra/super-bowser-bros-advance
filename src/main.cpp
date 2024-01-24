#include "bn_core.h"
#include "bn_regular_bg_items_background.h"
#include "bn_regular_bg_ptr.h"

int main()
{
    bn::core::init();
    // (520, -48) are magic numbers to align the 1280x256 .bmp image with the lower-left corner of the display
    bn::regular_bg_ptr regular_bg = bn::regular_bg_items::background.create_bg(520, -48);

    while(true)
    {
        bn::core::update();
    }
}
