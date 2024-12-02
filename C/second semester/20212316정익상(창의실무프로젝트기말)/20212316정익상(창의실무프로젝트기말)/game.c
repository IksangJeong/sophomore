// game.c
#include "game.h"
#include "input.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

bool InitGame(GameState* game, HINSTANCE hInstance) {
    // �⺻�� �ʱ�ȭ
    game->screenWidth = SCREEN_WIDTH;
    game->screenHeight = SCREEN_HEIGHT;
    game->isRunning = true;
    game->lastTime = GetTime();
    game->deltaTime = 0.0;

    // ������ Ŭ���� ���
    WNDCLASSEXA wc = { 0 };
    wc.cbSize = sizeof(WNDCLASSEXA);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszClassName = "GameClass";

    if (!RegisterClassExA(&wc)) {
        DWORD error = GetLastError();
        char msg[256];
        sprintf_s(msg, sizeof(msg), "RegisterClassEx failed: %d", error);
        MessageBoxA(NULL, msg, "Error", MB_OK);
        return false;
    }

    // ������ ����
    game->window = CreateWindowExA(
        0,                      // Ȯ�� ��Ÿ��
        "GameClass",            // Ŭ���� �̸�
        "Game",                 // ������ ����
        WS_OVERLAPPEDWINDOW,    // ��Ÿ��
        CW_USEDEFAULT,         // x ��ġ
        CW_USEDEFAULT,         // y ��ġ
        game->screenWidth,      // �ʺ�
        game->screenHeight,     // ����
        NULL,                   // �θ� ������
        NULL,                   // �޴�
        hInstance,              // �ν��Ͻ�
        NULL                    // �߰� �Ķ����
    );

    if (!game->window) {
        DWORD error = GetLastError();
        char msg[256];
        sprintf_s(msg, sizeof(msg), "CreateWindowEx failed: %d", error);
        MessageBoxA(NULL, msg, "Error", MB_OK);
        return false;
    }

    // ����̽� ���ؽ�Ʈ ��������
    game->deviceContext = GetDC(game->window);
    if (!game->deviceContext) {
        MessageBoxA(NULL, "Failed to get device context", "Error", MB_OK);
        return false;
    }

    // �� ���� ����
    game->backBuffer = CreateCompatibleDC(game->deviceContext);
    if (!game->backBuffer) {
        MessageBoxA(NULL, "Failed to create back buffer", "Error", MB_OK);
        return false;
    }

    // �� ���� ��Ʈ�� ����
    game->backBufferBitmap = CreateCompatibleBitmap(
        game->deviceContext,
        game->screenWidth,
        game->screenHeight
    );
    if (!game->backBufferBitmap) {
        MessageBoxA(NULL, "Failed to create back buffer bitmap", "Error", MB_OK);
        return false;
    }

    // �� ���ۿ� ��Ʈ�� ����
    HBITMAP oldBitmap = (HBITMAP)SelectObject(game->backBuffer, game->backBufferBitmap);
    if (!oldBitmap) {
        MessageBoxA(NULL, "Failed to select bitmap into back buffer", "Error", MB_OK);
        return false;
    }

    // Z-���� �Ҵ�
    game->zBuffer = (double*)malloc(sizeof(double) * game->screenWidth);
    if (!game->zBuffer) {
        MessageBoxA(NULL, "Failed to allocate Z-buffer", "Error", MB_OK);
        return false;
    }

    // �÷��̾� �ʱ�ȭ
    game->player.position = (Vector2D){ 22.0, 12.0 };
    game->player.direction = (Vector2D){ -1.0, 0.0 };
    game->player.plane = (Vector2D){ 0.0, 0.66 };
    game->player.moveSpeed = MOVE_SPEED;
    game->player.rotSpeed = ROT_SPEED;

    // �� �ʱ�ȭ
    game->map.width = MAP_WIDTH;
    game->map.height = MAP_HEIGHT;
    game->map.data = (int*)malloc(sizeof(int) * MAP_WIDTH * MAP_HEIGHT);
    if (!game->map.data) {
        MessageBoxA(NULL, "Failed to allocate map data", "Error", MB_OK);
        return false;
    }

    // â ���̱�
    ShowWindow(game->window, SW_SHOW);
    UpdateWindow(game->window);

    // �ʱ� ȭ�� �����
    RECT rect;
    GetClientRect(game->window, &rect);
    HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
    FillRect(game->backBuffer, &rect, blackBrush);
    DeleteObject(blackBrush);

    return true;
}

