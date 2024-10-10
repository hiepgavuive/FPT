#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "srecord.h"


#define joinTwoNumbers(A, B) (A << 4) | B

#define joinFourNumbers(A, B, C, D) (A << 12) | (B << 8) | (C << 4) | D


static int8_t hexToDec(int8_t temp);


static int8_t hexToDec(int8_t temp)
{
    if (temp >= '0' && temp <= '9')
    {
        temp = temp - '0';
    }
    else
    {
        temp = temp - 'A' + 10;
    }
    return temp;
}


void readLine(char* line, FILE* file, uint8_t* length)
{
    int ch;
    uint8_t index = 0;

    *length = 0;
    ch = fgetc(file);

    while (ch != '\n' && ch != EOF && index < MAX_LINE_LENGTH - 1)
    {
        line[index] = (char)ch;
        index++;
        (*length)++;
        ch = fgetc(file);
    }

    line[index] = '\0';

    if (ch == EOF && index == 0) {
        *length = 0;  // No characters read and EOF reached
    }
}

enum SRecordErrors checkS(uint8_t temp)
{
    enum SRecordErrors flag = NOT_ERROR;

    if (temp != 'S')
    {
        flag = ERROR_S;
    }

    return flag;
}


enum SRecordErrors checkType(uint8_t type)
{
    int8_t flag = NOT_ERROR;
    uint8_t index = 0;

    if (type < '0' || type > '9')
    {
        flag = ERROR_TYPE;
    }

    return flag;
}


enum SRecordErrors checkHex(int8_t* temp)
{
    int8_t flag = NOT_ERROR;

    ++temp;

    while (*temp != 0)
    {
        if (hexToDec(*temp) < 0 || hexToDec(*temp) > 15)
        {
            flag = ERROR_HEX;
        }
        ++temp;
    }

    return flag;
}


enum SRecordErrors checkByteCount(int8_t* str)
{
    int8_t flag = NOT_ERROR;
    uint8_t length = 0;
    uint8_t bytes = 0;

    length = strlen(str) - 4;
    bytes = joinTwoNumbers(hexToDec(str[2]), hexToDec(str[3]));

    if (bytes != (length / 2))
    {
        flag = ERROR_BYTE_COUNT;
    }

    return flag;
}


enum SRecordErrors checkSum(int8_t* str)
{
    int8_t flag = NOT_ERROR;
    uint8_t sum = 0;
    uint8_t index = 0;
    uint8_t lastByte = 0;
    uint8_t length = 0;

    length = strlen(str);
    lastByte = ~(joinTwoNumbers(hexToDec(str[length - 2]), hexToDec(str[length - 1])));

    for (index = 2; index < length - 2; index += 2)
    {
        sum += joinTwoNumbers(hexToDec(str[index]), hexToDec(str[index + 1]));
    }
    if (sum != lastByte)
    {
        flag = ERROR_SUM;
    }

    return flag;
}


enum SRecordErrors checkLineCount(int8_t* str, uint16_t count)
{
    int8_t flag = NOT_ERROR;
    uint16_t checkLine = 0;

    checkLine = joinFourNumbers(hexToDec(str[4]), hexToDec(str[5]), hexToDec(str[6]), hexToDec(str[7]));
    if (count != checkLine)
    {
        flag = ERROR_LINE_COUNT;
    }

    return flag;
}


enum SRecordErrors checkTerminate(int8_t temp, uint8_t type)
{
    int8_t flag = NOT_ERROR;

    if (temp != type)
    {
        flag = ERROR_TERMINATE;
    }

    return flag;
}


void printData(int8_t* str, uint8_t length, FILE* file, int8_t type)
{
    uint8_t index = 0;

    if (type == '3')
    {
        for (index = 4; index < length - 2; index++)
        {
            if (index == 12)
            {
                fprintf(file, "  ");
                printf("  ");
            }

            fprintf(file, "%c", str[index]);
        }
    }
    else if (type == '2')
    {
        for (index = 4; index < length - 2; index++)
        {
            if (index == 10)
            {
                fprintf(file, " ");
                printf(" ");
            }

            fprintf(file, "%c", str[index]);
        }
    }
    else if (type == '1')
    {
        for (index = 4; index < length - 2; index++)
        {
            if (index == 8)
            {
                fprintf(file, " ");
                printf(" ");
            }

            fprintf(file, "%c", str[index]);
        }
    }
}

/*******************************************************************************
* End of file
******************************************************************************/