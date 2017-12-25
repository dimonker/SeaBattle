#include "setShip.h"
#include "printMaps.h"
#include "time.h"


int setShip(int ship, char direction, int x, int y, Cell map[][10]){//������� ��� ����������� �������

    int i,j;
    int shipDirH = 0, shipDirV = 0;
    if (direction == HORIZONTALLY){
        shipDirH = ship - 1;
    }
    else shipDirV = ship - 1;

    if (inMap(x + shipDirH, y + shipDirV) == FALSE)
        return 1;//�� ������� ��������� ������� �� ������� ����� �� ������� �����

    for (i = y; i <= y + shipDirV; i++){
        for (j = x; j <= x + shipDirH; j++){
            if (i >=0 && i < 10 && j >=0 && j<10){
                if (map[i][j].status != EMPTY)
                    return 1;//�� ������� ��������� ������� �� ����� ���� ������ �������
            }
        }
    }

    for (i = y - 1; i <= y + shipDirV + 1; i++){
        for (j = x - 1; j <= x + shipDirH + 1 ; j++){
            if (i >=0 && i < 10 && j >=0 && j<10){
                if ((direction == HORIZONTALLY && i == y && j >= x && j<= x + shipDirH) || (direction == VERTICALLY && j == x && i >= y && i <= y + shipDirV)){
                    map[i][j].status = SHIP;
                    map[i][j].point.x = x;
                    map[i][j].point.y = y;
                    map[i][j].direction = direction;
                    map[i][j].length = ship;
                }
                else map[i][j].status = MISS;
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
                char direction;
                int z = rand() % 2;
                if (z == 0)
                    direction = HORIZONTALLY;
                else direction = VERTICALLY;//�������� �������� ����������� �������
                int x, y;
                x = rand() % 10;
                y = rand() % 10;//�������� �������� ��������� ���������� �������
                int e = setShip(ship, direction, x, y, map);//������������� �������
                if (e == 0)
                    ships[ship]--;//���� ������� ������� ����������, �� -1 ������� ������ �����
            }
        }
}

void manually(Cell map[][10], Cell hits[][10]){//����������� �������� �������
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    int ships[] = {0,4,3,2,1};
    char direction, s[10], a[10];
    int x = 0, y = 0, e = 0, ship = 0;
    while(ships[1] != 0 || ships[2] !=0 || ships[3] != 0 || ships[4] != 0){
        system("cls");
        printMaps(map, hits);
        printf("� ���: ������������ - %d, ������������ - %d, ����������� - %d, �������������� - %d\n",ships[1],ships[2],ships[3],ships[4]);
        printf("�������� �������:\n");
        scanf("%s", &s);
        ship = atoi(s);
        if (strlen(s) > 1 || ship == 0){
            printf("����� ������ ���� �����!\n");
            system("pause");
            continue;
        }
        if (ship >= 1 && ship <= 4 && ships[ship] > 0){ // ���� ���� ����� ������� � �� ������ �� 1 �� 4, �� ���������� ����
                if (ship != 1){
                        printf("�������� ���������� �������: h - ��������������, v - ������������\n");
                        direction = getche(); //����������� �������
                        printf("\n");
                        }
                else direction = HORIZONTALLY;
                if (direction != HORIZONTALLY && direction != VERTICALLY){
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
                if (inMap(x,y) != TRUE){
                    printf("�������� ����������\n");
                    system("pause");
                    continue;
                }
                e = setShip(ship, direction, x, y, map);// 1 - �� ������� ��������� ������� 2 - ������� ����������
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
