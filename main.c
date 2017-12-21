/*
Code::Blocks 16.01
GNU GCC Compiler
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <Windows.h>

enum ConsoleColor {
    Black = 0,
    Green = 2,
    Red = 4,
    Magenta = 5,
    Yellow = 14,
    White = 15
};
void printColor(int x){//����� ������� ��������
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (x == 0){
        SetConsoleTextAttribute(hConsole, (WORD) ((Black << 4) | Black));
        printf("�");
    }
    if (x == 1){
        SetConsoleTextAttribute(hConsole, (WORD) ((Black << 4) | Green));
        printf("o");
    }
    if (x == 2){
        SetConsoleTextAttribute(hConsole, (WORD) ((Black << 4) | Magenta));
        printf("o",x);
    }
    if (x == 3){
        SetConsoleTextAttribute(hConsole, (WORD) ((Black << 4) | Yellow));
        printf("o",x);
    }
    if (x == 4){
        SetConsoleTextAttribute(hConsole, (WORD) ((Black << 4) | Red));
        printf("o",x);
    }
    return 0;
}

typedef struct Point{
    int x,y; //���������� ������ ������� ����� ��� ����������� �������� �������
}Point;

typedef struct Cell{
    Point point; //��������� ���������� �������
    int status; //�������� ������
    int length; //����� �������
    char direction; //����������� �������
}Cell;

typedef struct Player{
    Cell ships[10][10];
    Cell hits[10][10];
}Player;

void clearMap(Cell map[][10]){//������ �������� ����� �������� ������
    int i,j;
    for (i = 0; i < 10; i++){
        for (j = 0; j < 10; j++){
            map[i][j].status = 0;
            map[i][j].direction = 0;
            map[i][j].length = 0;
            map[i][j].point.x = -1;
            map[i][j].point.y = -1;
        }
    }
}

void clearStatus(Cell map[][10]){//����� ����������� ������ ����� �� ������ ��������
    int i, j;
    for (i = 0; i < 10; i++){
        for(j = 0; j < 10; j++){
            if (map[i][j].status == 2){
                map[i][j].status = 0;
                map[i][j].direction = 0;
                map[i][j].length = 0;
                map[i][j].point.x = -1;
                map[i][j].point.y = -1;
            }
        }
    }
}

void printMaps(Cell ships[][10], Cell hits[][10]){//������� ����� �������� � ����� ���������
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int size = 10;
    printf("  #abcdefghij#   #abcdefghij#\n");
    int i,j;
    for (i = 0; i < size; i++){
        for (j = 0; j < 2 * size ; j++){
            SetConsoleTextAttribute(hConsole, (WORD) ((Black << 4) | White));
            if (j == 0)
                if (i+1 != 10)
                    printf(" %d|",i+1);
                else printf("%d|",i+1);
            if (j >= 0 && j <= 9)
                printColor(ships[i][j].status);
                SetConsoleTextAttribute(hConsole, (WORD) ((Black << 4) | White));
            if (j >=10 && j <= 19)
                printColor(hits[i][j-10].status);
                SetConsoleTextAttribute(hConsole, (WORD) ((Black << 4) | White));
            if (j == 9)
                if (i+1 != 10)
                    printf("|  %d|", i+1);
                else printf("| %d|", i+1);
            if (j == 19)
                    printf("|");
        }
        printf("\n");
    }
    printf("\n\n");
}

int setShip(int ship, char dir, int x, int y, Cell map[][10]){//������� ��� ����������� �������
    int i,j;
    int shipDirH = 0, shipDirV = 0;
    if (dir == 'h'){
        shipDirH = ship - 1;
    }
    else shipDirV = ship - 1;

    if (x + shipDirH > 9 || y + shipDirV > 9)
        return 1;//�� ������� ��������� ������� �� ������� ����� �� ������� �����

    for (i = y; i <= y + shipDirV; i++){
        for (j = x; j <= x + shipDirH; j++){
            if (i >=0 && i < 10 && j >=0 && j<10){
                if (map[i][j].status != 0)
                    return 1;//�� ������� ��������� ������� �� ����� ���� ������ �������
            }
        }
    }

    for (i = y - 1; i <= y + shipDirV + 1; i++){
        for (j = x - 1; j <= x + shipDirH + 1 ; j++){
            if (i >=0 && i < 10 && j >=0 && j<10){
                if ((dir == 'h' && i == y && j >= x && j<= x + shipDirH) || (dir == 'v' && j == x && i >= y && i <= y + shipDirV)){
                    map[i][j].status = 1;
                    map[i][j].point.x = x;
                    map[i][j].point.y = y;
                    map[i][j].direction = dir;
                    map[i][j].length = ship;
                }
                else map[i][j].status = 2;
            }
        }
    }
    return 0;// ������� ������ ����������
    /*
    � ����� �������� ���������:
�����:   012345..
       0|022220
       1|021120      � ������ � ���������� ������������ x = 2 y = 1 ���������� ������������ �������
       2|022220      � ������� �� ��������� 1 ���� ���������� ����� ������� � ��� �����������
       3|000000
       .
       .
     */
}
void randomShip(Cell map[][10]){//������� ��������� ����������� ��������
    int ships[] = {0,4,3,2,1};//������ 4 - ������������ 3 - ������������ 2 - ������������ � 1 - ���������������
    while(ships[1] != 0 || ships[2] !=0 || ships[3] != 0 || ships[4] != 0){// ���� ��������� ����� ��� ������� ����������
        int ship = 1 + rand() % 4;// �������� �������� ������ �� ��� �����
        if (ship >= 1 && ship <= 4 && ships[ship] > 0){
                char dir;
                int z = rand() % 2;
                if (z == 0)
                    dir = 'h';
                else dir = 'v';//�������� �������� ����������� �������
                int x,y;
                x = rand()%10;
                y = rand()%10;//�������� �������� ��������� ���������� �������
                int e = setShip(ship, dir, x, y, map);//������������� �������
                if (e == 0)
                    ships[ship]--;//���� ������� ������� ����������, �� -1 ������� ������ �����
            }
        }
}

