#define _CRT_SECURE_NO_WARNINGS 
// fopen_s �� ���� �������� ���� '_s'�� �߰��� ���·� �Լ����� ��ü�Ǿ��� ������ '_s'�� ���� 
// ������ �Լ�(fopen, strtok ��)�� ����ϱ� ���� �ش� �ڵ带 �����Ͽ����ϴ�.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX(a, b) (((a) > (b)) ? (a) : (b))// height �Լ����� ���
#define MAX_SIZE 10// ���Ͽ��� �о���� ���ڿ��� ������ �迭�� ũ��
#define MAX_QUEUE_SIZE 100// ť�� ũ��

int group[100];// "output.txt"���Ͽ����� �����͸� �ۼ��� �� �ʿ��� ������ ���� �����ϴ� �迭
int cnt;// group �迭�� �ε���
int exist;// delete ���꿡�� �����ؾ��� key���� ���� ������ �Ǻ��ϴ� ����
int height;//Ʈ���� ���̸� �����ϴ� ����
int sch;// search �Լ��� ��ȯ���� ������� ����

typedef struct {// Binary Search Tree ����� ����
	int key;// ����� key���� ����޴� ������ ����
	struct TreeNode* left, * right;// ����� ����, ������ �ڽĳ���� �ּҸ� ����޴� ������ ����
}TreeNode;

typedef struct{// ť�� ����
	TreeNode* data[MAX_QUEUE_SIZE];// ����� ������� ����Ǵ� �迭
	int front, rear;// front�� ť�� ù ��° ��� �ϳ� ���� �ε���, rear�� ������ ����� �ε���
}QueueType;

int get_height(TreeNode* root)// Ʈ���� ���̸� ���ϴ� �Լ� get_height
{
	height = 0;
	if (root->key != -1)// ����� key���� -1�� �ƴ� ������ ���� 
		height = 1 + max(get_height(root->left), get_height(root->right));
	//���ʰ� ������ Ʈ�� �� �ִ񰪰� root��� �ϳ��� ���� 1�� ���Ѵ�.
	return height;
}

void error(char* message)
// ���� ���꿡�� ���� �߻��� �޼����� ����ϴ� �Լ� error
{
	fprintf(stderr, "%s\n", message);
	exit(1);
}

void init_queue(QueueType* q)// ť�� �ʱ�ȭ �ϴ� �Լ� init
{
	q->front = q->rear = 0;
}

int empty(QueueType* q)// ť�� ��������� �� TRUE�� ��ȯ�ϴ� �Լ� empty
{
	return (q->front == q->rear);
}

int full(QueueType* q)// ť�� ��ȭ������ �� TRUE�� ��ȯ�ϴ� �Լ� full
{
	return ((q->rear + 1) % MAX_QUEUE_SIZE == q->front);
}

void enqueue(QueueType* q, TreeNode* item)// ť�� ��带 �����ϴ� �Լ� enqueue
{
	if (full(q))
		error("ť�� ��ȭ�����Դϴ�");
	q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;// ������ ������ ����Ͽ� �ε����� �������� ȸ����Ŵ
	q->data[q->rear] = item;
}

TreeNode* dequeue(QueueType* q)// ť�� ��带 �����ϴ� �Լ� dequeue
{
	if (empty(q))
		error("ť�� ��������Դϴ�");
	q->front = (q->front + 1) % MAX_QUEUE_SIZE;// ������ ������ ����Ͽ� �ε����� �������� ȸ����Ŵ
	return q->data[q->front];
}

