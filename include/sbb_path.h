#ifndef SBB_PATH_H
#define SBB_PATH_H

#include "sbb_scene.h"
#include "sbb_player.h"
#include "bn_fixed_point.h"

namespace sbb {
class Path {
private:
    Player* _player;
public:
    Path(Player &player);
    Scene execute(bn::fixed_point spawn);
};
}

#endif
