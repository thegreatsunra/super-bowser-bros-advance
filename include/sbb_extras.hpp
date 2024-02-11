#ifndef SBB_EXTRAS_H
#define SBB_EXTRAS_H

#include "bn_fixed_point.h"

namespace sbb {
[[nodiscard]] constexpr bn::fixed modulo(bn::fixed a, bn::fixed m) {
    return a - m * ((a / m).integer());
}
}

#endif
