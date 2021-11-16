#define _CRT_SECURE_NO_WARNINGS 
// fopen_s 와 같이 안정성을 위해 '_s'가 추가된 형태로 함수들이 대체되었기 때문에 '_s'가 없는 
// 기존의 함수(fopen, strtok 등)를 사용하기 위해 해당 코드를 삽입하였습니다.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX(a, b) (((a) > (b)) ? (a) : (b))// height 함수에서 사용
#define MAX_SIZE 10// 파일에서 읽어들일 문자열을 저장할 배열의 크기
#define MAX_QUEUE_SIZE 100// 큐의 크기

int group[100];// "output.txt"파일에서의 데이터를 작성할 때 필요한 데이터 값을 저장하는 배열
int cnt;// group 배열의 인덱스
int exist;// delete 연산에서 삭제해야할 key값의 존재 유무를 판별하는 변수
int height;//트리의 높이를 저장하는 변수
int sch;// search 함수의 반환값을 저장받을 변수

typedef struct {// Binary Search Tree 노드의 구조
	int key;// 노드의 key값을 저장받는 정수형 변수
	struct TreeNode* left, * right;// 노드의 왼쪽, 오른쪽 자식노드의 주소를 저장받는 포인터 변수
}TreeNode;

typedef struct{// 큐의 구조
	TreeNode* data[MAX_QUEUE_SIZE];// 노드의 입출력이 진행되는 배열
	int front, rear;// front는 큐의 첫 번째 요소 하나 앞의 인덱스, rear은 마지막 요소의 인덱스
}QueueType;

int get_height(TreeNode* root)// 트리의 높이를 구하는 함수 get_height
{
	height = 0;
	if (root->key != -1)// 노드의 key값이 -1이 아닐 때까지 진행 
		height = 1 + max(get_height(root->left), get_height(root->right));
	//왼쪽과 오른쪽 트리 중 최댓값과 root노드 하나의 높이 1을 더한다.
	return height;
}

void error(char* message)
// 각종 연산에서 오류 발생시 메세지를 출력하는 함수 error
{
	fprintf(stderr, "%s\n", message);
	exit(1);
}

void init_queue(QueueType* q)// 큐를 초기화 하는 함수 init
{
	q->front = q->rear = 0;
}

int empty(QueueType* q)// 큐가 공백상태일 때 TRUE를 반환하는 함수 empty
{
	return (q->front == q->rear);
}

int full(QueueType* q)// 큐가 포화상태일 때 TRUE를 반환하는 함수 full
{
	return ((q->rear + 1) % MAX_QUEUE_SIZE == q->front);
}

void enqueue(QueueType* q, TreeNode* item)// 큐에 노드를 삽입하는 함수 enqueue
{
	if (full(q))
		error("큐가 포화상태입니다");
	q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;// 나머지 연산을 사용하여 인덱스를 원형으로 회전시킴
	q->data[q->rear] = item;
}

TreeNode* dequeue(QueueType* q)// 큐의 노드를 제거하는 함수 dequeue
{
	if (empty(q))
		error("큐가 공백상태입니다");
	q->front = (q->front + 1) % MAX_QUEUE_SIZE;// 나머지 연산을 사용하여 인덱스를 원형으로 회전시킴
	return q->data[q->front];
}

