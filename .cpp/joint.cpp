
#include ".hpp/joint.hpp"

#include ".hpp/collobj.hpp"

_joint::_joint(CollObj* a, CollObj* b, b2BodyId id_a, b2BodyId id_b)
{
    obj_a = a;
    obj_b = b;

    body_a = id_a;
    body_b = id_b;
}

_joint::operator bool()
{
    return b2Joint_IsValid(collision_joint);
}

revolute_j::revolute_j(CollObj* a, CollObj* b, b2RevoluteJointDef joint_def) : _joint(a,b,a->get_collision_body(),b->get_collision_body()), joint_def(joint_def)
{
    joint_def.base.bodyIdA = body_a;
    joint_def.base.bodyIdB = body_b;

    collision_joint = b2CreateRevoluteJoint(get_current_coll_world(), &joint_def);
}

distance_j::distance_j(CollObj* a, CollObj* b, b2DistanceJointDef joint_def) : _joint(a,b,a->get_collision_body(),b->get_collision_body()), joint_def(joint_def)
{
    joint_def.base.bodyIdA = body_a;
    joint_def.base.bodyIdB = body_b;

    collision_joint = b2CreateDistanceJoint(get_current_coll_world(), &joint_def);
}

wheel_j::wheel_j(CollObj* a, CollObj* b, b2WheelJointDef joint_def) : _joint(a,b,a->get_collision_body(),b->get_collision_body()), joint_def(joint_def)
{
    joint_def.base.bodyIdA = body_a;
    joint_def.base.bodyIdB = body_b;

    collision_joint = b2CreateWheelJoint(get_current_coll_world(), &joint_def);
}

prismatic_j::prismatic_j(CollObj* a, CollObj* b, b2PrismaticJointDef joint_def) : _joint(a,b,a->get_collision_body(),b->get_collision_body()), joint_def(joint_def)
{
    joint_def.base.bodyIdA = body_a;
    joint_def.base.bodyIdB = body_b;

    collision_joint = b2CreatePrismaticJoint(get_current_coll_world(), &joint_def);
}

weld_j::weld_j(CollObj* a, CollObj* b, b2WeldJointDef joint_def) : _joint(a,b,a->get_collision_body(),b->get_collision_body()), joint_def(joint_def)
{
    joint_def.base.bodyIdA = body_a;
    joint_def.base.bodyIdB = body_b;

    collision_joint = b2CreateWeldJoint(get_current_coll_world(), &joint_def);
}

mouse_j::mouse_j(CollObj* a, CollObj* b, b2MouseJointDef joint_def) : _joint(a,b,a->get_collision_body(),b->get_collision_body()), joint_def(joint_def)
{
    joint_def.base.bodyIdA = body_a;
    joint_def.base.bodyIdB = body_b;

    collision_joint = b2CreateMouseJoint(get_current_coll_world(), &joint_def);
}

motor_j::motor_j(CollObj* a, CollObj* b, b2MotorJointDef joint_def) : _joint(a,b,a->get_collision_body(),b->get_collision_body()), joint_def(joint_def)
{
    joint_def.base.bodyIdA = body_a;
    joint_def.base.bodyIdB = body_b;

    collision_joint = b2CreateMotorJoint(get_current_coll_world(), &joint_def);
}
