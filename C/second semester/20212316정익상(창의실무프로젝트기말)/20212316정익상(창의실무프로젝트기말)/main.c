
#include <windows.h>
#include <stdbool.h>
#include <stdio.h>
#include "game.h"
#include "map.h"
#include "player.h"
#include "raycaster.h"
#include "input.h"

/**
 * @brief FPS�� ȭ�鿡 ǥ���ϱ� ���� �Լ�
 * @param game ���� ���� ����ü ������
 */
void DisplayFPS(GameState* game) {
    static int frameCount = 0;
    static double lastTime = 0;
    static double fps = 0;

    frameCount++;
    double currentTime = GetTime();

    // 1�ʸ��� FPS ���
    if (currentTime - lastTime > 1.0) {
        fps = frameCount / (currentTime - lastTime);
        frameCount = 0;
        lastTime = currentTime;

        char title[64];
        sprintf_s(title, sizeof(title), "Maze Game - FPS: %.2f", fps);
        SetWindowTextA(game->window, title);
    }
}

/**
 * @brief ���� �ʱ�ȭ ����
 * @param game ���� ���� ����ü ������
 * @param hInstance ���α׷� �ν��Ͻ� �ڵ�
 * @return �ʱ�ȭ ���� ����
 */
bool GameStartup(GameState* game, HINSTANCE hInstance) {
    // ���� ���� �ʱ�ȭ
    if (!InitGame(game, hInstance)) {
        MessageBoxA(NULL, "Failed to initialize game", "Error", MB_ICONERROR);
        return false;
    }
    // �� ����
    if (!CreateDefaultMap(&game->map)) {
        MessageBoxA(NULL, "Failed to create map", "Error", MB_ICONERROR);
        return false;
    }
    // ����ĳ���� �ʱ�ȭ
    if (!InitRaycaster(game)) {
        MessageBoxA(NULL, "Failed to initialize raycaster", "Error", MB_ICONERROR);
        return false;
    }

    return true;
}

/**
 * @brief ���� ���ҽ� ����
 * @param game ���� ���� ����ü ������
 */
void GameShutdown(GameState* game) {
    CleanupRaycaster(game);
    CleanupMap(&game->map);
    CleanupInput();
    CleanupGame(game);
}

/**
 * @brief �޽��� ó�� �Լ�
 */
bool ProcessMessages(void) {
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            return false;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return true;
}

/**
 * @brief Windows ���� ������
 */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    GameState game = { 0 };
    if (!GameStartup(&game, hInstance)) {
        return 1;
    }

    // �Է� ���� �ʱ�ȭ
    InputState inputState = { 0 };
    InitInput(&inputState);

    MSG msg = { 0 };  // msg ������ ���⼭ �� ���� ����

    // ���� ���� ����
    while (game.isRunning) {
        // �޽��� ó��
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                game.isRunning = false;
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (!game.isRunning) break;

        // �Է� ó��
        UpdateInput(&inputState, &game);

        // ��� ����� (������)
        RECT rect;
        GetClientRect(game.window, &rect);
        HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
        FillRect(game.backBuffer, &rect, blackBrush);
        DeleteObject(blackBrush);

        // �׽�Ʈ�� �簢�� �׸��� (�÷��̾� ��ġ)
        HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
        RECT testRect = {
            (int)(game.player.position.x * 20),  // ȭ�鿡�� �� ũ�� ���̵��� ��ġ ����
            (int)(game.player.position.y * 20),
            (int)(game.player.position.x * 20) + 20,
            (int)(game.player.position.y * 20) + 20
        };
        FillRect(game.backBuffer, &testRect, whiteBrush);
        DeleteObject(whiteBrush);

        // �÷��̾� ��ġ ������ ��� �߰�
        char debugTitle[128];
        sprintf_s(debugTitle, sizeof(debugTitle),
            "Player Position: (%.2f, %.2f)",
            game.player.position.x,
            game.player.position.y);
        SetWindowTextA(game.window, debugTitle);

        // �� ���۸� ȭ�鿡 �׸���
        BitBlt(game.deviceContext, 0, 0, game.screenWidth, game.screenHeight,
            game.backBuffer, 0, 0, SRCCOPY);

        // FPS ǥ��
        DisplayFPS(&game);

        // ������ ����
        Sleep(1);
    }

    GameShutdown(&game);
    return (int)msg.wParam;
}

#ifdef _DEBUG
int main(void) {
    return WinMain(GetModuleHandle(NULL), NULL, GetCommandLine(), SW_SHOWNORMAL);
}
#endif