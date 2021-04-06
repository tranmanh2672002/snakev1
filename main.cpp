#include <iostream>
#include <vector>
#include <ctime>
#include <cmath>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include "SDL_utils.h"

using namespace std;


SDL_Window* window;
SDL_Renderer* renderer;
SDL_Surface* Gameover = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Event event;

int SCREEN_WIDTH = 960;
int SCREEN_HEIGHT = 640;
int side_of_aUnit=20;

int Column_Board=SCREEN_WIDTH/side_of_aUnit, // 48 cot
    Row_Board=SCREEN_HEIGHT/side_of_aUnit;   // 32 hang

struct Point{
    int x, y;
};

enum object
{
    Snake,
    SnakeHEAD,
    Wall,
    Fruit,
    Blank
};

enum Direction
{
    Freeze,
    Up,
    Down,
    Left,
    Right
} ;

enum Mouse {
    DOWN_LEFT,
    DOWN_RIGHT,
    UP_LEFT,
    UP_RIGHT,
    NOTHING
};

Direction old_DIRECTION=Freeze;

class snake {
public:
    Point HEAD;
    vector <Point> body;
    void event_handle(SDL_Event &event, bool &in_game, bool &running);
    void Move(Direction &old_DIRECTION, int column, int row);
    void update_in_array();
    bool eatFruit(Point fruit);
    void getLonger();
    Direction DIRECTION = Freeze;
    bool CRASH();
private:
    Point old_Point;
    int body_CELL=1;
};

class Map {
public:
    int row, column;

    Map(int getCOL, int getROW) {
        row = getROW; column = getCOL;
    }
    vector<vector<object>> board;
    void create_Map();
    Point getFruit(snake SNAKE);
    Point fruit;
    vector<vector<object>> show_in_2Darray(snake SNAKE);
};
Map MAP(Column_Board, Row_Board);

void render_with_2Darray(vector<vector<object>> MAP, int side_of_aUnit, SDL_Renderer *ren);

// handle

void Event_Handle(bool &in_game, bool &running, snake &SNAKE);

// main
int main(int argc, char* argv[])
{
    const int time_per_frame=1000/60;
    initSDL(window, renderer, SCREEN_WIDTH, SCREEN_HEIGHT, "Snake handsome" );
    gScreenSurface = SDL_GetWindowSurface(window);
    Gameover = IMG_Load("loading.jpg");
    SDL_BlitSurface(Gameover, NULL, gScreenSurface, NULL);
    SDL_UpdateWindowSurface(window);
    SDL_Delay(3000);

    bool in_game=1, running = 1;

    while(running) {
        snake *SNAKE = new snake;
        MAP.fruit = MAP.getFruit(*SNAKE);

        (*SNAKE).HEAD = {Column_Board/2, Row_Board/2};
        (*SNAKE).body.push_back({Column_Board/2-1, Row_Board/2});

    int count=0;
        while(in_game) {
            int start_time=SDL_GetTicks();

            SDL_SetRenderDrawColor(renderer, 0, 0, 0,255);
            SDL_RenderClear(renderer);

            MAP.create_Map();

            Event_Handle(in_game, running, *SNAKE);
            if(SNAKE->CRASH())
            {
                gScreenSurface = SDL_GetWindowSurface(window);
                Gameover = IMG_Load("gameover.jpg");
                SDL_BlitSurface(Gameover, NULL, gScreenSurface, NULL);
                SDL_UpdateWindowSurface(window);
                cin>>running; break;
            }

            if( SNAKE->eatFruit(MAP.fruit) ) {
                SNAKE->getLonger();
                MAP.fruit = MAP.getFruit(*SNAKE);
                cout<<++count<<endl;
            }
            SNAKE->Move(old_DIRECTION, Column_Board, Row_Board);
    //        SNAKE->update_in_array();
            vector<vector<object>> update_Map = MAP.show_in_2Darray(*SNAKE);

            render_with_2Darray(update_Map, side_of_aUnit, renderer);
            SDL_RenderPresent(renderer);

            int mainloop_time = SDL_GetTicks() - start_time;
            if(mainloop_time < time_per_frame) ;
            SDL_Delay(125);

        }


        delete SNAKE;

    }

    quitSDL(window, renderer, Gameover);

    return 0;
}

void snake :: event_handle(SDL_Event &event, bool &in_game, bool &running)
{
    if((event).type == SDL_KEYDOWN) {
        switch( (event).key.keysym.sym ) {
            case SDLK_UP: DIRECTION = Up; break;
            case SDLK_DOWN: DIRECTION = Down; break;
            case SDLK_LEFT: DIRECTION = Left; break;
            case SDLK_RIGHT: DIRECTION = Right; break;
            case SDLK_ESCAPE: in_game = false; running =0; break;
            default: DIRECTION = Freeze; break;
        }
    }
}

