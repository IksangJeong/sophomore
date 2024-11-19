#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include <stdbool.h>

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

// ���� �Ӽ�
enum boardProperty {
    BoardWidth = 14,  // ���� ���� ����
    BoardHeight = 22, // ���� ���� ����
    BoardX = 4,      // �ܼ�â���� ��µ� ���� X��ǥ
    BoardY = 3       // �ܼ�â���� ��µ� ���� Y��ǥ
};

// Ű �Է� �ڵ�
enum Keys {
    LEFT = 75,     // Ű���� ���� Ű
    RIGHT = 77,    // Ű���� ������ Ű
    UP = 72,       // Ű���� �� Ű
    DOWN = 80,     // Ű���� �Ʒ� Ű
    ARROW = 224,   // Ű���� ����Ű �Ǻ�
    SPACEBAR = 32  // Ű���� �����̽� Ű
};

// ���� �ӵ�(���̵�)
typedef enum gameSpeed {
    easy = 230,    // ���� ���̵� ����
    normal = 170,  // ���� ���̵� ����
    hard = 150     // ���� ���̵� �����
} SPEED;

// ��� �Ӽ�
enum blockProperty {
    BlockSIZE = 4,     // ����� ũ��
    BlockRotate = 4,   // ����� ȸ��
    BlockType = 7,     // ����� ����
    BlockStartX = 14,  // ����� Ŀ��â���� ���۵� x��ǥ
    BlockStartY = 4    // ����� Ŀ��â���� ���۵� y��ǥ
};

// ���� ���� ����
typedef struct _GAME_STATE {
    int score;
    int level;
    bool isGameOver;
    bool isPaused;
    DWORD lastUpdateTime;
    DWORD lastMoveTime;
} GAME_STATE;

// ��� ���� ����ü
typedef struct _BLOCK_INFO {
    int shape;
    int rotation;
    int x;
    int y;
    bool isActive;
} BLOCK_INFO;

// ���� ���� ����
extern int nkey;                  // ����ڰ� �Է��� Ư�� ����Ű �ƽ�Ű �ڵ尪
extern short curShape;           // ���� ��� ���
extern short nexShape;           // ���� ��� ���
extern short BlockCOL;          // ����� ���� ��ǥ
extern short BlockROW;
extern short prevBlockCOL;      // ����� ���� ��ǥ
extern short prevBlockROW;
extern COORD Cursor;            // ��Ʈ���� ���� �� ���� ��ġ�ϴ� Ŀ��
extern COORD previewPoint;     // �̸����� ����� ��ǥ
extern bool IsNextBlock;       // ���� ��� ǥ�� ����
extern short turn;            // ��� ȸ�� ����
extern int score;            // ���� ����
extern short gameLevel;      // ���� ���̵�
extern GAME_STATE gameState; // ���� ����
extern BLOCK_INFO currentBlock; // ���� ��� ����

// �Լ� ����
void InitGame(void);              // ���� �ʱ�ȭ
void UpdateGame(void);            // ���� ���� ������Ʈ
void ProcessInput(void);          // Ű �Է� ó��
void UpdateBlockPosition(void);    // ��� ��ġ ������Ʈ
bool CheckCollision(int shape, int rotate, int x, int y);  // �浹 �˻�
void RotateBlock(void);           // ��� ȸ��
void FixBlock(void);              // ��� ����
void CheckLines(void);            // �� �ϼ� �˻�
void RemoveLine(int line);        // �� ����
bool IsGameOver(void);            // ���� ���� �˻�
void UpdateScore(int lines);      // ���� ������Ʈ
void printScore(void);  // ���� ��� �Լ� ����

// ��� ���� �Լ�
void CurrentShape(void);          // ���� ��� ����
void NextShape(void);             // ���� ��� ����
void deletePrevBlock(void);       // ���� ��� �����
void newBlock(void);              // ���ο� ��� ����
void blockFixed(int shape, int rotate);  // ��� ����
bool IsCollision(int shape, int rotate, int curX, int curY);  // �浹 �˻�
bool IsMaxLine(void);             // �� �ϼ� �˻�
void deleteLine(void);            // �� ����
bool IsOverHeight(void);          // �ִ� ���� �ʰ� �˻�
void previewBlock(int shape, int rotate);  // �̸����� ��� ǥ��
void GameProcess(void);           // ���� ���μ��� ����

// �߰��� ���� ����ȭ ���� ���
#define MIN_UPDATE_INTERVAL 16    // �ּ� ������Ʈ ���� (ms)
#define RENDER_INTERVAL 33        // ������ ���� (�� 30 FPS)
#define INPUT_CHECK_INTERVAL 10   // �Է� üũ ���� (ms)