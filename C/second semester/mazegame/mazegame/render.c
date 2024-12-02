/* render.c */
#include <math.h>
#include <stdio.h>
#include "raycaster.h"

/* �� �������� ����� ���� �迭 */
static const char WALL_CHARS[] = { ' ', '#', '@', '$', '=', '*' };
static const char SHADED_WALL_CHARS[] = { ' ', '.', ',', '\'', '-', '+' };

/* ȭ���� �׸��� �Լ� */
void draw_screen(struct s_game_state* state)
{
    int i, j;

    /* Ŀ���� ȭ�� ���� ������� �̵� */
    set_cursor_position(0, 0);

    /* ������ ������ ȭ�鿡 ��� */
    for (i = 0; i < SCREEN_HEIGHT; i++) {
        for (j = 0; j < SCREEN_WIDTH; j++) {
            putchar(state->screen[i][j]);
        }
        putchar('\n');
    }
}

/* ����ĳ���� ������ ���� */
void render(struct s_game_state* state)
{
    int x;

    /* ȭ�� ���� �ʱ�ȭ */
    clear_screen(state);

    /* �� �������� ���� ����ĳ���� ���� */
    for (x = 0; x < SCREEN_WIDTH; x++) {
        /* ī�޶� ������ x��ǥ ��� (-1.0 ~ 1.0) */
        double camera_x = 2.0 * x / (double)SCREEN_WIDTH - 1.0;

        /* ������ ���� ���� ��� */
        double ray_dir_x = state->dir_x + state->plane_x * camera_x;
        double ray_dir_y = state->dir_y + state->plane_y * camera_x;

        /* ���� �� ��ġ */
        int map_x = (int)state->pos_x;
        int map_y = (int)state->pos_y;

        /* DDA �˰����� ���� ������ */
        double delta_dist_x = fabs(ray_dir_x) < 1e-10 ? 1e10 : fabs(1.0 / ray_dir_x);
        double delta_dist_y = fabs(ray_dir_y) < 1e-10 ? 1e10 : fabs(1.0 / ray_dir_y);

        double side_dist_x, side_dist_y;
        int step_x, step_y;
        int hit = 0;  /* ���� �浹 ���� */
        int side;     /* NS �Ǵ� EW ���� �浹 */

        /* �ܰ� ����� �ʱ� ���̵� �Ÿ� ��� */
        if (ray_dir_x < 0) {
            step_x = -1;
            side_dist_x = (state->pos_x - map_x) * delta_dist_x;
        }
        else {
            step_x = 1;
            side_dist_x = (map_x + 1.0 - state->pos_x) * delta_dist_x;
        }

        if (ray_dir_y < 0) {
            step_y = -1;
            side_dist_y = (state->pos_y - map_y) * delta_dist_y;
        }
        else {
            step_y = 1;
            side_dist_y = (map_y + 1.0 - state->pos_y) * delta_dist_y;
        }

        /* DDA �˰��� ���� */
        while (hit == 0) {
            /* x��� y�� �� ����� ������ �̵� */
            if (side_dist_x < side_dist_y) {
                side_dist_x += delta_dist_x;
                map_x += step_x;
                side = 0;
            }
            else {
                side_dist_y += delta_dist_y;
                map_y += step_y;
                side = 1;
            }

            /* ���� �浹 üũ */
            if (g_world_map[map_x][map_y] > 0)
                hit = 1;
        }

        /* �������� ���� �Ÿ� ��� */
        double perp_wall_dist;
        if (side == 0)
            perp_wall_dist = (map_x - state->pos_x + (1 - step_x) / 2) / ray_dir_x;
        else
            perp_wall_dist = (map_y - state->pos_y + (1 - step_y) / 2) / ray_dir_y;

        /* ȭ�鿡 �׸� �������� ���� ��� */
        int line_height = (int)(SCREEN_HEIGHT / perp_wall_dist);

        /* �������� �������� ���� ��� */
        int draw_start = -line_height / 2 + SCREEN_HEIGHT / 2;
        if (draw_start < 0)
            draw_start = 0;

        int draw_end = line_height / 2 + SCREEN_HEIGHT / 2;
        if (draw_end >= SCREEN_HEIGHT)
            draw_end = SCREEN_HEIGHT - 1;

        /* �� Ÿ�Կ� ���� ���� ���� */
        int wall_type = g_world_map[map_x][map_y];
        char wall_char = (side == 1) ?
            SHADED_WALL_CHARS[wall_type] : WALL_CHARS[wall_type];

        /* ������ �׸��� */
        int y;
        for (y = draw_start; y <= draw_end; y++) {
            state->screen[y][x] = wall_char;
        }

        /* �ٴڰ� õ�� �׸��� */
        for (y = 0; y < draw_start; y++)
            state->screen[y][x] = ' ';  /* õ�� */
        for (y = draw_end + 1; y < SCREEN_HEIGHT; y++)
            state->screen[y][x] = '.';  /* �ٴ� */
    }
}