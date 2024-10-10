#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "srecord.h"


int main(int argc, char* argv[])
{
    uint8_t length = 0;
    uint8_t count = 0;
    uint16_t index = 0;
    uint16_t lineCount = 1;
    int8_t type = 0;
    int8_t line[MAX] = "";
    int8_t endType = 0;

    FILE* inputFile = NULL;
    FILE* outputFile = NULL;

    if (argc != 3)
    {
        printf("Error: main.exe <Srec.txt> <Output.txt>");
    }
    
    inputFile = fopen(argv[1], "r");
    outputFile = fopen(argv[2], "w+");

    if (inputFile == NULL)
    {
        printf("\nError: File input is not exist!");
    }
    else
    {
        while (!feof(inputFile))
        {
            if (fgetc(inputFile) == '\n')
            {
                lineCount++;
            }
        }
    }

    fseek(inputFile, 0, SEEK_SET);
    /* If there is not any error, check error and convert input srecord file */ 
    for (index = 1; index < lineCount; index++)
    {
        fprintf(outputFile, "%d ", index);
        readLine(line, inputFile, &length);

        if (line[1] == '1' || line[1] == '2' || line[1] == '3' || line[1] == '0')
        {
            if (line[1] == '1')
            {
                type = '1';
                endType = '9';
            }
            else if (line[1] == '2')
            {
                type = '2';
                endType = '8';
            }
            else if (line[1] = '3')
            {
                type = '3';
                endType = '7';
            }
            count++;
        }

        /* Check start with S */ 
        if (checkS(line[0]) == NOT_ERROR)
        {
            /* Check type of record */ 
            if (checkType(line[1]) == NOT_ERROR)
            {
                /* Check if all characters are hex base */
                if (checkHex(line) == NOT_ERROR)
                {
                    /* Check byte count */ 
                    if (checkByteCount(line) == NOT_ERROR)
                    {
                        /* Check checksum */ 
                        if (checkSum(line) == NOT_ERROR)
                        {
                            if (type == 5 || type == 6)
                            {
                                /* Check line count */ 
                                if (checkLineCount(line, count) == NOT_ERROR)
                                {
                                    printData(line, length, outputFile, type);
                                }
                                else 
                                {
                                    fprintf(outputFile, "\t Error: Line count invalid!");
                                    printf("\n%d ", index);
                                    printf("\t Error: Line count invalid!");
                                    return -1;
                                }
                            }                       
                            else if (index == lineCount)
                            {
                                /* Check termination */ 
                                if (checkTerminate(line[1], endType) == NOT_ERROR)
                                {
                                    printData(line, length, outputFile, type);
                                    
                                }
                                else 
                                {
                                    fprintf(outputFile, "\t Error: Terminated!");
                                    printf("\n%d ", index);
                                    printf("\t Error: Terminated!");
                                    return -1;
                                }
                            }
                            else 
                            {
                                printData(line, length, outputFile, type);
                            }
                        }
                        else
                        {
                            fprintf(outputFile, "\t Error: Checksum!");
                            printf("\n%d ", index);
                            printf("\t Error: Checksum!");
                            return -1;
                        }
                    }
                    else
                    {
                        fprintf(outputFile, "\t Error: Byte count invalid!");
                        printf("\n%d ", index);
                        printf("\t Error: Byte count invalid!");
                        return -1;
                    }
                }
                else
                {
                    fprintf(outputFile, "\t Error: Hex base invalid!");
                    printf("\n%d ", index);
                    printf("\t Error: Hex base invalid!");
                    return -1;
                }
            }
            else
            {
                fprintf(outputFile, "\t Error: Record type invalid!");
                printf("\n%d ", index);
                printf("\t Error: Record type invalid!");
                return -1;
            }
        }
        else
        {
            fprintf(outputFile, "\t Error: Not start with S!");
            printf("\n%d ", index);
            printf("\t Error: Not start with S!");
            return -1;
        }
        fprintf(outputFile, "\n");
    }

    /* Close file after modification */ 
    fclose(inputFile);
    fclose(outputFile);

    return 0;
}

/*******************************************************************************
* End of file
******************************************************************************/