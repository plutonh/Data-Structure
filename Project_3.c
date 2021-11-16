#define _CRT_SECURE_NO_WARNINGS 
// fopen_s 와 같이 안정성을 위해 '_s'가 추가된 형태로 함수들이 대체되었기 때문에 '_s'가 없는 
// 기존의 함수(fopen, strtok 등)를 사용하기 위해 해당 코드를 삽입하였습니다.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_SIZE 30

typedef struct {// 히프 구조체 HeapType
	int heap[MAX_SIZE];// 데이터가 저장될 배열
	int heap_size;// 히프의 크기
}HeapType;

void error(char* message)// 각종 연산에서 오류 발생시 메세지를 출력하는 함수 error
{
    fprintf(stderr, "%s\n", message);
    exit(1);
}

void init(HeapType* h)// 초기화 함수 init
{
	h->heap_size = 0;
}

void swap(HeapType* h, int a, int b)// a, b의 위치를 바꾸는 함수 swap
{
    int temp = h->heap[a];
    h->heap[a] = h->heap[b];
    h->heap[b] = temp;
}

void seque_min_heapify(HeapType* h, int size)// 반복문을 사용하여 Min Heapify로 정렬하는 함수 seque_min_heapify
{
    for (int i = 1; i < size; i++)
    {
        int c = i;//자식 노드
        do
        {//자식 노드에서 1을 빼고 2를 나누면 부모 노드가 나온다
            int p = (c - 1) / 2;
            //만약 부모 노드보다 자식 노드가 더 크다면 값을 바꿔준다.
            if (h->heap[p] < h->heap[c])
                swap(&h->heap, p, c);
            //루트까지 진행
            c = p;
        } while (c != 0);
    }
    //가장 큰 값이 루트가 되었기 때문에 루트를 가장 밑으로 내려 오름차순으로 정렬
    for (int i = size - 1; i >= 0; i--)//크기를 줄여가며 반복적으로 힙을 구성
    {
        swap(&h->heap, 0, i);// 가장 큰 값을 맨 뒤로 보낸다
        int p = 0;
        int c = 1;
        do// 힙 구조를 만든다
        {
            c = p * 2 + 1;
            if (h->heap[c] < h->heap[c + 1] && c < i - 1)// 자식 중에 더 큰 값을 찾고 범위를 제한한다
                c++;
            if (h->heap[p] < h->heap[c] && c < i)// 부모보다 자식이 더 크다면 교환
                swap(&h->heap, p, c);
            p = c;// 다음 연산을 위해 부모를 현재의 자식으로 저장
        } while (c < i);
    }
}

void heapify_recur(HeapType* h, int k, int size)// 부모보다 큰 자식을 서로 교환하는 함수 heapify_recur
{
    int p = k;// 부모
    int left = k * 2 + 1;// 왼쪽 자식
    int right = k * 2 + 2;// 오른쪽 자식
    if (left < size && h->heap[left] > h->heap[k])// 왼쪽 자식이 부모보다 클 경우
        p = left;
    if (right < size && h->heap[right] > h->heap[p])// 오른쪽 자식이 부모보다 클 경우
        p = right;
    if (p != k)// 위의 두 연산 중 하나라도 실행되었을 경우
    {
        swap(&h->heap, k, p);// 부모와 자식의 위치를 교환한다
        heapify_recur(h->heap, p, size);// 현재의 자식을 부모로 하는 연산을 진행하기 위해 재귀함수 실행
    }
}

void make_heap(HeapType* h, int size)// 힙 구조를 생성하는 함수 make_heap
{
    int i = (size / 2) - 1;// 하나 이상의 자식을 가지는 노드에 대해 heapify_recur 함수를 호출하여 최소힙 특성을 만족하게 함
    for (; i >= 0; --i)// 인덱스를 줄이며 heapify_recur 함수를 통해 정렬
        heapify_recur(h->heap, i, size);
}

void recur_min_heapify(HeapType* h, int size)// 재귀함수를 사용하여 Min Heapify로 정렬하는 함수 recur_min_heapify
{
    int i = 0;
    make_heap(h->heap, size);// make_heap 함수를 통해 힙 구조를 생성한다
    for (i = size - 1; i > 0; --i) 
    {
        swap(&h->heap, 0, i);// 힙 구조를 생성하며 가장 큰 값이 루트가 되었기 때문에 맨 뒤로 보낸다
        // 반복적으로 큰 값이 아래로 내려가며 오름차순으로 정렬
        heapify_recur(h->heap, 0, --size);// 크기를 줄이며 heapify_recur 함수를 통해 정렬
    }
}