void level_order(TreeNode* root)// level order 순서로 파일에 데이터를 출력하는 함수 level_order
{
	QueueType q;// 큐 구조체 변수 q 선언
	TreeNode* zl, * zr;// 노드 구조체 변수 zl, zr 선언

	init_queue(&q);// 큐 초기화

	if (root == NULL)// 노드가 하나도 없다면 바로 함수를 빠져나간다
		return;
	enqueue(&q, root);// root 노드를 먼저 큐에 삽입
	while (!empty(&q))// 큐가 공백상태가 아닐 때까지
	{
		root = dequeue(&q);// 큐에 저장된 노드를 꺼내서 root에 저장
		if (cnt < powf(2, height) - 1 && root->key != -1)
		{
		// 큐에서 반환된 데이터를 파일에 출력할 순서로 저장하는 배열 group
		// 이진트리에서 노드의 최대 개수는 2를 트리의 높이만큼 제곱한 값에서 1을 뺀 값과 같다
		// 비어있는 노드를 0으로 출력하기 위해 비어있는 노드를 추가로 -1의 값을 가지는 노드로
		// 동적할당 하였으며 -1의 key값을 가지는 노드가 NULL을 대신하게 됩니다.
		// 자세한 원리는 아래에서 설명하겠습니다
		// 따라서 이진트리에서 허용되는 최대 노드의 개수 이하이고 key값이 -1이 아닌 경우 아래 문장을 실행 
			group[cnt] = root->key;// key값을 group 배열에 저장
			cnt++;// 인덱스 증가
		}
		else if (cnt < powf(2, height) - 1 && root->key == -1)
		// key값이 -1인 노드는 비어있는 노드를 0으로 추가하기 위해 동적할당되어 추가된 노드입니다
		{
			group[cnt] = 0;// 0을 배열에 저장
			cnt++;
		}

		if (root->left != NULL && cnt < powf(2, height) - 1)
		// root의 왼쪽 자식노드가 NULL이 아니고 노드 개수가 최대허용개수 이하일 경우
			enqueue(&q, root->left);// 왼쪽 자식노드를 큐에 삽입
		else if (root->left == NULL && cnt < powf(2, height) - 1)
		// root의 왼쪽 자식노드가 NULL 이고 노드 개수가 최대허용개수 이하일 경우
		{
			// NULL, 즉 비어있는 노드를 0으로 출력하기 위해 key값에 -1을 저장하고
			// 해당 노드의 왼쪽 자식노드인 zl을 동적할당하여 연결해줍니다
			zl = (TreeNode*)malloc(sizeof(TreeNode));
			if (zl == NULL)// 동적할당 오류에 대비
			{
				error("동적할당 오류");
				return;
			}
			zl->key = -1;// key값으로 -1 저장
			zl->left = zl->right = NULL;// zl노드의 왼쪽, 오른쪽 노드를 NULL로 설정
			root->left = zl;// root->left에 zl 노드를 저장
			enqueue(&q, root->left);// -1의 key값을 가지는 root->left 노드를 큐에 삽입
		}
		if (root->right != NULL && cnt < powf(2, height) - 1)
		// root의 오른쪽 자식노드가 NULL이 아니고 노드 개수가 최대허용개수 이하일 경우
			enqueue(&q, root->right);
		else if (root->right == NULL && cnt < powf(2, height) - 1)
		{
			zr = (TreeNode*)malloc(sizeof(TreeNode));// 오른쪽 자식노드 zr 동적할당
			if (zr == NULL)// 동적할당 오류에 대비
			{
				error("동적할당 오류");
				return;
			}
			zr->key = -1;
			zr->left = zr->right = NULL;
			root->right = zr;// root->right에 zr 노드를 저장
			enqueue(&q, root->right);// -1의 key값을 가지는 root->right 노드를 큐에 삽입
		}
	}
}

void init(TreeNode* p)// p가 참조하는 노드를 초기화하는 함수
{
	p->left = p->right = p->key = NULL;
}

void free_postorder(TreeNode* root)// 노드를 해제하는 함수 free_postorder
// 비어있는 노드를 0으로 출력하기 위해 -1의 key값을 가지도록 동적할당 된 노드들이 삭제연산 등으로 인해
// 필요 없어졌을 경우 노드를 후위순회 순서로 해제해줍니다.
{
	if (root)
	{
		free_postorder(root->left);
		free_postorder(root->right);
		free(root);
	}
}

