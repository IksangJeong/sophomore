#pragma warning(disable:4996)
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void size_add(int* s, int* arr);

void main() {
	//size�� 3�� �迭 ����
	int* Np;
	int i = 0;
	int nSize = 3;
	Np = malloc(sizeof(int) * nSize);

	scanf_s("%d", &Np[0]);

	if (Np[0] != -1) {
		for (i = 1;; i++) {
			scanf_s("%d", &Np[i]);
			if (Np[i] == -1)
				break;
			if (i + 1 >= nSize) {
				//�ִ�� ������ų �� �ִ� �迭�� ũ��� 20���� ����
				size_add(&nSize, &Np);
				if (nSize > 20) {
					printf("�迭 ũ��� 20�� �ʰ��� �� �����ϴ�.\n");
					free(Np);
					return 1; //���� �ڵ� ��ȯ
				}
			}

		}

		for (int j = 0; j <= i; j++)
			printf("%d", Np[j]);
	}
}

void size_add(int* s, int** arr) {
	//���ο� Size�� ȹ���Ѵ�.
	int nNewSize = *s + 3;
	int* temp;
	temp = malloc(sizeof(int) * nNewSize);

	//temp�� ������ �迭 ���ҵ��� �����Ѵ�.
	for (int i = 0; i < *s; i++)
		temp[i] = (*arr)[i];

	//������ arr�޸𸮵��� �����Ѵ�.
	free(*arr);

	*arr = temp;
	*s = nNewSize;
}