#ifndef EVENTHANDLE_H
#define EVENTHANDLE_H
#include <iostream>
#include "SDL_utils.h"


using namespace std;

void Event_Handle(bool &in_game, bool &running, snake &SNAKE);
enum Mouse {
    DOWN_LEFT,
    DOWN_RIGHT,
    UP_LEFT,
    UP_RIGHT,
    NOTHING
};

#endif // EVENTHANDLE_H
