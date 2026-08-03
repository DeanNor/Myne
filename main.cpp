
#include ".hpp/game.hpp"
#include ".hpp/process.hpp"
#include "SDL3/SDL_video.h"

int main()
{
    game gameplay("HI", SDL_WINDOW_RESIZABLE, {1500,800});
    set_current_game(&gameplay);

    gameplay.set_root(new Process);

    gameplay.start();

    gameplay.get_root()->del();
    
    return 0;
}