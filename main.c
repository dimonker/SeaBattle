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
    clearMap(human.hits);//обнуляем значения карты
    clearMap(human.ships);
    while(1){
        system("cls");
        printf("Как расставить корабли?\n 1 - случайно \n 2 - вручную\n\n\nПримеры ввода координат: a1\n\n");
        char n1[10], n2[10];
        scanf("%s", &n1);
        if (strlen(n1) > 1){
            printf("Некорректный ввод\n");
            system("pause");
            continue;
        }
        strncpy(n2,n1,1);
        int n = atoi(n2);
        if (n == 1){
            randomShip(human.ships);//расставляем корабли рандомно
            break;
        }
        else if (n == 2){
            manually(human.ships, human.hits);//расставляем корабли вручную
            break;
        }
        else{
           printf("Некорректный ввод\n");
           system("pause");
        }
    }
    return human;
}

Point humanInputCoordinates(Cell hits[][MAX_SIZE]){
    printf("Введите координаты:\n");
    Point point;
    int x,y;
    char s[10], a[10];
    scanf("%s", &s);
    strncpy(a, s, 1);
    int sz = strlen(s);
    memmove(s, s+1, sz-1);
    s[sz-1] = 0;//удаляем первый символ
    y = atoi(s) - 1;
    x = *a - 'a';
    if (inMap(x, y) != TRUE){
        printf("Неверно введены координаты\n");
        point.x = -1;
        system("pause");
    }
    else if (hits[y][x].status != EMPTY){
        printf("Невозможно выстрелить\n");
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
    SetConsoleOutputCP(1251);//для корректного изображения русских букв в консоле
    srand(time(NULL));//чтобы нормально работал рандом
    Player human = createHumanPlayer();//создаем игрока человека и расставляем корабли
    Bot bot = createBot(); //создаем бота с расставленными рандомно кораблями
    int x, y;//координаты точки, которые вводит игрок
    int flag;//для выстрела 0-убил 1-попал 2-промах
    int xc, yc;//координаты выстрела бота
    int turn = 1;//1 - ходит игрок, 2 - ходит бот
    Point point;
    while(human.score < 10 && bot.score < 10){//10 - это количество кораблей; когда игрок или бот убивают 10 кораблей игра завершается
        if (turn == 1){
            refreshMaps(human.ships, human.hits, human.score, bot.score);
            point = humanInputCoordinates(human.hits);//игрок вводит координаты для стрельбы
            y = point.y;
            x = point.x;
            if (x == -1)//если неверно введены координаты, то начинаем цикл сначала
                continue;
            flag = shot(bot.maps.ships, human.hits, x, y);//производим выстрел
            refreshMaps(human.ships, human.hits, human.score, bot.score);
            if (flag == 0){
                human.score++;//увеличиваем количество очков у игрока
                printf("Убил!\n");
            }
            if (flag == 1){
                printf("Попал!\n");
            }
            if (flag == 2){
                printf("Промах!\n");
                turn = 2;// предоставляем ход боту т.к. мы не попали
            }
            system("pause");
        }
        else {//ход бота
            point = botSelectsPoint(&bot);
            xc = point.x;
            yc = point.y;
            flag = shot(human.ships, bot.maps.hits, xc, yc);//производим выстрел
            refreshMaps(human.ships, human.hits, human.score, bot.score);
            if (flag == 0){
                botKilled(&bot);
                printf("Противник убил ваш корабль!: %c%d\n", xc + 'a', yc + 1);
            }
            if (flag == 1){//бот попал
                if (bot.hit == 1)//если до этого уже было одно попадание
                    botSecondHit(&bot, xc, yc, bot.maps.hits);
                if (bot.hit == 0)//если еще не было попаданий
                    botFirstHit(&bot, xc, yc);
                printf("Противник попал: %c%d\n", xc + 'a', yc + 1);
            }
            if (flag == 2){//бот промахнулся
                    printf("Противник промахнулся: %c%d\n", xc + 'a', yc + 1);
                    printf("Ваш ход\n");
                    turn = 1;
            }
            system("pause");
        }
    }
    system("cls");
    if (human.score == 10)
        printf("Вы победили!\n");
    if (bot.score == 10)
        printf("Вы проиграли!\n");
    return 0;
}
