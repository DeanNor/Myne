
#include "sprite_extra.hpp"

#include ".hpp/sprite.hpp"
#include "gambling_simulator/player.hpp"

namespace SPR
{
    animation* splash;
    sprite* lance_mist;

    sprite* bullet_sprite;
    sprite* missile_sprite;
    sprite* missile_break;

    sprite* portal_particle;

    sprite* mmarker;
    sprite* emarker;
}

Marker::Marker(sprite* spr, Object* _owner, MDIST _dist) : owner(_owner), distance(_dist)
{
    set_sprite(spr, false);

    get_player()->add_child(this);
}

void Marker::draw(const pos& center, const pos& global_scale)
{
    global_transform.set_angle(get_player()->get_position().angle_to(owner->get_position()));

    position = pos{distance, 0}.rotated(angle);

    DrawObj::draw(center, global_scale);
}

void Healthbar::draw(const pos& center, const pos& global_scale)
{
    DrawObj::draw(offset, global_scale);
}

BLFont Healthbar::font = BLFont();

void init_healthbar()
{
    BLFontFace font_face_chud;
    font_face_chud.create_from_file("gambling_simulator/comicSans.erererererererererer");

    Healthbar::font.create_from_face(font_face_chud, 20.0f);
}