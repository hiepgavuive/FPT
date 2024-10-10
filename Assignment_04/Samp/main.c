#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "primeNum.h"

/*******************************************************************************
* Code
******************************************************************************/
int main(int argc, char* argv[]) 
{
    if (argc != 3) 
    {
        if (argc < 3) 
        {
            printf("Error: Less than two numbers provided.\n");
        }
        else 
        {
            printf("Error: More than two numbers provided.\n");
        }
        help();
        return 1;
    }

    char* endptr;
    long value1 = strtol(argv[1], &endptr, 10);
    if (*endptr != '\0') 
    {
        printf("Error: Invalid input '%s'.\n", argv[1]);
        help();
        return 1;
    }

    long value2 = strtol(argv[2], &endptr, 10);
    if (*endptr != '\0') 
    
    {
        printf("Error: Invalid input '%s'.\n", argv[2]);
        help();
        return 1;
    }

    int numbers[2] = { (int)value1, (int)value2 };

    for (int i = 0; i < 2; ++i) 
    {
        if (isPrime(numbers[i])) 
        {
            printf("Number %d is a prime number.\n", numbers[i]);
        }
        else 
        {
            printf("Number %d is not a prime number.\n", numbers[i]);
        }
    }

    return 0;
}
