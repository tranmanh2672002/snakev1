#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include <iostream>
#include <vector>

using namespace std;

enum object
{
    Snake,
    SnakeHEAD,
    Wall,
    Fruit,
    Blank
};

struct Point{
    int x, y;

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

void render_with_2Darray();

#endif // MAP_H_INCLUDED
