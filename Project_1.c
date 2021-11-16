/*#define _CRT_SECURE_NO_WARNINGS 
// fopen_s 와 같이 안정성을 위해 '_s'가 추가된 형태로 함수들이 대체되었기 때문에 '_s'가 없는 기존의 함수(fopen, strtok 등)를
// 사용하기 위해 해당 코드를 삽입하였습니다.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 10

typedef struct {//연결 리스트의 노드의 구조
	int coef;//계수를 나타내는 변수
	int expon;//차수를 나타내는 변수
	struct ListNode* link;//다음 노드의 주소값을 저장하는 포인터 변수
}ListNode;

typedef struct {//연결 리스트 헤더
	int length;//노드의 길이를 나타내는 변수
	ListNode* head;//첫번째 노드
	ListNode* tail;//마지막 노드
}ListHeader;

void init(ListHeader* plist)//초기화 함수
{
	plist->length = 0;//길이를 0으로 하여 초기화
	plist->head = plist->tail = NULL;//첫번째, 마지막 노드를 NULL로 초기화
}

void insert_node(ListHeader* plist, int coef, int expon)//plist 헤더에서 노드를 추가하는 함수
{//plist는 연결 리스트의 헤더를 가리키는 포인터, coef는 계수, expon는 지수
	for (ListNode* a = plist->head; a != NULL; a = a->link)
	{//plist의 head 노드를 참조하는 포인터 변수 a를 선언하고
	 //head 노드에서 시작하여 마지막 노드까지(NULL이 아닐 때까지) 이동
		if (a->expon == expon)//추가하려는 expon과 같은 expon을 가지는 노드가 plist에 존재할 경우  
		{
			a->coef += coef;//같은 expon을 가지는 노드에 coef를 더한다.
			return;//필요한 연산이 끝났으므로 함수를 빠져나간다.
		}
		else if (a->expon < expon)
		{//추가하려는 expon이 plist에 존재하는 노드 a의 expon보다 큰 경우가 있을 때
			ListNode* temp = (ListNode*)malloc(sizeof(ListNode));
			//해당 plist의 노드 a에 새로운 coef, expon 값을 입력하기 위해 노드 a의 값을  
			//입력할 노드 temp를 동적 할당하여 생성

			if (temp == NULL)// 메모리 할당에 실패하였을 경우 대비
				printf("메모리 할당 오류");

			//노드 a가 가지는 값을 temp 노드에 입력한다
			//따라서 기존 plist의 노드 a와 동적 할당하여 생성한 노드 temp는 같은 값을 가진다.
			temp->coef = a->coef;//coef 저장
			temp->expon = a->expon;//expon 저장
			temp->link = a->link;//a 노드가 가리키는 다음 노드 주소값 저장
			//기존의 노드 a에 있던 값을 temp 노드로 모두 복사했으므로 새로운 coef, expon값을
			//노드 a에 저장한다
			a->coef = coef;//coef 저장
			a->expon = expon;//expon 저장
			a->link = temp;//기존 노드 a의 expon보다 새로운 expon이 더 크기 때문에 노드 a는
			//더 작은 expon을 가지는 temp 노드의 주소값을 가지게 한다.

			if (temp->link == NULL)//기존의 노드 a의 링크가 NULL이었다면 노드 a가 tail 노드였기 때문에 
				plist->tail = temp;//temp 노드를 새로운 tail 노드로 정한다.

			plist->length++;//plist에 새로운 노드가 추가되었기 때문에 길이를 1 증가시킨다.
			return;//필요한 연산이 끝났으므로 함수를 빠져나간다.
		}
	}
	//expon이 노드 a의 모든 expon보다 작을 경우, 즉 위의 for문 내부의 조건에 해당되지 않을 경우
	ListNode* last = (ListNode*)malloc(sizeof(ListNode));
	//새로운 coef, expon 값을 저장하기 위한 노드를 동적할당으로 생성한다

	if (last == NULL)// 메모리 할당에 실패하였을 경우 대비
		printf("메모리 할당 오류");

	last->coef = coef;//함수에서 받아온 coef값을 저장한다.
	last->expon = expon;//함수에서 받아온 expon값을 저장한다.
	last->link = NULL;
	//expon 값이 plist에서 가장 작으므로 temp 노드가 마지막이기 때문에 다음 노드 주소값을 NULL로 한다.

	if (plist->head == NULL)
		plist->head = plist->tail = last;
	//기존의 plist에 노드가 하나도 없을 경우 추가하는 temp 노드가 유일하기 때문에
	//plist의 첫 노드인 head, 마지막 노드인 tail을 모두 temp 노드로 한다.

	else
	{
		plist->tail->link = last;
		//plist에 노드가 이미 존재할 경우 temp 노드를 마지막 노드로 삽입하기 때문에
		//기존의 tail 노드의 link를 다음 노드인 temp의 주소값으로 설정한다.
		plist->tail = last;
		//temp 노드가 가장 마지막이기 때문에 plist의 tail 노드로 설정한다.
	}
	plist->length++;//plist에 새로운 노드가 추가되었기 때문에 길이를 1 증가시킨다.
}

void poly_add(ListHeader* plist1, ListHeader* plist2, ListHeader* plist3)
{// list3 = list1 + list2
	for (ListNode* a = plist1->head; a != NULL; a = a->link)
		//plist1의 head 노드를 참조하는 포인터 변수 a를 선언하고
		//head 노드에서 시작하여 마지막 노드까지(NULL이 아닐 때까지) 이동
		insert_node(plist3, a->coef, a->expon);
	//a가 참조하는 plist1의 노드의 coef, expon 값을 plist3 헤더에 저장하기 위해
	//insert_node함수를 호출한다.

	for (ListNode* a = plist2->head; a != NULL; a = a->link)
		//포인터 변수 a가 plist2의 head 노드를 참조하게 한다.
		//head 노드에서 시작하여 마지막 노드까지(NULL이 아닐 때까지) 이동
		insert_node(plist3, a->coef, a->expon);
	//a가 참조하는 plist2의 노드의 coef, expon 값을 plist3 헤더에 저장하기 위해
	//insert_node함수를 호출한다.
}

void poly_mul(ListHeader* plist1, ListHeader* plist2, ListHeader* plist3)
{// list3 = list1 * list2
	//다항식의 곱은 각각의 다항식의 모든 항들이 서로 곱해져야 하기 때문에
	ListHeader* p = plist3;
	for (ListNode* a = plist1->head; a != NULL; a = a->link)
		//첫번째 for문은 plist1의 head 노드를 참조하는 포인터 변수 a를 선언하고 마지막 노드까지 이동
		for (ListNode* b = plist2->head; b != NULL; b = b->link)
			//두번째 for문은 plist2의 head 노드를 참조하는 포인터 변수 b를 선언하고 마지막 노드까지 이동
			//이렇게 이중 for문을 구성하여 plist1, plist2의 각각의 모든 노드들끼리 곱셈이 진행되게 한다.
			insert_node(plist3, a->coef * b->coef, a->expon + b->expon);
	//다항식의 항들을 곱하면 계수끼리의 곱셈은 계수들간의 직접적인 곱셈으로 표현되고 
	//차수끼리의 곱셈은 차수들간의 덧셈으로 표현된다.
	//따라서 a->coef와 b->coef의 곱, a->expon과 b->expon의 합을 새로 추가될 노드의
	//coef, expon으로 전달한다.
}

void write_file(ListHeader* plist1, ListHeader* plist2)
//output.txt 파일을 생성하고 다항식의 덧셈, 곱셈의 결과를 파일에 기록하는 함수
{
	ListNode* a = plist1->head;// plist1의 head노드를 참조하는 포인터 변수 a생성
	ListNode* b = plist2->head;// plist2의 head노드를 참조하는 포인터 생성 b생성

	FILE* file_read;//fopen이 반환하는 FILE 포인터를 받을 변수 file_read 선언
	file_read = fopen("output.txt", "w");
	if (file_read == NULL) //파일열기에 실패하였을 경우를 대비
		printf("파일열기 실패\n");
	//fopen 함수로 "output.txt" 파일을 쓰기 모드로 열고 FILE 포인터 값을 file_read에 저장한다.

	fputs("Addition\n", file_read);
	//file_read가 가리키는 파일에 "Addition"이라는 문자열을 입력한 후 개행을 실행한다.
	for (; a != NULL; a = a->link)//마지막 노드까지(NULL이 아닐 때까지) 이동
		fprintf(file_read, "%d %d\n", a->coef, a->expon);
	//plist1의 노드들의 coef, expon 묶음을 개행을 통해 구분하며 순차적으로 파일에 작성

	fputs("Multiplication\n", file_read);
	//file_read가 가리키는 파일에 "Addition"이라는 문자열을 입력한 후 개행을 실행한다.
	for (; b != NULL; b = b->link)//마지막 노드까지(NULL이 아닐 때까지) 이동
		fprintf(file_read, "%d %d\n", b->coef, b->expon);
	//plist2의 노드들의 coef, expon 묶음을 개행을 통해 구분하며 순차적으로 파일에 작성

	fclose(file_read);//파일에 대한 작업이 완료되었으니 파일을 닫아준다.
}

int main(void)
{
	ListHeader list1, list2, add_list3, mul_list3;
	//ListHeader형 변수 list1, list2, add_list3, mul_list3 선언

	//연결 리스트의 초기화
	init(&list1);
	init(&list2);
	init(&add_list3);
	init(&mul_list3);

	int coef, expon;//계수, 차수에 대한 값을 저장할 변수 coef, expon
	char source[MAX_SIZE];//파일에서 읽어들인 문자열을 저장하기 위한 배열
	char* buf;//분할된 문자열의 포인터 값을 저장할 변수
	int data[MAX_SIZE];//strtok 함수로 분할한 문자열 각각의 값을 저장하기 위한 배열
	int list_num = 1;
	//첫번째, 두번째 다항식을 구분하기 위한 변수(자세한 설명은 밑의 if문 주석에 있습니다.)

	FILE* file_read;//fopen이 반환하는 FILE 포인터를 받을 변수 file_read 선언
	file_read = fopen("input.txt", "r");
	//fopen 함수로 "input.txt" 파일을 읽기 모드로 열고 FILE 포인터 값을 file_read에 저장한다.
	if (file_read == NULL) //파일열기에 실패하였을 경우를 대비
		printf("파일열기 실패\n");

	while (fgets(source, sizeof(source), file_read) != NULL)
	{//fgets 함수로 파일의 마지막 문장을 읽을 때까지 실행
		int count = 0;//문자열을 분할한 횟수를 기록하는 변수
		buf = strtok(source, " ");//strtok 함수로 공백을 기준으로 문자열을 분할한 뒤 포인터 반환

		while (buf != NULL)//자른 문자열이 나오지 않을 때까지 반복한다.
		{
			data[count] = atoi(buf);//첫번째 값으로 계수, 두번째 값으로 차수가 배열에 저장된다.
			//정수의 형태로 이후의 연산이 진행되기 때문에 문자형식의 값을 정수형태로 변환
			count++;//while문이 실행될 때마다 1 증가
			buf = strtok(NULL, " ");//다음 문자열을 잘라서 포인터를 반환한다.
		}

		if (count < 2)
		{//"input.txt" 파일에서는 두 다항식 데이터를 중간의 공백 문장으로 구분한다.
		 // 따라서 만약 이 부분을 fgets 함수가 읽고 strtok 함수로 분할하면
		 // 개행문자 하나만 존재하기 때문에 count가 1이 된다.
		 // 따라서 공백 문장을 읽을때만 이 조건문이 실행된다.
			list_num = 2;
			//count가 1이 되면 공백문장 이후의 데이터, 즉 두번째 다항식 데이터를 읽어들인다.
			//따라서 if(count < 2) 조건문이 실행되기 전까지는 첫번째 다항식의 데이터가
			//list1 헤더의 노드의 데이터 값으로 사용되고
			//조건문 실행 이후에는 list2 헤더의 노드의 데이터 값으로 사용된다.
			continue;
			//공백 문장은 노드에 입력될 데이터가 없으므로 이후의 문장을 실행하지 않고
			//while문 조건 판단부분으로 되돌아간다.
		}

		if (list_num == 1)
			//처음에 list_num을 1로 초기화 했으므로 if(count < 2) 조건문이 실행되기 전까지,
			//즉 두 다항식 데이터 사이의 공백 문장이 실행되기 전까지 첫번째 다항식 데이터
			//를 list1 헤드의 노드의 값으로 저장한다.
			insert_node(&list1, data[0], data[1]);
		else
			//if(count < 2) 조건문에서 list_num에 2가 저장되므로 해당 if문 실행 이후
			//두번째 다항식 데이터를 list2 헤더의 노드 값으로 저장한다.
			insert_node(&list2, data[0], data[1]);
	}

	poly_add(&list1, &list2, &add_list3);//두 다항식을 더하기 위한 함수 호출

	poly_mul(&list1, &list2, &mul_list3);//두 다항식을 곱하기 위한 함수 호출

	write_file(&add_list3, &mul_list3);//"output.txt"파일을 생성하기 위한 함수를 호출

	fclose(file_read);//파일에 대한 작업이 완료되었으므로 파일을 닫는다.
	return 0;
}*/