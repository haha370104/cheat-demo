#include <stdio.h>


int result = 1;

int add2(int a) {
	return a + 2;
}

int add3(int a) {
	return a + 3;
}

int addGlobal(int x) {
	result = result + x;
	return result;
}

int func1(int a, int b) {
	return add2(a);
}

void printResult() {
	printf("%d\n", result);
}

void printAAddress() {
    printf("%lu\n", &result);
}

int main()
{
    printAAddress();
	int input = 0;
	while (1) {
		scanf("%d", &input);
		addGlobal(input);
		printResult();
	}
}
