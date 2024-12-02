#pragma once
// raycaster.h
#ifndef RAYCASTER_H
#define RAYCASTER_H

#include "game.h"
#include "map.h"
#include <windows.h>

/**
 * @brief ���� ��Ʈ ������ �����ϴ� ����ü
 */
typedef struct {
    Vector2D rayDir;         // ������ ���� ����
    double perpWallDist;     // �������� ���� �Ÿ�
    int mapX;               // �ε��� ���� X ��ǥ
    int mapY;               // �ε��� ���� Y ��ǥ
    int side;               // �ε��� ���� (0: x��, 1: y��)
    double wallX;           // ���� �ε��� ��Ȯ�� ��ġ
    int lineHeight;         // ȭ�鿡 �׷��� ���� ����
    int drawStart;          // �� �׸��� ���� Y ��ǥ
    int drawEnd;            // �� �׸��� ���� Y ��ǥ
} RaycastHit;

/**
 * @brief ����ĳ���� �ʱ�ȭ �Լ�
 * @param game ���� ���� ����ü ������
 * @return �ʱ�ȭ ���� ����
 */
bool InitRaycaster(GameState* game);

/**
 * @brief ���� ���� ĳ���� �Լ�
 * @param game ���� ���� ����ü ������
 * @param x ��ũ�� X ��ǥ
 * @param hit ���� ��Ʈ ������ ������ ����ü ������
 */
void CastRay(GameState* game, int x, RaycastHit* hit);

/**
 * @brief DDA(Digital Differential Analysis) �˰��� ���� �Լ�
 * @param game ���� ���� ����ü ������
 * @param hit ���� ��Ʈ ������ ������ ����ü ������
 */
void PerformDDA(GameState* game, RaycastHit* hit);

/**
 * @brief ������ �� ������ �Ÿ� ��� �Լ�
 * @param hit ���� ��Ʈ ���� ����ü ������
 * @param player �÷��̾� ����ü ������
 */
void CalculateWallDistance(RaycastHit* hit, Player* player);

/**
 * @brief ȭ�鿡 �׸� ���� ���̿� ��ġ ��� �Լ�
 * @param hit ���� ��Ʈ ���� ����ü ������
 * @param screenHeight ȭ�� ����
 */
void CalculateWallDrawing(RaycastHit* hit, int screenHeight);

/**
 * @brief ȭ�鿡 ������ �׸��� �Լ�
 * @param game ���� ���� ����ü ������
 * @param x ��ũ�� X ��ǥ
 * @param hit ���� ��Ʈ ���� ����ü ������
 * @param color �� ����
 */
void DrawVerticalLine(GameState* game, int x, RaycastHit* hit, COLORREF color);

/**
 * @brief ��ü ȭ�� ������ �Լ�
 * @param game ���� ���� ����ü ������
 */
void RenderFrame(GameState* game);

/**
 * @brief �� ���� ���� �Լ�
 * @param hit ���� ��Ʈ ���� ����ü ������
 * @param map �� ����ü ������
 * @return �� ����
 */
COLORREF GetWallColor(RaycastHit* hit, Map* map);

/**
 * @brief ����ĳ���� ���� �Լ�
 * @param game ���� ���� ����ü ������
 */
void CleanupRaycaster(GameState* game);

#endif // RAYCASTER_H