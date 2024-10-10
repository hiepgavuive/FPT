#ifndef __ARRAY_MANAGEMENT__
#define __ARRAY_MANAGEMENT__

#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
* Definitions
******************************************************************************/

/*******************************************************************************
 * @brief Node structure for the singly linked list.
******************************************************************************/
typedef struct Node
{
	uint8_t value;
	struct Node* next;
} Node;


/*******************************************************************************
* API
******************************************************************************/

/*******************************************************************************
 * @brief Inserts a value into the specified position of the array and into the sorted linked list.
 * @param array The array to insert the value into.
 * @param nodes The pre-allocated nodes for the linked list.
 * @param head Pointer to the head of the linked list.
 * @param position Position in the array to insert the value.
 * @param value Value to insert.
 * @return True if the value was inserted successfully, otherwise false.
******************************************************************************/
bool insertValue(uint8_t array[], Node nodes[], Node** head, uint8_t position, uint8_t value);

/*******************************************************************************
 * @brief Deletes a value from the array and the linked list.
 * @param array The array to delete the value from.
 * @param nodes The pre-allocated nodes for the linked list.
 * @param head Pointer to the head of the linked list.
 * @param value Value to delete.
 * @return True if the value was deleted successfully, otherwise false.
******************************************************************************/
bool deleteValue(uint8_t array[], Node nodes[], Node** head, uint8_t value);

/*******************************************************************************
 * @brief Prints the values of the elements in the array.
 * @param array The array whose values are to be printed.
 * @param head The head of the linked list.
 * @param sorted If true, print the values sorted, otherwise print unsorted.
******************************************************************************/
void printArray2(const uint8_t array[], const Node* head, bool sorted);

#endif // __ARRAY_MANAGEMENT__
