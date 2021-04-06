#include "Map.h"


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

Map MAP(Column_Board, Row_Board);

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
                SDL_SetRenderDrawColor(renderer,55, 129, 81,255);
                rect.x = side_of_aUnit * j;
                rect.y = side_of_aUnit * i;
                rect.h = side_of_aUnit;
                rect.w = side_of_aUnit;
                SDL_RenderFillRect (ren,&rect);
            }
            if(MAP[i][j]== Fruit) {
                SDL_SetRenderDrawColor(renderer,164, 10, 73,255);
                rect.x = side_of_aUnit * j;
                rect.y = side_of_aUnit * i;
                rect.h = side_of_aUnit;
                rect.w = side_of_aUnit;
                SDL_RenderFillRect (renderer,&rect);
            }
            if(MAP[i][j]== SnakeHEAD) {
                SDL_SetRenderDrawColor(renderer,13, 20, 145,255);
                rect.x = side_of_aUnit * j;
                rect.y = side_of_aUnit * i;
                rect.h = side_of_aUnit;
                rect.w = side_of_aUnit;
                SDL_RenderFillRect (renderer,&rect);
            }
            if(MAP[i][j]== Wall) {
                SDL_SetRenderDrawColor(renderer,255, 0, 0,0);
                rect.x = side_of_aUnit * j;
                rect.y = side_of_aUnit * i;
                rect.h = side_of_aUnit;
                rect.w = side_of_aUnit;
                SDL_RenderFillRect (renderer,&rect);
            }
        }
    }
}