void level_order(TreeNode* root)// level order ������ ���Ͽ� �����͸� ����ϴ� �Լ� level_order
{
	QueueType q;// ť ����ü ���� q ����
	TreeNode* zl, * zr;// ��� ����ü ���� zl, zr ����

	init_queue(&q);// ť �ʱ�ȭ

	if (root == NULL)// ��尡 �ϳ��� ���ٸ� �ٷ� �Լ��� ����������
		return;
	enqueue(&q, root);// root ��带 ���� ť�� ����
	while (!empty(&q))// ť�� ������°� �ƴ� ������
	{
		root = dequeue(&q);// ť�� ����� ��带 ������ root�� ����
		if (cnt < powf(2, height) - 1 && root->key != -1)
		{
		// ť���� ��ȯ�� �����͸� ���Ͽ� ����� ������ �����ϴ� �迭 group
		// ����Ʈ������ ����� �ִ� ������ 2�� Ʈ���� ���̸�ŭ ������ ������ 1�� �� ���� ����
		// ����ִ� ��带 0���� ����ϱ� ���� ����ִ� ��带 �߰��� -1�� ���� ������ ����
		// �����Ҵ� �Ͽ����� -1�� key���� ������ ��尡 NULL�� ����ϰ� �˴ϴ�.
		// �ڼ��� ������ �Ʒ����� �����ϰڽ��ϴ�
		// ���� ����Ʈ������ ���Ǵ� �ִ� ����� ���� �����̰� key���� -1�� �ƴ� ��� �Ʒ� ������ ���� 
			group[cnt] = root->key;// key���� group �迭�� ����
			cnt++;// �ε��� ����
		}
		else if (cnt < powf(2, height) - 1 && root->key == -1)
		// key���� -1�� ���� ����ִ� ��带 0���� �߰��ϱ� ���� �����Ҵ�Ǿ� �߰��� ����Դϴ�
		{
			group[cnt] = 0;// 0�� �迭�� ����
			cnt++;
		}

		if (root->left != NULL && cnt < powf(2, height) - 1)
		// root�� ���� �ڽĳ�尡 NULL�� �ƴϰ� ��� ������ �ִ���밳�� ������ ���
			enqueue(&q, root->left);// ���� �ڽĳ�带 ť�� ����
		else if (root->left == NULL && cnt < powf(2, height) - 1)
		// root�� ���� �ڽĳ�尡 NULL �̰� ��� ������ �ִ���밳�� ������ ���
		{
			// NULL, �� ����ִ� ��带 0���� ����ϱ� ���� key���� -1�� �����ϰ�
			// �ش� ����� ���� �ڽĳ���� zl�� �����Ҵ��Ͽ� �������ݴϴ�
			zl = (TreeNode*)malloc(sizeof(TreeNode));
			if (zl == NULL)// �����Ҵ� ������ ���
			{
				error("�����Ҵ� ����");
				return;
			}
			zl->key = -1;// key������ -1 ����
			zl->left = zl->right = NULL;// zl����� ����, ������ ��带 NULL�� ����
			root->left = zl;// root->left�� zl ��带 ����
			enqueue(&q, root->left);// -1�� key���� ������ root->left ��带 ť�� ����
		}
		if (root->right != NULL && cnt < powf(2, height) - 1)
		// root�� ������ �ڽĳ�尡 NULL�� �ƴϰ� ��� ������ �ִ���밳�� ������ ���
			enqueue(&q, root->right);
		else if (root->right == NULL && cnt < powf(2, height) - 1)
		{
			zr = (TreeNode*)malloc(sizeof(TreeNode));// ������ �ڽĳ�� zr �����Ҵ�
			if (zr == NULL)// �����Ҵ� ������ ���
			{
				error("�����Ҵ� ����");
				return;
			}
			zr->key = -1;
			zr->left = zr->right = NULL;
			root->right = zr;// root->right�� zr ��带 ����
			enqueue(&q, root->right);// -1�� key���� ������ root->right ��带 ť�� ����
		}
	}
}

void init(TreeNode* p)// p�� �����ϴ� ��带 �ʱ�ȭ�ϴ� �Լ�
{
	p->left = p->right = p->key = NULL;
}

