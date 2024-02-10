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
            Player *_player;
        public:
            Path(Player &player);
            Scene execute(bn::fixed_point spawn);
    };
}

#endif
