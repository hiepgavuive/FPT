#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "arrayManagement.h"

/*******************************************************************************
* Prototypes
******************************************************************************/

/*******************************************************************************
* Array Management Program
* '1' add information of all students
* '2' remove value form the array
* '3' prints the values in the array
* '4' to exit the program
******************************************************************************/
void printMenu() 
{
    printf("Array Management Program\n");
    printf("1. Enter a value into a specified element of the array\n");
    printf("2. Delete a specific value from the array\n");
    printf("3. Print the values of the elements entered into the array\n");
    printf("4. End the program\n");
    printf("Enter your choice: ");
}

/*******************************************************************************
* @brief Clear input buffer to avoid invalid characters.
******************************************************************************/
void clearInputBuffer() 
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


int main(int argc, char* argv[]) 
{   
    uint8_t assignment8[20];
    Node nodes[20];
    Node* head = NULL;
    int choice = 0;

    for (int i = 0; i < 20; i++)
    {
        assignment8[i] = 0xFF;
        nodes[i].value = 0xFF;
        nodes[i].next = NULL;
    }

    while (true) 
    {
        printMenu();
        if (scanf("%d", &choice) != 1) 
        {
            printf("Invalid input. Please enter a number.\n");
            clearInputBuffer();
            continue;
        }

        switch (choice)
        {  
        case 1:
        {
            uint8_t value, pos;
            printf("Enter position (0-19): ");
            if (scanf("%hhu", &pos) != 1) 
            {
                printf("Invalid input. Please enter a number between 0 and 19.\n");
                clearInputBuffer();
                continue;
            }
            printf("Enter value (0-100): ");
            if (scanf("%hhu", &value) != 1) 
            {
                printf("Invalid input. Please enter a number between 0 and 100.\n");
                clearInputBuffer();
                continue;
            }
            if (insertValue(assignment8, nodes, &head, pos, value))
            {
                printf("Value %d inserted at position %d successfully.\n", value, pos);
            }
            break;
        }
        case 2:
        {
            uint8_t value;
            printf("Enter value to delete (0-100): ");
            if (scanf("%hhu", &value) != 1) 
            {
                printf("Invalid input. Please enter a number between 0 and 100.\n");
                clearInputBuffer();
                continue;
            }
            if (deleteValue(assignment8, nodes, &head, value)) 
            {
                printf("Value %d deleted successfully.\n", value);
            }
            break;
        }
        case 3:
        {
            int sub_choice;
            printf("Enter 1 to print unsorted, 2 to print sorted: ");
            if (scanf("%d", &sub_choice) != 1) 
            {
                printf("Invalid input. Please enter 1 or 2.\n");
                clearInputBuffer();
                continue;
            }
            if (sub_choice == 1) 
            {
                printArray2(assignment8, head, false);
            }
            else if (sub_choice == 2) 
            {
                printArray2(assignment8, head, true);
            }
            else 
            {
                printf("Invalid choice. Please enter 1 or 2.\n");
            }
            break;
        }
        case 4:
        {
            printf("Exiting the program.\n");
            /* Free the linked list memory */
            while (head != NULL) 
            {
                Node* temp = head;
                head = head->next;
                free(temp);
            }
            return 1;
        }
        default:
        {
            printf("Invalid choice. Please try again.\n");
        }
        }
    }

    return 0;
}

/*******************************************************************************
* End of file
******************************************************************************/