void free_postorder(TreeNode* root)// ��带 �����ϴ� �Լ� free_postorder
// ����ִ� ��带 0���� ����ϱ� ���� -1�� key���� �������� �����Ҵ� �� ������ �������� ������ ����
// �ʿ� �������� ��� ��带 ������ȸ ������ �������ݴϴ�.
{
	if (root)
	{
		free_postorder(root->left);
		free_postorder(root->right);
		free(root);
	}
}

void insert_node(TreeNode* root, int key)
// root�� �����ϴ� ��忡 �ش� key���� ������ ��带 �����ϴ� �Լ�
{
	TreeNode* p, * t;// p�� �θ���, t�� ������
	TreeNode* n, * zl, * zr;// n�� ���ο� ���
	t = root;
	p = NULL;
	while (t->key != -1)// ���ڷ� ���� key���� �˸��� ��ġ�� ã�´�
	{
		if (t->key == NULL)
			// root ����� key���� NULL�� �ʱ�ȭ �Ǿ����Ƿ� 
			// ó�� �޴� key���� root ����� key������ ����ǰ� �մϴ�
		{
			t->key = key;
			//root ����� ����, ������ �ڽĳ�尡 ��� NULL �̱� ������ -1�� ���� ������ ���� �����մϴ�
			zl = (TreeNode*)malloc(sizeof(TreeNode));// ���� �ڽĳ�� �����Ҵ�
			if (zl == NULL)// �����Ҵ� ������ ���
			{
				error("�����Ҵ� ����");
				return;
			}
			zr = (TreeNode*)malloc(sizeof(TreeNode));// ������ �ڽĳ�� �����Ҵ�
			if (zr == NULL)// �����Ҵ� ������ ���
			{
				error("�����Ҵ� ����");
				return;
			}
			zl->key = -1;
			zl->left = zl->right = NULL;
			zr->key = -1;
			zr->left = zr->right = NULL;
			t->left = zl;// root->left ��忡 ����
			t->right = zr;// root->right ��忡 ����
			height = get_height(root);// level_order �Լ����� ���� �������� height�� Ʈ���� ���� ����
			return;// ��带 �߰��� �ʿ䰡 �����Ƿ� �Լ��� ����������.
		}
		p = t;// ���� ����� ������ �θ���� ����ϱ� ���� ������ t ��带 p ��忡 ����
		if (key < p->key)
			t = p->left;
		// ���ڷ� ���� key���� ��� p�� key������ ���� ��� p ����� ���� �ڽĳ��� �̵� 
		else if (key > p->key)
			t = p->right;
		// ���ڷ� ���� key���� ��� p�� key������ Ŭ ��� p ����� ������ �ڽĳ��� �̵�
		else
		{
			t = p->left;
			break;
		}
		//���ڷ� ���� key���� ��� p�� key���� ���� ��� p ����� ���� �ڽĳ��� �̵�
	}
	n = (TreeNode*)malloc(sizeof(TreeNode));// key�� Ʈ�� �ȿ� �����Ƿ� ���� ����
	if (n == NULL)// �����Ҵ� ������ ���
	{
		error("�����Ҵ� ����");
		return;
	}
	n->key = key;// ������ ����
	n->left = t->left;
	if (n->left == NULL && key != p->key)
	{// t->left ��尡 NULL �̰� key���� �θ����� key���� ���� ���� ��� ���� �ڽĳ�带 �����Ҵ��Ͽ� ���� 
		zl = (TreeNode*)malloc(sizeof(TreeNode));
		if (zl == NULL)// �����Ҵ� ������ ���
		{
			error("�����Ҵ� ����");
			return;
		}
		zl->key = -1;
		zl->left = zl->right = NULL;
		n->left = zl;
	}
	n->right = t->right;
	if (n->right == NULL || key == p->key)
	{// t->right ��尡 NULL �̰ų� key���� �θ����� key���� ���� ��� ������ �ڽĳ�带 �����Ҵ��Ͽ� ����
		zr = (TreeNode*)malloc(sizeof(TreeNode));
		if (zr == NULL)// �����Ҵ� ������ ���
		{
			error("�����Ҵ� ����");
			return;
		}
		zr->key = -1;
		zr->left = zr->right = NULL;
		n->right = zr;
	}
	if (p != NULL)// �θ���� ��ũ ����
	{
		if (key < p->key)
			p->left = n;
			// ���ڷ� ���� key���� ��� p�� key������ ���� ��� p ����� ���� �ڽĳ�忡 ����
		else if (key > p->key)
			p->right = n;
			// ���ڷ� ���� key���� ��� p�� key������ Ŭ ��� p ����� ������ �ڽĳ�忡 ����
		else
		{// ������ key���� ������ ��带 �����ϴ� ��� ���� ���õ� �ٿ� ���� left child, �� ���� �ڽĳ�忡
		 // �����ϱ� ���� ������ key���� ������ �θ��� p�� ���� �ڽĳ��� ����
			n->left = t;//������ p ����� ���� �ڽĳ���� t ��带 ���� ���ԵǴ� ��� n�� ���� �ڽĳ��� ����
			p->left = n;// p ����� ���� �ڽĳ�忡 ��� n�� ����
		}
	}
	else
		root = n;// root ��尡 NULL�̹Ƿ� root ��忡 ��� n�� ����
	height = get_height(root);// level_order �Լ����� ���� �������� height�� Ʈ���� ���� ����
}

