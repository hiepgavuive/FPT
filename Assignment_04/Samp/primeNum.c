#include "primeNum.h"
#include <stdio.h>
#include <math.h>


/*******************************************************************************
* Prototypes
******************************************************************************/
bool isPrime(int num) 
{
    if (num <= 1) 
    {
        return false;
    }
    for (int i = 2; i <= sqrt(num); ++i) 
    {
        if (num % i == 0) 
        {
            return false;
        }
    }
    return true;
}

void help() 
{
    printf("This program allows you to input two integers as command-line arguments.\n");
    printf("It will check and display whether these two numbers are prime numbers.\n");
    printf("Error messages will be displayed if:\n");
    printf(" - You provide no input.\n");
    printf(" - You provide a character.\n");
    printf(" - You provide only one number.\n");
    printf(" - You provide more than two numbers.\n");
    printf("Usage: ./program_name.exe num1 num2\n");
}
