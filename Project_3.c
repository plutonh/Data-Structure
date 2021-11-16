#define _CRT_SECURE_NO_WARNINGS 
// fopen_s �� ���� �������� ���� '_s'�� �߰��� ���·� �Լ����� ��ü�Ǿ��� ������ '_s'�� ���� 
// ������ �Լ�(fopen, strtok ��)�� ����ϱ� ���� �ش� �ڵ带 �����Ͽ����ϴ�.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_SIZE 30

typedef struct {// ���� ����ü HeapType
	int heap[MAX_SIZE];// �����Ͱ� ����� �迭
	int heap_size;// ������ ũ��
}HeapType;

void error(char* message)// ���� ���꿡�� ���� �߻��� �޼����� ����ϴ� �Լ� error
{
    fprintf(stderr, "%s\n", message);
    exit(1);
}

void init(HeapType* h)// �ʱ�ȭ �Լ� init
{
	h->heap_size = 0;
}

void swap(HeapType* h, int a, int b)// a, b�� ��ġ�� �ٲٴ� �Լ� swap
{
    int temp = h->heap[a];
    h->heap[a] = h->heap[b];
    h->heap[b] = temp;
}

void seque_min_heapify(HeapType* h, int size)// �ݺ����� ����Ͽ� Min Heapify�� �����ϴ� �Լ� seque_min_heapify
{
    for (int i = 1; i < size; i++)
    {
        int c = i;//�ڽ� ���
        do
        {//�ڽ� ��忡�� 1�� ���� 2�� ������ �θ� ��尡 ���´�
            int p = (c - 1) / 2;
            //���� �θ� ��庸�� �ڽ� ��尡 �� ũ�ٸ� ���� �ٲ��ش�.
            if (h->heap[p] < h->heap[c])
                swap(&h->heap, p, c);
            //��Ʈ���� ����
            c = p;
        } while (c != 0);
    }
    //���� ū ���� ��Ʈ�� �Ǿ��� ������ ��Ʈ�� ���� ������ ���� ������������ ����
    for (int i = size - 1; i >= 0; i--)//ũ�⸦ �ٿ����� �ݺ������� ���� ����
    {
        swap(&h->heap, 0, i);// ���� ū ���� �� �ڷ� ������
        int p = 0;
        int c = 1;
        do// �� ������ �����
        {
            c = p * 2 + 1;
            if (h->heap[c] < h->heap[c + 1] && c < i - 1)// �ڽ� �߿� �� ū ���� ã�� ������ �����Ѵ�
                c++;
            if (h->heap[p] < h->heap[c] && c < i)// �θ𺸴� �ڽ��� �� ũ�ٸ� ��ȯ
                swap(&h->heap, p, c);
            p = c;// ���� ������ ���� �θ� ������ �ڽ����� ����
        } while (c < i);
    }
}

void heapify_recur(HeapType* h, int k, int size)// �θ𺸴� ū �ڽ��� ���� ��ȯ�ϴ� �Լ� heapify_recur
{
    int p = k;// �θ�
    int left = k * 2 + 1;// ���� �ڽ�
    int right = k * 2 + 2;// ������ �ڽ�
    if (left < size && h->heap[left] > h->heap[k])// ���� �ڽ��� �θ𺸴� Ŭ ���
        p = left;
    if (right < size && h->heap[right] > h->heap[p])// ������ �ڽ��� �θ𺸴� Ŭ ���
        p = right;
    if (p != k)// ���� �� ���� �� �ϳ��� ����Ǿ��� ���
    {
        swap(&h->heap, k, p);// �θ�� �ڽ��� ��ġ�� ��ȯ�Ѵ�
        heapify_recur(h->heap, p, size);// ������ �ڽ��� �θ�� �ϴ� ������ �����ϱ� ���� ����Լ� ����
    }
}

void make_heap(HeapType* h, int size)// �� ������ �����ϴ� �Լ� make_heap
{
    int i = (size / 2) - 1;// �ϳ� �̻��� �ڽ��� ������ ��忡 ���� heapify_recur �Լ��� ȣ���Ͽ� �ּ��� Ư���� �����ϰ� ��
    for (; i >= 0; --i)// �ε����� ���̸� heapify_recur �Լ��� ���� ����
        heapify_recur(h->heap, i, size);
}