void delete_node(TreeNode* root, int key)
// root�� �����ϴ� ��忡�� �ش� key���� ������ ��带 �����ϴ� �Լ�
{
	TreeNode* p, * t, *left, *right, * child, * small, * min;
	// p�� �θ���, t�� ������, left�� ���� �ڽĳ��, right�� ������ �ڽĳ��
	// child�� �ڽĳ��, small�� �ּڰ��� ������ ���, min�� key���� �ߺ��� �Ǻ��ϴ� ��带 ���մϴ�

	exist = 1;// ���ڷ� ���� key���� ���� ������ �Ǻ��ϴ� ������ 1�� �ʱ�ȭ
	t = root;
	// �ش� key���� ã�� ���ؼ��� root ���κ��� ����Ʈ���� �̵��ؾ� �ϹǷ� ó�� Ž���� ��带
	// root ���� �����Ѵ�
	p = NULL;
	min = NULL;

	while (t->key != -1 && t->key != key)// key���� ������ ��� t�� ã�´�
	{
		p = t;// ������ ��� t�� �Ʒ��� ���� ���� ���� t�� ���� ��尡 �Ǳ� ������ p�� �����Ѵ�
		t = (key < t->key) ? t->left : t->right;
		// ���ڷ� ���� key���� ��� t�� key�� ���� ������ ���� ���, ũ�� ������ ���� �̵�
	}
	if (t->key == -1)
	{
		exist = 0;// key�� �������� ���� ��� exist ������ 0�� �����ϰ� �ٷ� �Լ��� ����������.
		height = get_height(root);// level_order �Լ����� ���� �������� height�� Ʈ���� ���� ����
		return;
	}
	left = t->left;
	right = t->right;
	if ((left->key == -1) && (right->key == -1))// �����Ϸ��� ����� �ڽĳ���� key���� ��� -1�� ���
	{
		if (p != NULL)// t�� �θ����� p�� NULL�� �ƴ� ���
			t->key = -1;// ��� t�� key���� -1�� ���������ν� ���ϴ� key���� �����
		else// p�� NULL�̶�� root ��尡 �����ȴ�.
			root = NULL;
		free_postorder(left);// �ʿ������ �����Ҵ�� ��� ����
		free_postorder(right);
		t->left = t->right = NULL;// ����ų �ڽĳ�尡 �����Ƿ� NULL�� ����
	}
	else if ((left->key == -1) || (right->key == -1))
		// �����Ϸ��� ����� �ڽĳ�� �� �ϳ��� key���� -1�� ���
	{
		child = (left->key != -1) ? left : right;
		// ����, ������ ��� �� NULL�� �ƴ� ��带 child ������ ����
		if (p != NULL)
		{
			if (p->left == t)// ���� �ڽĳ�尡 -1�� �ƴ� key���� ���� ���
			{
				p->left = child;// ���� �ڽĳ�带 �θ����� ���� �ڽĳ��� ����
				t->left = NULL;// t->left�� ����ų ��尡 �����Ƿ� NULL�� �����Ѵ�
			}
			else// ������ �ڽĳ�尡 -1�� �ƴ� key���� ���� ���
			{
				p->right = child;// ������ �ڽĳ�带 �θ����� ���� �ڽĳ��� ����
				t->right = NULL;// t->right�� ����ų ��尡 �����Ƿ� NULL�� �����Ѵ�
			}
		}
		else
			root = child;// p�� NULL�̶�� child ��尡 root ��尡 �ȴ�.
		free_postorder(t);// �ʿ������ t ��带 �����Ѵ�
	}
	else// �����Ϸ��� ����� �ڽĳ�尡 ��� NULL�� �ƴ� ���
	{
		small = t->right;// �����Ϸ��� ����� ������ �ڽĳ��� �̵�
		left = small->left;
		right = small->right;
		while (left->key != -1)
			// ���� �ش� Ʈ�������� �ּڰ��� ã�� ���� -1�� key���� ���� �� ���� ���� �ڽĳ��� �̵�
		{
			small = left;// ������ left ��带 small ��忡 �����Ͽ� ���� ���꿡���� �θ���� ����Ѵ�
			left = small->left;// ���� �ڽĳ��� �̵�
			right = small->right;// ������ �ڽĳ��� �̵�
			if (left->key == small->key)// �ߺ��Ǵ� key���� ������ ��� �ش� ��带 min�� �����մϴ�
				min = small;
		}
		if (min != NULL && min->key == left->key)
			// ���� �ߺ��Ǵ� key���� �ּڰ��� ��� ���� ���� �ߺ� �� ���� ���� Ž���Ǵ� ����
			// �����ϱ� ���� ���ʷ� ����� ��� min�� left�� �����մϴ�.
			left = min;
		t->key = small->key;// t ����� ���� ����Ʈ�������� �ּڰ��� t ����� key������ �����Ѵ�.
		if (right->key == -1)
			// ������ �ڽĳ���� key���� -1�̶�� ���� small����� �ڽĳ���� key���� ��� -1, �� �ڽĳ�尡
			// ���ٴ� �ǹ��̴�
		{
			// ���� ������ �̹� ����� �����Ϸ��� ����� �ڽĳ���� key���� ��� -1�� ���� �����ϰ� ����
			small->key = -1;
			free_postorder(left);
			free_postorder(right);
			small->left = small->right = NULL;
		}
		else// �ּڰ��� ������ �ڽĳ�尡 �ϳ��� �ڽ��� ���� ���
		{
			child = right;//�ּڰ��� ������ small����� ������ �ڽĳ�带 child ��忡 �����Ѵ�.
			free_postorder(left);
			// small��尡 child ����� ������ ����ϰ� �Ѵ�
			small->key = child->key;
			small->left = child->left;
			small->right = child->right;
			free(child);// �ʿ������ child ��� ����
		}
	}
	height = get_height(root);// level_order �Լ����� ���� �������� height�� Ʈ���� ���� ����
}

