/*
Code::Blocks 16.01
GNU GCC Compiler
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include "setShip.h"
#include "printMaps.h"
#include "shot.h"
#include "bot.h"

Player createHumanPlayer(){
    Player human;
    human.score = 0;
    clearMap(human.hits);//�������� �������� �����
    clearMap(human.ships);
    while(1){
        system("cls");
        printf("��� ���������� �������?\n 1 - �������� \n 2 - �������\n\n\n������� ����� ���������: a1\n\n");
        char n1[10], n2[10];
        scanf("%s", &n1);
        if (strlen(n1) > 1){
            printf("������������ ����\n");
            system("pause");
            continue;
        }
        strncpy(n2,n1,1);
        int n = atoi(n2);
        if (n == 1){
            randomShip(human.ships);//����������� ������� ��������
            break;
        }
        else if (n == 2){
            manually(human.ships, human.hits);//����������� ������� �������
            break;
        }
        else{
           printf("������������ ����\n");
           system("pause");
        }
    }
    return human;
}

Point humanInputCoordinates(Cell hits[][MAX_SIZE]){
    printf("������� ����������:\n");
    Point point;
    int x,y;
    char s[10], a[10];
    scanf("%s", &s);
    strncpy(a, s, 1);
    int sz = strlen(s);
    memmove(s, s+1, sz-1);
    s[sz-1] = 0;//������� ������ ������
    y = atoi(s) - 1;
    x = *a - 'a';
    if (inMap(x, y) != TRUE){
        printf("������� ������� ����������\n");
        point.x = -1;
        system("pause");
    }
    else if (hits[y][x].status != EMPTY){
        printf("���������� ����������\n");
        point.x = -1;
        system("pause");
    }else {
        point.x = x;
        point.y = y;
    }
    return point;
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);//��� ����������� ����������� ������� ���� � �������
    srand(time(NULL));//����� ��������� ������� ������
    Player human = createHumanPlayer();//������� ������ �������� � ����������� �������
    Bot bot = createBot(); //������� ���� � �������������� �������� ���������
    int x, y;//���������� �����, ������� ������ �����
    int flag;//��� �������� 0-���� 1-����� 2-������
    int xc, yc;//���������� �������� ����
    int turn = 1;//1 - ����� �����, 2 - ����� ���
    Point point;
    while(human.score < 10 && bot.score < 10){//10 - ��� ���������� ��������; ����� ����� ��� ��� ������� 10 �������� ���� �����������
        if (turn == 1){
            refreshMaps(human.ships, human.hits, human.score, bot.score);
            point = humanInputCoordinates(human.hits);//����� ������ ���������� ��� ��������
            y = point.y;
            x = point.x;
            if (x == -1)//���� ������� ������� ����������, �� �������� ���� �������
                continue;
            flag = shot(bot.maps.ships, human.hits, x, y);//���������� �������
            refreshMaps(human.ships, human.hits, human.score, bot.score);
            if (flag == 0){
                human.score++;//����������� ���������� ����� � ������
                printf("����!\n");
            }
            if (flag == 1){
                printf("�����!\n");
            }
            if (flag == 2){
                printf("������!\n");
                turn = 2;// ������������� ��� ���� �.�. �� �� ������
            }
            system("pause");
        }
        else {//��� ����
            point = botSelectsPoint(&bot);
            xc = point.x;
            yc = point.y;
            flag = shot(human.ships, bot.maps.hits, xc, yc);//���������� �������
            refreshMaps(human.ships, human.hits, human.score, bot.score);
            if (flag == 0){
                botKilled(&bot);
                printf("��������� ���� ��� �������!: %c%d\n", xc + 'a', yc + 1);
            }
            if (flag == 1){//��� �����
                if (bot.hit == 1)//���� �� ����� ��� ���� ���� ���������
                    botSecondHit(&bot, xc, yc, bot.maps.hits);
                if (bot.hit == 0)//���� ��� �� ���� ���������
                    botFirstHit(&bot, xc, yc);
                printf("��������� �����: %c%d\n", xc + 'a', yc + 1);
            }
            if (flag == 2){//��� �����������
                    printf("��������� �����������: %c%d\n", xc + 'a', yc + 1);
                    printf("��� ���\n");
                    turn = 1;
            }
            system("pause");
        }
    }
    system("cls");
    if (human.score == 10)
        printf("�� ��������!\n");
    if (bot.score == 10)
        printf("�� ���������!\n");
    return 0;
}
