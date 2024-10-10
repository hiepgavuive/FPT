#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "srecordParse.h"

STYPE getTypeSrec(char typeIn)
{
	switch (typeIn)
	{
	case 0:
		return S0;
	case 1:
		return S1;
	case 2:
		return S2;
	case 3:
		return S3;
	case 4:
		return S4;
	case 5:
		return S5;
	case 6:
		return S6;
	case 7:
		return S7;
	case 8:
		return S8;
	case 9:
		return S9;
	default:
		return UNKNOWN;
	}
}

char* GetError(ERR_CODE code)
{
	switch (code)
	{
		case OK:					//Every things is OK
		{
			return "OK";
		}
		case START_CHAR_INVALID:	//not start by 'S'
		{
			return "START_CHAR_INVALID";
		}
		case TYPE_INVALID:		//not are S0 - S9
		{
			return "TYPE_INVALID";
		}
		case BYTECOUNT_INVALID:	//real array length not enough
		{
			return "BYTECOUNT_INVALID";
		}
		case CHECKSUM_INVALID:	//checksum value not match
		{
			return "CHECKSUM_INVALID";
		}
		case CHAR_DATA_INVALID:   //the char in the file: S, 0-9, A-F
		{
			return "CHAR_DATA_INVALID";
		}
		default:
		{
			return "UNKNOWN";
		}
	}
}

uint8_t ConvertCharToInt8(char val0, char val1)
{
	char arrTmpValue[3] = { val0, val1, 0 };
	return ((uint8_t)strtol(arrTmpValue, NULL, 16));
}

ERR_CODE checkTypeID(SREC* srecData, const char* lineTxt)
{
	ERR_CODE ret = OK;
	ret = (lineTxt[0] == 'S') ? OK : START_CHAR_INVALID;
	return ret;
}

ERR_CODE getTypeID(SREC* srecData, const char* lineTxt)
{
	ERR_CODE ret = OK;
	srecData->type = getTypeSrec(lineTxt[1] - 48);
	ret = (srecData->type == UNKNOWN) ? TYPE_INVALID : OK;
	return ret;
}

ERR_CODE verifyTextCharacter(const char* lineTxt)
{
	ERR_CODE ret = OK;
	for (int i = 1; i < strlen(lineTxt); i++)
	{
		if ((lineTxt[i] < '0' || lineTxt[i] > '9')
			&& (lineTxt[i] < 'A' || lineTxt[i] > 'F'))
		{
			printf("CHAR_DATA_INVALID at pos = [%d], value = [%c]\n", i, lineTxt[i]);
			ret = CHAR_DATA_INVALID;
		}
	}
	return ret;
}

ERR_CODE getByteCount(SREC* srecData, const char* lineTxt)
{
	ERR_CODE ret = OK;
	srecData->byteCount = ConvertCharToInt8(lineTxt[2], lineTxt[3]);
	if ((srecData->byteCount > (strlen(lineTxt) - 4) / 2) || (srecData->byteCount < 3))
	{
		ret = BYTECOUNT_INVALID;
	}
	return ret;
}

ERR_CODE calChecksum(const char* dataLine)
{
	uint8_t tmpSum = 0;
	uint8_t checkSumComputed = 0;
	uint8_t checkSumInput = 0;
	char* tmpData = (char*)dataLine + 2;

	//checksum from dataLine
	checkSumInput = ConvertCharToInt8(dataLine[strlen(tmpData)], dataLine[strlen(tmpData) + 1]);

	//compute the checksum
	for (int i = 0; i < strlen(tmpData) - 2; i += 2)
	{
		tmpSum = tmpSum + ConvertCharToInt8(tmpData[i], tmpData[i + 1]);
	}
	checkSumComputed = 0xff - (0xff & tmpSum);

	return (checkSumComputed == checkSumInput) ? OK : CHECKSUM_INVALID;
}

void getAddress(SREC* srecData, const char* lineTxt)
{
	char strAddress[9] = { 0, };	//max is 32bit = 4 byte = 8 characters
	uint8_t addressWidth = 0;   	//number of text charactor
	addressWidth = (srecData->type == S1 ? 4 : (srecData->type == S2) ? 6 : 8);
	memcpy(strAddress, &lineTxt[4], addressWidth);
	srecData->address = strtol(strAddress, NULL, 16);
}

void getData(SREC* srecData, const char* lineTxt)
{
	uint8_t dataLen = 0;		//number of text charactor
	uint8_t addressWidth = 0;	//number of text charactor, max is 32bit = 4 bytes = 8 characters
	addressWidth = (srecData->type == S1 ? 4 : (srecData->type == S2) ? 6 : 8);
	dataLen = strlen(lineTxt) - 2 - 2 - addressWidth - 2;
	memcpy(&srecData->data, &lineTxt[2 + 2 + addressWidth], dataLen);
}

ERR_CODE SRecordParser(SREC* srecData, const char* lineTxt)
{
	ERR_CODE ret = OK;

	//check the all of error first
	// 1st. get type ID
	ret = checkTypeID(srecData, lineTxt);
	if (ret == OK)
	{
		ret = getTypeID(srecData, lineTxt);
		if (ret)
		{
			return ret;
		}
	}
	else
	{
		return ret;
	}

	// 2nd. Verify all of character in this line data
	ret = verifyTextCharacter(lineTxt);
	if (ret)
	{
		return ret;
	}
	//3rd. check byte count with real length
	ret = getByteCount(srecData, lineTxt);
	if (ret)
	{
		return ret;
	}
	//5th. check checksum
	ret = calChecksum(lineTxt);
	if (ret)
	{
		return ret;
	}

	//only get address and data if type are S1, S2, S3
	if (srecData->type != S1 && srecData->type != S2 && srecData->type != S3)
	{
		return ret;
	}

	//when everythins are correct
	//6th. get Address
	getAddress(srecData, lineTxt);
#ifdef DEBUG
	printf("[DEBUG] Address is %dbit, Value = [0x%08X]\n"
		, srecData->type == S1 ? 16 : ((srecData->type == S2) ? 24 : 32)
		, srecData->address);
#endif

	//7th get Data array
	getData(srecData, lineTxt);
#ifdef DEBUG
	printf("[DEBUG] Data: ");
	for (int i = 0; i < strlen((char*)srecData->data); i += 2)
	{
		printf(" %c%c", srecData->data[i], srecData->data[i + 1]);
	}
	printf("\n");
#endif

	return ret;
}