void manually(Cell map[][10], Cell hits[][10]){//����������� �������� �������
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    int ships[] = {0,4,3,2,1};
    char dir, s[10], a[10];
    int x = 0, y = 0, e = 0, ship = 0;
    while(ships[1] != 0 || ships[2] !=0 || ships[3] != 0 || ships[4] != 0){
        system("cls");
        printMaps(map, hits);
        printf("� ���: ������������ - %d, ������������ - %d, ����������� - %d, �������������� - %d\n",ships[1],ships[2],ships[3],ships[4]);
        printf("�������� �������:\n");
        scanf("%s",&s);
        ship = atoi(s);
        if (strlen(s) > 1 || ship == 0){
            printf("����� ������ ���� �����!\n");
            system("pause");
            continue;
        }
        if (ship >= 1 && ship <= 4 && ships[ship] > 0){ // ���� ���� ����� ������� � �� ������ �� 1 �� 4, �� ���������� ����
                if (ship != 1){
                        printf("�������� ���������� �������: h - ��������������, v - ������������\n");
                        dir = getche(); //����������� �������
                        printf("\n");
                        }
                else dir = 'h';
                if (dir != 'h' && dir != 'v'){
                    printf("������� �������� �����������\n");
                    system("pause");
                    continue;
                }
                printf("������� ����������:\n");
                scanf("%s",&s);
                strncpy(a,s,1);
                int sz = strlen(s);
                memmove(s,s+1,sz-1);
                s[sz-1] = 0;
                y = atoi(s) - 1;
                x = *a - 'a';
                if (inMap(x,y) != 1){
                    printf("�������� ����������\n");
                    system("pause");
                    continue;
                }
                e = setShip(ship, dir, x, y, map);// 1 - �� ������� ��������� ������� 2 - ������� ����������
                if (e == 0){
                    ships[ship]--;
                }
                else{
                    printf("�� ������� ��������� �������\n");
                    system("pause");
                }
            }
        else {
            printf("������ ������� ���\n");
            system("pause");
            continue;
        }
    }
}

