// raycaster.c
#include "raycaster.h"
#include <math.h>

bool InitRaycaster(GameState* game) {
    // Z-���� �ʱ�ȭ
    if (game->zBuffer) {
        free(game->zBuffer);
    }
    game->zBuffer = (double*)malloc(sizeof(double) * game->screenWidth);
    if (!game->zBuffer) {
        return false;
    }

    // Z-���� �ʱⰪ ����
    for (int i = 0; i < game->screenWidth; i++) {
        game->zBuffer[i] = 0.0;
    }

    return true;
}

void CastRay(GameState* game, int x, RaycastHit* hit) {
    // ī�޶� x ��ǥ ��� (-1 ~ 1)
    double cameraX = 2 * x / (double)game->screenWidth - 1;

    // ������ ���� ���� ���
    hit->rayDir.x = game->player.direction.x + game->player.plane.x * cameraX;
    hit->rayDir.y = game->player.direction.y + game->player.plane.y * cameraX;

    // ���� �� ��ġ
    hit->mapX = (int)game->player.position.x;
    hit->mapY = (int)game->player.position.y;

    // DDA �˰��� �غ�
    PerformDDA(game, hit);

    // �������� �Ÿ� ���
    CalculateWallDistance(hit, &game->player);

    // ȭ�鿡 �׸� ���� ���̿� ��ġ ���
    CalculateWallDrawing(hit, game->screenHeight);

    // Z-���� ������Ʈ
    game->zBuffer[x] = hit->perpWallDist;
}

void PerformDDA(GameState* game, RaycastHit* hit) {
    // deltaDistX�� deltaDistY ���
    double deltaDistX = fabs(1.0 / hit->rayDir.x);
    double deltaDistY = fabs(1.0 / hit->rayDir.y);

    double sideDistX;
    double sideDistY;
    int stepX;
    int stepY;

    // ������ ���⿡ ���� �̵� ����� �ʱ� ���̵� �Ÿ� ���
    if (hit->rayDir.x < 0) {
        stepX = -1;
        sideDistX = (game->player.position.x - hit->mapX) * deltaDistX;
    }
    else {
        stepX = 1;
        sideDistX = (hit->mapX + 1.0 - game->player.position.x) * deltaDistX;
    }

    if (hit->rayDir.y < 0) {
        stepY = -1;
        sideDistY = (game->player.position.y - hit->mapY) * deltaDistY;
    }
    else {
        stepY = 1;
        sideDistY = (hit->mapY + 1.0 - game->player.position.y) * deltaDistY;
    }

    // DDA �˰��� ����
    int hit_wall = 0;
    while (hit_wall == 0) {
        // ���� �� ��ġ�� �̵�
        if (sideDistX < sideDistY) {
            sideDistX += deltaDistX;
            hit->mapX += stepX;
            hit->side = 0;
        }
        else {
            sideDistY += deltaDistY;
            hit->mapY += stepY;
            hit->side = 1;
        }

        // �� �浹 üũ
        if (IsWall(&game->map, hit->mapX, hit->mapY)) {
            hit_wall = 1;
        }
    }
}

void CalculateWallDistance(RaycastHit* hit, Player* player) {
    if (hit->side == 0) {
        hit->perpWallDist = (hit->mapX - player->position.x +
            (1 - (hit->rayDir.x < 0 ? -1 : 1)) / 2) / hit->rayDir.x;
    }
    else {
        hit->perpWallDist = (hit->mapY - player->position.y +
            (1 - (hit->rayDir.y < 0 ? -1 : 1)) / 2) / hit->rayDir.y;
    }

    // wallX ��� (�ؽ�ó ������ ����)
    if (hit->side == 0) {
        hit->wallX = player->position.y + hit->perpWallDist * hit->rayDir.y;
    }
    else {
        hit->wallX = player->position.x + hit->perpWallDist * hit->rayDir.x;
    }
    hit->wallX -= floor(hit->wallX);
}

void CalculateWallDrawing(RaycastHit* hit, int screenHeight) {
    // ���� ���� ���
    hit->lineHeight = (int)(screenHeight / hit->perpWallDist);

    // ȭ�鿡 �׸� �������� ���� ���
    hit->drawStart = -hit->lineHeight / 2 + screenHeight / 2;
    if (hit->drawStart < 0) {
        hit->drawStart = 0;
    }

    hit->drawEnd = hit->lineHeight / 2 + screenHeight / 2;
    if (hit->drawEnd >= screenHeight) {
        hit->drawEnd = screenHeight - 1;
    }
}

void DrawVerticalLine(GameState* game, int x, RaycastHit* hit, COLORREF color) {
    // y�鿡 �ε����� ��� ������ ��Ӱ�
    if (hit->side == 1) {
        int r = GetRValue(color) / 2;
        int g = GetGValue(color) / 2;
        int b = GetBValue(color) / 2;
        color = RGB(r, g, b);
    }

    // õ�� �׸��� (������)
    HPEN pen = CreatePen(PS_SOLID, 1, color);
    HPEN oldPen = (HPEN)SelectObject(game->backBuffer, pen);

    for (int y = 0; y < hit->drawStart; y++) {
        SetPixel(game->backBuffer, x, y, RGB(0, 0, 0));
    }

    // �� �׸���
    for (int y = hit->drawStart; y < hit->drawEnd; y++) {
        SetPixel(game->backBuffer, x, y, color);
    }

    // �ٴ� �׸��� (ȸ��)
    for (int y = hit->drawEnd; y < game->screenHeight; y++) {
        SetPixel(game->backBuffer, x, y, RGB(64, 64, 64));
    }

    SelectObject(game->backBuffer, oldPen);
    DeleteObject(pen);
}

COLORREF GetWallColor(RaycastHit* hit, Map* map) {
    // �� Ÿ�Կ� ���� ���� ��ȯ
    int wallType = GetTile(map, hit->mapX, hit->mapY);
    switch (wallType) {
    case 1: return RGB(255, 0, 0);    // ������
    case 2: return RGB(0, 255, 0);    // �ʷϻ�
    case 3: return RGB(0, 0, 255);    // �Ķ���
    case 4: return RGB(255, 255, 0);  // �����
    case 5: return RGB(255, 0, 255);  // �����
    default: return RGB(128, 128, 128); // ȸ��
    }
}

void RenderFrame(GameState* game) {
    RaycastHit hit;

    // ȭ���� �� �������� ���� ����ĳ���� ����
    for (int x = 0; x < game->screenWidth; x++) {
        // ���� �߻�
        CastRay(game, x, &hit);

        // �� ���� ���
        COLORREF color = GetWallColor(&hit, &game->map);

        // ������ �׸���
        DrawVerticalLine(game, x, &hit, color);
    }

    // �� ���۸� ȭ�鿡 ǥ��
    BitBlt(game->deviceContext, 0, 0, game->screenWidth, game->screenHeight,
        game->backBuffer, 0, 0, SRCCOPY);
}

void CleanupRaycaster(GameState* game) {
    if (game->zBuffer) {
        free(game->zBuffer);
        game->zBuffer = NULL;
    }
}