void recur_min_heapify(HeapType* h, int size)// ����Լ��� ����Ͽ� Min Heapify�� �����ϴ� �Լ� recur_min_heapify
{
    int i = 0;
    make_heap(h->heap, size);// make_heap �Լ��� ���� �� ������ �����Ѵ�
    for (i = size - 1; i > 0; --i) 
    {
        swap(&h->heap, 0, i);// �� ������ �����ϸ� ���� ū ���� ��Ʈ�� �Ǿ��� ������ �� �ڷ� ������
        // �ݺ������� ū ���� �Ʒ��� �������� ������������ ����
        heapify_recur(h->heap, 0, --size);// ũ�⸦ ���̸� heapify_recur �Լ��� ���� ����
    }
}

void insert_recur(HeapType* h, int item, int i)// Min Heap�� �����͸� �����ϴ� �Լ� insert_recur
{
    if ((i != 0) && (item <= h->heap[(i - 1) / 2]))// �ε����� 0�� �ƴϰ� �θ𺸴� ���� ���
    {
        h->heap[i] = h->heap[(i - 1) / 2];// �θ��� �����͸� ���� �ε��� ��ġ�� ����
        insert_recur(&h->heap, item, (i - 1) / 2);// ���� �θ���� �񱳸� ���� ����Լ� ���
        return;
    }
    h->heap[i] = item;// ���� �������� ���� �ε����� item�� �����Ѵ�
    h->heap_size++;// �����͸� �ϳ� ���������Ƿ� ũ�⸦ 1 ������Ų��
}

int delete_recur(HeapType* h, int item, int temp, int size, int p, int c)
// Min heap���� ���� ���� ���� �����ϴ� �Լ� delete_recur
{
    if (size <= 0)// ������ ����� ��� NULL�� ��ȯ
        return NULL;
    if (c <= size)// �ε����� ������ ũ�⺸�� ���� ���
    {
        if ((c + 1 <= size) && (h->heap[c]) > h->heap[c + 1])
        //������ �ڽ��� �ε��� ������ ũ�⺸�� ���� ��� �� �ڽ� �߿��� ���� ���� ���� 
            c++;
        if (temp <= h->heap[c] || c == size)
        // ������ �ε����� �����Ͱ� �ڽĺ��� �۰ų� ���� ��� �Ǵ� �ڽ��� �ε����� �迭�� �������� ���������� 
        {
            h->heap[p] = temp;// temp�� �θ� ����
            return item;// ������ ������ item�� ��ȯ
        }
        h->heap[p] = h->heap[c];// ���� ������ ���� ������ �ڽ��� �θ� ����
        if (c <= size)
        // �θ𿡴� ������ �ڽ�, �ڽĿ��� 2 * (c + 1) - 1 ������ ���� ���� �θ��� �ڽ��� ������ �Ͽ� ����Լ� ���
            item = delete_recur(&h->heap, item, temp, size, c, 2 * (c + 1) - 1);
        else// �ڽ��� �ε����� ���� ���̸� �ʰ��� ��� �θ� temp�� �����ϰ� item�� ��ȯ
        {
            h->heap[p] = temp;
            return item;
        }
    }
    else// �ڽ��� �ε����� ���� ���̸� �ʰ��� ��� �θ� temp�� �����ϰ� item�� ��ȯ
    {
        h->heap[p] = temp;
        return item;
    }
}

void write_file(FILE* file_close, HeapType* h, char type, int key)// ���� ������ �ۼ��� �Լ� write_file
{
    int i = 1;// ������ ��� for���� ���� ����
    int height;// ������ ���̸� ������� ����
    int sum = 1;
    while(1)// ������ ���̸� ���ϴ� ����
    {
        if (h->heap_size <= sum)// sum�� ���� ������ ũ�⺸�� ũ�ų� ���� ��� i���� height�� �����ϰ� Ż��
        {
            height = i;
            break;
        }
        else
        {
            sum += (int)powf(2, i);
            // ������ ������ �������� 2�� ���̸�ŭ ������ ��尡 �����ϹǷ� ���� ����� ���� ���ϱ� ���� �����Ѵ�
            i++;
        }
    }
    if (type == 'i')// insert ������ ���
    {
        fprintf(file_close, "%c%d : ", type, key);
        // "input.txt" ������ ���� ó�� ������ ������ ������ ������ ���� �����͸� ǥ���Ѵ�
        fprintf(file_close, "%d", h->heap[0]);// ù ��° ���� �޸� ��ȣ�� �ʿ� �����Ƿ� ���� ���
        for (i = 1; i < h->heap_size; i++)// ������ ������ ������ �޸� ��ȣ�� ǥ���� ���� ���� ���
            fprintf(file_close, ", %d", h->heap[i]);
        for (i = h->heap_size; i < sum; i++)// level order ������ ���� �� ������ �ִ� ���� ��ŭ �������� 0���� ���
            fprintf(file_close, ", %d", 0);
        fputs("\n", file_close);// ���� ���� ����� ���� ����
    }
    else if (type == 'd')// delete ������ ���
    {
        if (h->heap_size == 0)// �������� ���� �� ������ �����Ͱ� �������� ���� ���
            fprintf(file_close, "%c% : %d / Not Exist", type, key);
        else if (h->heap_size < 0)// ������ �����Ͱ� �������� �ʾ� ���������� ������� ���� ���
            fprintf(file_close, "Not Exist");
        else
        {
            fprintf(file_close, "%c% : %d / ", type, key);
            fprintf(file_close, "%d", h->heap[0]);// ù ��° ���� �޸� ��ȣ�� �ʿ� �����Ƿ� ���� ���
            for (i = 1; i < h->heap_size; i++)// ������ ������ ������ �޸� ��ȣ�� ǥ���� ���� ���� ���
                fprintf(file_close, ", %d", h->heap[i]);
            for (i = h->heap_size; i < sum; i++)// level order ������ ���� �� ������ �ִ� ���� ��ŭ �������� 0���� ���
                fprintf(file_close, ", %d", 0);
        }
        fputs("\n", file_close);// ���� ���� ����� ���� ����
    }
}

