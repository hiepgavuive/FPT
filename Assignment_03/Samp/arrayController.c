#include "arrayController.h"
#include <stdio.h>
#include <limits.h>

/*******************************************************************************
* Prototypes
******************************************************************************/

uint8_t random(int minN, int maxN)
{
    return minN + rand() % (maxN + 1 - minN);
}

void setUp(uint8_t array[], int size, int minN, int maxN)
{
    srand((int)time(0));
    for (int i = 0; i < size; ++i)
    {
        array[i] = random(minN, maxN);
    }
}

void clearArray(uint8_t array[], int size) 
{
    for (int i = 0; i < size; ++i) 
    {
        array[i] = 255; /* 255 represents an empty value */
    }
}

void printArray(uint8_t array[], int size) 
{
    int isEmpty = 1;
    for (int i = 0; i < size; ++i) 
    {
        if (array[i] != 255) 
        {
            printf("%d ", array[i]);
            isEmpty = 0;
        }
    }
    if (isEmpty) 
    {
        printf("The array is empty.");
    }
    printf("\n");
}

void insertElement(uint8_t array[], int size, int position, uint8_t value) 
{
    if (position >= 0 && position < size) 
    {
        if (array[position] == 255) 
        {
            array[position] = value;
        }
        else 
        {
            printf("Position %d already contains a value.\n", position);
        }
    }
    else 
    {
        printf("Invalid position.\n");
    }
}

void deleteElement(uint8_t array[], int size, int position) 
{
    if (position >= 0 && position < size) 
    {
        if (array[position] != 255) 
        {
            array[position] = 255;
        }
        else 
        {
            printf("No element at position %d.\n", position);
        }
    }
    else 
    {
        printf("Invalid position.\n");
    }
}

void sortArrayAscending(uint8_t array[], int size) 
{
    for (int i = 0; i < size - 1; ++i) 
    {
        for (int j = i + 1; j < size; ++j) 
        {
            if (array[i] > array[j] && array[i] != 255 && array[j] != 255) 
            {
                uint8_t temp = array[i];
                array[i] = array[j];
                array[j] = temp;
            }
        }
    }
    printArray(array, size);
}

void sortArrayDescending(uint8_t array[], int size) 
{
    for (int i = 0; i < size - 1; ++i) 
    {
        for (int j = i + 1; j < size; ++j) 
        {
            if (array[i] < array[j] && array[i] != 255 && array[j] != 255) 
            {
                uint8_t temp = array[i];
                array[i] = array[j];
                array[j] = temp;
            }
        }
    }
    printArray(array, size);
}

int searchElement(uint8_t array[], int size, uint8_t value) 
{
    for (int i = 0; i < size; ++i) 
    {
        if (array[i] == value) 
        {
            return i;
        }
    }
    return -1;
}

uint8_t findMax(uint8_t array[], int size) 
{
    uint8_t max = 0;
    for (int i = 0; i < size; ++i) 
    {
        if (array[i] != 255 && array[i] > max) 
        {
            max = array[i];
        }
    }
    return max;
}

uint8_t findMin(uint8_t array[], int size) 
{
    uint8_t min = 100;
    for (int i = 0; i < size; ++i) 
    {
        if (array[i] != 255 && array[i] < min)
        {
            min = array[i];
        }
    }
    return min;
}
