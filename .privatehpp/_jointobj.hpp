
#pragma once

#include "b2.h"

class CollObj;

struct _Joint // Parent Joint Struct
{
public:
    b2JointId collision_joint;

    CollObj* obj_a;
    b2BodyId body_a;

    CollObj* obj_b;
    b2BodyId body_b;

    _Joint(CollObj* a, CollObj* b);
};

struct RevoluteJoint : public _Joint
{
    b2RevoluteJointDef joint_def;
};

struct DistanceJoint : public _Joint
{
    b2DistanceJointDef joint_def;
};

struct WheelJoint : public _Joint
{
    b2WheelJointDef joint_def;
};

struct PrismaticJoint : public _Joint
{
    b2PrismaticJointDef joint_def;
};

struct WeldJoint : public _Joint
{
    b2WeldJointDef joint_def;
};

struct MouseJoint : public _Joint
{
    b2MouseJointDef joint_def;
};

struct MotorJoint : public _Joint
{
    b2MotorJointDef joint_def;
};