int main(void)
{
    HeapType h;// ��������ü ���� h
    init(&h);// �ʱ�ȭ

    char source[MAX_SIZE];//���Ͽ��� �о���� ���ڿ��� �����ϱ� ���� �迭
    char* buf;//���ҵ� ���ڿ��� ������ ���� ������ ����
    char type;// insert, delete �� � ���������� ���� ���� i, d�� ����Ǵ� ����
    int num = 0;// "input.txt"���Ͽ��� �����ͷ� �̷���� ù ����� ������ ������� �����ϱ� ���� ����
    int delete = 0;// delete_recur �Լ��� ��ȯ���� ������� ����
    int data;// key���� ����Ǵ� ����
    int len;// buf�� ���̸� ������� ����
    int expon;// �������� ������ ����
    int i;// for���� ���� ����

    FILE* file_open;// fopen�� ��ȯ�ϴ� FILE �����͸� ���� ���� file_open ����
    file_open = fopen("input.txt", "r");
    // fopen �Լ��� "input.txt" ������ �б� ���� ���� FILE ������ ���� file_open�� �����Ѵ�.
    if (file_open == NULL)// ���Ͽ��⿡ �����Ͽ��� ��츦 ���
        error("���Ͽ��� ����");
    FILE* file_close1;// fopen�� ��ȯ�ϴ� FILE �����͸� ���� ���� file_close1 ����
    file_close1 = fopen("output1.txt", "a");
    // fopen �Լ��� "output.txt" ������ �߰� ����(a) ���� ���� FILE ������ ���� file_close1�� �����Ѵ�.
    // "output1.txt" ���Ͽ��� "Sequential Min Heapify"�� ���ĵ� ������ ��ϵȴ�.
    if (file_close1 == NULL)// ���Ͽ��⿡ �����Ͽ��� ��츦 ���
        error("���Ͽ��� ����");

    while (fgets(source, sizeof(source), file_open) != NULL)
    {//fgets �Լ��� ������ ������ ������ ���� ������ ����
        if (num == 0)// ù ������ ��쿡�� ����
        {
            buf = strtok(source, " ");//strtok �Լ��� ������ �������� ���ڿ��� ������ �� ������ ��ȯ
            while (buf != NULL)//�ڸ� ���ڿ��� ������ ���� ������ �ݺ��Ѵ�.
            {
                h.heap[h.heap_size] = atoi(buf);// �������·� ��ȯ
                h.heap_size++;
                buf = strtok(NULL, " ");//���� ���ڿ��� �߶� �����͸� ��ȯ�Ѵ�.
            }
            seque_min_heapify(&h, h.heap_size);// �ݺ����� ����Ͽ� ������ �����Ѵ�.
            num = 1;// num�� ���� �ٲپ� insert, delete�� ���� �������� �ٲ۴�
        }
        else
        {
            data = 0;// ������ ���� �� ���� �ʱ�ȭ �ʿ� 
            expon = 0;// ������ ���� �� ���� �ʱ�ȭ �ʿ�
            type = source[0];// i, d�� ���� ���ڸ� type ������ ����
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
            {// insert_recur �Լ��� ȣ���Ͽ� ���Կ��� ���� �� write_file �Լ��� ���Ͽ� ����Ѵ�.
                insert_recur(&h, data, h.heap_size);
                write_file(file_close1, &h, type, data);
            }
            else if (type == 'd')
            {// delete_recur �Լ��� ȣ���Ͽ� ���Կ��� ���� �� write_file �Լ��� ���Ͽ� ����Ѵ�.
                delete = delete_recur(&h, h.heap[0], h.heap[h.heap_size], h.heap_size--, 0, 1);// ��ȯ�� ���� delete�� ����
                write_file(file_close1, &h, type, delete);
            }
            else
            {// i, d�� �ƴ� �ٸ� ������ ��� ������ �߻������� ǥ���Ѵ�.
                error("\"input.txt\"���Ͽ� ���ʿ��� ���ڰ� ���ԵǾ����ϴ�");
                // "input.txt"���Ͽ� ���๮�ڷθ� �̷���� ������ ������ ��쿡�� ������ �߻��մϴ�
                break;
            }
        }
    }
    fputs("\n", file_close1);// ���� ���� ������� ����� �����ϱ� ���� ����
    fclose(file_open);// "input.txt"���Ͽ� ���� �۾� ����
    fclose(file_close1);// "output1.txt"���Ͽ� ���� �۾� ����

    init(&h);// ������ ������ ���� �ʱ�ȭ

    file_open = fopen("input.txt", "r");
    // fopen �Լ��� "input.txt" ������ �б� ���� ���� FILE ������ ���� file_open�� �����Ѵ�.
    if (file_open == NULL)// ���Ͽ��⿡ �����Ͽ��� ��츦 ���
        error("���Ͽ��� ����");
    FILE* file_close2;// fopen�� ��ȯ�ϴ� FILE �����͸� ���� ���� file_close2 ����
    file_close2 = fopen("output2.txt", "a");
    // fopen �Լ��� "output.txt" ������ �߰� ����(a) ���� ���� FILE ������ ���� file_close2�� �����Ѵ�.
    // "output2.txt" ���Ͽ��� "Recursive Min Heapify"�� ���ĵ� ������ ��ϵȴ�.
    if (file_close2 == NULL)// ���Ͽ��⿡ �����Ͽ��� ��츦 ���
        error("���Ͽ��� ����");

    num = 0;// ù ������ �����͸� �б� ���� 0�� ����

    while (fgets(source, sizeof(source), file_open) != NULL)
    {//fgets �Լ��� ������ ������ ������ ���� ������ ����
        if (num == 0)// ù ������ ��쿡�� ����
        {
            buf = strtok(source, " ");//strtok �Լ��� ������ �������� ���ڿ��� ������ �� ������ ��ȯ
            while (buf != NULL)//�ڸ� ���ڿ��� ������ ���� ������ �ݺ��Ѵ�.
            {
                h.heap[h.heap_size] = atoi(buf);// �������·� ��ȯ
                h.heap_size++;
                buf = strtok(NULL, " ");//���� ���ڿ��� �߶� �����͸� ��ȯ�Ѵ�.
            }
            recur_min_heapify(&h, h.heap_size);// ����Լ��� ����Ͽ� ������ �����Ѵ�.
            num = 1;// num�� ���� �ٲپ� insert, delete�� ���� �������� �ٲ۴�
        }
        else
        {
            data = 0;// ������ ���� �� ���� �ʱ�ȭ �ʿ� 
            expon = 0;// ������ ���� �� ���� �ʱ�ȭ �ʿ�
            type = source[0];// i, d�� ���� ���ڸ� type ������ ����
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
            {// insert_recur �Լ��� ȣ���Ͽ� ���Կ��� ���� �� write_file �Լ��� ���Ͽ� ����Ѵ�.
                insert_recur(&h, data, h.heap_size);
                write_file(file_close2, &h, type, data);
            }
            else if (type == 'd')
            {// delete_recur �Լ��� ȣ���Ͽ� ���Կ��� ���� �� write_file �Լ��� ���Ͽ� ����Ѵ�.
                delete = delete_recur(&h, h.heap[0], h.heap[h.heap_size], h.heap_size--, 0, 1);// ��ȯ�� ���� delete�� ����
                write_file(file_close1, &h, type, delete);
            }
            else
            {// i, d�� �ƴ� �ٸ� ������ ��� ������ �߻������� ǥ���Ѵ�.
                error("\"input.txt\"���Ͽ� ���ʿ��� ���ڰ� ���ԵǾ����ϴ�");
                // "input.txt"���Ͽ� ���๮�ڷθ� �̷���� ������ ������ ��쿡�� ������ �߻��մϴ�
                break;
            }
        }
    }
    fputs("\n", file_close2);// ���� ���� ������� ����� �����ϱ� ���� ����
    fclose(file_open);// "input.txt"���Ͽ� ���� �۾� ����
    fclose(file_close2);// "output2.txt"���Ͽ� ���� �۾� ����
    return 0;
}