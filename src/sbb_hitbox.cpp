#include "sbb_hitbox.hpp"

namespace sbb {
Hitbox::Hitbox(bn::fixed x, bn::fixed y, bn::fixed width, bn::fixed height) :
    m_pos(x, y), m_width(width), m_height(height)
{}

bn::fixed_point Hitbox::t_pos() {
    return m_pos;
}

bn::fixed Hitbox::t_x() {
    return m_pos.x();
}

bn::fixed Hitbox::t_y() {
    return m_pos.y();
}

bn::fixed Hitbox::t_width() {
    return m_width;
}

bn::fixed Hitbox::t_height() {
    return m_height;
}

void Hitbox::t_set_x(bn::fixed x) {
    m_pos.set_x(x);
}

void Hitbox::t_set_y(bn::fixed y) {
    m_pos.set_y(y);
}

void Hitbox::t_set_width(bn::fixed width) {
    m_width = width;
}

void Hitbox::t_set_height(bn::fixed height) {
    m_height = height;
}
}
