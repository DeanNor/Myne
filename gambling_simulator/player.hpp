
#pragma once

#include ".hpp/SDL3.h"
#include ".hpp/collobj.hpp"
#include ".hpp/drawobj.hpp"
#include ".hpp/game.hpp"
#include ".hpp/sprite.hpp"
#include "SDL3/SDL_keycode.h"
#include "SDL3/SDL_render.h"
#include "box2d/box2d.h"
#include "box2d/id.h"
#include "box2d/types.h"
#include "camera.hpp"
#include "bullet.hpp"
#include "core/rgba.h"
#include "gambling_simulator/damage.hpp"
#include "sprite_extra.hpp"
#include <cmath>
#include <cstdlib>

void set_player(Object* new_player);

Object* get_player();

extern pos PLAYER_VEL;

class Player : public Damage
{
    b2ShapeId shape_id;

    b2Filter standard_filter = {MASK::PLANE, MASK::WALL | MASK::MISSILE,0};
    b2Filter lance_filter = {MASK::PLANE,MASK::MISSILE, 0};

    keyboard_watcher left, right, speed, slow ,shot;

    pos acceleration = {15000,0};
    double max_vel = 400;

    pos deceleration = {-5000, 0};
    
    double lift_push = 0.4;

    float turn_speed = 2200.f;
    float auto_right_speed = 600.f;

    float mass;

    double half_width;

    DrawObj* spr;

    Follow* high_g;

    float last_vel = 0;
    float high_g_vel = 6.5;

    float g_high = 0;
    float g_recovery = 0.01;

    static constexpr double lance_vel = 300;

    bool lancing = false;

    pos bullet_vel = {50, 0};
    bool can_fire = true;

    constexpr static double fire_timer = 1 / 8.;
    double fire_delay = fire_timer;

    DrawObj* portal;

    constexpr static double portal_delay = 0.5;
    double portal_charge = portal_delay;

    pos portal_vel = {max_vel, 0};
    bool wait_release = false;

    audio_type shoot_audio;

    Dial* d_health;
    Dial* d_portal;

    double initial_health;

public:
    Player() : Damage(100, 100), left(SDLK_V), right(SDLK_N), speed(SDLK_B), slow(SDLK_SPACE), shot(SDLK_C), initial_health(health)
    {
        high_g = new Follow;

        high_g->set_sprite("img/high_g.png");

        high_g->set_depth(211);
        high_g->set_scale(get_current_game()->get_game_window()->get_size() / high_g->get_texture()->size);

        get_current_game()->get_root()->add_child(high_g);

        high_g->set_follow(this);

        spr = new DrawObj;

        spr->set_sprite("img/player.png");

        spr->set_scale({1/40., 1/40.});

        spr->set_depth(100);

        add_child(spr);

        pos spr_size = spr->get_texture()->half_size * spr->get_scale();
        half_width = spr_size.x;

        collision_def.type = b2_dynamicBody;
        collision_def.enableSleep = false;
        collision_def.angularDamping = 0.5;

        collision_body = b2CreateBody(get_current_coll_world(), &collision_def);

        b2Polygon box = b2MakeBox(spr_size.x , spr_size.y);
        
        b2ShapeDef fixtureDef = b2DefaultShapeDef();
        fixtureDef.density = 0.5;
        fixtureDef.material.friction = 0;
        fixtureDef.material.restitution = 1;
        fixtureDef.filter = standard_filter;
        fixtureDef.enableContactEvents = true;

        shape_id = b2CreatePolygonShape(collision_body, &fixtureDef, &box);

        mass = b2Body_GetMass(collision_body);

        game* g = get_current_game();

        g->get_keyboard().add_watcher(&left);
        g->get_keyboard().add_watcher(&right);
        g->get_keyboard().add_watcher(&speed);
        g->get_keyboard().add_watcher(&slow);
        g->get_keyboard().add_watcher(&shot);

        set_position({0, -500});

        set_player(this);

        Camera* cam = new Camera;
        cam->set_follow(this);
        add_child(cam);

        SDL_Renderer* renderer = get_current_game()->get_game_window()->get_renderer();
        SPR::bullet_sprite = new sprite("img/bullet.png", renderer);

        SPR::lance_mist = new sprite("img/lance_mist.png", renderer);
        SDL_SetTextureAlphaModFloat(SPR::lance_mist->get(), 0.2);

        portal = new DrawObj;
        portal->set_sprite("img/portal.png");
        get_current_game()->get_root()->add_child(portal);
        portal->set_depth(109);

        portal->set_scale({1/15., 1/15.});
        portal->set_active(false);

        SPR::portal_particle = new sprite("img/portal_particle.png", renderer);

        shoot_audio = load_audio("img/crying.wav");

        d_portal = new Dial(30, BLRgba32(0xFF800080));
        add_child(d_portal);
        d_portal->set_depth(200);

        d_portal->set_angle(rad(rand()));

        d_health = new Dial(50, BLRgba32(0xFFFF3765));
        add_child(d_health);
        d_health->set_depth(200);

        d_health->set_angle(rad::PI() / 2.);
        d_health->set_percent(1);
    }