int inMap(int x, int y){//��������� ��������� �� ����� � �������� �����
    if (x >= 0 && x <=9 && y>=0 && y <= 9)
        return 1;//����� � �������� �����
    else return 0;
}

void markKill(Cell ships[][10], Cell hits[][10], int y, int x){//�������� ���������� ������ ������ ������� � ������ ������� �������
    hits[y][x].status = 4;
    ships[y][x].status = 4;//�������� ������ ������� �� ���� ������
    int i,j;
    for (i = y - 1; i <= y+1; i++)
        for (j = x - 1; j <= x + 1; j++)
            if (inMap(i,j) == 1)
                if(ships[i][j].status == 0){
                        ships[i][j].status = 2;
                        hits[i][j].status = 2;
                }
}

int wasKill(Cell ships[][10], Cell hits[][10], int i, int j){
    int m,x,y;
    x = ships[j][i].point.x;
    y = ships[j][i].point.y;
    if (ships[j][i].direction=='h'){//��� ��������������� �������
        for (m = x; m < x + ships[j][i].length; m++){
                if(ships[y][m].status == 1)
                    return 0;//�� ���� �.�. � ��������������� ������� ��� ���� ����� ������
                }
        }
    if (ships[j][i].direction=='v'){//��� ������������� �������
        for (m = y; m < y + ships[j][i].length; m++){
                if(ships[m][x].status == 1)
                    return 0;//�� ���� �.�. � ������������� ������� ��� ���� ����� ������
                }
        }
    //���� ��������� ������ ���������� �������, ������ � ������� �� �������� ����� ������ �.�. ������� ����
    if (ships[j][i].direction == 'h'){//��� ��������������� �������
        for (m = x; m < x + ships[j][i].length; m++){
                if(ships[y][m].status == 3){
                    markKill(ships, hits, y, m);// �������� ��� ���������� ������ ������ �������
                }
            }
        }
    if (ships[j][i].direction == 'v'){//��� ������������� �������
        for (m = y; m < y + ships[j][i].length; m++){
                if(ships[m][x].status == 3){
                    markKill(ships, hits, m, x);// �������� ��� ���������� ������ ������ �������
                }
            }
        }
    return 1;//����
}

int shot(Cell ships[][10], Cell hits[][10], int x, int y){
    if (ships[y][x].status == 1){//������ � �������
        hits[y][x].status = 3;//
        ships[y][x].status = 3;//�������� �������
        if(wasKill(ships,hits, x, y) == 1){//��������� ��� �� ���� �������
            return 0;//����
        }
        return 1;//�����
    }else {
        hits[y][x].status = 2;//�������� ������ � ������� ��� �������
        ships[y][x].status = 2;
        return 2;//������
    }
}

typedef struct Recommend{// ���������� � ������� ������������� �������� ����
    Point point;
}Recommend;