void search_preorder(TreeNode* root, int data)
// root �� �����ϴ� ��忡�� key���� ���������� �Ǻ��ϴ� �Լ� search_preorder
// ������ȸ(preorder) ������ ����
{
	if (root->key != -1)
	{
		if (root->key == data)
			sch = data;
			// ���ϴ� data ���� root ��尡 ���Ե� Ʈ���� ������ ��� �������� sch�� data�� ����
		search_preorder(root->left, data);
		search_preorder(root->right, data);
	}
}

void write_file(FILE* file_close, TreeNode* root, char type, int key)// ���� ������ �ۼ��� �Լ� write_file
// ������ ���� sch�� search ������ ����� ����� �� ���� ����
{
	int i;// ������ ��� for���� ���� ����
	cnt = 0;// ����� �����Ͱ� ����Ǵ� �迭 group�� �ε��� cnt�� �ʱ�ȭ�Ѵ�.

	if (type == 'i')// insert ������ ���
	{
		fprintf(file_close, "%c%d : ", type, key);
		// "input.txt" ������ ���� ó�� ������ ������ ������ ������ ���� �����͸� ǥ���Ѵ�
		level_order(root);
		fprintf(file_close, "%d", group[0]);// ù ��° ���� �޸� ��ȣ�� �ʿ� �����Ƿ� ���� ���
		for (i = 1; i < cnt; i++)// ������ ������ ������ �޸� ��ȣ�� ǥ���� ���� ���� ���
			fprintf(file_close, ", %d", group[i]);
		fputs("\n", file_close);// ���� ���� ����� ���� ����
	}
	else if (type == 'd')// delete ������ ���
	{
		fprintf(file_close, "%c%d : ", type, key);
		// "input.txt" ������ ���� ó�� ������ ������ ������ ������ ���� �����͸� ǥ���Ѵ�
		if (exist == 0)// exist ������ 0�̶�� ���� �ش� key���� �������� �ʴ´ٴ� �ǹ��̴�.
			fprintf(file_close, "Not Exist");
		else
		{
			level_order(root);
			fprintf(file_close, "%d", group[0]);// ù ��° ���� �޸� ��ȣ�� �ʿ� �����Ƿ� ���� ���
			for (i = 1; i < cnt; i++)// ������ ������ ������ �޸� ��ȣ�� ǥ���� ���� ���� ���
				fprintf(file_close, ", %d", group[i]);
		}
		fputs("\n", file_close);// ���� ���� ����� ���� ����
	}
	else if (type == 's')// search_preorder ������ ���
	{
		fprintf(file_close, "%c%d : ", type, key);
		// "input.txt" ������ ���� ó�� ������ ������ ������ ������ ���� �����͸� ǥ���Ѵ�
		// sch ������ �ش� key���� ���� ��� NULL���� ����Ǳ� ������ ���� ��¿��� sch ���
		// �Է����Ͽ��� ���� ���� key�� ����Ѵ�.
		if (sch != NULL)
			fprintf(file_close, "Exist");// NULL�� �ƴϸ� ����
		else
			fprintf(file_close, "Not Exist");// NULL�̸� �������� �ʴ´�
		fputs("\n", file_close);// ���� ���� ����� ���� ����
	}
}