    ~Player()
    {
        shoot_audio.free_data();
    }

    virtual void collision_process() override
    {
        double len_velocity = lin_velocity.len();
        double root_velocity = std::sqrt(len_velocity);

        rad vel_dir = lin_velocity.direction();
        float angle_dif = vel_dir - angle;

        if (speed.down())
        {
            b2Body_ApplyForceToCenter(collision_body, acceleration.rotated(angle), true);
        }

        else if (slow.down() && len_velocity > deceleration.x)
        {
            b2Body_ApplyForceToCenter(collision_body, deceleration.rotated(angle), true);
        }

        if (left.down())
        {
            b2Body_ApplyTorque(collision_body, -turn_speed * root_velocity, true);
        }

        if (right.down())
        {
            b2Body_ApplyTorque(collision_body, turn_speed * root_velocity, true);
        }

        b2Body_ApplyTorque(collision_body, angle_dif * auto_right_speed * root_velocity, true);

        pos lift_push_force = {lift_push * len_velocity * mass * std::abs(std::sin(angle - vel_dir)), 0};
        lift_push_force = lift_push_force.rotated(angle) - lift_push_force.rotated(vel_dir) ;

        b2Body_ApplyForceToCenter(collision_body, lift_push_force, true);

        if (len_velocity > max_vel) lin_velocity = lin_velocity.limited(max_vel);

        g_high = std::max(std::abs(((float)(len_velocity) - last_vel) * angle_dif / high_g_vel), g_high);
        high_g->set_alpha(g_high);

        g_high -= g_recovery;

        last_vel = len_velocity;

        if (len_velocity >= lance_vel)
        {
            lancing = true;

            b2Shape_SetFilter(shape_id, lance_filter);

            parent->add_child(new Particle(SPR::lance_mist, angle + ((rand() % 200) - 100) / 200., position - pos{half_width, 0}.rotated(angle), len_velocity / 2.5, 3));
        }

        else
        {
            lancing = false;

            b2Shape_SetFilter(shape_id, standard_filter);
        }

        if (position.x > 5000 || position.x < -5000 || position.y < -5000)
        {
            start_delete();

            get_current_game()->exit();
        }

        else if (position.y > 4750)
        {
            spr->set_active(false);

            PlayerSplash* s = new PlayerSplash(position);
            s->set_depth(160);
            s->set_scale({1/5., 1/5.});
            s->set_angle(rad::PI() / 2. + angle);

            parent->add_child(s);

            start_delete();

            set_player(s);
        }

        PLAYER_VEL = lin_velocity;
    }

    virtual void process() override
    {
        double delta = get_current_game()->get_delta();

        if (portal_charge <= 0)
        {
            d_portal->set_percent(1);

            if (wait_release)
            {
                parent->add_child(new Particle(SPR::portal_particle, portal->get_position().angle_to(get_current_game()->get_mouse().position) + ((rand() % 100) - 100) / 200., portal->get_position(), 700, 0.2));

                if (get_current_game()->get_mouse().ldown == false)
                {
                    position = portal->get_position();
                    angle = position.angle_to(get_current_game()->get_mouse().position);

                    lin_velocity = portal_vel.rotated(angle);
                    rot_velocity = 0;
                    last_vel = portal_vel.x; // I like to be able to see, and 0 to 300 in 0.016s is a little spine breaking 

                    portal->set_active(false);

                    portal_charge = portal_delay;
                    wait_release = false;

                    d_portal->set_angle(rad(rand()));
                }
            }
            
            else if (get_current_game()->get_mouse().ljust_down == true)
            {
                wait_release = true;
                portal->set_position(get_current_game()->get_mouse().position);
                portal->set_active(true);
            }
        }

        else
        {
            double p = (portal_delay - portal_charge) / portal_delay;

            d_portal->set_percent(p);

            portal_charge -= delta;
        }

        if (!can_fire)
        {
            fire_delay -= delta;

            if (fire_delay <= 0)
            {
                can_fire = true;
            }
        }

        else if (shot.down())
        {
            Bullet* bullet = new Bullet(SPR::bullet_sprite);
            bullet->set_position(position + pos{9, 0}.rotated(angle));
            bullet->set_angle(angle);
            bullet->add_vel(lin_velocity + bullet_vel.rotated(angle));
            get_current_game()->get_root()->add_child(bullet);

            fire_delay = fire_timer;
            can_fire = false;

            //play_audio(shoot_audio, get_current_game(), 0.1);
        }
    }

    virtual void collide_begin(CollObj* other) override
    {
        Damage* damage = dynamic_cast<Damage*>(other);

        if (damage)
        {
            health -= damage->get_damage();

            if (health <= 0)
            {
                get_current_game()->exit();
            }

            else
            {
                d_health->set_percent(health / initial_health);
            }
        }
    }
};