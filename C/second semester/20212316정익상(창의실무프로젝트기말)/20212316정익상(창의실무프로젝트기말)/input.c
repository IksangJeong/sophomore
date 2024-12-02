// input.c
#include "input.h"
#include "player.h"
#include <string.h>

// ���� Ű ���¸� �����ϴ� �迭
static bool previousKeyState[256] = { false };

void InitInput(InputState* input) {
    // ��� �Է� ���¸� false�� �ʱ�ȭ
    input->up = false;
    input->down = false;
    input->left = false;
    input->right = false;
    input->rotLeft = false;
    input->rotRight = false;
    input->escape = false;

    // ���� Ű ���� �迭 �ʱ�ȭ
    memset(previousKeyState, 0, sizeof(previousKeyState));
}

void UpdateInput(InputState* input, GameState* game) {
    // ESC Ű ó��
    if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
        game->isRunning = false;
        return;
    }

    // �̵� Ű ���� ������Ʈ
    input->up = (GetAsyncKeyState('W') & 0x8000) || (GetAsyncKeyState(VK_UP) & 0x8000);
    input->down = (GetAsyncKeyState('S') & 0x8000) || (GetAsyncKeyState(VK_DOWN) & 0x8000);
    input->left = GetAsyncKeyState('A') & 0x8000;
    input->right = GetAsyncKeyState('D') & 0x8000;
    input->rotLeft = GetAsyncKeyState('Q') & 0x8000 || (GetAsyncKeyState(VK_LEFT) & 0x8000);
    input->rotRight = GetAsyncKeyState('E') & 0x8000 || (GetAsyncKeyState(VK_RIGHT) & 0x8000);

    // �÷��̾� �̵� ó��
    if (input->up) {
        MovePlayer(&game->player, &game->map, MOVE_FORWARD, game->deltaTime);
    }
    if (input->down) {
        MovePlayer(&game->player, &game->map, MOVE_BACKWARD, game->deltaTime);
    }
    if (input->left) {
        MovePlayer(&game->player, &game->map, MOVE_LEFT, game->deltaTime);
    }
    if (input->right) {
        MovePlayer(&game->player, &game->map, MOVE_RIGHT, game->deltaTime);
    }

    // ȸ�� ó��
    if (input->rotLeft) {
        RotatePlayer(&game->player, 1.0, game->deltaTime);
    }
    if (input->rotRight) {
        RotatePlayer(&game->player, -1.0, game->deltaTime);
    }
}

bool IsKeyPressed(unsigned char key) {
    bool currentState = (GetAsyncKeyState(key) & 0x8000) != 0;
    bool wasPressed = !previousKeyState[key] && currentState;
    previousKeyState[key] = currentState;
    return wasPressed;
}

bool IsKeyReleased(unsigned char key) {
    bool currentState = (GetAsyncKeyState(key) & 0x8000) != 0;
    bool wasReleased = previousKeyState[key] && !currentState;
    previousKeyState[key] = currentState;
    return wasReleased;
}

void CleanupInput(void) {
    // �Է� �ý��� ����
    memset(previousKeyState, 0, sizeof(previousKeyState));
}