#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>
#include <windows.h>

#define screenWidth 120
#define screenHeight 40
#define mapWidth 32
#define mapHeight 32

// �̷� ��
int worldMap[mapWidth][mapHeight] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

// �÷��̾� ����
double posX = 1.5, posY = 1.5;    // ���� ��ġ
double dirX = -1, dirY = 0;       // ���� ����
double planeX = 0, planeY = 0.60; // ī�޶� ���

void render(char screen[screenHeight][screenWidth]) {
    // ȭ�� �ʱ�ȭ
    for (int y = 0; y < screenHeight; y++)
        for (int x = 0; x < screenWidth; x++)
            screen[y][x] = ' ';

    // ����ĳ����
    for (int x = 0; x < screenWidth; x++) {
        double cameraX = 2 * x / (double)screenWidth - 1;
        double rayDirX = dirX + planeX * cameraX;
        double rayDirY = dirY + planeY * cameraX;

        int mapX = (int)posX;
        int mapY = (int)posY;

        double sideDistX, sideDistY;
        double deltaDistX = fabs(1 / rayDirX);
        double deltaDistY = fabs(1 / rayDirY);
        double perpWallDist;

        int stepX, stepY;
        int hit = 0;
        int side;

        if (rayDirX < 0) {
            stepX = -1;
            sideDistX = (posX - mapX) * deltaDistX;
        }
        else {
            stepX = 1;
            sideDistX = (mapX + 1.0 - posX) * deltaDistX;
        }
        if (rayDirY < 0) {
            stepY = -1;
            sideDistY = (posY - mapY) * deltaDistY;
        }
        else {
            stepY = 1;
            sideDistY = (mapY + 1.0 - posY) * deltaDistY;
        }

        while (hit == 0) {
            if (sideDistX < sideDistY) {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            }
            else {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }
            if (worldMap[mapX][mapY] > 0) hit = 1;
        }

        if (side == 0)
            perpWallDist = (mapX - posX + (1 - stepX) / 2) / rayDirX;
        else
            perpWallDist = (mapY - posY + (1 - stepY) / 2) / rayDirY;

        int lineHeight = (int)(screenHeight * 1.6 / perpWallDist);
        int drawStart = -lineHeight / 2 + screenHeight / 2;
        if (drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 2 + screenHeight / 2;
        if (drawEnd >= screenHeight) drawEnd = screenHeight - 1;

        // �� ���� ����
        char wallChar;
        if (worldMap[mapX][mapY] == 2) {
            wallChar = 'X';  // �ⱸ
        }
        else {
            // �Ÿ��� ���鿡 ���� �� ǥ��
            if (side == 0) {
                if (perpWallDist < 3) wallChar = '#';
                else if (perpWallDist < 5) wallChar = 'x';
                else if (perpWallDist < 7) wallChar = '+';
                else wallChar = ':';
            }
            else {
                // ������ �� �ܰ� ��Ӱ�
                if (perpWallDist < 3) wallChar = 'x';
                else if (perpWallDist < 5) wallChar = '+';
                else wallChar = ':';
            }
        }

        // �� �׸���
        for (int y = drawStart; y < drawEnd; y++) {
            if (y >= 0 && y < screenHeight && x >= 0 && x < screenWidth) {
                screen[y][x] = wallChar;
            }
        }

        // �ٴ� �׸���
        for (int y = drawEnd; y < screenHeight; y++) {
            if (y >= 0 && y < screenHeight && x >= 0 && x < screenWidth) {
                double currentDist = screenHeight / (2.0 * y - screenHeight);
                if (currentDist < 2.0) screen[y][x] = '+';
                else if (currentDist < 3.0) screen[y][x] = '.';
                else screen[y][x] = ' ';
            }
        }
    }
}

void display(char screen[screenHeight][screenWidth]) {
    printf("\033[H");
    for (int y = 0; y < screenHeight; y++) {
        for (int x = 0; x < screenWidth; x++) {
            char c = screen[y][x];
            if (c == 'X') {
                printf("\033[31;1mX\033[0m");  // �ⱸ - ������
            }
            else if (c == ' ') {
                printf("\033[40m \033[0m");  // �� ���� - ������
            }
            else if (y > screenHeight / 2) {
                printf("\033[44m%c\033[0m", c);  // �ٴ� - �Ķ��� ���
            }
            else {
                putchar(c);  // �� - �⺻ ����
            }
        }
        printf("\n");
    }
}

// movePlayer �Լ��� ������ ����
void movePlayer(int forward) {
    double moveSpeed = 0.15;
    double newPosX = posX + dirX * moveSpeed * forward;
    double newPosY = posY + dirY * moveSpeed * forward;

    if (worldMap[(int)newPosX][(int)posY] == 0) {
        posX = newPosX;
    }
    else if (worldMap[(int)newPosX][(int)posY] == 2) {
        system("cls");
        printf("\033[32mŻ�� ����!\033[0m\n");
        printf("�ƹ� Ű�� ������ ����˴ϴ�...");
        _getch();
        exit(0);
    }

    if (worldMap[(int)posX][(int)newPosY] == 0) {
        posY = newPosY;
    }
    else if (worldMap[(int)posX][(int)newPosY] == 2) {
        system("cls");
        printf("\033[32mŻ�� ����!\033[0m\n");
        printf("�ƹ� Ű�� ������ ����˴ϴ�...");
        _getch();
        exit(0);
    }
}

// rotatePlayer �Լ��� ������ ����
void rotatePlayer(int direction) {
    double rotSpeed = 0.08;
    double oldDirX = dirX;
    dirX = dirX * cos(rotSpeed * direction) - dirY * sin(rotSpeed * direction);
    dirY = oldDirX * sin(rotSpeed * direction) + dirY * cos(rotSpeed * direction);
    double oldPlaneX = planeX;
    planeX = planeX * cos(rotSpeed * direction) - planeY * sin(rotSpeed * direction);
    planeY = oldPlaneX * sin(rotSpeed * direction) + planeY * cos(rotSpeed * direction);
}

int main() {
    char screen[screenHeight][screenWidth];
    printf("\033[2J\033[?25l");  // ȭ�� �ʱ�ȭ �� Ŀ�� �����

    LARGE_INTEGER frequency, lastTime, currentTime;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&lastTime);

    while (1) {
        if (_kbhit()) {
            char input = _getch();
            switch (input) {
            case 'q': goto cleanup;
            case 'a': rotatePlayer(1); break;
            case 'd': rotatePlayer(-1); break;
            }
        }

        if (GetAsyncKeyState('W') & 0x8000) {
            movePlayer(1);
        }
        if (GetAsyncKeyState('S') & 0x8000) {
            movePlayer(-1);
        }

        render(screen);
        display(screen);

        // 60 FPS ����
        QueryPerformanceCounter(&currentTime);
        double elapsedTime = (double)(currentTime.QuadPart - lastTime.QuadPart) / frequency.QuadPart;
        if (elapsedTime < 1.0 / 60.0) {
            Sleep((DWORD)((1.0 / 60.0 - elapsedTime) * 1000));
        }
        QueryPerformanceCounter(&lastTime);
    }

cleanup:
    printf("\033[?25h");
    return 0;
}