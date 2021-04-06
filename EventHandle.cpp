#include "event_Handle.h"

void Event_Handle(bool &in_game, bool &running, snake &SNAKE)
{
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        if(event.type == SDL_QUIT) {
            running = 0;
            return;
        }
        SNAKE.event_handle(event, in_game, running);
    }
}
