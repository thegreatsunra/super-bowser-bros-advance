#ifndef SBB_STAGE_H
#define SBB_STAGE_H

#include "bn_fixed_point.h"

#include "sbb_scene.hpp"
#include "sbb_player.hpp"

namespace sbb
{
    class Stage
    {
        private:
            Player *m_player;
        public:
            Stage(Player &m_player);
            Scene execute(bn::fixed_point t_spawn);
    };
}

#endif
