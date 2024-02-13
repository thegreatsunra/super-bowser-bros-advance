#ifndef SBB_PATH_H
#define SBB_PATH_H

#include "bn_fixed_point.h"

#include "sbb_scene.hpp"
#include "sbb_player.hpp"

namespace sbb
{
    class Path
    {
        private:
            Player *m_player;
        public:
            Path(Player &m_player);
            Scene execute(bn::fixed_point t_spawn);
    };
}

#endif
