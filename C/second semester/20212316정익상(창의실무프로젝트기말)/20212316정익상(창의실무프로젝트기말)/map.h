// map.h
#ifndef MAP_H
#define MAP_H

#include "game.h"
#include <stdbool.h>

/**
 * @brief �� Ÿ���� ������ �����ϴ� ������
 */
typedef enum {
    TILE_EMPTY = 0,    // �� ����
    TILE_WALL = 1,     // �⺻ ��
    TILE_WALL2 = 2,    // �ٸ� ������ ��
    TILE_WALL3 = 3,    // �� �ٸ� ������ ��
    TILE_WALL4 = 4     // �� ��° ������ ��
} MapTileType;

/**
 * @brief ���� �ʱ�ȭ�ϴ� �Լ�
 * @param map �ʱ�ȭ�� �� ����ü ������
 * @param width ���� �ʺ�
 * @param height ���� ����
 * @return �ʱ�ȭ ���� ����
 */
bool InitMap(Map* map, int width, int height);

/**
 * @brief �� �����͸� ���Ͽ��� �ε��ϴ� �Լ�
 * @param map �� ����ü ������
 * @param filename �� ���� ���
 * @return �ε� ���� ����
 */
bool LoadMapFromFile(Map* map, const char* filename);

/**
 * @brief �⺻ �� �����͸� �����ϴ� �Լ�
 * @param map �� ����ü ������
 * @return ���� ���� ����
 */
bool CreateDefaultMap(Map* map);

/**
 * @brief Ư�� ��ġ�� Ÿ���� ������ Ȯ���ϴ� �Լ�
 * @param map �� ����ü ������
 * @param x X ��ǥ
 * @param y Y ��ǥ
 * @return �� ���� (true: ��, false: �� ����)
 */
bool IsWall(Map* map, int x, int y);

/**
 * @brief ���� Ư�� ��ġ�� Ÿ�� ���� ��ȯ�ϴ� �Լ�
 * @param map �� ����ü ������
 * @param x X ��ǥ
 * @param y Y ��ǥ
 * @return Ÿ�� ��
 */
int GetTile(Map* map, int x, int y);

/**
 * @brief ���� Ư�� ��ġ�� Ÿ���� �����ϴ� �Լ�
 * @param map �� ����ü ������
 * @param x X ��ǥ
 * @param y Y ��ǥ
 * @param tileType ������ Ÿ�� ����
 */
void SetTile(Map* map, int x, int y, MapTileType tileType);

/**
 * @brief �� �ڿ��� �����ϴ� �Լ�
 * @param map ������ �� ����ü ������
 */
void CleanupMap(Map* map);

/**
 * @brief Ư�� ��ǥ�� �� ���� ���� �ִ��� Ȯ���ϴ� �Լ�
 */
bool IsInMapBounds(Map* map, int x, int y);

/**
 * @brief Ư�� ��ǥ�� �̵� ������ ��ġ���� Ȯ���ϴ� �Լ�
 */
bool IsPassable(Map* map, double x, double y);

/**
 * @brief ���� Ư�� ������ ä��� �Լ�
 */
void FillMapArea(Map* map, int startX, int startY, int width, int height, MapTileType tileType);

/**
 * @brief ���� �ؽ�Ʈ ���Ϸ� �����ϴ� �Լ�
 */
bool SaveMapToFile(Map* map, const char* filename);

/**
 * @brief Ư�� ��ġ �ֺ��� �� ������ ���� �Լ�
 */
int CountSurroundingWalls(Map* map, int x, int y);

#endif // MAP_H