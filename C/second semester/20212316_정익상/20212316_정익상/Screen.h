#pragma once
#include "Blocks.h"
#include "Controller.h"
#include <Windows.h>

#ifndef SCREEN_H
#define SCREEN_H

// �ܼ� ȭ�� ���� ���� ����ü
typedef struct _SCREEN_BUFFER {
    HANDLE hBuffer;           // ���� �ڵ�
    CHAR_INFO* bufferData;    // ���� ������
    COORD bufferSize;         // ���� ũ��
    COORD bufferCoord;        // ���� ��ǥ
    SMALL_RECT writeRegion;   // ���� ����
} SCREEN_BUFFER;

// ���� ���� ����
extern SCREEN_BUFFER g_screenBuffer;
extern HANDLE g_originalConsoleHandle;

// ���� ���۸� ���� �Լ� ����
void InitScreenBuffer(void);              // ��ũ�� ���� �ʱ�ȭ
void ClearScreenBuffer(void);             // ��ũ�� ���� Ŭ����
void FlipScreenBuffer(void);              // ��ũ�� ���� ��ȯ
void ReleaseScreenBuffer(void);           // ��ũ�� ���� ����
void WriteToScreenBuffer(int x, int y, char* str, WORD color);  // ���ۿ� ���ڿ� ����

// ȭ�� ũ�� ���
#define CONSOLE_WIDTH 76
#define CONSOLE_HEIGHT 28

// ��� ���� ���
#define BLOCK_CHAR "��"
#define EMPTY_CHAR "  "
#define WALL_CHAR "��"

// ���� ������
typedef enum Color {
    BLACK,           //0
    DARK_BLUE,       //1
    DARK_GREEN,      //2
    DARK_SKYBLUE,    //3
    DARK_RED,        //4
    DARK_VOILET,     //5
    DARK_YELLOW,     //6
    GRAY,            //7
    DARK_GRAY,       //8
    BLUE,            //9
    GREEN,           //10
    SKYBLUE,         //11
    RED,             //12
    VIOLET,          //13
    YELLOW,          //14
    WHITE            //15
} COLOR;

// ���� ������
int static level;
extern int NextBlock[6][6];
int static Rotation = 0;

// ���� �Լ���
void textcolor(int color_number);
void GameTitle(void);
void InGameFrame(void);
void ShowNextBlock(void);
void DeleteNextBlock(void);
void EndGameFrame(void);
void createBoards(void);
void printBoards(void);
void addBlock(int shape, int rotate);
void deleteBlock(void);

// ���� ����
extern int board[BoardHeight][BoardWidth];

#endif // SCREEN_H