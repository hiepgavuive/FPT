#ifndef _SRECORD_PARSE_H_
#define _SRECORD_PARSE_H_

#include <stdint.h>
#include <stdbool.h>

//#define DEBUG

typedef enum e_ERR_CODE
{
	OK = 0,
	START_CHAR_INVALID,	//not start by 'S'
	TYPE_INVALID,		//not are S0 - S9
	BYTECOUNT_INVALID,	//real array length not enough
	CHECKSUM_INVALID,	//checksum value not match
	CHAR_DATA_INVALID   //the char in the file: S, 0-9, A-F
} ERR_CODE;

typedef enum e_STYPE
{
	S0 = 0,
	S1,
	S2,
	S3,
	S4,
	S5,
	S6,
	S7,
	S8,
	S9,
	UNKNOWN
}STYPE;

typedef struct t_SREC
{
	STYPE type;
	uint8_t byteCount;
	uint32_t address;
	uint8_t data[91];
}SREC;

/*
*   API
*/
char* GetError(ERR_CODE code);

ERR_CODE SRecordParser(SREC* srecData, const char* lineTxt);

#endif /* _SRECORD_PARSE_H_ */
