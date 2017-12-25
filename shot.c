#include "shot.h"
#include "printMaps.h"

void markKill(Cell ships[][10], Cell hits[][10], int y, int x){//�������� ���������� ������ ������ ������� � ������ ������� �������
    hits[y][x].status = KILLED;
    ships[y][x].status = KILLED;//�������� ������ ������� �� ���� ������
    int i,j;
    for (i = y - 1; i <= y+1; i++)
        for (j = x - 1; j <= x + 1; j++)
            if (inMap(i,j) == 1)
                if(ships[i][j].status == EMPTY){
                        ships[i][j].status = MISS;
                        hits[i][j].status = MISS;
                }
}

int wasKill(Cell ships[][10], Cell hits[][10], int i, int j){
    int m,x,y;
    x = ships[j][i].point.x;
    y = ships[j][i].point.y;
    if (ships[j][i].direction == 'h'){//��� ��������������� �������
        for (m = x; m < x + ships[j][i].length; m++){
                if(ships[y][m].status == SHIP)
                    return 0;//�� ���� �.�. � ��������������� ������� ��� ���� ����� ������
                }
        }
    if (ships[j][i].direction == 'v'){//��� ������������� �������
        for (m = y; m < y + ships[j][i].length; m++){
                if(ships[m][x].status == SHIP)
                    return 0;//�� ���� �.�. � ������������� ������� ��� ���� ����� ������
                }
        }
    //���� ��������� ������ ���������� �������, ������ � ������� �� �������� ����� ������ �.�. ������� ����
    if (ships[j][i].direction == 'h'){//��� ��������������� �������
        for (m = x; m < x + ships[j][i].length; m++){
                if(ships[y][m].status == INJURED){
                    markKill(ships, hits, y, m);// �������� ��� ���������� ������ ������ �������
                }
            }
        }
    if (ships[j][i].direction == 'v'){//��� ������������� �������
        for (m = y; m < y + ships[j][i].length; m++){
                if(ships[m][x].status == INJURED){
                    markKill(ships, hits, m, x);// �������� ��� ���������� ������ ������ �������
                }
            }
        }
    return 1;//����
}

int shot(Cell ships[][10], Cell hits[][10], int x, int y){
    if (ships[y][x].status == SHIP){//������ � �������
        hits[y][x].status = INJURED;//
        ships[y][x].status = INJURED;//�������� �������
        if(wasKill(ships,hits, x, y) == TRUE){//��������� ��� �� ���� �������
            return 0;//����
        }
        return 1;//�����
    }else {
        hits[y][x].status = MISS;//�������� ������ � ������� ��� �������
        ships[y][x].status = MISS;
        return 2;//������
    }
}