void RunGameLoop(GameState* game) {
    MSG msg = { 0 };
    char titleBuffer[128];
    int frameCount = 0;
    double lastFPSTime = GetTime();

    // �Է� ���� �ʱ�ȭ
    InputState input = { 0 };
    InitInput(&input);

    while (game->isRunning) {
        // ������ �޽��� ó��
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                game->isRunning = false;
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (!game->isRunning) break;

        // �ð� ������Ʈ
        double currentTime = GetTime();
        game->deltaTime = currentTime - game->lastTime;
        game->lastTime = currentTime;

        // �Է� ó��
        UpdateInput(&input, game);

        // ���� ���� ������Ʈ
        UpdateGame(game);

        // ��� �����
        RECT rect;
        GetClientRect(game->window, &rect);
        HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
        FillRect(game->backBuffer, &rect, blackBrush);
        DeleteObject(blackBrush);

        // �׽�Ʈ�� �簢�� �׸���
        HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
        RECT testRect = {
            (int)game->player.position.x,  // �÷��̾� ��ġ�� ������� �簢�� ��ġ ����
            (int)game->player.position.y,
            (int)game->player.position.x + 50,
            (int)game->player.position.y + 50
        };
        FillRect(game->backBuffer, &testRect, whiteBrush);
        DeleteObject(whiteBrush);

        // �� ���۸� ȭ�鿡 �׸���
        BitBlt(game->deviceContext, 0, 0, game->screenWidth, game->screenHeight,
            game->backBuffer, 0, 0, SRCCOPY);

        // FPS ��� �� ǥ��
        frameCount++;
        if (currentTime - lastFPSTime >= 1.0) {
            sprintf_s(titleBuffer, sizeof(titleBuffer),
                "Game - FPS: %d, Pos: (%.1f, %.1f)",
                frameCount,
                game->player.position.x,
                game->player.position.y);
            SetWindowTextA(game->window, titleBuffer);
            frameCount = 0;
            lastFPSTime = currentTime;
        }

        // ������ ����
        Sleep(1);
    }
}

void UpdateGame(GameState* game) {
    CalculateDeltaTime(game);
}

double GetTime(void) {
    LARGE_INTEGER frequency, counter;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&counter);
    return (double)counter.QuadPart / (double)frequency.QuadPart;
}

void CalculateDeltaTime(GameState* game) {
    double currentTime = GetTime();
    game->deltaTime = currentTime - game->lastTime;
    game->lastTime = currentTime;

    if (game->deltaTime > 0.016667f) {
        game->deltaTime = 0.016667f;
    }
}

void CleanupGame(GameState* game) {
    if (game->zBuffer) {
        free(game->zBuffer);
        game->zBuffer = NULL;
    }

    if (game->map.data) {
        free(game->map.data);
        game->map.data = NULL;
    }

    if (game->backBufferBitmap) {
        DeleteObject(game->backBufferBitmap);
        game->backBufferBitmap = NULL;
    }

    if (game->backBuffer) {
        DeleteDC(game->backBuffer);
        game->backBuffer = NULL;
    }

    if (game->deviceContext) {
        ReleaseDC(game->window, game->deviceContext);
        game->deviceContext = NULL;
    }

    if (game->window) {
        DestroyWindow(game->window);
        game->window = NULL;
    }
}