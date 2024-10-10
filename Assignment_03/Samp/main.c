#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdint.h>
#include "arrayController.h"

/*******************************************************************************
* Definitions
******************************************************************************/
#define ARRAY_SIZE 100
#define MIN_Value 0
#define MAX_Value 100

/*******************************************************************************
* Variables
******************************************************************************/
uint8_t array[ARRAY_SIZE];

/*******************************************************************************
* Prototypes
******************************************************************************/
void help() {
    printf("\nMenu:\n");
    printf("Enter 'c' to clear the data of the current array and allow the user to input new values.\n");
    printf("Enter 'p' to print the array.\n");
    printf("Enter 'i' to insert an element into the array at position k.\n");
    printf("Enter 'd' to delete an element at position k.\n");
    printf("Enter 's' to print the array in ascending order.\n");
    printf("Enter 'x' to print the array in descending order.\n");
    printf("Enter 't' to search for a number x in the array and display its position if found.\n");
    printf("Enter 'a' to print the largest number in the array.\n");
    printf("Enter 'w' to print the smallest number in the array.\n");
    printf("Enter 'e' to exit the program.\n");
}


/*******************************************************************************
* Code
******************************************************************************/
int main(int argc, char* argv[]) 
{
    char choice = "";
    int position;
    uint8_t value;
    int foundIndex;

    setUp(array, ARRAY_SIZE, MIN_Value, MAX_Value);

    while (choice != 'e') 
    {
        help();
        printf("Enter your choice: ");
        scanf(" %c", &choice);

        switch (choice)
        {
            case 'c': 
            {   
                clearArray(array, ARRAY_SIZE);
                printf("Array data cleared.\n");
                break;
            }
            case 'p':
            {
                printArray(array, ARRAY_SIZE);
                break;
            }
            case 'i':
            {
                printf("Enter position to insert (0-99): ");
                scanf("%d", &position);
                printf("Enter value (0-100): ");
                scanf("%hhu", &value);
                if (value <= 100) 
                {
                    insertElement(array, ARRAY_SIZE, position, value);
                }
                else 
                {
                    printf("Invalid value.\n");
                }
                break;
            }
            case 'd':
            {
                printf("Enter position to delete (0-99): ");
                scanf("%d", &position);
                deleteElement(array, ARRAY_SIZE, position);
                break;
            }
            case 's':
            {
                sortArrayAscending(array, ARRAY_SIZE);
                break;
            }
            case 'x':
            {
                sortArrayDescending(array, ARRAY_SIZE);
                break;
            }
            case 't':
            {
                printf("Enter value to search: ");
                scanf("%hhu", &value);
                foundIndex = searchElement(array, ARRAY_SIZE, value);
                if (foundIndex != -1) 
                {
                    printf("Value %d found at position %d.\n", value, foundIndex);
                }
                else 
                {
                    printf("Value %d not found.\n", value);
                }
                break;
            }
            case 'a':
            {
                printf("The largest value in the array is: %d\n", findMax(array, ARRAY_SIZE));
                break;
            }
            case 'w':
            {
                printf("The smallest value in the array is: %d\n", findMin(array, ARRAY_SIZE));
                break;
            }
            case 'e':
            {
                printf("Exiting the program.\n");
                break;
            }
            default:
            {
                printf("Invalid choice. Please try again.\n");
                break;
            }
        }
    }
    return 0;
}
