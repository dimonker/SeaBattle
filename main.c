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


int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);//��� ����������� ����������� ������� ���� � �������
    srand(time(NULL));//����� ��������� ������� ������

    Player human;//������� ������ ��������
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
            clearStatus(human.ships);//��������� �� ����� ������ �������
            break;
        }
        else if (n == 2){
            manually(human.ships, human.hits);//����������� ������� �������
            system("cls");
            clearStatus(human.ships);//��������� �� ����� ������ �������
            printMaps(human.ships, human.hits);
            break;
        }
        else{
           printf("������������ ����\n");
           system("pause");
        }
    }
    int x, y;//���������� ����� ������� ������ �������
    int flag;//��� �������� 0-���� 1-����� 2-������
    int xc, yc;//���������� �������� ����
    int j = 0;//���������� ��� ������� �� ������������� �������

    Bot bot = createBot(); //������� ����
    randomShip(bot.maps.ships); //�������� ����������� �������
    clearStatus(bot.maps.ships); // ��������� �� ����� ������ �������

    int turn = 1;//1 - ����� �������, 2 - ����� ���
    int score_human = 0, score_comp = 0;//������� ���������� ������ ��������
    while(score_human < 10 && score_comp < 10){
        if (turn == 1){
            refreshMaps(human.ships, human.hits, score_human, score_comp);
            printf("������� ����������:\n");
            char s[10], a[10];
            scanf("%s", &s);
            strncpy(a, s, 1);
            int sz = strlen(s);
            memmove(s, s+1, sz-1);
            s[sz-1] = 0;//������� ������ ������
            y = atoi(s) - 1;
            x = *a - 'a';
            if (inMap(x, y) != TRUE || sz > 3){
                printf("������� ������� ����������\n");
                system("pause");
                continue;
            }
            if (human.hits[y][x].status != EMPTY){
                printf("���������� ����������\n");
                system("pause");
                continue;
            }
            flag = shot(bot.maps.ships, human.hits, x, y);//���������� �������
            if (flag == 0){
                score_human++;//����������� ���������� ����� � ������
                refreshMaps(human.ships, human.hits, score_human, score_comp);
                printf("����!\n");
                system("pause");
            }
            if (flag == 1){
                refreshMaps(human.ships, human.hits, score_human, score_comp);
                printf("�����!\n");
                system("pause");
            }
            if (flag == 2){
                refreshMaps(human.ships, human.hits, score_human, score_comp);
                printf("������!\n");
                turn = 2;// ������������� ��� ���� �.�. �� �� ������
                system("pause");
            }
        }
        else {//��� ����
            if (bot.mode == 1){//��������� �������
                xc = rand() % 10;//��� �� ���� ��������� ������� �������� ��������� ����������
                yc = rand() % 10;
            }
            else {//mode == 2 ���������
                if (bot.hit != 0){
                    if (bot.recommend[j].point.x == -1){//��������� ���� �� �������� � ������������� �����
                        j++;   //���� ���, �� ��������� � ��������� �����
                        continue;
                    }
                    xc = bot.recommend[j].point.x;
                    yc = bot.recommend[j].point.y;
                    j++;
                    if (inMap(xc, yc) == FALSE){//���� ���������� �� � �������� ����� �� ��������� � ��������� ������������� ����������
                        continue;
                    }
                }
            }
            if (bot.maps.hits[yc][xc].status != EMPTY){//���� ��� �������� �� ��� ���������� ������
                continue;//���������� ������ ����������
            }
            flag = shot(human.ships, bot.maps.hits, xc, yc);//���������� �������
            if (flag == 0){
                score_comp++;//����������� ���������� ����� ����
                clearRec(&bot);
                j = 0;  //������� ���������� ��� ������� �� ������������� ������
                bot.mode = 1; // ��������� � ����� ��������� ��������
                bot.hit = 0; //��������� �������� ��������
                refreshMaps(human.ships, human.hits, score_human, score_comp);
                printf("��������� ���� ��� �������!: %c%d\n", xc + 'a', yc + 1);
                system("pause");
            }
            if (flag == 1){//��� �����
                if (bot.hit == 1){//���� �� ����� ��� ���� ���� ���������
                    int k;
                    clearRec(&bot);//������� �� ���������� ��������
                    botSecondHit(&bot, xc, yc, bot.maps.hits);
                    j = 0;//������� ���������� ��� ������� �� ������������� ������
                    bot.hit = 2;//���� ��� ��� ��� ������ ���������
                }
                if (bot.hit == 0){//���� ��� �� ���� ���������

                    botFirstHit(&bot, xc, yc);
                    bot.hit = 1;//���� ���� ���������

                }
                bot.mode = 2;
                refreshMaps(human.ships, human.hits, score_human, score_comp);
                printf("��������� �����: %c%d\n", xc + 'a', yc + 1);
                system("pause");
            }
            if (flag == 2){//��� �����������
                    refreshMaps(human.ships, human.hits, score_human, score_comp);
                    printf("��������� �����������\n");
                    printf("��� ���\n");
                    turn = 1;
                    system("pause");
                    system("cls");
            }
        }
    }
    system("cls");
    if (score_human == 10){
        printf("�� ��������!\n");
    }
    if (score_comp == 10){
        printf("�� ���������!\n");
    }
    return 0;
}
