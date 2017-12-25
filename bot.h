#ifndef BOT_H
#define BOT_H

#include "shot.h"
#include "printMaps.h"

typedef struct Recommend{// ���������� � ������� ������������� �������� ����
    Point point;
}Recommend;

typedef struct Bot{
    Recommend recommend[4];
    Player maps;
    int mode;//1 - ��������� �������, 2 - ���������
    int hit;//���� �� ��������� � ����
} Bot;

void botFirstHit(Bot *bot, int xc, int yc);
void botSecondHit(Bot *bot, int xc, int yc, Cell hits[10][10]);
void clearRec(Bot *bot);//�������� ������������� ����������
Bot createBot();
#endif // BOT_H
