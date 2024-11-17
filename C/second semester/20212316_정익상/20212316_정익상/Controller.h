#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include <stdbool.h>

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

enum boardProperty {
	BoardWidth=14, //���� ���� ����
	BoardHeight=22, //���� ���� ����
	BoardX=4, //�ܼ�â���� ��µ� ���� X��ǥ
	BoardY=3 //�ܼ�â���� ��µ� ���� Y��ǥ
};

enum Keys {
	LEFT=75, //Ű���� ���� Ű
	RIGHT=77, //Ű���� ������ Ű
	UP=72, //Ű���� �� Ű
	DOWN=80, //Ű���� �Ʒ� Ű
	ARROW=224, //Ű���� ����Ű �Ǻ�
	SPACEBAR=32 //Ű���� �����̽� Ű
};

typedef enum gameSpeed {
	easy=230, //���� ���̵� ����
	normal=170, //���� ���̵� ����
	hard=150 //���� ���̵� �����
}SPPED;

enum blockProperty {
	BlockSIZE=4, //����� ũ��
	BlockRotate=4, //����� ȸ��
	BlockType=7, //����� ����
	BlockStartX=14, //����� Ŀ��â���� ���۵� x��ǥ
	BlockStartY=4 //����� Ŀ��â���� ���۵� y��ǥ
};

//����ڰ� �Է��� Ư�� ����Ű �ƽ�Ű �ڵ尪
int nkey;
//���� ��� ���
short curShape;
void CurrentShape();
//���� ��� ���
short nexShape;
void NextShape();
//����� ���� ��ǥ
short BlockCOL;
short BlockROW;
//����� ���� ��ǥ
short prevBlockCOL;
short prevBlockROW;
//��Ʈ���� ���� �� ���� ��ġ�ϴ� Ŀ��
COORD Cursor;
//���� ��� �����
void deletePrevBlock();
//���ھ�
int static score;
void printScore();
//���� ���̵�
short gameLevel;
//���ο� ��� ���� ����
void newBlock();

boolean IsNextBlock;

short turn;
void RotateBlock(); //����Ű ���� �� rotate�� ���� ����Ű ����

boolean IsCollision(int shape, int rotate, int curX, int curY);
//����� bottom�� ����� ���
void blockFixed(int shape, int rotate);

boolean IsMaxLine();
void deleteLine();
//ó�� ������ ����� õ��� ��Ҵ°�
boolean IsOverHeight();
//�̸����� ��� ����� ���� �� �ش� ��ǥ �����ؾ� �Ѵ�
COORD previewPoint;
//�̸����� ���, ���� ��� ���Ŀ� ���� ��� ȭ�鿡 ǥ��
void previewBlock(int shape, int rotate);
//��Ʈ���� ���ư��µ� �־� ���� �߿��� �Լ�(�����Լ���)
void GameProcess();