void snake::Move(Direction &old_DIRECTION, int column, int row)
{
    old_Point = HEAD;
    while(true) {
        if(DIRECTION == Right && old_DIRECTION != Left ) {
            HEAD.x = (HEAD.x + 1)% column;
            old_DIRECTION = DIRECTION; break;
        }
        else if(DIRECTION == Left && old_DIRECTION != Right) {
            HEAD.x = (column + HEAD.x -1) % column;
            old_DIRECTION = DIRECTION;  break;
        }
        else if(DIRECTION == Down && old_DIRECTION != Up) {
            HEAD.y = (HEAD.y + 1) % row;
            old_DIRECTION = DIRECTION;  break;
        }
        else if(DIRECTION == Up && old_DIRECTION != Down) {
            HEAD.y = (row + HEAD.y - 1) % row;
            old_DIRECTION = DIRECTION;  break;
        }
        else if(DIRECTION==Freeze) {
            old_DIRECTION = DIRECTION; break;
        }
        else DIRECTION = old_DIRECTION;
    }
//    body.resize(body_CELL);
    if(DIRECTION != Freeze)
        for(int i=0;i<body.size();i++) {
            Point tmp_point = body[i];
            body [i] = old_Point;
            old_Point = tmp_point;
        }
}

bool snake::eatFruit(Point fruit)
{
    if(fruit.x == HEAD.x && fruit.y == HEAD.y) return 1;
    else return 0;
}

void snake::getLonger()
{
    body_CELL++;
    body.resize(body_CELL);
}

bool snake::CRASH() {
        for(int i=0;i<body.size();i++){
        if( (HEAD.x==body[i].x && HEAD.y==body[i].y) ) {cout<<"happened"<<endl; DIRECTION =Freeze; return 1;}
    }
    if(HEAD.x == 1 || HEAD.y == 1 || HEAD.x == Column_Board-6|| HEAD.y == Row_Board-6) {cout<<"happened"<<endl; DIRECTION =Freeze; return 1;}
    return 0;
}

// Map
void Map::create_Map()
{
    board.resize(row);
    for(int i=0;i<row;i++) board[i].assign(column, Blank);
}

Point Map::getFruit(snake SNAKE)
{
    srand(time(0));

    Point fruit;
    bool loop = false;
    do {
        fruit.x = rand()% ((Column_Board-6)-2 +1)+2;

        fruit.y = rand()% ((Row_Board-6)-2 +1)+2;

        for(int i=0;i<SNAKE.body.size();i++) {
            if((fruit.x == SNAKE.body[i].x && fruit.y == SNAKE.body[i].y) ||
                    (fruit.x == SNAKE.HEAD.x && fruit.y == SNAKE.HEAD.y)) {
                loop = true;
                break;
            }
        }
    }
    while(loop);
    return fruit;
}



vector<vector<object>> Map::show_in_2Darray(snake SNAKE)
{
    board[SNAKE.HEAD.y][SNAKE.HEAD.x] = SnakeHEAD;
    for(int i=0;i<SNAKE.body.size();i++) {
        board [SNAKE.body[i].y][SNAKE.body[i].x] = Snake;
    }
    board[fruit.y][fruit.x] = Fruit;

    for (int i=0; i<Row_Board-4; i++)
    {
        for (int j=0; j<Column_Board-4; j++)
        {
            if (i==0) board[i][j] = Wall;
            if (j==0) board[i][j] = Wall;
            if (j==Column_Board-5) board[i][j] = Wall;
            if (i==Row_Board-5) board[i][j] = Wall;

        }
    }

    return board;
}


void render_with_2Darray(vector<vector<object>> MAP, int side_of_aUnit, SDL_Renderer *ren)
{

    for(int i=0;i<MAP.size();i++) {
        for(int j=0;j<MAP[0].size();j++)
        {
            SDL_Rect rect;
            if(MAP[i][j]== Snake) {
                SDL_SetRenderDrawColor(renderer, 192, 192, 192 ,255);
                rect.x = side_of_aUnit * j;
                rect.y = side_of_aUnit * i;
                rect.h = side_of_aUnit;
                rect.w = side_of_aUnit;
                SDL_RenderFillRect (ren,&rect);
            }
            if(MAP[i][j]== Fruit) {
                SDL_SetRenderDrawColor(renderer,0, 255, 0,0);
                rect.x = side_of_aUnit * j;
                rect.y = side_of_aUnit * i;
                rect.h = side_of_aUnit;
                rect.w = side_of_aUnit;
                SDL_RenderFillRect (renderer,&rect);
            }
            if(MAP[i][j]== SnakeHEAD) {
                SDL_SetRenderDrawColor(renderer,255, 255, 255,255);
                rect.x = side_of_aUnit * j;
                rect.y = side_of_aUnit * i;
                rect.h = side_of_aUnit;
                rect.w = side_of_aUnit;
                SDL_RenderFillRect (renderer,&rect);
            }
            if(MAP[i][j]== Wall) {
                SDL_SetRenderDrawColor(renderer, 128, 0, 0 ,255);
                rect.x = side_of_aUnit * j;
                rect.y = side_of_aUnit * i;
                rect.h = side_of_aUnit;
                rect.w = side_of_aUnit;
                SDL_RenderFillRect (renderer,&rect);
            }
        }
    }
}

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
