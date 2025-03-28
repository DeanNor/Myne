
#pragma once

#include "b2.h"

#include "game.hpp"

class CollObj;

struct _joint // Parent Joint Struct
{
public:
    b2JointId collision_joint;

    CollObj* obj_a;
    b2BodyId body_a;

    CollObj* obj_b;
    b2BodyId body_b;

    _joint(CollObj* a, CollObj* b, b2BodyId id_a, b2BodyId id_b);

    _joint();

    virtual void remove();

    ~_joint();
};

struct revolute_j : public _joint
{
    b2RevoluteJointDef joint_def;

    revolute_j(CollObj* a, CollObj* b, b2RevoluteJointDef joint_def);
};

struct distance_j : public _joint
{
    b2DistanceJointDef joint_def;

    distance_j(CollObj* a, CollObj* b, b2DistanceJointDef joint_def);
};

struct wheel_j : public _joint
{
    b2WheelJointDef joint_def;

    wheel_j(CollObj* a, CollObj* b, b2WheelJointDef joint_def);
};

struct prismatic_j : public _joint
{
    b2PrismaticJointDef joint_def;

    prismatic_j(CollObj* a, CollObj* b, b2PrismaticJointDef joint_def);
};

struct weld_j : public _joint
{
    b2WeldJointDef joint_def;

    weld_j(CollObj* a, CollObj* b, b2WeldJointDef joint_def);
};

struct mouse_j : public _joint
{
    b2MouseJointDef joint_def;

    mouse_j(CollObj* a, CollObj* b, b2MouseJointDef joint_def);
};

struct motor_j : public _joint
{
    b2MotorJointDef joint_def;

    motor_j(CollObj* a, CollObj* b, b2MotorJointDef joint_def);
};

struct null_j : public _joint
{
    b2NullJointDef joint_def;

    null_j(CollObj* a, CollObj* b, b2NullJointDef joint_def);
};