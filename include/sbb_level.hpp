#ifndef SBB_LEVEL_H
#define SBB_LEVEL_H

#include "bn_affine_bg_ptr.h"
#include "bn_vector.h"

namespace sbb
{
    class Level
    {
        private:
            bn::vector<int, 32> m_floor_tiles;
            bn::vector<int, 32> m_wall_tiles;
            bn::vector<int, 32> m_ceil_tiles;

        public:
            Level(bn::affine_bg_ptr bg);

            [[nodiscard]] bn::vector<int, 32> t_floor_tiles();
            [[nodiscard]] bn::vector<int, 32> t_wall_tiles();
            [[nodiscard]] bn::vector<int, 32> t_ceil_tiles();
    };
}

#endif