void insert_node(TreeNode* root, int key)
// root가 참조하는 노드에 해당 key값을 가지는 노드를 삽입하는 함수
{
	TreeNode* p, * t;// p는 부모노드, t는 현재노드
	TreeNode* n, * zl, * zr;// n은 새로운 노드
	t = root;
	p = NULL;
	while (t->key != -1)// 인자로 받은 key값에 알맞은 위치를 찾는다
	{
		if (t->key == NULL)
			// root 노드의 key값이 NULL로 초기화 되었으므로 
			// 처음 받는 key값은 root 노드의 key값으로 저장되게 합니다
		{
			t->key = key;
			//root 노드의 왼쪽, 오른쪽 자식노드가 모두 NULL 이기 때문에 -1의 값을 가지는 노드로 연결합니다
			zl = (TreeNode*)malloc(sizeof(TreeNode));// 왼쪽 자식노드 동적할당
			if (zl == NULL)// 동적할당 오류에 대비
			{
				error("동적할당 오류");
				return;
			}
			zr = (TreeNode*)malloc(sizeof(TreeNode));// 오른쪽 자식노드 동적할당
			if (zr == NULL)// 동적할당 오류에 대비
			{
				error("동적할당 오류");
				return;
			}
			zl->key = -1;
			zl->left = zl->right = NULL;
			zr->key = -1;
			zr->left = zr->right = NULL;
			t->left = zl;// root->left 노드에 저장
			t->right = zr;// root->right 노드에 저장
			height = get_height(root);// level_order 함수에서 사용될 전역변수 height에 트리의 높이 저장
			return;// 노드를 추가할 필요가 없으므로 함수를 빠져나간다.
		}
		p = t;// 이후 진행될 연산의 부모노드로 사용하기 위해 현재의 t 노드를 p 노드에 저장
		if (key < p->key)
			t = p->left;
		// 인자로 받은 key값이 노드 p의 key값보다 작을 경우 p 노드의 왼쪽 자식노드로 이동 
		else if (key > p->key)
			t = p->right;
		// 인자로 받은 key값이 노드 p의 key값보다 클 경우 p 노드의 오른쪽 자식노드로 이동
		else
		{
			t = p->left;
			break;
		}
		//인자로 받은 key값이 노드 p의 key값과 같을 경우 p 노드의 왼쪽 자식노드로 이동
	}
	n = (TreeNode*)malloc(sizeof(TreeNode));// key가 트리 안에 없으므로 삽입 가능
	if (n == NULL)// 동적할당 오류에 대비
	{
		error("동적할당 오류");
		return;
	}
	n->key = key;// 데이터 복사
	n->left = t->left;
	if (n->left == NULL && key != p->key)
	{// t->left 노드가 NULL 이고 key값이 부모노드의 key값과 같지 않을 경우 왼쪽 자식노드를 동적할당하여 삽입 
		zl = (TreeNode*)malloc(sizeof(TreeNode));
		if (zl == NULL)// 동적할당 오류에 대비
		{
			error("동적할당 오류");
			return;
		}
		zl->key = -1;
		zl->left = zl->right = NULL;
		n->left = zl;
	}
	n->right = t->right;
	if (n->right == NULL || key == p->key)
	{// t->right 노드가 NULL 이거나 key값이 부모노드의 key값과 같을 경우 오른쪽 자식노드를 동적할당하여 삽입
		zr = (TreeNode*)malloc(sizeof(TreeNode));
		if (zr == NULL)// 동적할당 오류에 대비
		{
			error("동적할당 오류");
			return;
		}
		zr->key = -1;
		zr->left = zr->right = NULL;
		n->right = zr;
	}
	if (p != NULL)// 부모노드와 링크 연결
	{
		if (key < p->key)
			p->left = n;
			// 인자로 받은 key값이 노드 p의 key값보다 작을 경우 p 노드의 왼쪽 자식노드에 연결
		else if (key > p->key)
			p->right = n;
			// 인자로 받은 key값이 노드 p의 key값보다 클 경우 p 노드의 오른쪽 자식노드에 연결
		else
		{// 동일한 key값을 가지는 노드를 삽입하는 경우 명세에 제시된 바와 같이 left child, 즉 왼쪽 자식노드에
		 // 삽입하기 위해 동일한 key값을 가지는 부모노드 p의 왼쪽 자식노드와 연결
			n->left = t;//기존의 p 노드의 왼쪽 자식노드인 t 노드를 새로 삽입되는 노드 n의 왼쪽 자식노드로 저장
			p->left = n;// p 노드의 왼쪽 자식노드에 노드 n을 저장
		}
	}
	else
		root = n;// root 노드가 NULL이므로 root 노드에 노드 n을 저장
	height = get_height(root);// level_order 함수에서 사용될 전역변수 height에 트리의 높이 저장
}

