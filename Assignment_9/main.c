#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "file.h"

/*******************************************************************************
* Definitions
******************************************************************************/
#define MAX 256


/*******************************************************************************
* Prototypes
******************************************************************************/
void printMenu()
{
    printf("\nMenu:\n");
    printf("1. Display file content\n");
    printf("2. Append a string to the file\n");
    printf("3. Find a string in the file\n");
    printf("4. Exit\n");
    printf("Enter your choice: ");
}

int main(int argc, char* argv[]) 
{
    if (argc < 2) {
        printf("Usage: %s <Assignment_09.txt>\n", argv[0]);
        return 1;
    }

    char filepath[MAX];
    strncpy(filepath, argv[1], MAX);
    filepath[MAX - 1] = '0';

    if (!openOrCreateFile(filepath)) {
        return 1;
    }

    while (true) {
        printMenu();

        int choice;
        scanf("%d", &choice);
        getchar(); /* Clear the newline character from the buffer */

        switch (choice) {
        case 1:
            displayFileContent(filepath);
            break;
        case 2: {
            char inputString[MAX];
            printf("Enter the string to append: ");
            fgets(inputString, MAX, stdin);
            inputString[strcspn(inputString, "\n")] = 0; /*  Remove newline character */
            appendStringToFile(filepath, inputString);
            break;
        }
        case 3: {
            char searchString[MAX];
            printf("Enter the string to find: ");
            fgets(searchString, MAX, stdin);
            searchString[strcspn(searchString, "\n")] = 0; /* Remove newline character */ 
            findStringInFile(filepath, searchString);
            break;
        }
        case 4:
            printf("Exiting the program.\n");
            return 0;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }


    return 0;
}

/*******************************************************************************
* End of file
******************************************************************************/