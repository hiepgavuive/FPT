#include "palindrome.h"

// Ham kiem tra palindrome
int isPalindrome(char* str, int length) {
    for (int i = 0; i < length / 2; ++i) {
        if (str[i] != str[length - i - 1]) {
            return 0; // Khong phai palindrome
        }
    }
    return 1; // La palindrome
}

// Ham dao nguoc
void reverseString(char* str, int length, char* reversedStr) {
    for (int i = 0; i < length; ++i) {
        reversedStr[i] = str[length - i - 1];
    }
    reversedStr[length] = '\0';
}
