#include <string.h>
#include "queueArray.h"

void InitQueue(QUEUE* q)
{
	memset(q, 0, sizeof(QUEUE));
	q->index = -1;
}

/*
*
*  return number of character was enqueued to queue->data
* 
*/
uint8_t Enqueue(QUEUE* q, char* data)
{
	int ret = 0;
	uint8_t maxSize = QUEUE_MAX_SIZE - 1;
	if (q->index >= maxSize)
	{
		return 0;
	}
	else
	{
		q->index++;
		memset(q->data[q->index], 0, DATA_MAX_LEN);
		memcpy(q->data[q->index], data, strlen(data));
		ret = strlen(q->data[q->index]);
	}
	return ret;
}

/*
*
*  return number of character was dequeued from queue->data
*
*/
uint8_t Dequeue(QUEUE* q, char* data)
{
	int ret = 0;
	if (q->index < 0)
	{
		memset(data, 0, DATA_MAX_LEN);
		return 0;
	}
	else
	{
		//reset data buffer to NULL
		memset(data, 0, DATA_MAX_LEN);
		//copy the first element to data string
		memcpy(data, q->data[0], strlen(q->data[0]));
		//clear the first element
		memset(q->data[0], 0, DATA_MAX_LEN);
		ret = strlen(data);
		//move all element remain to head of array
		for (int i = 0; i < q->index; i++)
		{
			memcpy(q->data[i], q->data[i + 1], strlen(q->data[i + 1]));
			memset(q->data[i + 1], 0, DATA_MAX_LEN);
		}
		q->index--;
	}
	return (ret);
}