void insert_recur(HeapType* h, int item, int i)// Min Heap에 데이터를 삽입하는 함수 insert_recur
{
    if ((i != 0) && (item <= h->heap[(i - 1) / 2]))// 인덱스가 0이 아니고 부모보다 작을 경우
    {
        h->heap[i] = h->heap[(i - 1) / 2];// 부모의 데이터를 현재 인덱스 위치에 저장
        insert_recur(&h->heap, item, (i - 1) / 2);// 다음 부모와의 비교를 위해 재귀함수 사용
        return;
    }
    h->heap[i] = item;// 위의 조건으로 구한 인덱스에 item을 저장한다
    h->heap_size++;// 데이터를 하나 삽입했으므로 크기를 1 증가시킨다
}

int delete_recur(HeapType* h, int item, int temp, int size, int p, int c)
// Min heap에서 가장 작은 값을 삭제하는 함수 delete_recur
{
    if (size <= 0)// 히프가 비었을 경우 NULL을 반환
        return NULL;
    if (c <= size)// 인덱스가 히프의 크기보다 작을 경우
    {
        if ((c + 1 <= size) && (h->heap[c]) > h->heap[c + 1])
        //오른쪽 자식의 인덱스 히프의 크기보다 작을 경우 두 자식 중에서 작은 값을 선택 
            c++;
        if (temp <= h->heap[c] || c == size)
        // 마지막 인덱스의 데이터가 자식보다 작거나 같을 경우 또는 자식의 인덱스가 배열의 마지막에 도달했을때 
        {
            h->heap[p] = temp;// temp를 부모에 저장
            return item;// 삭제된 데이터 item을 반환
        }
        h->heap[p] = h->heap[c];// 다음 연산을 위해 현재의 자식을 부모에 저장
        if (c <= size)
        // 부모에는 현재의 자식, 자식에는 2 * (c + 1) - 1 연산을 통해 다음 부모의 자식을 변수로 하여 재귀함수 사용
            item = delete_recur(&h->heap, item, temp, size, c, 2 * (c + 1) - 1);
        else// 자식의 인덱스가 히프 길이를 초과한 경우 부모에 temp를 저장하고 item을 반환
        {
            h->heap[p] = temp;
            return item;
        }
    }
    else// 자식의 인덱스가 히프 길이를 초과한 경우 부모에 temp를 저장하고 item을 반환
    {
        h->heap[p] = temp;
        return item;
    }
}

void write_file(FILE* file_close, HeapType* h, char type, int key)// 파일 내용을 작성할 함수 write_file
{
    int i = 1;// 데이터 출력 for문에 사용될 변수
    int height;// 히프의 깊이를 저장받을 변수
    int sum = 1;
    while(1)// 히프의 깊이를 구하는 과정
    {
        if (h->heap_size <= sum)// sum의 값이 히프의 크기보다 크거나 같을 경우 i값을 height에 저장하고 탈출
        {
            height = i;
            break;
        }
        else
        {
            sum += (int)powf(2, i);
            // 히프의 각각의 레벨에는 2를 깊이만큼 제곱한 노드가 존재하므로 전제 노드의 수를 구하기 위해 누적한다
            i++;
        }
    }
    if (type == 'i')// insert 연산일 경우
    {
        fprintf(file_close, "%c%d : ", type, key);
        // "input.txt" 파일의 내용 처럼 연산의 유형과 연산을 진행할 정수 데이터를 표시한다
        fprintf(file_close, "%d", h->heap[0]);// 첫 번째 값은 콤마 기호가 필요 없으므로 따로 출력
        for (i = 1; i < h->heap_size; i++)// 이후의 나머지 값들은 콤마 기호를 표시한 다음 값을 출력
            fprintf(file_close, ", %d", h->heap[i]);
        for (i = h->heap_size; i < sum; i++)// level order 순서를 위해 각 깊이의 최대 노드수 만큼 나머지를 0으로 출력
            fprintf(file_close, ", %d", 0);
        fputs("\n", file_close);// 다음 문장 출력을 위한 개행
    }
    else if (type == 'd')// delete 연산일 경우
    {
        if (h->heap_size == 0)// 삭제연산 진행 후 히프에 데이터가 존재하지 않을 경우
            fprintf(file_close, "%c% : %d / Not Exist", type, key);
        else if (h->heap_size < 0)// 히프에 데이터가 존재하지 않아 삭제연산이 진행되지 않은 경우
            fprintf(file_close, "Not Exist");
        else
        {
            fprintf(file_close, "%c% : %d / ", type, key);
            fprintf(file_close, "%d", h->heap[0]);// 첫 번째 값은 콤마 기호가 필요 없으므로 따로 출력
            for (i = 1; i < h->heap_size; i++)// 이후의 나머지 값들은 콤마 기호를 표시한 다음 값을 출력
                fprintf(file_close, ", %d", h->heap[i]);
            for (i = h->heap_size; i < sum; i++)// level order 순서를 위해 각 깊이의 최대 노드수 만큼 나머지를 0으로 출력
                fprintf(file_close, ", %d", 0);
        }
        fputs("\n", file_close);// 다음 문장 출력을 위한 개행
    }
}

