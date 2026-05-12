#include "player.hpp"

Object* current_player = nullptr;

pos PLAYER_VEL;

void set_player(Object* new_player)
{
    current_player = new_player;
}

Object* get_player()
{
    return current_player;
}