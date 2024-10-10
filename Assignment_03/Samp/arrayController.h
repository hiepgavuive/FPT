#ifndef __ARRAY_CONTROLLER_H__
#define __ARRAY_CONTROLLER_H__

#include <stdint.h>

/*******************************************************************************
* API
******************************************************************************/
void clearArray(uint8_t array[], int size);
void printArray(uint8_t array[], int size);
void insertElement(uint8_t array[], int size, int position, uint8_t value);
void deleteElement(uint8_t array[], int size, int position);
void sortArrayAscending(uint8_t array[], int size);
void sortArrayDescending(uint8_t array[], int size);
int searchElement(uint8_t array[], int size, uint8_t value);
uint8_t findMax(uint8_t array[], int size);
uint8_t findMin(uint8_t array[], int size);

#endif /* __ARRAY_CONTROLLER_H__ */
