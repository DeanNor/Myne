
#pragma once

#include ".hpp/SDL3.h"
#include ".hpp/blendobj.hpp"
#include ".hpp/drawobj.hpp"
#include ".hpp/game.hpp"
#include ".hpp/sprite.hpp"
#include "SDL3/SDL_render.h"
#include "core/api.h"
#include "core/geometry.h"
#include "core/path.h"
#include "core/rgba.h"

namespace SPR
{
    extern animation* splash;

    extern sprite* lance_mist;

    extern sprite* bullet_sprite;
    extern sprite* missile_sprite;
    extern sprite* missile_break;

    extern sprite* portal_particle;

    extern sprite* mmarker;
    extern sprite* emarker;
}

class Directional : public DrawObj
{
public:
    virtual void draw(const pos& origin, const pos& scale) override
    {
        rad ang = global_transform.parent->compute_angle();
        angle = ang.nearest(rad::PI() / 4.) - ang;

        DrawObj::draw(origin, scale);
    }
};

class AlphaObject : public DrawObj
{
public:
    void set_alpha(float alpha)
    {
        SDL_SetTextureAlphaModFloat(texture->get(), alpha);
    }
};

class VisibleNode : public DrawObj
{
    pos offset;
public:
    VisibleNode(pos _offset) : offset(_offset)
    {

    }

    virtual void draw(const pos& origin, const pos& scale) override
    {
        position = window->get_top_left() + offset;

        DrawObj::draw(origin, scale);
    }
};

class Follow : public AlphaObject
{
    Object* follow;
public:
    void set_follow(Object* who)
    {
        follow = who;
    }

    virtual void draw(const pos& origin, const pos& scale) override
    {
        position = window->get_center();

        DrawObj::draw(origin, scale);
    }

    void set_alpha(float alpha)
    {
        SDL_SetTextureAlphaModFloat(texture->get(), alpha);
    }
};

class Particle : public DrawObj
{
    pos velocity;
    double lifespan;

public:
    Particle(sprite* _spr, rad _angle, pos _position, double _velocity, double _lifespan) : velocity(pos{_velocity, 0}.rotated(_angle)), lifespan(_lifespan)
    {
        texture = _spr;
        angle = _angle;

        position = _position;

        scale = {1/10., 1/10.};

        set_depth(169);
    }

    virtual void process() override
    {
        auto delta = get_current_game()->get_delta();
        lifespan -= delta;

        if (lifespan < 0.5)
        {
            if (lifespan <= 0)
            {
                start_delete();
            }
        }

        position += velocity * delta;
    }
};

class Splash : public DrawObj
{
    size_t my_frame = 0;

public:
    Splash(pos _position)
    {
        position = _position;
    }

    virtual void draw(const pos& center, const pos& global_scale) override
    {
        texture = SPR::splash->at(my_frame++);

        DrawObj::draw(center, global_scale);

        if (my_frame >= SPR::splash->get_len()) start_delete();
    }
};

class PlayerSplash : public Splash
{
public:
    PlayerSplash(pos _position) : Splash(_position)
    {
        //play_audio("img/splash.wav", get_current_game());
    }

    ~PlayerSplash()
    {
        get_current_game()->exit();
    }
};

class Explosion : public AlphaObject
{
    double alpha = 1;

public:
    Explosion(pos _position, sprite* particle_sprite)
    {
        position = _position;

        set_sprite("img/explosion.png");

        for (int x = 0; x < rand() % 10; x++)
        {
            Particle* particle = new Particle(particle_sprite, rad(rand()), {0,0}, 67, (rand() % 300) / 100.);
            add_child(particle);
        }
    }

    virtual void draw(const pos& center, const pos& global_scale) override
    {
        set_alpha(alpha);
        alpha -= 0.01;

        DrawObj::draw(center, global_scale);
    }
};

enum MDIST
{
    MMARKER = 100,
    EMARKER = 60,
};

class Marker : public DrawObj
{
    Object* owner; 

    double distance;

public:
    Marker(sprite* spr, Object* _owner, MDIST _dist);

    virtual void draw(const pos& center, const pos& global_scale) override;
};

// 5px radius inside radius used
class Dial : public BlendObj
{
    pos half_size;

    BLRgba32 color;

public:
    Dial(double radius, BLRgba32 _color) : color(_color)
    {
        image.create(radius * 2, radius * 2, BLEND_FORMAT);

        half_size = {radius,radius};
    }

    void set_percent(double _per)
    {
        BLContext ctx(image);

        ctx.set_stroke_width(5);
        ctx.set_stroke_start_cap(BL_STROKE_CAP_TRIANGLE);
        ctx.set_stroke_end_cap(BL_STROKE_CAP_TRIANGLE);

        ctx.clear_all();

        BLArc arc(half_size.x, half_size.y, half_size.x - 5, half_size.y - 5, 0, rad::PI_d() * 2. * _per);

        ctx.stroke_arc(arc, color);

        ctx.end();

        update_image();
    }
};

class Healthbar : public BlendObj
{
    const char* name;
    size_t name_len;
    pos offset;

    bool side;

public:
    static BLFont font;

    Healthbar(const char* _name, pos _offset, bool left) : name(_name), name_len(std::strlen(_name)), offset(_offset), side(left)
    {
        image.create(500, 100, BLEND_FORMAT);
    }

    virtual void draw(const pos& center, const pos& global_scale) override;

    void set(double percent)
    {
        BLContext ctx(image);

        ctx.clear_all();

        ctx.set_fill_style((BLRgba32)0x55FFFFFF);
        ctx.fill_box(0,0,500,80);

        ctx.set_fill_style((BLRgba32)0x55FF0000);

        if (side)
        {
            ctx.fill_box(500 * (1-percent),0,500,80);

            ctx.fill_utf8_text(BLPoint{10, 100}, font, name, name_len);
        }

        else
        {
            ctx.fill_box(0,0,500 * percent,80);

            ctx.fill_utf8_text(BLPoint{300, 100}, font, name, name_len);
        }



        ctx.end();

        update_image();
    }
};

void init_healthbar();