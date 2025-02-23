#include "bn_fixed_point.h"

#include "sbb_hitbox.hpp"

namespace sbb {
bool check_collisions(Hitbox boxA, Hitbox boxB) {
	bool result = boxA.t_x() - boxA.t_width() / 2 < boxB.t_x() + boxB.t_width() / 2 &&
								boxA.t_x() + boxA.t_width() / 2 > boxB.t_x() - boxB.t_width() / 2 &&
								boxA.t_y() - boxA.t_height() / 2 < boxB.t_y() + boxB.t_height() / 2 &&
								boxA.t_y() + boxA.t_height() / 2 > boxB.t_y() - boxB.t_height() / 2;
	return result;
}

bool check_collisions(Hitbox boxA, bn::fixed x, bn::fixed y, bn::fixed w, bn::fixed h) {
	bool result = boxA.t_x() - boxA.t_width() / 2 < x + w / 2 && boxA.t_x() + boxA.t_width() / 2 > x - w / 2 &&
								boxA.t_y() - boxA.t_height() / 2 < y + h / 2 && boxA.t_y() + boxA.t_height() / 2 > y - h / 2;
	return result;
}
} // namespace sbb
