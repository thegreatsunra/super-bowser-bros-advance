#ifndef SBB_HITBOX_H
#define SBB_HITBOX_H

#include "bn_fixed.h"
#include "bn_fixed_point.h"

namespace sbb
{
    class Hitbox
    {
        private:
            bn::fixed_point m_pos;
            bn::fixed m_width;
            bn::fixed m_height;

        public:
            Hitbox();
            Hitbox(bn::fixed x, bn::fixed y, bn::fixed width, bn::fixed height);

            bn::fixed t_height();
            bn::fixed_point t_pos();
            bn::fixed t_width();
            bn::fixed t_x();
            bn::fixed t_y();

            void t_set_height(bn::fixed height);
            void t_set_width(bn::fixed width);
            void t_set_x(bn::fixed x);
            void t_set_y(bn::fixed y);
    };
}

#endif
