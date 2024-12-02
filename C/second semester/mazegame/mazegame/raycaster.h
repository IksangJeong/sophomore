#pragma once
/* raycaster.h */
#ifndef RAYCASTER_H
#define RAYCASTER_H

/* �⺻ ��� ���� */
#define MAP_WIDTH 24
#define MAP_HEIGHT 24
#define SCREEN_WIDTH 120   /* �ܼ� ȭ�� �ʺ� */
#define SCREEN_HEIGHT 40   /* �ܼ� ȭ�� ���� */
#define TRUE 1
#define FALSE 0

/* ����Ű ��� */
#define KEY_UP 'w'
#define KEY_DOWN 's'
#define KEY_LEFT 'a'
#define KEY_RIGHT 'd'
#define KEY_ESC 27

/* �� Ÿ�� Ÿ�� */
#define TILE_EMPTY 0
#define TILE_WALL_1 1
#define TILE_WALL_2 2
#define TILE_WALL_3 3
#define TILE_WALL_4 4
#define TILE_WALL_5 5

/* ���� ���� ����ü */
struct s_game_state {
    /* �÷��̾� ��ġ */
    double pos_x;
    double pos_y;

    /* �÷��̾� ���� */
    double dir_x;
    double dir_y;

    /* ī�޶� ��� */
    double plane_x;
    double plane_y;

    /* �̵� ���� */
    double move_speed;
    double rot_speed;

    /* ȭ�� ���� */
    char screen[SCREEN_HEIGHT][SCREEN_WIDTH];
};

/* �ʱ�ȭ �Լ� */
void init_game_state(struct s_game_state* state);
void setup_console(void);

/* ȭ�� ���� �Լ� */
void clear_screen(struct s_game_state* state);
void set_cursor_position(int x, int y);
void hide_cursor(void);

/* ������ �Լ� */
void render(struct s_game_state* state);
void draw_screen(struct s_game_state* state);

/* �Է� ó�� �Լ� */
int handle_input(struct s_game_state* state);

/* ��ƿ��Ƽ �Լ� */
double get_time(void);
void delay(int milliseconds);

/* �� ���� ���� ���� ���� */
extern const int g_world_map[MAP_WIDTH][MAP_HEIGHT];

#endif /* RAYCASTER_H */