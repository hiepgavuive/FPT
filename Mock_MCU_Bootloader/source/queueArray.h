#ifndef _QUEUE_ARRAY_H_
#define _QUEUE_ARRAY_H_

#include <stdint.h>

#define QUEUE_MAX_SIZE    (4U)
#define DATA_MAX_LEN      (91U)

typedef struct t_QUEUE
{
	char data[QUEUE_MAX_SIZE][DATA_MAX_LEN];
	int index;
}QUEUE;

void    InitQueue(QUEUE* q);
uint8_t Enqueue(QUEUE* q, char* data);
uint8_t Dequeue(QUEUE* q, char* data);

#endif /* _QUEUE_ARRAY_H_ */