void delete_node(TreeNode* root, int key)
// root가 참조하는 노드에서 해당 key값을 가지는 노드를 삭제하는 함수
{
	TreeNode* p, * t, *left, *right, * child, * small, * min;
	// p는 부모노드, t는 현재노드, left는 왼쪽 자식노드, right는 오른쪽 자식노드
	// child는 자식노드, small은 최솟값을 가지는 노드, min은 key값의 중복을 판별하는 노드를 뜻합니다

	exist = 1;// 인자로 받은 key값의 존재 유무를 판별하는 변수를 1로 초기화
	t = root;
	// 해당 key값을 찾기 위해서는 root 노드로부터 서브트리로 이동해야 하므로 처음 탐색할 노드를
	// root 노드로 설정한다
	p = NULL;
	min = NULL;

	while (t->key != -1 && t->key != key)// key값을 가지는 노드 t를 찾는다
	{
		p = t;// 현재의 노드 t는 아래의 연산 실행 이후 t의 선행 노드가 되기 때문에 p에 저장한다
		t = (key < t->key) ? t->left : t->right;
		// 인자로 받은 key값이 노드 t의 key값 보다 작으면 왼쪽 노드, 크면 오른쪽 노드로 이동
	}
	if (t->key == -1)
	{
		exist = 0;// key값 존재하지 않을 경우 exist 변수에 0을 저장하고 바로 함수를 빠져나가다.
		height = get_height(root);// level_order 함수에서 사용될 전역변수 height에 트리의 높이 저장
		return;
	}
	left = t->left;
	right = t->right;
	if ((left->key == -1) && (right->key == -1))// 삭제하려는 노드의 자식노드의 key값이 모두 -1인 경우
	{
		if (p != NULL)// t의 부모노드인 p가 NULL이 아닐 경우
			t->key = -1;// 노드 t의 key값을 -1로 설정함으로써 원하는 key값을 지운다
		else// p가 NULL이라면 root 노드가 삭제된다.
			root = NULL;
		free_postorder(left);// 필요없어진 동적할당된 노드 해제
		free_postorder(right);
		t->left = t->right = NULL;// 가리킬 자식노드가 없으므로 NULL로 설정
	}
	else if ((left->key == -1) || (right->key == -1))
		// 삭제하려는 노드의 자식노드 중 하나만 key값이 -1인 경우
	{
		child = (left->key != -1) ? left : right;
		// 왼쪽, 오른쪽 노드 중 NULL이 아닌 노드를 child 변수에 저장
		if (p != NULL)
		{
			if (p->left == t)// 왼쪽 자식노드가 -1이 아닌 key값을 갖는 경우
			{
				p->left = child;// 왼쪽 자식노드를 부모노드의 왼쪽 자식노드로 저장
				t->left = NULL;// t->left가 가리킬 노드가 없으므로 NULL로 설정한다
			}
			else// 오른쪽 자식노드가 -1이 아닌 key값을 갖는 경우
			{
				p->right = child;// 오른쪽 자식노드를 부모노드의 왼쪽 자식노드로 저장
				t->right = NULL;// t->right가 가리킬 노드가 없으므로 NULL로 설정한다
			}
		}
		else
			root = child;// p가 NULL이라면 child 노드가 root 노드가 된다.
		free_postorder(t);// 필요없어진 t 노드를 해제한다
	}
	else// 삭제하려는 노드의 자식노드가 모두 NULL이 아닐 경우
	{
		small = t->right;// 삭제하려는 노드의 오른쪽 자식노드로 이동
		left = small->left;
		right = small->right;
		while (left->key != -1)
			// 이후 해당 트리에서의 최솟값을 찾기 위해 -1의 key값을 만날 때 까지 왼쪽 자식노드로 이동
		{
			small = left;// 현재의 left 노드를 small 노드에 저장하여 다음 연산에서의 부모노드로 사용한다
			left = small->left;// 왼쪽 자식노드로 이동
			right = small->right;// 오른쪽 자식노드로 이동
			if (left->key == small->key)// 중복되는 key값이 존재할 경우 해당 노드를 min에 저장합니다
				min = small;
		}
		if (min != NULL && min->key == left->key)
			// 만약 중복되는 key값이 최솟값일 경우 명세에 따라 중복 시 가장 먼저 탐색되는 것을
			// 삭제하기 위해 최초로 저장된 노드 min을 left에 저장합니다.
			left = min;
		t->key = small->key;// t 노드의 왼쪽 서브트리에서의 최솟값을 t 노드의 key값으로 저장한다.
		if (right->key == -1)
			// 오른쪽 자식노드의 key값이 -1이라는 것은 small노드의 자식노드의 key값이 모두 -1, 즉 자식노드가
			// 없다는 의미이다
		{
			// 따라서 위에서 이미 진행된 삭제하려는 노드의 자식노드의 key값이 모두 -1인 경우와 동일하게 진행
			small->key = -1;
			free_postorder(left);
			free_postorder(right);
			small->left = small->right = NULL;
		}
		else// 최솟값을 가지는 자식노드가 하나의 자식을 가질 경우
		{
			child = right;//최솟값을 가지는 small노드의 오른쪽 자식노드를 child 노드에 저장한다.
			free_postorder(left);
			// small노드가 child 노드의 역할을 대신하게 한다
			small->key = child->key;
			small->left = child->left;
			small->right = child->right;
			free(child);// 필요없어진 child 노드 해제
		}
	}
	height = get_height(root);// level_order 함수에서 사용될 전역변수 height에 트리의 높이 저장
}

