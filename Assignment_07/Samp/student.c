#define _CRT_SECURE_NO_WARNINGS
#include "student.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief The function adds students to the beginning of the linked list.
 */
bool addStudent(Student** head, const char* name, int id, float math_score) 
{
    if (head == NULL || name == NULL) 
    {
        return false;
    }

    Student* newStudent = (Student*)malloc(sizeof(Student));
    if (newStudent == NULL) 
    {
        return false;
    }

    strcpy(newStudent->name, name);
    newStudent->id = id;
    newStudent->math_score = math_score;
    newStudent->next = *head;
    *head = newStudent;

    return true;
}

/**
 * @brief The function to remove students from a linked list based on ID.
 */
bool removeStudent(Student** head, int id) 
{
    if (head == NULL || *head == NULL) 
    {
        return false;
    }

    Student* current = *head;
    Student* previous = NULL;

    while (current != NULL && current->id != id) 
    {
        previous = current;
        current = current->next;
    }

    if (current == NULL) 
    {
        return false;
    }

    if (previous == NULL) 
    {
        *head = current->next;
    }
    else 
    {
        previous->next = current->next;
    }

    free(current);
    return true;
}

/**
 * @brief The function to search for students in a linked list based on ID.
 */
Student* findStudent(Student* head, int id) 
{
    Student* current = head;
    while (current != NULL) {
        if (current->id == id) 
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

/**
 * @brief Function to print information of a student.
 */
void printStudent(const Student* student) 
{
    if (student != NULL) 
    {
        printf("Name: %s\n", student->name);
        printf("ID: %d\n", student->id);
        printf("Math Score: %.2f\n", student->math_score);
    }
}

/**
 * @brief The function prints information of all students in the linked list.
 */
void printAllStudents(const Student* head) 
{
    const Student* current = head;
    while (current != NULL) 
    {
        printStudent(current);
        printf("----------\n");
        current = current->next;
    }
}

/*******************************************************************************
* End of file
******************************************************************************/