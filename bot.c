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


void botFirstHit(Bot *bot, int xc, int yc){
        int f = 1, r, arr[4], i;
        arr[0] = rand() % 4;
        while (f < 4){
            M1: arr[f] = rand() % 4;
            for (i = 0; i < f; i++){
                if(arr[f] == arr[i])
                    goto M1;//���� ����� �����������, �� ������ ���
            }
            f++;
        }//���������� ������ �� 4 ��������� ����� �� 0 �� 3 ����� ������������� ������ ���������� ��������
            /*   ������:
              0123456
            0|0000000
            1|00r0000 3 (INJURED) - ������ � ������� ������
            2|0r3r000 r - ������������� ������ ��� ��������
            3|00r0000
            4|0000000
                     */
        if (inMap(xc + 1, yc) == TRUE){
            bot->recommend[arr[0]].point.x = xc + 1;
            bot->recommend[arr[0]].point.y = yc;
        }
        if (inMap(xc - 1, yc) == TRUE){
            bot->recommend[arr[1]].point.x = xc - 1;
            bot->recommend[arr[1]].point.y = yc;
        }
        if (inMap(xc, yc + 1) == TRUE){
            bot->recommend[arr[2]].point.x = xc;
            bot->recommend[arr[2]].point.y = yc + 1;
        }
        if (inMap(xc, yc - 1) == TRUE){
            bot->recommend[arr[3]].point.x = xc;
            bot->recommend[arr[3]].point.y = yc - 1;
        }
}

void botSecondHit(Bot *bot, int xc, int yc, Cell hits[10][10]){
        /* ������:
            01234567
            0|00000000
            1|00000000 3(INJURED) - ������ � ������� ������
            2|0rr33rr0 r - ������������� ����� ��� ��������
            3|00000000
            4|00000000

                    */
        if (inMap(xc+1,yc) == TRUE){
            if(hits[yc][xc+1].status == INJURED){//���� ������ ���������� ���������
                bot->recommend[0].point.x = xc - 1;
                bot->recommend[0].point.y = yc;
                bot->recommend[1].point.x = xc + 2;
                bot->recommend[1].point.y = yc;
                bot->recommend[2].point.x = xc - 2;
                bot->recommend[2].point.y = yc;
                bot->recommend[3].point.x = xc + 3;
                bot->recommend[3].point.y = yc;

            }

        }
        if (inMap(xc-1,yc) == TRUE){
            if(hits[yc][xc-1].status == INJURED){//���� ����� ���������� ���������
                bot->recommend[0].point.x = xc - 2;
                bot->recommend[0].point.y = yc;
                bot->recommend[1].point.x = xc + 1;
                bot->recommend[1].point.y = yc;
                bot->recommend[2].point.x = xc - 3;
                bot->recommend[2].point.y = yc;
                bot->recommend[3].point.x = xc + 2;
                bot->recommend[3].point.y = yc;
            }
        }
        if (inMap(xc,yc+1) == TRUE){
            if(hits[yc+1][xc].status == INJURED){//���� ����� ���������� ���������
                bot->recommend[0].point.x = xc;
                bot->recommend[0].point.y = yc + 2;
                bot->recommend[1].point.x = xc;
                bot->recommend[1].point.y = yc - 1;
                bot->recommend[2].point.x = xc;
                bot->recommend[2].point.y = yc - 2;
                bot->recommend[3].point.x = xc;
                bot->recommend[3].point.y = yc + 3;
            }
        }
        if (inMap(xc,yc-1) == TRUE){
            if(hits[yc-1][xc].status == INJURED){//���� ������ ���������� ���������
                bot->recommend[0].point.x = xc;
                bot->recommend[0].point.y = yc - 2;
                bot->recommend[1].point.x = xc;
                bot->recommend[1].point.y = yc + 1;
                bot->recommend[2].point.x = xc;
                bot->recommend[2].point.y = yc - 3;
                bot->recommend[3].point.x = xc;
                bot->recommend[3].point.y = yc + 2;
            }
        }
}


void clearRec(Bot *bot){//������� ������������ ������ ��� ����
    int i;
    for (i = 0; i < 4; i++){
        bot->recommend[i].point.x = -1;
        bot->recommend[i].point.y = -1;
    }
}

Bot createBot(){//������� ����
    Bot bot;
    bot.mode = 1;
    bot.hit = 0;
    clearRec(&bot); //������� ������������� �����
    clearMap(bot.maps.ships); //������� �������� ����
    clearMap(bot.maps.hits); //
    return bot;
}

