#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "student.h"


/*******************************************************************************
* Prototypes
******************************************************************************/

/*******************************************************************************
* Student Management Program
* '1' add information of all students
* '2' remove information of students in a linked list
* '3' find information of students in a linked list
* '4' prints information of all students
* '5' to exit the program
******************************************************************************/
void printMenu() 
{
    printf("Student Management Program\n");
    printf("Choose an option:\n");
    printf("1. Add Student\n");
    printf("2. Remove Student\n");
    printf("3. Find Student\n");
    printf("4. Print All Students\n");
    printf("5. Exit\n");
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
    Student* head = NULL;
    int choice = 0;
    char name[50];
    int id;
    float math_score;

    while (true) {
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
            printf("Enter student's name: ");
            scanf("%s", name);
            printf("Enter student's ID: ");
            scanf("%d", &id);
            printf("Enter student's Math score: ");
            scanf("%f", &math_score);

            if (addStudent(&head, name, id, math_score)) 
            {
                printf("Student added successfully.\n");
            }
            else 
            {
                printf("Failed to add student.\n");
            }
            break;
        case 2:
            printf("Enter student's ID to remove: ");
            scanf("%d", &id);

            if (removeStudent(&head, id)) 
            {
                printf("Student removed successfully.\n");
            }
            else 
            {
                printf("Failed to remove student.\n");
            }
            break;
        case 3:
            printf("Enter student's ID to find: ");
            scanf("%d", &id);
            Student* student = findStudent(head, id);

            if (student != NULL) 
            {
                printf("Student found:\n");
                printStudent(student);
            }
            else 
            {
                printf("Student not found.\n");
            }
            break;

        case 4:
            printAllStudents(head);
            break;
        case 5:
            while (head != NULL) 
            {
                Student* temp = head;
                head = head->next;
                free(temp);
            }
            printf("Exiting the program.\n");
            return 1;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

/*******************************************************************************
* End of file
******************************************************************************/