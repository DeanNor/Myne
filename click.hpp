
#pragma once

#include ".hpp/tfm.hpp"

class ClickManager;

struct click_rect
{
friend ClickManager;

private:
    tfm* scope;

    bool passthrough; // Pass click onto below objects as well

    std::function<void(void* owner, void* manager)> callback;
    void* caller;
    bool has_callback;

public:
    bool active = true;

    bool scales = true; // TODO decide on another way to make the Editorobj's scale when in the non-sprited form

    pos half_size;
    pos offset;

    click_rect() = default;

    click_rect(tfm* new_scope, pos new_half_size, pos new_offset, std::function<void(void* owner, void* manager)> function, void* owner, bool _passthrough) : scope(new_scope), half_size(new_half_size), offset(new_offset), passthrough(_passthrough), callback(function), caller(owner), has_callback(true)
    {

    }

    // Only to change hit to true, no callback called
    click_rect(tfm* new_scope, pos new_half_size, pos new_offset) : scope(new_scope), half_size(new_half_size), offset(new_offset), passthrough(true), has_callback(false)
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
        return (max.x >= click_pos.x and click_pos.x >= zero.x) and (max.y >= click_pos.y and click_pos.y >= zero.y); // TODO rotation!!!!!
    }

public:
    // Click pos as offset from game zero, not window zero
    void handle_click(pos click_pos, void* clicker, pos scale)
    {
        passthrough = true;
        hit = false;

        for (auto x : click_rects)
        {
            if (x->active)
            {
                pos computed = (x->scope->compute() + x->offset);

                pos size_scaled = x->scales ? x->half_size / scale : x->half_size;

                if (within(click_pos, computed - size_scaled, computed + size_scaled))
                {
                    if (x->has_callback)
                    {
                        x->callback(x->caller, clicker);

                        passthrough = false;
                    }

                    hit = true;

                    if (!x->passthrough)
                    {
                        return;
                    }
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

    void empty()
    {
        click_rects.clear();
    }
};