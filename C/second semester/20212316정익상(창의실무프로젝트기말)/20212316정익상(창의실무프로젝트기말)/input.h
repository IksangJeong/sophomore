// input.h
#ifndef INPUT_H
#define INPUT_H

#include "game.h"
#include <windows.h>
#include <stdbool.h>

// Ű ���¸� �����ϴ� ����ü
typedef struct {
    bool up;         // W or ���� ȭ��ǥ
    bool down;       // S or �Ʒ��� ȭ��ǥ
    bool left;       // A or ���� ȭ��ǥ
    bool right;      // D or ������ ȭ��ǥ
    bool rotLeft;    // Q or ���� ȸ��
    bool rotRight;   // E or ������ ȸ��
    bool escape;     // ESC Ű
} InputState;

// �Է� �ý��� �ʱ�ȭ
void InitInput(InputState* input);

// �Է� ���� ������Ʈ
void UpdateInput(InputState* input, GameState* game);

// Ư�� Ű�� ���� Ȯ�� �Լ���
bool IsKeyPressed(unsigned char key);    // �Ű����� Ÿ�� ����
bool IsKeyReleased(unsigned char key);   // �Ű����� Ÿ�� ����

// �Է� �ý��� ����
void CleanupInput(void);

#endif // INPUT_H