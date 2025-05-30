#pragma once

class Presenter;

struct editor : public game
{
public:
    Presenter* middle_box; // Da ting in the middle of the screen

    editor();

    virtual bool frame();
};

editor* editing_thing = nullptr;

void set_editor(editor* thing_that_edits)
{
    editing_thing = thing_that_edits;
}

editor* get_editor()
{
    return editing_thing;
}

class Window : public Object
{
public:
    SDL_Texture* screen;
    SDL_Renderer* renderer;

    Window()
    {
        renderer = get_current_game()->game_window->renderer;
    }

    Window(pos size)
    {
        renderer = get_current_game()->game_window->renderer;
        screen = SDL_CreateTexture(renderer, SDL_FORMAT, SDL_TEXTUREACCESS_TARGET, size.x, size.y);

        SDL_SetRenderTarget(renderer, screen);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        SDL_SetRenderTarget(renderer, nullptr);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    }

    void set_screen(SDL_Texture* new_screen)
    {
        screen = new_screen;
    }

    SDL_Texture* get_screen()
    {
        return screen;
    }

    void wipe() // Clears the screen to the draw color
    {
        SDL_SetRenderTarget(renderer, screen);
        SDL_RenderClear(renderer);
        SDL_SetRenderTarget(renderer, nullptr);
    }

    void wipe(Uint8 r, Uint8 g, Uint8 b, Uint8 a) // Clears the screen to the color
    {
        SDL_SetRenderDrawColor(renderer, r,g,b,a);
        wipe();
    }
};

class Presenter : public Window
{
public:
    // TODO auto-size, not set size
    Presenter(pos size) : Window(size), center(size / 2), half_size(center)
    {
        game_window = get_current_game()->game_window;
    }

    pos center;
    pos half_size;

    display* game_window;

    void process(double tim)
    {
        position = game_window->half_size;

        // TODO if mouse clicked but not grabbed something
    }

    void present()
    {
        SDL_FRect rect_what = pos::Make_SDL_FRect(center, half_size);
        SDL_FRect rect_where = pos::Make_SDL_FRect(position, half_size);
        SDL_RenderTexture(renderer, screen, &rect_what, &rect_where);
    }
};

editor::editor()
{
    set_current_game(this); // Used by the presenter

    middle_box = new Presenter({900, 900});
    root->add_child(middle_box);
}

bool editor::frame()
{
    view_events();
    run_processes();

    middle_box->present();

    finish_processes();

    middle_box->wipe(255,255,100,255);
    SDL_SetRenderDrawColor(game_window->renderer, 0,0,0,255);

    return running;
}

class RepresentObj;

class EditorObj : public BlendObj
{
    REGISTER_OBJECT(EditorObj)

public:
    std::string represents;

    RepresentObj* representer = nullptr;

    bool has_position = false;
    pos represented_pos;

    RepresentObj* visible_obj = nullptr;

    static EditorObj* represent(std::string new_class)
    {
        EditorObj* obj = new EditorObj;

        obj->set_representation(new_class);

        return obj;
    }

    EditorObj();

    void set_representation(std::string new_class)
    {
        represents = new_class;
        // TODO update variables
    }

    std::string get_representation()
    {
        return represents;
    }

    void set_represented_pos(pos represented)
    {
        represented_pos = represented;
    }

    pos get_represented_pos()
    {
        return represented_pos;
    }

    bool is_drop_target(pos mouse)
    {
        // TODO decide if inside 'mouse'
        // really I need to do a drag and drop global object attached to the editor
    }

    ARCHIVE(BlendObj, represents)
};

#include "editortests.hpp"

class RepresentObj : public Dragable
{
public:
    editor* editor_window;

    EditorObj* controller;

    RepresentObj()
    {
        editor_window = get_editor();
    }

    void draw(pos) // TODO make this set up properly, honestly make it so there is a call heirerarchy for draws
    {
        SDL_SetRenderTarget(window->renderer, editor_window->middle_box->screen);
        Dragable::draw(editor_window->middle_box->center); // TODO redo draw call to use renderer stuff and clipping
        SDL_SetRenderTarget(window->renderer, nullptr);
    }

    virtual void dropped()
    {
        controller->set_represented_pos(position);
    }
};

EditorObj::EditorObj()
{
    representer = new RepresentObj; // TODO on_load stuff
    representer->controller = this;

    editor* editer = get_editor();
    editer->middle_box->add_child(representer);
}
