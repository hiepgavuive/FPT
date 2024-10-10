#include "arrayManagement.h"

/*******************************************************************************
 * @brief Inserts a value into the specified position of the array and into the sorted linked list.
 * @param array The array to insert the value into.
 * @param nodes The pre-allocated nodes for the linked list.
 * @param head Pointer to the head of the linked list.
 * @param position Position in the array to insert the value.
 * @param value Value to insert.
 * @return True if the value was inserted successfully, otherwise false.
******************************************************************************/
bool insertValue(uint8_t array[], Node nodes[], Node** head, uint8_t position, uint8_t value)
{
    if (position >= 20) 
    {
        printf("Error: Position out of range. Valid range is 0 to 19.\n");
        return false;
    }
    if (array[position] != 0xFF) 
    {
        printf("Error: Position %d already filled.\n", position);
        return false;
    }
    if (value > 100) 
    {
        printf("Error: Value out of range. Valid range is 0 to 100.\n");
        return false;
    }
    /* Check if value already exists in array */
    for (int i = 0; i < 20; i++) 
    {
        if (array[i] == value) 
        {
            printf("Error: Value %d already exists in the array.\n", value);
            return false;
        }
    }

    array[position] = value;

    /* Insert value in sorted order in the linked list */
    Node* newNode = NULL;
    for (int i = 0; i < 20; i++) 
    {
        if (nodes[i].value == 0xFF) 
        {
            newNode = &nodes[i];
            newNode->value = value;
            newNode->next = NULL;
            break;
        }
    }
    if (newNode == NULL) 
    {
        printf("No available node in the static list.\n");
        return false;
    }

    if (*head == NULL || (*head)->value > value) 
    {
        newNode->next = *head;
        *head = newNode;
    }
    else 
    {
        Node* current = *head;
        while (current->next != NULL && current->next->value < value) 
        {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }
    return true;
}

/*******************************************************************************
 * @brief Deletes a value from the array and the linked list.
 * @param array The array to delete the value from.
 * @param nodes The pre-allocated nodes for the linked list.
 * @param head Pointer to the head of the linked list.
 * @param value Value to delete.
 * @return True if the value was deleted successfully, otherwise false.
******************************************************************************/
bool deleteValue(uint8_t array[], Node nodes[], Node** head, uint8_t value)
{
    if (value > 100) 
    {
        printf("Error: Value out of range. Valid range is 0 to 100.\n");
        return false;
    }
    /* Find value in array */
    int position = -1;
    for (int i = 0; i < 20; i++) 
    {
        if (array[i] == value) 
        {
            position = i;
            break;
        }
    }
    if (position == -1) 
    {
        printf("Error: Value %d does not exist in the array.\n", value);
        return false;
    }
    array[position] = 0xFF;

    /* Remove value from linked list */
    Node* current = *head;
    Node* previous = NULL;

    while (current != NULL && current->value != value) 
    {
        previous = current;
        current = current->next;
    }
    if (current == NULL) 
    {
        printf("Error: Value %d does not exist in the linked list.\n", value);
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
    current->value = 0xFF; 
    current->next = NULL;
    return true;
}

/*******************************************************************************
 * @brief Prints the values of the elements in the array.
 * @param array The array whose values are to be printed.
 * @param head The head of the linked list.
 * @param sorted If true, print the values sorted, otherwise print unsorted.
******************************************************************************/
void printArray2(const uint8_t array[], const Node* head, bool sorted)
{
    if (sorted) 
    {
        printf("Values in the array (sorted):\n");
        const Node* current = head;
        while (current != NULL) 
        {
            printf("%d ", current->value);
            current = current->next;
        }
    }
    else 
    {
        printf("Values in the array (unsorted):\n");
        for (int i = 0; i < 20; i++) 
        {
            if (array[i] != 0xFF) 
            {
                printf("%d ", array[i]);
            }
        }
    }
    printf("\n");
}