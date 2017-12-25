#include "printMaps.h"
#include "windows.h"

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
    if (x == EMPTY){
        SetConsoleTextAttribute(hConsole, (WORD) ((Black << 4) | Black));
        printf("�");
    }
    if (x == SHIP){
        SetConsoleTextAttribute(hConsole, (WORD) ((Black << 4) | Green));
        printf("o");
    }
    if (x == MISS){
        SetConsoleTextAttribute(hConsole, (WORD) ((Black << 4) | Magenta));
        printf("o",x);
    }
    if (x == INJURED){
        SetConsoleTextAttribute(hConsole, (WORD) ((Black << 4) | Yellow));
        printf("o",x);
    }
    if (x == KILLED){
        SetConsoleTextAttribute(hConsole, (WORD) ((Black << 4) | Red));
        printf("o",x);
    }
}

void clearMap(Cell map[][10]){//������ �������� ����� �������� ������
    int i,j;
    for (i = 0; i < 10; i++){
        for (j = 0; j < 10; j++){
            map[i][j].status = EMPTY;
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
            if (map[i][j].status == MISS){
                map[i][j].status = EMPTY;
                map[i][j].direction = 0;
                map[i][j].length = 0;
                map[i][j].point.x = -1;
                map[i][j].point.y = -1;
            }
        }
    }
}
int inMap(int x, int y){//��������� ��������� �� ����� � �������� �����
    if (x >= MIN_SIZE && x < MAX_SIZE && y >= MIN_SIZE && y < MAX_SIZE)
        return TRUE;//����� � �������� �����
    else return FALSE;
}

void printMaps(Cell ships[][10], Cell hits[][10]){//������� ����� �������� � ����� ���������
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    printf("  #abcdefghij#   #abcdefghij#\n");
    int i,j;
    for (i = 0; i < MAX_SIZE; i++){
        for (j = 0; j < 2 * MAX_SIZE; j++){
            SetConsoleTextAttribute(hConsole, (WORD) ((Black << 4) | White));
            if (j == 0)
                if (i+1 != 10)
                    printf(" %d|", i+1);
                else printf("%d|", i+1);
            if (j >= MIN_SIZE && j < MAX_SIZE)
                printColor(ships[i][j].status);
                SetConsoleTextAttribute(hConsole, (WORD) ((Black << 4) | White));
            if (j >= MAX_SIZE && j < 2 * MAX_SIZE)
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

void refreshMaps(Cell ships[10][10], Cell hits[10][10], int score_human, int score_comp){
    system("cls");
    printf("�� �����: %d ��������� ����: %d\n", score_human, score_comp);//���������� ����������� ��������
    printMaps(ships, hits);//����� ����� �������� � ����� ���������
}
