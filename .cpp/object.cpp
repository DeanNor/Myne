
#include ".hpp/object.hpp"

void Object::load(Loader* load)
{
    Process::load(load);

    position = load->load_complex<pos>();

    angle = load->load_complex<rad>();
}

void Object::save(Saver* save) const
{
    Process::save(save);

    save->save_complex(position);

    save->save_complex(angle);
}

void Object::set_parent(Process* new_parent)
{
    Process::set_parent(new_parent);

    Object* p = dynamic_cast<Object*>(new_parent);
    if (p != nullptr)
    {
        global_transform.parent = p->get_tfm();
    }
}

tfm* Object::get_tfm()
{
    return &global_transform;
}

pos Object::get_global_position()
{
    return global_transform.compute();
}

void Object::set_position(pos new_pos)
{
    position = new_pos;
}

pos Object::get_position()
{
    return position;
}

void Object::set_angle(rad new_angle)
{
    angle = new_angle;
}

rad Object::get_angle()
{
    return angle;
}

rad Object::get_global_angle()
{
    return global_transform.compute_angle();
}