int main(void)
// "input.txt", "output.txt" ������ ���� �����͸� �Է¹ް� �����Լ��� ȣ���� ���� ������ �ݴ� �Լ�
{
	TreeNode root;// TreeNode �� root ��� ����

	init(&root);// root ��� �ʱ�ȭ

	char source[MAX_SIZE];// ���Ͽ��� �о���� ���ڿ��� �����ϱ� ���� �迭
	char type;
	// insertion, deletion, search_preorder 3���� �� � ���������� ���� ���� i, d, s�� ����Ǵ� ����
	int data;// key���� ����Ǵ� ����
	int len;// buf�� ���̸� ������� ����
	int expon;// �������� ������ ����
	int i;// �Ʒ����� ������ for���� ���� ����

	FILE* file_open;// fopen�� ��ȯ�ϴ� FILE �����͸� ���� ���� file_open ����
	file_open = fopen("input.txt", "r");
	// fopen �Լ��� "input.txt" ������ �б� ���� ���� FILE ������ ���� file_open�� �����Ѵ�.
	if (file_open == NULL)// ���Ͽ��⿡ �����Ͽ��� ��츦 ���
		error("���Ͽ��� ����");
	FILE* file_close;// fopen�� ��ȯ�ϴ� FILE �����͸� ���� ���� file_close ����
	file_close = fopen("output.txt", "a");
	// fopen �Լ��� "output.txt" ������ �߰� ����(a) ���� ���� FILE ������ ���� file_close�� �����Ѵ�.
	if (file_close == NULL)// ���Ͽ��⿡ �����Ͽ��� ��츦 ���
		error("���Ͽ��� ����");

	while (fgets(source, sizeof(source), file_open) != NULL)
	{// fgets �Լ��� ������ ������ ������ ���� ������ ����
		data = 0;// ������ ���� �� ���� �ʱ�ȭ �ʿ� 
		expon = 0;// ������ ���� �� ���� �ʱ�ȭ �ʿ�
		type = source[0];// buf[0]���� i, d, s�� ���� ���ڰ� ��������Ƿ� type ������ ����
		len = strlen(source) - 2;
		// source�� ù �ε��� ��Ҵ� ���ڿ� ���� ���̰� ������ ����� �ε����� ���̺��� 1 ��ŭ �۱� ������
		// strlen���� ���� source�� ���̿� 2�� ���ָ� ���� ������ ���ڰ� ����� �κ��� ���̰� �ȴ�
		for (i = len; i > 0; i--)// ���������� ���ڸ� �������·� ��ȯ
		{
			data += (source[i] - '0') * (int)powf(10, expon);// powf �Լ��� ����� 10�� ���� �� ���
			// NULL�� ������ ������ ���ڿ��� 1�� �ڸ��� ���ϰ� source[0] ���ڸ����� �˸��� �ڸ�����
			// ������ ������ �������� �������·� ��ȯ�Ѵ�.
			expon++;
		}
		if (type == 'i')
		{// insert �Լ��� ȣ���Ͽ� ���Կ��� ���� �� 
		 // write_file �Լ��� type, data ���� ���ڷ� �����ϰ� root ��带 �����ϰ� �մϴ�.
			insert_node(&root, data);
			write_file(file_close, &root, type, data);
		}
		else if (type == 'd')
		{// delete �Լ��� ȣ���Ͽ� �������� ���� �� 
		 // write_file �Լ��� type, data ���� ���ڷ� �����ϰ� root ��带 �����ϰ� �մϴ�.
			delete_node(&root, data);
			write_file(file_close , &root, type, data);
		}
		else if (type == 's')
		{// search_preorder �Լ��� data ���� ���ڷ� �����ϰ� root ��带 �����ϰ� �մϴ�.
		 // �Լ����� ���ŵ� �������� sch�� write_file �Լ��� ����
			sch = NULL;
			// search_preorder �Լ����� sch�� ���ϴ� data���� ����Ǵ� ������
			// NULL ���� �����ǹǷ� ���ϴ� ���� ������ ��Ÿ����
			search_preorder(&root, data);
			write_file(file_close , &root, type, data);
		}
		else
		{// i, d, s�� �ƴ� �ٸ� ������ ��� ������ �߻������� ǥ���Ѵ�.
			error("\"input.txt\"���Ͽ� ���ʿ��� ���ڰ� ���ԵǾ����ϴ�");
			// "input.txt"���Ͽ� ���๮�ڷθ� �̷���� ������ ������ ��쿡�� ������ �߻��մϴ�
			break;
		}
	}
	fputs("\n", file_close);// ���� ���� ������� ����� �����ϱ� ���� ����
	fclose(file_close);//���Ͽ� ���� �۾��� �Ϸ�Ǿ����Ƿ� ������ �ݴ´�.
	fclose(file_open);
	return 0;
}