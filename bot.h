#ifndef BOT_H
#define BOT_H

#include "shot.h"
#include "printMaps.h"

typedef struct Recommend{// координаты в которые рекомендуется стрелять боту
    Point point;
}Recommend;

typedef struct Bot{
    Recommend recommend[4];
    Player maps;
    int mode;//1 - рандомный выстрел, 2 - добивание
    int hit;//было ли попадание у бота
} Bot;

void botFirstHit(Bot *bot, int xc, int yc);
void botSecondHit(Bot *bot, int xc, int yc, Cell hits[10][10]);
void clearRec(Bot *bot);//очистить рекомендуемые координаты
Bot createBot();
#endif // BOT_H
