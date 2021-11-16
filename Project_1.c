/*#define _CRT_SECURE_NO_WARNINGS 
// fopen_s �� ���� �������� ���� '_s'�� �߰��� ���·� �Լ����� ��ü�Ǿ��� ������ '_s'�� ���� ������ �Լ�(fopen, strtok ��)��
// ����ϱ� ���� �ش� �ڵ带 �����Ͽ����ϴ�.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 10

typedef struct {//���� ����Ʈ�� ����� ����
	int coef;//����� ��Ÿ���� ����
	int expon;//������ ��Ÿ���� ����
	struct ListNode* link;//���� ����� �ּҰ��� �����ϴ� ������ ����
}ListNode;

typedef struct {//���� ����Ʈ ���
	int length;//����� ���̸� ��Ÿ���� ����
	ListNode* head;//ù��° ���
	ListNode* tail;//������ ���
}ListHeader;

void init(ListHeader* plist)//�ʱ�ȭ �Լ�
{
	plist->length = 0;//���̸� 0���� �Ͽ� �ʱ�ȭ
	plist->head = plist->tail = NULL;//ù��°, ������ ��带 NULL�� �ʱ�ȭ
}

void insert_node(ListHeader* plist, int coef, int expon)//plist ������� ��带 �߰��ϴ� �Լ�
{//plist�� ���� ����Ʈ�� ����� ����Ű�� ������, coef�� ���, expon�� ����
	for (ListNode* a = plist->head; a != NULL; a = a->link)
	{//plist�� head ��带 �����ϴ� ������ ���� a�� �����ϰ�
	 //head ��忡�� �����Ͽ� ������ ������(NULL�� �ƴ� ������) �̵�
		if (a->expon == expon)//�߰��Ϸ��� expon�� ���� expon�� ������ ��尡 plist�� ������ ���  
		{
			a->coef += coef;//���� expon�� ������ ��忡 coef�� ���Ѵ�.
			return;//�ʿ��� ������ �������Ƿ� �Լ��� ����������.
		}
		else if (a->expon < expon)
		{//�߰��Ϸ��� expon�� plist�� �����ϴ� ��� a�� expon���� ū ��찡 ���� ��
			ListNode* temp = (ListNode*)malloc(sizeof(ListNode));
			//�ش� plist�� ��� a�� ���ο� coef, expon ���� �Է��ϱ� ���� ��� a�� ����  
			//�Է��� ��� temp�� ���� �Ҵ��Ͽ� ����

			if (temp == NULL)// �޸� �Ҵ翡 �����Ͽ��� ��� ���
				printf("�޸� �Ҵ� ����");

			//��� a�� ������ ���� temp ��忡 �Է��Ѵ�
			//���� ���� plist�� ��� a�� ���� �Ҵ��Ͽ� ������ ��� temp�� ���� ���� ������.
			temp->coef = a->coef;//coef ����
			temp->expon = a->expon;//expon ����
			temp->link = a->link;//a ��尡 ����Ű�� ���� ��� �ּҰ� ����
			//������ ��� a�� �ִ� ���� temp ���� ��� ���������Ƿ� ���ο� coef, expon����
			//��� a�� �����Ѵ�
			a->coef = coef;//coef ����
			a->expon = expon;//expon ����
			a->link = temp;//���� ��� a�� expon���� ���ο� expon�� �� ũ�� ������ ��� a��
			//�� ���� expon�� ������ temp ����� �ּҰ��� ������ �Ѵ�.

			if (temp->link == NULL)//������ ��� a�� ��ũ�� NULL�̾��ٸ� ��� a�� tail ��忴�� ������ 
				plist->tail = temp;//temp ��带 ���ο� tail ���� ���Ѵ�.

			plist->length++;//plist�� ���ο� ��尡 �߰��Ǿ��� ������ ���̸� 1 ������Ų��.
			return;//�ʿ��� ������ �������Ƿ� �Լ��� ����������.
		}
	}
	//expon�� ��� a�� ��� expon���� ���� ���, �� ���� for�� ������ ���ǿ� �ش���� ���� ���
	ListNode* last = (ListNode*)malloc(sizeof(ListNode));
	//���ο� coef, expon ���� �����ϱ� ���� ��带 �����Ҵ����� �����Ѵ�

	if (last == NULL)// �޸� �Ҵ翡 �����Ͽ��� ��� ���
		printf("�޸� �Ҵ� ����");

	last->coef = coef;//�Լ����� �޾ƿ� coef���� �����Ѵ�.
	last->expon = expon;//�Լ����� �޾ƿ� expon���� �����Ѵ�.
	last->link = NULL;
	//expon ���� plist���� ���� �����Ƿ� temp ��尡 �������̱� ������ ���� ��� �ּҰ��� NULL�� �Ѵ�.

	if (plist->head == NULL)
		plist->head = plist->tail = last;
	//������ plist�� ��尡 �ϳ��� ���� ��� �߰��ϴ� temp ��尡 �����ϱ� ������
	//plist�� ù ����� head, ������ ����� tail�� ��� temp ���� �Ѵ�.

	else
	{
		plist->tail->link = last;
		//plist�� ��尡 �̹� ������ ��� temp ��带 ������ ���� �����ϱ� ������
		//������ tail ����� link�� ���� ����� temp�� �ּҰ����� �����Ѵ�.
		plist->tail = last;
		//temp ��尡 ���� �������̱� ������ plist�� tail ���� �����Ѵ�.
	}
	plist->length++;//plist�� ���ο� ��尡 �߰��Ǿ��� ������ ���̸� 1 ������Ų��.
}

void poly_add(ListHeader* plist1, ListHeader* plist2, ListHeader* plist3)
{// list3 = list1 + list2
	for (ListNode* a = plist1->head; a != NULL; a = a->link)
		//plist1�� head ��带 �����ϴ� ������ ���� a�� �����ϰ�
		//head ��忡�� �����Ͽ� ������ ������(NULL�� �ƴ� ������) �̵�
		insert_node(plist3, a->coef, a->expon);
	//a�� �����ϴ� plist1�� ����� coef, expon ���� plist3 ����� �����ϱ� ����
	//insert_node�Լ��� ȣ���Ѵ�.

	for (ListNode* a = plist2->head; a != NULL; a = a->link)
		//������ ���� a�� plist2�� head ��带 �����ϰ� �Ѵ�.
		//head ��忡�� �����Ͽ� ������ ������(NULL�� �ƴ� ������) �̵�
		insert_node(plist3, a->coef, a->expon);
	//a�� �����ϴ� plist2�� ����� coef, expon ���� plist3 ����� �����ϱ� ����
	//insert_node�Լ��� ȣ���Ѵ�.
}

void poly_mul(ListHeader* plist1, ListHeader* plist2, ListHeader* plist3)
{// list3 = list1 * list2
	//���׽��� ���� ������ ���׽��� ��� �׵��� ���� �������� �ϱ� ������
	ListHeader* p = plist3;
	for (ListNode* a = plist1->head; a != NULL; a = a->link)
		//ù��° for���� plist1�� head ��带 �����ϴ� ������ ���� a�� �����ϰ� ������ ������ �̵�
		for (ListNode* b = plist2->head; b != NULL; b = b->link)
			//�ι�° for���� plist2�� head ��带 �����ϴ� ������ ���� b�� �����ϰ� ������ ������ �̵�
			//�̷��� ���� for���� �����Ͽ� plist1, plist2�� ������ ��� ���鳢�� ������ ����ǰ� �Ѵ�.
			insert_node(plist3, a->coef * b->coef, a->expon + b->expon);
	//���׽��� �׵��� ���ϸ� ��������� ������ ����鰣�� �������� �������� ǥ���ǰ� 
	//���������� ������ �����鰣�� �������� ǥ���ȴ�.
	//���� a->coef�� b->coef�� ��, a->expon�� b->expon�� ���� ���� �߰��� �����
	//coef, expon���� �����Ѵ�.
}

void write_file(ListHeader* plist1, ListHeader* plist2)
//output.txt ������ �����ϰ� ���׽��� ����, ������ ����� ���Ͽ� ����ϴ� �Լ�
{
	ListNode* a = plist1->head;// plist1�� head��带 �����ϴ� ������ ���� a����
	ListNode* b = plist2->head;// plist2�� head��带 �����ϴ� ������ ���� b����

	FILE* file_read;//fopen�� ��ȯ�ϴ� FILE �����͸� ���� ���� file_read ����
	file_read = fopen("output.txt", "w");
	if (file_read == NULL) //���Ͽ��⿡ �����Ͽ��� ��츦 ���
		printf("���Ͽ��� ����\n");
	//fopen �Լ��� "output.txt" ������ ���� ���� ���� FILE ������ ���� file_read�� �����Ѵ�.

	fputs("Addition\n", file_read);
	//file_read�� ����Ű�� ���Ͽ� "Addition"�̶�� ���ڿ��� �Է��� �� ������ �����Ѵ�.
	for (; a != NULL; a = a->link)//������ ������(NULL�� �ƴ� ������) �̵�
		fprintf(file_read, "%d %d\n", a->coef, a->expon);
	//plist1�� ������ coef, expon ������ ������ ���� �����ϸ� ���������� ���Ͽ� �ۼ�

	fputs("Multiplication\n", file_read);
	//file_read�� ����Ű�� ���Ͽ� "Addition"�̶�� ���ڿ��� �Է��� �� ������ �����Ѵ�.
	for (; b != NULL; b = b->link)//������ ������(NULL�� �ƴ� ������) �̵�
		fprintf(file_read, "%d %d\n", b->coef, b->expon);
	//plist2�� ������ coef, expon ������ ������ ���� �����ϸ� ���������� ���Ͽ� �ۼ�

	fclose(file_read);//���Ͽ� ���� �۾��� �Ϸ�Ǿ����� ������ �ݾ��ش�.
}

int main(void)
{
	ListHeader list1, list2, add_list3, mul_list3;
	//ListHeader�� ���� list1, list2, add_list3, mul_list3 ����

	//���� ����Ʈ�� �ʱ�ȭ
	init(&list1);
	init(&list2);
	init(&add_list3);
	init(&mul_list3);

	int coef, expon;//���, ������ ���� ���� ������ ���� coef, expon
	char source[MAX_SIZE];//���Ͽ��� �о���� ���ڿ��� �����ϱ� ���� �迭
	char* buf;//���ҵ� ���ڿ��� ������ ���� ������ ����
	int data[MAX_SIZE];//strtok �Լ��� ������ ���ڿ� ������ ���� �����ϱ� ���� �迭
	int list_num = 1;
	//ù��°, �ι�° ���׽��� �����ϱ� ���� ����(�ڼ��� ������ ���� if�� �ּ��� �ֽ��ϴ�.)

	FILE* file_read;//fopen�� ��ȯ�ϴ� FILE �����͸� ���� ���� file_read ����
	file_read = fopen("input.txt", "r");
	//fopen �Լ��� "input.txt" ������ �б� ���� ���� FILE ������ ���� file_read�� �����Ѵ�.
	if (file_read == NULL) //���Ͽ��⿡ �����Ͽ��� ��츦 ���
		printf("���Ͽ��� ����\n");

	while (fgets(source, sizeof(source), file_read) != NULL)
	{//fgets �Լ��� ������ ������ ������ ���� ������ ����
		int count = 0;//���ڿ��� ������ Ƚ���� ����ϴ� ����
		buf = strtok(source, " ");//strtok �Լ��� ������ �������� ���ڿ��� ������ �� ������ ��ȯ

		while (buf != NULL)//�ڸ� ���ڿ��� ������ ���� ������ �ݺ��Ѵ�.
		{
			data[count] = atoi(buf);//ù��° ������ ���, �ι�° ������ ������ �迭�� ����ȴ�.
			//������ ���·� ������ ������ ����Ǳ� ������ ���������� ���� �������·� ��ȯ
			count++;//while���� ����� ������ 1 ����
			buf = strtok(NULL, " ");//���� ���ڿ��� �߶� �����͸� ��ȯ�Ѵ�.
		}

		if (count < 2)
		{//"input.txt" ���Ͽ����� �� ���׽� �����͸� �߰��� ���� �������� �����Ѵ�.
		 // ���� ���� �� �κ��� fgets �Լ��� �а� strtok �Լ��� �����ϸ�
		 // ���๮�� �ϳ��� �����ϱ� ������ count�� 1�� �ȴ�.
		 // ���� ���� ������ �������� �� ���ǹ��� ����ȴ�.
			list_num = 2;
			//count�� 1�� �Ǹ� ���鹮�� ������ ������, �� �ι�° ���׽� �����͸� �о���δ�.
			//���� if(count < 2) ���ǹ��� ����Ǳ� �������� ù��° ���׽��� �����Ͱ�
			//list1 ����� ����� ������ ������ ���ǰ�
			//���ǹ� ���� ���Ŀ��� list2 ����� ����� ������ ������ ���ȴ�.
			continue;
			//���� ������ ��忡 �Էµ� �����Ͱ� �����Ƿ� ������ ������ �������� �ʰ�
			//while�� ���� �Ǵܺκ����� �ǵ��ư���.
		}

		if (list_num == 1)
			//ó���� list_num�� 1�� �ʱ�ȭ �����Ƿ� if(count < 2) ���ǹ��� ����Ǳ� ������,
			//�� �� ���׽� ������ ������ ���� ������ ����Ǳ� ������ ù��° ���׽� ������
			//�� list1 ����� ����� ������ �����Ѵ�.
			insert_node(&list1, data[0], data[1]);
		else
			//if(count < 2) ���ǹ����� list_num�� 2�� ����ǹǷ� �ش� if�� ���� ����
			//�ι�° ���׽� �����͸� list2 ����� ��� ������ �����Ѵ�.
			insert_node(&list2, data[0], data[1]);
	}

	poly_add(&list1, &list2, &add_list3);//�� ���׽��� ���ϱ� ���� �Լ� ȣ��

	poly_mul(&list1, &list2, &mul_list3);//�� ���׽��� ���ϱ� ���� �Լ� ȣ��

	write_file(&add_list3, &mul_list3);//"output.txt"������ �����ϱ� ���� �Լ��� ȣ��

	fclose(file_read);//���Ͽ� ���� �۾��� �Ϸ�Ǿ����Ƿ� ������ �ݴ´�.
	return 0;
}*/