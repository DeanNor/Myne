
#pragma once

#include ".hpp/tfm.hpp"

enum click_t
{
    TEST,
    LEFT,
    RIGHT,
};

struct click_rect
{
    tfm* scope;
    pos half_size;
    pos offset;

    bool passthrough; // Pass click onto below objects as well

    std::function<void(void* owner, void* manager)> callback;
    void* caller;

    bool active = true;

    click_rect() = default;

    click_rect(tfm* new_scope, pos new_half_size, pos new_offset, std::function<void(void* owner, void* manager)> function, void* owner) : scope(new_scope), half_size(new_half_size), offset(new_offset), passthrough(false), callback(function), caller(owner)
    {

    }

    click_rect(tfm* new_scope, pos new_half_size, pos new_offset) : scope(new_scope), half_size(new_half_size), offset(new_offset), passthrough(true)
    {

    }
};

class ClickManager
{
private:
    std::vector<click_rect*> click_rects;

    // If click was not within a rect that stopped things when handled
    bool passthrough = true;

    // If the click hit any rect, even one with passthrough, when handled
    bool hit = false;

    bool within(const pos& click_pos, pos zero, pos max)
    {
        return (max.x >= click_pos.x and click_pos.x >= zero.x) and (max.y >= click_pos.y and click_pos.y >= zero.y);
    }

public:

    // Click pos as offset from game zero, not window zero
    void handle_click(pos click_pos, void* clicker)
    {
        passthrough = true;
        hit = false;

        for (auto x : click_rects)
        {
            if (x->active)
            {
                pos computed = x->scope->compute() + x->offset;
                if (within(click_pos, computed - x->half_size, computed + x->half_size))
                {
                    if (!x->passthrough)
                    {
                        x->callback(x->caller, clicker);

                        passthrough = false;
                        hit = true;
                        return;
                    }

                    hit = true;
                }
            }
        }
    }

    void add_to_clicks(click_rect* to_add)
    {
        click_rects.push_back(to_add);
    }

    void remove_from_clicks(click_rect* who)
    {
        auto index = std::find(click_rects.begin(), click_rects.end(), who);

        if (index == click_rects.end()) return;

        click_rects.erase(index);
    }

    bool passedthrough()
    {
        return passthrough;
    }

    bool was_hit()
    {
        return hit;
    }
};