int main(void)
{
    HeapType h;// 히프구조체 변수 h
    init(&h);// 초기화

    char source[MAX_SIZE];//파일에서 읽어들인 문자열을 저장하기 위한 배열
    char* buf;//분할된 문자열의 포인터 값을 저장할 변수
    char type;// insert, delete 중 어떤 연산인지에 대한 문자 i, d가 저장되는 변수
    int num = 0;// "input.txt"파일에서 데이터로 이루어진 첫 문장과 나머지 문장들을 구분하기 위한 변수
    int delete = 0;// delete_recur 함수의 반환값을 저장받을 변수
    int data;// key값이 저장되는 변수
    int len;// buf의 길이를 저장받을 변수
    int expon;// 지수값을 저장할 변수
    int i;// for문에 사용될 변수

    FILE* file_open;// fopen이 반환하는 FILE 포인터를 받을 변수 file_open 선언
    file_open = fopen("input.txt", "r");
    // fopen 함수로 "input.txt" 파일을 읽기 모드로 열고 FILE 포인터 값을 file_open에 저장한다.
    if (file_open == NULL)// 파일열기에 실패하였을 경우를 대비
        error("파일열기 실패");
    FILE* file_close1;// fopen이 반환하는 FILE 포인터를 받을 변수 file_close1 선언
    file_close1 = fopen("output1.txt", "a");
    // fopen 함수로 "output.txt" 파일을 추가 쓰기(a) 모드로 열고 FILE 포인터 값을 file_close1에 저장한다.
    // "output1.txt" 파일에는 "Sequential Min Heapify"로 정렬된 히프가 기록된다.
    if (file_close1 == NULL)// 파일열기에 실패하였을 경우를 대비
        error("파일열기 실패");

    while (fgets(source, sizeof(source), file_open) != NULL)
    {//fgets 함수로 파일의 마지막 문장을 읽을 때까지 실행
        if (num == 0)// 첫 문장의 경우에만 실행
        {
            buf = strtok(source, " ");//strtok 함수로 공백을 기준으로 문자열을 분할한 뒤 포인터 반환
            while (buf != NULL)//자른 문자열이 나오지 않을 때까지 반복한다.
            {
                h.heap[h.heap_size] = atoi(buf);// 정수형태로 변환
                h.heap_size++;
                buf = strtok(NULL, " ");//다음 문자열을 잘라서 포인터를 반환한다.
            }
            seque_min_heapify(&h, h.heap_size);// 반복문을 사용하여 히프를 정렬한다.
            num = 1;// num의 값을 바꾸어 insert, delete에 대한 문장으로 바꾼다
        }
        else
        {
            data = 0;// 문장을 읽을 때 마다 초기화 필요 
            expon = 0;// 문장을 읽을 때 마다 초기화 필요
            type = source[0];// i, d와 같은 문자를 type 변수에 저장
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
            {// insert_recur 함수를 호출하여 삽입연산 실행 후 write_file 함수로 파일에 기록한다.
                insert_recur(&h, data, h.heap_size);
                write_file(file_close1, &h, type, data);
            }
            else if (type == 'd')
            {// delete_recur 함수를 호출하여 삽입연산 실행 후 write_file 함수로 파일에 기록한다.
                delete = delete_recur(&h, h.heap[0], h.heap[h.heap_size], h.heap_size--, 0, 1);// 반환된 값을 delete에 저장
                write_file(file_close1, &h, type, delete);
            }
            else
            {// i, d가 아닌 다른 문자일 경우 오류가 발생했음을 표시한다.
                error("\"input.txt\"파일에 불필요한 문자가 삽입되었습니다");
                // "input.txt"파일에 개행문자로만 이루어진 문장이 존재할 경우에도 오류가 발생합니다
                break;
            }
        }
    }
    fputs("\n", file_close1);// 다음 파일 입출력의 결과와 구분하기 위한 개행
    fclose(file_open);// "input.txt"파일에 대한 작업 종료
    fclose(file_close1);// "output1.txt"파일에 대한 작업 종료

    init(&h);// 다음의 연산을 위해 초기화

    file_open = fopen("input.txt", "r");
    // fopen 함수로 "input.txt" 파일을 읽기 모드로 열고 FILE 포인터 값을 file_open에 저장한다.
    if (file_open == NULL)// 파일열기에 실패하였을 경우를 대비
        error("파일열기 실패");
    FILE* file_close2;// fopen이 반환하는 FILE 포인터를 받을 변수 file_close2 선언
    file_close2 = fopen("output2.txt", "a");
    // fopen 함수로 "output.txt" 파일을 추가 쓰기(a) 모드로 열고 FILE 포인터 값을 file_close2에 저장한다.
    // "output2.txt" 파일에는 "Recursive Min Heapify"로 정렬된 히프가 기록된다.
    if (file_close2 == NULL)// 파일열기에 실패하였을 경우를 대비
        error("파일열기 실패");

    num = 0;// 첫 문장의 데이터를 읽기 위해 0을 저장

    while (fgets(source, sizeof(source), file_open) != NULL)
    {//fgets 함수로 파일의 마지막 문장을 읽을 때까지 실행
        if (num == 0)// 첫 문장의 경우에만 실행
        {
            buf = strtok(source, " ");//strtok 함수로 공백을 기준으로 문자열을 분할한 뒤 포인터 반환
            while (buf != NULL)//자른 문자열이 나오지 않을 때까지 반복한다.
            {
                h.heap[h.heap_size] = atoi(buf);// 정수형태로 변환
                h.heap_size++;
                buf = strtok(NULL, " ");//다음 문자열을 잘라서 포인터를 반환한다.
            }
            recur_min_heapify(&h, h.heap_size);// 재귀함수를 사용하여 히프를 정렬한다.
            num = 1;// num의 값을 바꾸어 insert, delete에 대한 문장으로 바꾼다
        }
        else
        {
            data = 0;// 문장을 읽을 때 마다 초기화 필요 
            expon = 0;// 문장을 읽을 때 마다 초기화 필요
            type = source[0];// i, d와 같은 문자를 type 변수에 저장
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
            {// insert_recur 함수를 호출하여 삽입연산 실행 후 write_file 함수로 파일에 기록한다.
                insert_recur(&h, data, h.heap_size);
                write_file(file_close2, &h, type, data);
            }
            else if (type == 'd')
            {// delete_recur 함수를 호출하여 삽입연산 실행 후 write_file 함수로 파일에 기록한다.
                delete = delete_recur(&h, h.heap[0], h.heap[h.heap_size], h.heap_size--, 0, 1);// 반환된 값을 delete에 저장
                write_file(file_close1, &h, type, delete);
            }
            else
            {// i, d가 아닌 다른 문자일 경우 오류가 발생했음을 표시한다.
                error("\"input.txt\"파일에 불필요한 문자가 삽입되었습니다");
                // "input.txt"파일에 개행문자로만 이루어진 문장이 존재할 경우에도 오류가 발생합니다
                break;
            }
        }
    }
    fputs("\n", file_close2);// 다음 파일 입출력의 결과와 구분하기 위한 개행
    fclose(file_open);// "input.txt"파일에 대한 작업 종료
    fclose(file_close2);// "output2.txt"파일에 대한 작업 종료
    return 0;
}