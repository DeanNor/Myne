

#include ".hpp/drawobj.hpp"

class DrawTarget : public DrawObj
{
protected:
    std::vector<DrawObj*> drawers;

    pos origin = {0,0};

public:
    virtual void draw(const pos& global_origin) override;

    void add_to_draws(DrawObj* who)
    {
        drawers.push_back(who);
    }

    void remove_from_draws(DrawObj* who)
    {
        auto index = std::find(drawers.begin(), drawers.end(), who);

        if (index == drawers.end()) return;

        drawers.erase(index);
    }

    void set_origin(pos new_origin)
    {
        origin = new_origin;
    }

    pos get_origin() const
    {
        return origin;
    }

    pos get_zero()
    {
        return origin - half_size;
    }

    pos get_max()
    {
        return origin + half_size;
    }
};