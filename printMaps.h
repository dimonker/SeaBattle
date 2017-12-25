#ifndef PRINTMAPS_H
#define PRINTMAPS_H

#include "windows.h"

#define EMPTY 0
#define SHIP 1
#define MISS 2
#define INJURED 3
#define KILLED 4
#define MAX_SIZE 10
#define MIN_SIZE 0
#define HORIZONTALLY 'h'
#define VERTICALLY 'v'

enum ConsoleColor;
void printColor(int x);

typedef struct Point{
    int x,y; //координаты начала корабля нужны для определения убийства корабля
}Point;

typedef struct Cell{
    Point point; //начальные координаты корабля
    int status; //значение ячейки
    int length; //длина корабля
    char direction; //направление корабля
}Cell;

typedef struct Player{
    Cell ships[10][10];
    Cell hits[10][10];
}Player;

void clearMap(Cell map[][10]);
void clearStatus(Cell map[][10]);
int inMap(int x, int y);

void printMaps(Cell ships[][10], Cell hits[][10]);
void refreshMaps(Cell ships[10][10], Cell hits[10][10], int score_human, int score_comp);

#endif // PRINTMAPS_H
