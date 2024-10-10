#ifndef __STUDENT_MANAGEMENT_H__
#define __STUDENT_MANAGEMENT_H__

#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
* Definitions
******************************************************************************/

/**
 * @brief Defines a Student structure that contains information about students.
 */
typedef struct Student 
{
    char name[50];
    int id;
    float math_score;
    struct Student* next;
} Student;


/*******************************************************************************
* API
******************************************************************************/

/**
 * @brief The function adds students to the beginning of the linked list.
 */
bool addStudent(Student** head, const char* name, int id, float math_score);

/**
 * @brief The function to remove students from a linked list based on ID.
 */
bool removeStudent(Student** head, int id);

/**
 * @brief The function to search for students in a linked list based on ID.
 */
Student* findStudent(Student* head, int id);

/**
 * @brief Function to print information of a student.
 */
void printStudent(const Student* student);

/**
 * @brief The function prints information of all students in the linked list.
 */
void printAllStudents(const Student* head);

#endif /* __STUDENT_MANAGEMENT_H__ */

/*******************************************************************************
* End of file
******************************************************************************/