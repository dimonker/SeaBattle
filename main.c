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
    SetConsoleOutputCP(1251);//для корректного изображения русских букв в консоле
    srand(time(NULL));//чтобы нормально работал рандом

    Player human;//создаем игрока человека
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
            clearStatus(human.ships);//оставляем на карте только корабли
            break;
        }
        else if (n == 2){
            manually(human.ships, human.hits);//расставляем корабли вручную
            system("cls");
            clearStatus(human.ships);//оставляем на карте только корабли
            printMaps(human.ships, human.hits);
            break;
        }
        else{
           printf("Некорректный ввод\n");
           system("pause");
        }
    }
    int x, y;//координаты точки которые вводит человек
    int flag;//для выстрела 0-убил 1-попал 2-промах
    int xc, yc;//координаты выстрела бота
    int j = 0;//переменная для прохода по рекомендуемым клеткам

    Bot bot = createBot(); //создаем бота
    randomShip(bot.maps.ships); //рандомно расставляем корабли
    clearStatus(bot.maps.ships); // оставляем на карте только корабли

    int turn = 1;//1 - ходит человек, 2 - ходит бот
    int score_human = 0, score_comp = 0;//считаем количество убитых кораблей
    while(score_human < 10 && score_comp < 10){
        if (turn == 1){
            refreshMaps(human.ships, human.hits, score_human, score_comp);
            printf("Введите координаты:\n");
            char s[10], a[10];
            scanf("%s", &s);
            strncpy(a, s, 1);
            int sz = strlen(s);
            memmove(s, s+1, sz-1);
            s[sz-1] = 0;//удаляем первый символ
            y = atoi(s) - 1;
            x = *a - 'a';
            if (inMap(x, y) != TRUE || sz > 3){
                printf("Неверно введены координаты\n");
                system("pause");
                continue;
            }
            if (human.hits[y][x].status != EMPTY){
                printf("Невозможно выстрелить\n");
                system("pause");
                continue;
            }
            flag = shot(bot.maps.ships, human.hits, x, y);//производим выстрел
            if (flag == 0){
                score_human++;//увеличиваем количество очков у игрока
                refreshMaps(human.ships, human.hits, score_human, score_comp);
                printf("Убил!\n");
                system("pause");
            }
            if (flag == 1){
                refreshMaps(human.ships, human.hits, score_human, score_comp);
                printf("Попал!\n");
                system("pause");
            }
            if (flag == 2){
                refreshMaps(human.ships, human.hits, score_human, score_comp);
                printf("Промах!\n");
                turn = 2;// предоставляем ход боту т.к. мы не попали
                system("pause");
            }
        }
        else {//ход бота
            if (bot.mode == 1){//рандомный выстрел
                xc = rand() % 10;//еще не было попадания поэтому выбираем рандомные координаты
                yc = rand() % 10;
            }
            else {//mode == 2 добивание
                if (bot.hit != 0){
                    if (bot.recommend[j].point.x == -1){//проверяем есть ли значения у рекомендуемой точки
                        j++;   //если нет, то переходим к следующей точке
                        continue;
                    }
                    xc = bot.recommend[j].point.x;
                    yc = bot.recommend[j].point.y;
                    j++;
                    if (inMap(xc, yc) == FALSE){//если координаты не в пределах карты то переходим к следующей рекомендуемой координате
                        continue;
                    }
                }
            }
            if (bot.maps.hits[yc][xc].status != EMPTY){//если бот стреляет по уже помеченной клетке
                continue;//выбираются другие координаты
            }
            flag = shot(human.ships, bot.maps.hits, xc, yc);//производим выстрел
            if (flag == 0){
                score_comp++;//увеличиваем количество очков бота
                clearRec(&bot);
                j = 0;  //обнулем переменную для прохода по рекомендуемым точкам
                bot.mode = 1; // переходим в режим рандомной стрельбы
                bot.hit = 0; //обунуляем значения попадний
                refreshMaps(human.ships, human.hits, score_human, score_comp);
                printf("Противник убил ваш корабль!: %c%d\n", xc + 'a', yc + 1);
                system("pause");
            }
            if (flag == 1){//бот попал
                if (bot.hit == 1){//если до этого уже было одно попадание
                    int k;
                    clearRec(&bot);//очищаем от предыдущих значений
                    botSecondHit(&bot, xc, yc, bot.maps.hits);
                    j = 0;//обнулем переменную для прохода по рекомендуемым точкам
                    bot.hit = 2;//было уже два или больше попаданий
                }
                if (bot.hit == 0){//если еще не было попаданий

                    botFirstHit(&bot, xc, yc);
                    bot.hit = 1;//было одно попадание

                }
                bot.mode = 2;
                refreshMaps(human.ships, human.hits, score_human, score_comp);
                printf("Противник попал: %c%d\n", xc + 'a', yc + 1);
                system("pause");
            }
            if (flag == 2){//бот промахнулся
                    refreshMaps(human.ships, human.hits, score_human, score_comp);
                    printf("Противник промахнулся\n");
                    printf("Ваш ход\n");
                    turn = 1;
                    system("pause");
                    system("cls");
            }
        }
    }
    system("cls");
    if (score_human == 10){
        printf("Вы победили!\n");
    }
    if (score_comp == 10){
        printf("Вы проиграли!\n");
    }
    return 0;
}
