#include "sbb_level.hpp"

#include "bn_affine_bg_ptr.h"
#include "bn_affine_bg_map_ptr.h"
#include "bn_span.h"

namespace sbb
{
    Level::Level(bn::affine_bg_ptr bg)
    {
        bool processed_map = false;
        int index = 0;
        int empties = 0;
        m_floor_tiles = {};
        m_wall_tiles = {};
        m_ceil_tiles = {};
        bn::span<const bn::affine_bg_map_cell> cells = bg.map().cells_ref().value();

        while (!processed_map) {
            if (cells.at(index) == 0) {
                ++empties;

                if (empties > 2) {
                    processed_map = true;
                }
            } else {
                if (empties == 0) {
                    m_floor_tiles.push_back(cells.at(index));
                } else if (empties == 1) {
                    m_wall_tiles.push_back(cells.at(index));
                } else if (empties == 2) {
                    m_ceil_tiles.push_back(cells.at(index));
                }
            }

            ++index;
        }
    }

    bn::vector<int, 32> Level::t_floor_tiles()
    {
        return m_floor_tiles;
    }

    bn::vector<int, 32> Level::t_wall_tiles()
    {
        return m_wall_tiles;
    }

    bn::vector<int, 32> Level::t_ceil_tiles()
    {
        return m_ceil_tiles;
    }

}
