//#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "palindrome.h"

#define MAX_LENGTH 255

int main() {
    char input[MAX_LENGTH + 1];
    char reversed[MAX_LENGTH + 1];
    int length = 0;
    char c;

    printf("Nhap chuoi (toi da 255 ky tu): ");

    while ((c = getchar()) != '\n' && length < MAX_LENGTH) {
        input[length] = c;
        ++length;
    }
    input[length] = '\0'; // ket thuc chuoi

    if (isPalindrome(input, length)) {
        printf("Chuoi ban nhap la palindrome.\n");
    }
    else {
        reverseString(input, length, reversed);
        printf("Chuoi ban nhap khong phai la palindrome.\n");
        printf("Chuỗi đảo ngược là: %s\n", reversed);
    }

    return 0;
}

