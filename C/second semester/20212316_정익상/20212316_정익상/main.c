/*���� �Լ� ����*/

#include "Blocks.h"
#include "Controller.h"
#include "Screen.h"
#include "ConsoleCursor.h"

void main()
{
	CursorView(false);
	system("mode con cols=76 lines=28 | title â�ǽǹ�������Ʈ_��Ʈ���� �ǽ�");
	
	/*
	�Ʒ��� PlaySound �޼���� .wav ���� ������ ��������ִ� �Լ���.
	Controller.h ������Ͽ� include�� mmsystem.h ��������� ���� .wav������ ��½�Ű�µ�,
	������ ����� ��� �ش� ��Ḧ Controller.h�� BGM�� ��θ� ���ڿ��� �����ϸ� �ȴ�.
	�� ��, PlaySound(TEXT(BGM), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); ����
	*/

	while (true)
	{
		GameTitle();
		GameProcess();
	}
}