/*�ܼ�â�� Ŀ���� ��ġ ���� �Լ����� ���ǵǾ� ����*/

#include "ConsoleCursor.h"
#include <Windows.h>

/*
�ܼ�â���� Ŀ���� ���ϴ� ��ǥ�� �̵�
���ڰ� x, y�� ��ġ�� Ŀ���� �̵�
*/

void GotoXY(int x, int y) {
	//Ŀ���� ���ϰ����ϴ� ��ǥ�� �̵�
	COORD Pos = { 0, 0 };	
	Pos.X = x;
	Pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

/*
getCursor() ��ȯ���� COORD �ڷ����� Pos�����ε�, 
COORD�� ���� ��ŷ�� ����, ������ x, y ���� ���� ����ü��.
���� Pos�� ��ȯ�Ѵٴ� ���� GetStdHandle�� ���ؼ�
�ܼ�â���� ��ġ�ϴ� Ŀ���� ��ǥ x, y�� ����ü Coord�� Pos�� ���� �����Ѵ�.
�׸��� �ܼ�â���� ��µǰ� �ִ� ���� Ŀ�� ��ġ�� ��ȯ�Ѵ�.
*/

COORD getCursor(void)
{
	//���� Ŀ�� ��ġ�� ��ȯ
	COORD Pos = {0, 0}; //�ʱ�ȭ
	CONSOLE_SCREEN_BUFFER_INFO curInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	Pos.X = curInfo.dwCursorPosition.X;
	Pos.Y = curInfo.dwCursorPosition.Y;
	return Pos;
}

/*
CursorView(boolean show)
���ڰ��� ���� Ŀ���� �����̴°� ��������, ��������
CursorInfo�� �����ִ� �Լ���.
*/

void CursorView(boolean show) {
	CONSOLE_CURSOR_INFO ConsoleCursor = { 0, 0 };
	ConsoleCursor.bVisible = show;
	ConsoleCursor.dwSize = 10;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ConsoleCursor);
}