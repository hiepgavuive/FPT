
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MKE16Z4.h"
#include "board.h"
#include "queueArray.h"
#include "srecordParse.h"
#include "Flash.h"

/*******************************************************************************
 * Defines
 ******************************************************************************/
#define BAUDRATE_UART0 (9600U)
#define LED_GREEN      (4U)
#define OS_TEXT        (WINDOWS)

char g_rxBuff[DATA_MAX_LEN];
bool g_isLineEnding = false;
uint8_t flashBuffer[DATA_MAX_LEN]; /* Buffer for flash data */


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Initialize UART0 with specific settings
 */
void InitUart0()
{
    uint8_t OSR = 15;
    uint32_t clkHz = 0;
    uint16_t SBR = 0;

    /* get clk */
    clkHz = CLOCK_GetFreq(kCLOCK_ScgFircClk);
    SBR = clkHz / BAUDRATE_UART0 / (OSR+1);

    /* enable clock */
    PCC->CLKCFG[PCC_LPUART0_INDEX] = (1<<30) | (3<<24); /* clock FIRC 48Mhz */
    PCC->CLKCFG[PCC_PORTB_INDEX] = (1<<30);
    /* config PTB0, PTB1 are UART's pinout */
    PORTB->PCR[0] = (0x2<<8);
    PORTB->PCR[1] = (0x2<<8);

    /* numBit Data (8), stop bit=1, parity=None */
    LPUART0->BAUD = (OSR << 24) | (SBR<<0);
    /* enable Tx and Rx and interrupt */
    LPUART0->CTRL = (1<<21) | (1<<19) | (1<<18);

    /* enable interrupt */
    EnableIRQ(LPUART0_IRQn);
}

/*!
 * @brief Initialize LED
 */
void InitLED()
{
    PCC->CLKCFG[PCC_PORTB_INDEX] |= 1 << 30;
    PORTB->PCR[LED_GREEN] |= 1 << 8;
    GPIOB->PDDR |= 1<<LED_GREEN;
    GPIOB->PDOR |= 1<<LED_GREEN;
    GPIOB->PSOR |= 1<<LED_GREEN;
}

/*!
 * @brief Send a string through UART0
 * @param str: String to be sent
 */
void Uart0SentString(char* str)
{
    while(*str != *(char*)NULL)
    {
        /* check idle Tx and Rx */
        while(!(LPUART0->STAT & (1<<23)) && !(LPUART0->STAT & (1<<24)))
        {
            /* Wait */
        }
        LPUART0->DATA = *str;
        str++;
        while(!(LPUART0->STAT & (1<<22)))
        {
            /* Wait */
        }
    }
}

/*!
 * @brief UART0 interrupt handler
 */
void LPUART0_IRQHandler()
{
    static uint8_t count = 0;
    uint8_t tmpData = 0;

    if(LPUART0->STAT & (1<<21))	/* flag full data buffer */
    {
        tmpData = LPUART0->DATA & 0xFF;
#if (OS == WINDOWS)
        if(tmpData == '\r')
        {
            g_rxBuff[count] = 0; /* The null character */
            g_isLineEnding = true;
            count = 0;
        }
        else if(tmpData != '\n')
        {
            if (count < DATA_MAX_LEN - 1)
            {
                g_rxBuff[count] = tmpData;
                count++;
            }
        }
    }
#endif /* OS */
}

/*!
 * @brief Flash data to memory with error handling
 * @param srecData: SREC data structure
 */
void FlashDataToMemory(SREC *srecData)
{
    const int maxRetries = 3;
    int attempts = 0;
    bool success = false;

    /* Check if data is different before writing */
    Read_FlashAddress(srecData->address);
    memcpy(flashBuffer, srecData->data, srecData->byteCount);
    if (memcmp((void *)srecData->address, flashBuffer, srecData->byteCount) != 0)
    {
        /* Try to erase and program with retries */
        while (attempts < maxRetries && !success)
        {
            if (Erase_Sector(srecData->address) == 1)
            {
                if (Program_LongWord_8B(srecData->address, flashBuffer) == 1)
                {
                    success = true;
                }
                else
                {
                    Uart0SentString("[ERROR] Program LongWord Failed\r\n");
                }
            }
            else
            {
                Uart0SentString("[ERROR] Erase Sector Failed\r\n");
            }
            attempts++;
        }

        if (!success)
        {
            Uart0SentString("[ERROR] Flash operation failed after retries\r\n");
            GPIOB->PCOR |= 1<<LED_GREEN; /* Turn off LED to indicate error */
        }
        else
        {
            GPIOB->PSOR |= 1<<LED_GREEN; /* Turn on LED to indicate success */
        }
    }
}

/*!
 * @brief Main function
 * @return 0 if successful
 */
int main(void)
{
    char strDebug[128] = {0, };
    uint32_t lineCounter = 0;
    QUEUE* q = NULL;
    SREC* arrSrecData = NULL;
    ERR_CODE retParse = OK;

    /* init KE16Z4 */
    BOARD_InitBootClocks();
    InitUart0();
    InitLED();

    /* init somethings */
    q = calloc(1, sizeof(QUEUE));
    if (q == NULL)
    {
        Uart0SentString("[ERROR] Memory Allocation Failed\r\n");
        return -1;
    }
    arrSrecData = calloc(1, sizeof(SREC));
    if (arrSrecData == NULL)
    {
        free(q);
        Uart0SentString("[ERROR] Memory Allocation Failed\r\n");
        return -1;
    }
    InitQueue(q);

    /* sent hello message */
    Uart0SentString("ABCDEFGH\r\n");

    while(1)
    {
        /* if detected EOL from uart buffer */
        /* just enqueue the line text data */
        if(g_isLineEnding)
        {
            if(!Enqueue(q, g_rxBuff))
            {
                Uart0SentString("[ERROR] Enqueue Buffer S-rec\r\n");
            }
            memset(g_rxBuff, 0, sizeof(g_rxBuff)/sizeof(uint8_t));
            g_isLineEnding = false;
        }
        /* process parse s-rec and write to flash */
        else if(g_isLineEnding == false && q->index>=0)
        {
            char strSrecRawData[128] = {0, };
            if(Dequeue(q, strSrecRawData))
            {
                lineCounter++;

                /* start parse s-record line text data */
                retParse = SRecordParser(arrSrecData, strSrecRawData);
                memset(strDebug, 0, 128);

                if(retParse == OK)
                {
                    FlashDataToMemory(arrSrecData);
                    sprintf(strDebug, "%d\r\n", lineCounter);
                    Uart0SentString(strDebug);
                }
                else
                {
                    sprintf(strDebug, "[DEBUG] Line %d: , err: %s, %s\r\n", lineCounter, GetError(retParse), strSrecRawData);
                    Uart0SentString(strDebug);
                }
            }
            else
            {
                Uart0SentString("[ERROR] Dequeue Buffer S-rec\r\n");
            }
        }
    }

    free(q);
    free(arrSrecData);

    return 0;
}
