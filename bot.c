#include "shot.h"
#include "printMaps.h"

typedef struct Recommend{// ���������� � ������� ������������� �������� ����
    Point point;
}Recommend;

typedef struct Bot{
    Recommend recommend[4];
    Player maps;
    int score;
    int mode;//1 - ��������� �������, 2 - ���������
    int hit;//���� �� ��������� � ����
    int iterator;//��� ������ ������������� �����
} Bot;

void clearRec(Bot *bot){//������� ������������ ������ ��� ����
    int i;
    for (i = 0; i < 4; i++){
        bot->recommend[i].point.x = -1;
        bot->recommend[i].point.y = -1;
    }
}

void botFirstHit(Bot *bot, int xc, int yc){
        bot->hit = 1;//��� ����� ���� ���
        bot->mode = 2;//��� ��������� � ����� ���������
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

void botSecondHit(Bot *bot, int xc, int yc, Cell hits[MAX_SIZE][MAX_SIZE]){
        /* ������:
            01234567
            0|00000000
            1|00000000 3(INJURED) - ������ � ������� ������
            2|0rr33rr0 r - ������������� ����� ��� ��������
            3|00000000
            4|00000000
                    */
        clearRec(bot);//������� �� ���������� ��������
        bot->hit = 2;//���� ��� ��� ��� ������ ���������
        bot->iterator = 0;//������� ���������� ��� ������� �� ������������� ������
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

void botKilled(Bot *bot){
    bot->score++;//����������� ���������� ����� ����
    clearRec(bot);//�������� ������������� �����
    bot->mode = 1; // ��������� � ����� ��������� ��������
    bot->hit = 0; //�������� �������� ��������
    bot->iterator = 0;  //������� ���������� ��� ������� �� ������������� ������
}

Point botSelectsPoint(Bot *bot){
    Point point;
    int xc, yc;
    M2:
    if (bot->mode == 1){//��������� �������
                xc = rand() % MAX_SIZE;//��� �� ���� ��������� ������� �������� ��������� ����������
                yc = rand() % MAX_SIZE;
            }
            else if (bot->hit != 0){//mode = 2 ����� ���������
                    if (bot->recommend[bot->iterator].point.x == -1){//��������� ���� �� �������� � ������������� �����
                        bot->iterator++;   //���� ���, �� ��������� � ��������� �����
                        goto M2;
                    }
                    xc = bot->recommend[bot->iterator].point.x;
                    yc = bot->recommend[bot->iterator].point.y;
                    bot->iterator++;
            }
    if (bot->maps.hits[yc][xc].status != EMPTY)//���� ��� �������� �� ��� ���������� ������
        goto M2;//���������� ������ ����������
    point.x = xc;
    point.y = yc;
    return point;
}

Bot createBot(){//������� ����
    Bot bot;
    bot.mode = 1;
    bot.hit = 0;
    bot.score = 0;
    bot.iterator = 0;
    clearRec(&bot); //������� ������������� �����
    clearMap(bot.maps.ships); //������� �������� ����
    clearMap(bot.maps.hits); //
    randomShip(bot.maps.ships); //�������� ����������� �������
    return bot;
}