void search_preorder(TreeNode* root, int data)
// root 가 참조하는 노드에서 key값의 존재유무를 판별하는 함수 search_preorder
// 전위순회(preorder) 순서로 진행
{
	if (root->key != -1)
	{
		if (root->key == data)
			sch = data;
			// 원하는 data 값이 root 노드가 포함된 트리에 존재할 경우 전역변수 sch에 data값 저장
		search_preorder(root->left, data);
		search_preorder(root->right, data);
	}
}

void write_file(FILE* file_close, TreeNode* root, char type, int key)// 파일 내용을 작성할 함수 write_file
// 정수형 변수 sch는 search 연산의 결과를 출력할 때 사용될 변수
{
	int i;// 데이터 출력 for문에 사용될 변수
	cnt = 0;// 출력할 데이터가 저장되는 배열 group의 인덱스 cnt를 초기화한다.

	if (type == 'i')// insert 연산일 경우
	{
		fprintf(file_close, "%c%d : ", type, key);
		// "input.txt" 파일의 내용 처럼 연산의 유형과 연산을 진행할 정수 데이터를 표시한다
		level_order(root);
		fprintf(file_close, "%d", group[0]);// 첫 번째 값은 콤마 기호가 필요 없으므로 따로 출력
		for (i = 1; i < cnt; i++)// 이후의 나머지 값들은 콤마 기호를 표시한 다음 값을 출력
			fprintf(file_close, ", %d", group[i]);
		fputs("\n", file_close);// 다음 문장 출력을 위한 개행
	}
	else if (type == 'd')// delete 연산일 경우
	{
		fprintf(file_close, "%c%d : ", type, key);
		// "input.txt" 파일의 내용 처럼 연산의 유형과 연산을 진행할 정수 데이터를 표시한다
		if (exist == 0)// exist 변수가 0이라는 뜻은 해당 key값이 존재하지 않는다는 의미이다.
			fprintf(file_close, "Not Exist");
		else
		{
			level_order(root);
			fprintf(file_close, "%d", group[0]);// 첫 번째 값은 콤마 기호가 필요 없으므로 따로 출력
			for (i = 1; i < cnt; i++)// 이후의 나머지 값들은 콤마 기호를 표시한 다음 값을 출력
				fprintf(file_close, ", %d", group[i]);
		}
		fputs("\n", file_close);// 다음 문장 출력을 위한 개행
	}
	else if (type == 's')// search_preorder 연산일 경우
	{
		fprintf(file_close, "%c%d : ", type, key);
		// "input.txt" 파일의 내용 처럼 연산의 유형과 연산을 진행할 정수 데이터를 표시한다
		// sch 변수는 해당 key값이 없을 경우 NULL값이 저장되기 때문에 파일 출력에는 sch 대신
		// 입력파일에서 받은 값인 key를 사용한다.
		if (sch != NULL)
			fprintf(file_close, "Exist");// NULL이 아니면 존재
		else
			fprintf(file_close, "Not Exist");// NULL이면 존재하지 않는다
		fputs("\n", file_close);// 다음 문장 출력을 위한 개행
	}
}

