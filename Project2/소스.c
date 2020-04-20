#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define IN_FILE_NAME "Keylog_in_large.txt"
#define OUT_FILE_NAME "output.txt"
#define BUFFER_SIZE 1000

int line = 1; // ���൵

void decodeString(char*, char*, int);

typedef struct {
	char *data;
	int top;
}stack;
void push(stack*, char, int);
char pop(stack*);
char top(stack*);
int isEmpty(stack*);

int main() {
	double start, finish;
	FILE* input = fopen(IN_FILE_NAME, "rt");
	// �� ���� ���̸� ��� ���� ���� ������ �� �� �� ����
	FILE* input_sub = fopen(IN_FILE_NAME, "rt");
	FILE* output = fopen(OUT_FILE_NAME, "wt");
	if (input == NULL || output == NULL) {
		printf("error!\n");
		return 1;
	}
	char* buffer;
	char* decoded;
	// �� �� �о decoded�Լ� ����(�ݺ�)
	// �ð� ���
	start = clock();
	// ���ڿ� �а� buffer�� ����
	int i = 0; // �� ���� ���� ���� ����
	while (1) {
		// �� ���� ���� �� ���� for���� ���� ���̸� ��
		for (i = 1; i < 10000; i++) {
			if (fgetc(input_sub) == '\n')
				break;
		}
		// i�� �����Ͽ� ������ ũ��� �Ҵ��Ѵ�
		int new_buffer_size = BUFFER_SIZE * (1 + (i / BUFFER_SIZE));
		buffer = (char*)malloc(new_buffer_size);
		decoded = (char*)malloc(new_buffer_size);
		if (fgets(buffer, new_buffer_size, input) == NULL)
			break;
		decodeString(buffer, decoded, new_buffer_size);
		fputs(decoded, output);
		printf("���൵ : %.0lf%%\n", ((double)line / (double)541) * (float)100);
		line++;
		system("cls");
		free(buffer);
		free(decoded);
	}

	// �ð� Ȯ��
	finish = clock();
	printf("time : %.3f\n", ((finish - start) / CLK_TCK) / 1000);

	int state1, state2, state3;
	state1 = fclose(input);
	state3 = fclose(input_sub);
	state2 = fclose(output);
	if (state1 != 0 || state2 != 0 || state3 != 0) {
		printf("error!\n");
		return 1;
	}
}

void decodeString(char* buffer, char* decoded, int new_buffer_size) {
	stack* dec = (stack*)malloc(sizeof(stack)); // ���� �Ҵ�
	stack* temp = (stack*)malloc(sizeof(stack));
	dec->top = -1;
	temp->top = -1;
	dec->data = (char*)malloc(new_buffer_size); // ���� ���� �迭 �Ҵ�
	temp->data = (char*)malloc(new_buffer_size);
	// buffer�� ���ڷ� decoded ä���
	int i;
	for (i = 0; i < strlen(buffer); i++) {
		if (buffer[i] == '<') {
			if (!isEmpty(dec)) // �� �迭�� �ƴϸ�
				push(temp, pop(dec), new_buffer_size);
		}
		else if (buffer[i] == '>') {
			if (!isEmpty(temp)) {
				push(dec, pop(temp), new_buffer_size);
			}
		}
		else if (buffer[i] == '-') {
			if (!isEmpty(dec)) {
				pop(dec);
			}
		}
		else {
			push(dec, buffer[i], new_buffer_size);
		}
	}
	// dec �� temp�� ���ļ� decoded�� �ű�.
	// ���� \0 �־��ְ� string�Լ� ���
	push(dec, '\0', new_buffer_size);
	push(temp, '\0', new_buffer_size);
	strrev(temp->data);
	strcat(dec->data,temp->data);
	strcpy(decoded, dec->data);
	// ����
	free(dec->data);
	free(temp->data);
	free(dec);
	free(temp);
}
// new_buffer_size : �����÷ο츦 ���� ���� �Ҵ�� ũ�⸦ �˷��ִ� �μ�
void push(stack* s, char element, int new_buffer_size) {
	if (s->top > new_buffer_size -2) {
		printf("Stack is Overflow!\n");
		exit(1);
	}
	else
		s->data[++s->top] = element;
}
char pop(stack* s) {
	if (isEmpty(s)) {
		printf("Stack is Empty!\n");
		exit(1);
	}
	else
		return s->data[s->top--];
}
char top(stack* s) {
	if (isEmpty(s)) {
		printf("Stack is Empty!\n");
		exit(1);
	}
	else
		return s->data[s->top];
}
int isEmpty(stack* s) {
	if (s->top == -1)
		return 1;
	else
		return 0;
}