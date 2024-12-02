// map.c
#include "map.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

bool InitMap(Map* map, int width, int height) {
    // ��ȿ�� �˻�
    if (width <= 0 || height <= 0) {
        return false;
    }

    // �� ũ�� ����
    map->width = width;
    map->height = height;

    // �� �����͸� ���� �޸� �Ҵ�
    map->data = (int*)malloc(sizeof(int) * width * height);
    if (!map->data) {
        return false;
    }

    // ��� Ÿ���� �� �������� �ʱ�ȭ
    memset(map->data, TILE_EMPTY, sizeof(int) * width * height);

    return true;
}

bool LoadMapFromFile(Map* map, const char* filename) {
    FILE* file;
    errno_t err = fopen_s(&file, filename, "r");
    if (err != 0) {
        return false;
    }

    // �� ũ�� �б�
    int width, height;
    if (fscanf_s(file, "%d %d", &width, &height) != 2) {
        fclose(file);
        return false;
    }

    // �� �ʱ�ȭ
    if (!InitMap(map, width, height)) {
        fclose(file);
        return false;
    }

    // �� ������ �б�
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int tile;
            if (fscanf_s(file, "%d", &tile) != 1) {
                fclose(file);
                return false;
            }
            SetTile(map, x, y, (MapTileType)tile);
        }
    }

    fclose(file);
    return true;
}

bool CreateDefaultMap(Map* map) {
    // �⺻ �� ũ��� �ʱ�ȭ
    if (!InitMap(map, MAP_WIDTH, MAP_HEIGHT)) {
        return false;
    }

    // �⺻ �� ������ ����
    const int defaultMap[MAP_HEIGHT][MAP_WIDTH] = {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
        {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
        {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };

    // �� ������ ����
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            SetTile(map, x, y, defaultMap[y][x]);
        }
    }

    return true;
}

bool IsInMapBounds(Map* map, int x, int y) {
    return (x >= 0 && x < map->width && y >= 0 && y < map->height);
}

bool IsWall(Map* map, int x, int y) {
    // �� ���� �˻�
    if (!IsInMapBounds(map, x, y)) {
        return true;    // �� ���� ������ ó��
    }

    // �ش� ��ġ�� Ÿ���� ������ Ȯ��
    return map->data[y * map->width + x] != TILE_EMPTY;
}

int GetTile(Map* map, int x, int y) {
    // �� ���� �˻�
    if (!IsInMapBounds(map, x, y)) {
        return TILE_WALL;    // �� ���� ������ ó��
    }

    return map->data[y * map->width + x];
}

void SetTile(Map* map, int x, int y, MapTileType tileType) {
    // �� ���� �˻�
    if (!IsInMapBounds(map, x, y)) {
        return;    // �� ���� ����
    }

    map->data[y * map->width + x] = tileType;
}

bool IsPassable(Map* map, double x, double y) {
    // �Ǽ� ��ǥ�� ���� ��ǥ�� ��ȯ
    int mapX = (int)x;
    int mapY = (int)y;

    // �� ���� üũ
    if (!IsInMapBounds(map, mapX, mapY)) {
        return false;
    }

    // �ش� ��ġ�� �� �������� Ȯ��
    return GetTile(map, mapX, mapY) == TILE_EMPTY;
}

void FillMapArea(Map* map, int startX, int startY, int width, int height, MapTileType tileType) {
    for (int y = startY; y < startY + height && y < map->height; y++) {
        for (int x = startX; x < startX + width && x < map->width; x++) {
            if (IsInMapBounds(map, x, y)) {
                SetTile(map, x, y, tileType);
            }
        }
    }
}

bool SaveMapToFile(Map* map, const char* filename) {
    FILE* file;
    errno_t err = fopen_s(&file, filename, "w");
    if (err != 0) {
        return false;
    }

    // �� ũ�� ����
    fprintf(file, "%d %d\n", map->width, map->height);

    // �� ������ ����
    for (int y = 0; y < map->height; y++) {
        for (int x = 0; x < map->width; x++) {
            fprintf(file, "%d ", GetTile(map, x, y));
        }
        fprintf(file, "\n");
    }

    fclose(file);
    return true;
}

int CountSurroundingWalls(Map* map, int x, int y) {
    int count = 0;
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (dx == 0 && dy == 0) continue;
            if (IsWall(map, x + dx, y + dy)) {
                count++;
            }
        }
    }
    return count;
}

void CleanupMap(Map* map) {
    if (map->data) {
        free(map->data);
        map->data = NULL;
    }
    map->width = 0;
    map->height = 0;
}