void clearRec(Recommend rec[4]){//�������� ������������� ����������
    int i;
    for (i = 0; i < 4; i++){
        rec[i].point.x = -1;
        rec[i].point.y = -1;
    }
}

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
        printf("��� ���������� �������?\n 1 - �������� \n 2 - �������\n\n\n������� ����� ���������: a1, g10\n\n");
        int n;
        scanf("%d", &n);
        if (n == 1){
            randomShip(human.ships);//����������� ������� ��������
            clearStatus(human.ships);//��������� �� ����� ������ �������
            printMaps(human.ships,human.ships);//������� ����� �������� � ����� ���������
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
    Player comp;//������� ����
    clearMap(comp.hits);//�������� �������� �����
    clearMap(comp.ships);
    randomShip(comp.ships);// �������� ����������� �������
    clearStatus(comp.ships);// ��������� �� ����� ������ �������
    int x,y;//���������� ����� ������� ������ �������
    int flag;//��� �������� 0-���� 1-����� 2-������
    int xc,yc;//���������� �������� ����������
    int hit = 0;//���� �� ��������� � ����������
    int j = 0;//���������� ��� ������� �� ������������� �������
    Recommend recommend[4];//������� ������������� ������ ��� �������� ����
    clearRec(recommend);//������� ��������
    int turn = 1;//1 - ����� �������, 2 - ����� ���������
    int mode = 1;//��� ����: 1 - ��������� ������� 2 - ���������
    int win1 = 0,win2 = 0;//������� ���������� ������ ��������
    while(win1 < 10 && win2 < 10){
        if (turn == 1){
            system("cls");//������� ��������
            printf("�� �����: %d ��������� ����: %d\n", win1, win2);//���������� ����������� ��������
            printMaps(human.ships, human.hits);//����� ����� �������� � ����� ���������
            printf("������� ����������:\n");
            char s[10], a[10];
            scanf("%s", &s);
            strncpy(a, s, 1);
            int sz = strlen(s);
            memmove(s, s+1, sz-1);
            s[sz-1] = 0;//������� ������ ������
            y = atoi(s) - 1;
            x = *a - 'a';
            if (inMap(x, y) != 1 || sz > 3){
                printf("������� ������� ����������\n");
                system("pause");
                continue;
            }
            if (human.hits[y][x].status != 0){
                printf("���������� ����������\n");
                system("pause");
                continue;
            }
            flag = shot(comp.ships, human.hits, x, y);//���������� �������
            if (flag == 0){
                system("cls");
                win1++;//����������� ���������� ����������� �������� � ������
                printf("�� �����: %d ��������� ����: %d\n", win1, win2);
                printMaps(human.ships, human.hits);
                printf("����!\n");
                system("pause");
            }
            if (flag == 1){
                system("cls");
                printf("�� �����: %d ��������� ����: %d\n", win1, win2);
                printMaps(human.ships, human.hits);
                printf("�����!\n");
                system("pause");
            }
            if (flag == 2){
                system("cls");
                printf("�� �����: %d ��������� ����: %d\n", win1, win2);
                printMaps(human.ships, human.hits);
                printf("������!\n");
                turn = 2;// ������������� ��� ���������� �.�. �� �� ������
                system("pause");
            }
        }
        else {//��� ��
            if (mode == 1){//��������� �������
                xc = rand() % 10;//��� �� ���� ��������� ������� �������� ��������� ����������
                yc = rand() % 10;
            }
            else {//mode == 2 ���������
                if (hit != 0){
                    if (recommend[j].point.x == -1){//��������� ���� �� �������� � ������������� �����
                        j++;   //���� ���, �� ��������� � ��������� �����
                        continue;
                    }
                    xc = recommend[j].point.x;
                    yc = recommend[j].point.y;
                    j++;
                    if (inMap(xc,yc) != 1){//���� ���������� �� � �������� ����� �� ��������� � ��������� ������������� ����������
                        continue;
                    }
                }
            }
            if (comp.hits[yc][xc].status != 0){//���� ��������� �������� �� ��� ���������� ������
                continue;//���������� ������ ����������
            }
            flag = shot(human.ships, comp.hits, xc, yc);//���������� �������
            if (flag == 0){
                win2++;//��������� ���� �������
                clearRec(recommend);
                j = 0;  //������� ���������� ��� ������� �� ������������� ������
                mode = 1; // ��������� � ����� ��������� ��������
                hit = 0; //��������� �������� ��������
                system("cls");
                printf("�� �����: %d ��������� ����: %d\n", win1, win2);
                printMaps(human.ships, human.hits);
                printf("��������� ���� ��� �������!: %c%d\n", xc + 'a', yc + 1);
                system("pause");
            }
            if (flag == 1){//��������� �����
                if (hit == 1){//���� �� ����� ��� ���� ���� ���������
                    int k;
                    clearRec(recommend);//������� �� ���������� ��������
                    /* ������:
                            01234567
                          0|00000000
                          1|00000000 3 - ������ � ������� ������
                          2|0rr33rr0 r - ������������� ����� ��� ��������
                          3|00000000
                          4|00000000

                    */
                    if (inMap(xc+1,yc) == 1){
                            if(comp.hits[yc][xc+1].status == 3){//���� ������ ���������� ���������
                               recommend[0].point.x = xc - 1;
                               recommend[0].point.y = yc;
                               recommend[1].point.x = xc + 2;
                               recommend[1].point.y = yc;
                               recommend[2].point.x = xc - 2;
                               recommend[2].point.y = yc;
                               recommend[3].point.x = xc + 3;
                               recommend[3].point.y = yc;

                            }

                    }
                    if (inMap(xc-1,yc) == 1){
                        if(comp.hits[yc][xc-1].status == 3){//���� ����� ���������� ���������
                               recommend[0].point.x = xc - 2;
                               recommend[0].point.y = yc;
                               recommend[1].point.x = xc + 1;
                               recommend[1].point.y = yc;
                               recommend[2].point.x = xc - 3;
                               recommend[2].point.y = yc;
                               recommend[3].point.x = xc + 2;
                               recommend[3].point.y = yc;
                            }
                    }
                    if (inMap(xc,yc+1) == 1){
                        if(comp.hits[yc+1][xc].status == 3){//���� ����� ���������� ���������
                               recommend[0].point.x = xc;
                               recommend[0].point.y = yc + 2;
                               recommend[1].point.x = xc;
                               recommend[1].point.y = yc - 1;
                               recommend[2].point.x = xc;
                               recommend[2].point.y = yc - 2;
                               recommend[3].point.x = xc;
                               recommend[3].point.y = yc + 3;

                            }
                    }
                    if (inMap(xc,yc-1) == 1){
                        if(comp.hits[yc-1][xc].status == 3){//���� ������ ���������� ���������
                               recommend[0].point.x = xc;
                               recommend[0].point.y = yc - 2;
                               recommend[1].point.x = xc;
                               recommend[1].point.y = yc + 1;
                               recommend[2].point.x = xc;
                               recommend[2].point.y = yc - 3;
                               recommend[3].point.x = xc;
                               recommend[3].point.y = yc + 2;
                            }
                    }
                    j = 0;//������� ���������� ��� ������� �� ������������� ������
                    hit = 2;//���� ��� ��� ��� ������ ���������
                }
                if (hit == 0){//���� ��� �� ���� ���������
                    int f = 1, r , arr[4], i;
                    arr[0] = rand() % 4;
                    while (f < 4){
                        M1: arr[f] = rand() % 4;
                        for (i = 0; i < f; i++){
                            if(arr[f] == arr[i])
                                goto M1;
                        }
                        f++;
                    }//���������� ������ �� 4 ��������� ����� �� 0 �� 3 ����� ������������� ������ ���������� ��������
                     /*   ������:
                            0123456
                          0|0000000
                          1|00r0000 3 - ������ � ������� ������
                          2|0r3r000 r - ������������� ������ ��� ��������
                          3|00r0000
                          4|0000000
                     */
                    if (inMap(xc + 1, yc) == 1){
                        recommend[arr[0]].point.x = xc + 1;
                        recommend[arr[0]].point.y = yc;
                    }
                    if (inMap(xc - 1, yc) == 1){
                        recommend[arr[1]].point.x = xc - 1;
                        recommend[arr[1]].point.y = yc;
                    }
                    if (inMap(xc, yc + 1) == 1){
                        recommend[arr[2]].point.x = xc;
                        recommend[arr[2]].point.y = yc + 1;
                    }
                    if (inMap(xc, yc - 1) == 1){
                        recommend[arr[3]].point.x = xc;
                        recommend[arr[3]].point.y = yc - 1;
                    }
                    hit = 1;//���� ���� ���������

                }
                mode = 2;
                system("cls");
                printf("�� �����: %d ��������� ����: %d\n", win1, win2);
                printMaps(human.ships, human.hits);
                printf("��������� �����: %c%d\n", xc+'a', yc+1);
                system("pause");
            }
            if (flag == 2){
                    system("cls");
                    printf("�� �����: %d ��������� ����: %d\n", win1, win2);
                    printMaps(human.ships, human.hits);
                    printf("��������� �����������\n");
                    printf("��� ���\n");
                    turn = 1;
                    system("pause");
                    system("cls");
            }
        }
    }
    system("cls");
    if (win1 == 10){
        printf("�� ��������!\n");
    }
    if (win2 == 10){
        printf("�� ���������!\n");
    }
    return 0;
}