int main(void)
// "input.txt", "output.txt" 파일을 열고 데이터를 입력받고 각종함수를 호출한 다음 파일을 닫는 함수
{
	TreeNode root;// TreeNode 형 root 노드 선언

	init(&root);// root 노드 초기화

	char source[MAX_SIZE];// 파일에서 읽어들인 문자열을 저장하기 위한 배열
	char type;
	// insertion, deletion, search_preorder 3가지 중 어떤 연산인지에 대한 문자 i, d, s가 저장되는 변수
	int data;// key값이 저장되는 변수
	int len;// buf의 길이를 저장받을 변수
	int expon;// 지수값을 저장할 변수
	int i;// 아래에서 등장할 for문에 사용될 변수

	FILE* file_open;// fopen이 반환하는 FILE 포인터를 받을 변수 file_open 선언
	file_open = fopen("input.txt", "r");
	// fopen 함수로 "input.txt" 파일을 읽기 모드로 열고 FILE 포인터 값을 file_open에 저장한다.
	if (file_open == NULL)// 파일열기에 실패하였을 경우를 대비
		error("파일열기 실패");
	FILE* file_close;// fopen이 반환하는 FILE 포인터를 받을 변수 file_close 선언
	file_close = fopen("output.txt", "a");
	// fopen 함수로 "output.txt" 파일을 추가 쓰기(a) 모드로 열고 FILE 포인터 값을 file_close에 저장한다.
	if (file_close == NULL)// 파일열기에 실패하였을 경우를 대비
		error("파일열기 실패");

	while (fgets(source, sizeof(source), file_open) != NULL)
	{// fgets 함수로 파일의 마지막 문장을 읽을 때까지 실행
		data = 0;// 문장을 읽을 때 마다 초기화 필요 
		expon = 0;// 문장을 읽을 때 마다 초기화 필요
		type = source[0];// buf[0]에는 i, d, s와 같은 문자가 들어있으므로 type 변수에 저장
		len = strlen(source) - 2;
		// source의 첫 인덱스 요소는 문자에 대한 값이고 마지막 요소의 인덱스는 길이보다 1 만큼 작기 때문에
		// strlen으로 구한 source의 길이에 2를 빼주면 숫자 형태의 문자가 저장된 부분의 길이가 된다
		for (i = len; i > 0; i--)// 문자형식의 숫자를 정수형태로 변환
		{
			data += (source[i] - '0') * (int)powf(10, expon);// powf 함수를 사용해 10의 지수 값 계산
			// NULL을 제외한 마지막 문자열을 1의 자리로 정하고 source[0] 앞자리까지 알맞은 자리수가
			// 곱하진 값들을 누적시켜 정수형태로 변환한다.
			expon++;
		}
		if (type == 'i')
		{// insert 함수를 호출하여 삽입연산 실행 후 
		 // write_file 함수에 type, data 값을 인자로 전달하고 root 노드를 참조하게 합니다.
			insert_node(&root, data);
			write_file(file_close, &root, type, data);
		}
		else if (type == 'd')
		{// delete 함수를 호출하여 삭제연산 실행 후 
		 // write_file 함수에 type, data 값을 인자로 전달하고 root 노드를 참조하게 합니다.
			delete_node(&root, data);
			write_file(file_close , &root, type, data);
		}
		else if (type == 's')
		{// search_preorder 함수에 data 값을 인자로 전달하고 root 노드를 참조하게 합니다.
		 // 함수에서 갱신된 전역변수 sch를 write_file 함수로 전달
			sch = NULL;
			// search_preorder 함수에서 sch에 원하는 data값이 저장되니 않으면
			// NULL 값이 유지되므로 원하는 값이 없음을 나타낸다
			search_preorder(&root, data);
			write_file(file_close , &root, type, data);
		}
		else
		{// i, d, s가 아닌 다른 문자일 경우 오류가 발생했음을 표시한다.
			error("\"input.txt\"파일에 불필요한 문자가 삽입되었습니다");
			// "input.txt"파일에 개행문자로만 이루어진 문장이 존재할 경우에도 오류가 발생합니다
			break;
		}
	}
	fputs("\n", file_close);// 다음 파일 입출력의 결과와 구분하기 위한 개행
	fclose(file_close);//파일에 대한 작업이 완료되었으므로 파일을 닫는다.
	fclose(file_open);
	return 0;
}