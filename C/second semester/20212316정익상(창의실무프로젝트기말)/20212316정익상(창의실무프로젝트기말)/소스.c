#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdbool.h>

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define MAX_BULLETS 50
#define MAX_ENEMIES 20

// ����ü ����
typedef struct {
    int x, y;
    int health;
    int score;
} Player;

typedef struct {
    int x, y;
    bool isAlive;
    int pattern;
} Enemy;

typedef struct {
    int x, y;
    bool isActive;
    bool isEnemy;
} Bullet;

// ���� ����
Player player;
Enemy enemies[MAX_ENEMIES];
Bullet bullets[MAX_BULLETS];
int level = 1;
bool gameOver = false;

// �Լ� ����
void initGame();
void movePlayer();
void moveEnemies();
void updateBullets();
void checkCollisions();
void renderScreen();
void gotoxy(int x, int y);
void hideCursor();

// ���� �ʱ�ȭ
void initGame() {
    // �÷��̾� �ʱ�ȭ
    player.x = SCREEN_WIDTH / 2;
    player.y = SCREEN_HEIGHT - 2;
    player.health = 3;
    player.score = 0;

    // �� �ʱ�ȭ
    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].x = (i % 10) * 8 + 5;
        enemies[i].y = (i / 10) * 2 + 2;
        enemies[i].isAlive = true;
        enemies[i].pattern = i % 3;
    }

    // �Ѿ� �ʱ�ȭ
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].isActive = false;
    }

    hideCursor();
}

// �÷��̾� �̵�
void movePlayer() {
    if (GetAsyncKeyState(VK_LEFT) && player.x > 1)
        player.x--;
    if (GetAsyncKeyState(VK_RIGHT) && player.x < SCREEN_WIDTH - 2)
        player.x++;
    if (GetAsyncKeyState(VK_SPACE)) {
        // �Ѿ� �߻�
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (!bullets[i].isActive) {
                bullets[i].x = player.x;
                bullets[i].y = player.y - 1;
                bullets[i].isActive = true;
                bullets[i].isEnemy = false;
                break;
            }
        }
        Sleep(100); // ���� �߻� ����
    }
}

// �� �̵�
void moveEnemies() {
    static int moveCounter = 0;
    moveCounter++;

    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].isAlive) {
            switch (enemies[i].pattern) {
            case 0: // �¿� �̵�
                enemies[i].x += (moveCounter % 40 < 20) ? 1 : -1;
                break;
            case 1: // ������� �̵�
                enemies[i].x += (moveCounter % 20 < 10) ? 1 : -1;
                if (moveCounter % 40 == 0) enemies[i].y++;
                break;
            case 2: // ���� �ϰ�
                if (moveCounter % 30 == 0) enemies[i].y++;
                break;
            }

            // �� �Ѿ� �߻�
            if (rand() % 100 < 2) {
                for (int j = 0; j < MAX_BULLETS; j++) {
                    if (!bullets[j].isActive) {
                        bullets[j].x = enemies[i].x;
                        bullets[j].y = enemies[i].y + 1;
                        bullets[j].isActive = true;
                        bullets[j].isEnemy = true;
                        break;
                    }
                }
            }
        }
    }
}

// �Ѿ� ������Ʈ
void updateBullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].isActive) {
            if (!bullets[i].isEnemy)
                bullets[i].y--;
            else
                bullets[i].y++;

            if (bullets[i].y < 0 || bullets[i].y > SCREEN_HEIGHT)
                bullets[i].isActive = false;
        }
    }
}

// �浹 �˻�
void checkCollisions() {
    // �÷��̾� �Ѿ˰� �� �浹
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].isActive && !bullets[i].isEnemy) {
            for (int j = 0; j < MAX_ENEMIES; j++) {
                if (enemies[j].isAlive &&
                    bullets[i].x >= enemies[j].x - 1 &&
                    bullets[i].x <= enemies[j].x + 1 &&
                    bullets[i].y == enemies[j].y) {
                    enemies[j].isAlive = false;
                    bullets[i].isActive = false;
                    player.score += 100;
                }
            }
        }
    }

    // �� �Ѿ˰� �÷��̾� �浹
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].isActive && bullets[i].isEnemy) {
            if (bullets[i].x >= player.x - 1 &&
                bullets[i].x <= player.x + 1 &&
                bullets[i].y == player.y) {
                player.health--;
                bullets[i].isActive = false;
                if (player.health <= 0)
                    gameOver = true;
            }
        }
    }
}

// ȭ�� ������
void renderScreen() {
    system("cls");

    // ������ ����� ǥ��
    gotoxy(0, 0);
    printf("Score: %d  Lives: %d  Level: %d", player.score, player.health, level);

    // �÷��̾� ������
    gotoxy(player.x, player.y);
    printf("^");

    // �� ������
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].isAlive) {
            gotoxy(enemies[i].x, enemies[i].y);
            printf("W");
        }
    }

    // �Ѿ� ������
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].isActive) {
            gotoxy(bullets[i].x, bullets[i].y);
            printf(bullets[i].isEnemy ? "v" : "|");
        }
    }
}

// Ŀ�� ��ġ �̵�
void gotoxy(int x, int y) {
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// Ŀ�� �����
void hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo = { 1, FALSE };
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

// ���� �Լ�
int main() {
    initGame();

    while (!gameOver) {
        movePlayer();
        moveEnemies();
        updateBullets();
        checkCollisions();
        renderScreen();

        // ���� Ŭ���� üũ
        bool allEnemiesDead = true;
        for (int i = 0; i < MAX_ENEMIES; i++) {
            if (enemies[i].isAlive) {
                allEnemiesDead = false;
                break;
            }
        }

        if (allEnemiesDead) {
            level++;
            initGame(); // ���� ���� ����
        }

        Sleep(50); // ���� �ӵ� ����
    }

    // ���� ���� ȭ��
    system("cls");
    gotoxy(SCREEN_WIDTH / 2 - 5, SCREEN_HEIGHT / 2);
    printf("GAME OVER\n");
    gotoxy(SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 + 1);
    printf("Final Score: %d\n", player.score);

    return 0;
}