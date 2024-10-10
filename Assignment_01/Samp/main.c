//#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "fibonacci.h"

int main() {
    int n;
    printf("Nhap gia tri cua N: ");
    scanf_s("%d", &n);

    if (n < 1 || n > 100) {
        printf("Vui long nhap gia tri trong khoang tu 1 den 100.\n");
        return 1;
    }

    int soFibonacci = fibonacci(n);
    printf("So Fibonacci thu %d là: %d\n", n, soFibonacci);

    return 0;
}
