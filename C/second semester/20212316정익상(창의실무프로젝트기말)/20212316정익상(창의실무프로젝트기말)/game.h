// game.h
#ifndef GAME_H
#define GAME_H

#include <windows.h>
#include <stdbool.h>

// ������ �⺻ ��� ����
#define SCREEN_WIDTH    640     // ���� â�� �ʺ� (�ȼ�)
#define SCREEN_HEIGHT   480     // ���� â�� ���� (�ȼ�)
#define MAP_WIDTH       24      // ���� �ʺ� (Ÿ�� ����)
#define MAP_HEIGHT      24      // ���� ���� (Ÿ�� ����)
#define MOVE_SPEED      5.0     // �÷��̾� �̵� �ӵ�
#define ROT_SPEED       3.0     // �÷��̾� ȸ�� �ӵ�
#define WALL_HEIGHT     64      // ���� �⺻ ����
#define FOV            66       // �þ߰� (degrees)

/**
 * @brief 2D ���͸� ǥ���ϱ� ���� ����ü
 * x, y ��ǥ�� ������ ��Ÿ���µ� ���
 */
typedef struct {
    double x;  // x ��ǥ �Ǵ� x ���� ����
    double y;  // y ��ǥ �Ǵ� y ���� ����
} Vector2D;

/**
 * @brief ���� �� ������ �����ϴ� ����ü
 */
typedef struct {
    int width;          // ���� �ʺ�
    int height;         // ���� ����
    int* data;          // �� ������ �迭 (1���� �迭�� 2D �� ǥ��)
} Map;

/**
 * @brief �÷��̾� ������ �����ϴ� ����ü
 */
typedef struct {
    Vector2D position;   // ���� ��ġ
    Vector2D direction;  // �ٶ󺸴� ����
    Vector2D plane;      // ī�޶� ���
    double moveSpeed;    // �̵� �ӵ�
    double rotSpeed;     // ȸ�� �ӵ�
} Player;

/**
 * @brief ������ ��ü ���¸� �����ϴ� ����ü
 */
typedef struct {
    // ȭ�� ����
    int screenWidth;           // ȭ�� �ʺ�
    int screenHeight;          // ȭ�� ����

    // ���� ��ü��
    Player player;             // �÷��̾� ����
    Map map;                   // �� ����

    // ������ ����
    double* zBuffer;           // ���� ���� (����ĳ���ÿ�)
    HWND window;              // ������ �ڵ�
    HDC deviceContext;        // ���� ����̽� ���ؽ�Ʈ
    HDC backBuffer;           // ���� ���۸��� �� ����
    HBITMAP backBufferBitmap; // �� ���� ��Ʈ��

    // ���� ���� ����
    bool isRunning;           // ���� ���� ����
    double deltaTime;         // ������ �� ��� �ð�
    double lastTime;          // ���� �������� �ð�
} GameState;

/**
 * @brief ���� �ʱ�ȭ �Լ�
 * @param game �ʱ�ȭ�� ���� ���� ����ü ������
 * @param hInstance ���� ���α׷��� �ν��Ͻ� �ڵ�
 * @return �ʱ�ȭ ���� ����
 */
bool InitGame(GameState* game, HINSTANCE hInstance);

/**
 * @brief ���� ���� ���� ���� �Լ�
 * @param game ���� ���� ����ü ������
 */
void RunGameLoop(GameState* game);

/**
 * @brief ���� ���� ������Ʈ �Լ�
 * @param game ������Ʈ�� ���� ���� ����ü ������
 */
void UpdateGame(GameState* game);

/**
 * @brief ���� �ð��� �� ������ ��ȯ�ϴ� �Լ�
 * @return ���� �ð� (��)
 */
double GetTime(void);

/**
 * @brief ������ �� ��� �ð��� ����ϴ� �Լ�
 * @param game ���� ���� ����ü ������
 */
void CalculateDeltaTime(GameState* game);

/**
 * @brief ���� �ڿ� ���� �Լ�
 * @param game ������ ���� ���� ����ü ������
 */
void CleanupGame(GameState* game);

#endif // GAME_H