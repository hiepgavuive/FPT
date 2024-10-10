#include "MKE16Z4.h"
#include "board.h"

#define BAUDRATE_UART0 (9600U)   /* Define the baud rate for UART0 communication */
#define UART_TX_PIN    (0U)      /* Define PTB0 as UART0 TX pin */
#define UART_RX_PIN    (1U)      /* Define PTB1 as UART0 RX pin */
#define LED_PIN        (4U)      /* Define PTB4 as LED control pin */

/**
 * @brief   Initialize UART0 module
 */
void InitUart0() {
    uint8_t OSR = 15;               /* Set oversampling ratio to 15 */
    uint32_t clkHz = 0;             /* Variable to store clock frequency */
    uint16_t SBR = 0;               /* Variable to store baud rate divisor */

    /* Get the clock frequency for UART0 */
    clkHz = CLOCK_GetFreq(kCLOCK_ScgFircClk);
    SBR = clkHz / BAUDRATE_UART0 / (OSR + 1);

    /* Enable clocks for LPUART0 and PORTB */
    PCC->CLKCFG[PCC_LPUART0_INDEX] = (1 << 30) | (3 << 24);  /* Enable LPUART0 clock */
    PCC->CLKCFG[PCC_PORTB_INDEX] = (1 << 30);                /* Enable PORTB clock */

    /* Configure PTB0 and PTB1 as UART pins */
    PORTB->PCR[UART_TX_PIN] = (0x2 << 8);  /* Set PTB0 as UART0 TX */
    PORTB->PCR[UART_RX_PIN] = (0x2 << 8);  /* Set PTB1 as UART0 RX */

    /* Configure UART0: 8 data bits, 1 stop bit, no parity */
    LPUART0->BAUD = (OSR << 24) | (SBR << 0);
    /* Enable transmitter and receiver */
    LPUART0->CTRL = (1 << 18) | (1 << 19);

    /* Enable receiver interrupt */
    LPUART0->CTRL |= (1 << 21);
    NVIC_EnableIRQ(LPUART0_IRQn);   /* Enable UART0 interrupt in NVIC */
}

/**
 * @brief   Send a string over UART0
 * @param str  Pointer to the string to be sent
 */
void Uart0SentString(const char* str) {
    while (*str != '\0') {          /* Loop until the end of the string */
        while (!(LPUART0->STAT & (1 << 23))) {
            /* Wait until the transmit buffer is empty */
        }
        LPUART0->DATA = *str++;     /* Send the current character and move to the next */
    }
}

/**
 * @brief   Receive a string over UART0
 * @param buffer  Pointer to the buffer where received data will be stored
 * @param length  Maximum length of the string to be received
 */
void Uart0ReceiveString(char* buffer, uint32_t length) {
    uint32_t index = 0;             /* Index to track the position in the buffer */

    while (index < length - 1) {    /* Loop until the buffer is full */
        while (!(LPUART0->STAT & (1 << 21))) {
            /* Wait until there is data in the receive buffer */
        }
        buffer[index++] = LPUART0->DATA;  /* Store the received data in the buffer */
    }

    buffer[index] = '\0';           /* Null-terminate the string */
}

/**
 * @brief   Initialize the LED pin
 */
void InitLED() {
    /* Enable clock for PORTB */
    PCC->CLKCFG[PCC_PORTB_INDEX] |= 1 << 30;

    /* Set PTB4 as GPIO and output */
    PORTB->PCR[LED_PIN] |= 1 << 8;
    GPIOB->PDDR |= 1 << LED_PIN;
    GPIOB->PDOR |= 1 << LED_PIN;  /* Turn off LED initially */
}

/**
 * @brief   Toggle the state of the LED
 */
void ToggleLED() {
    GPIOB->PTOR |= 1 << LED_PIN;  /* Toggle the state of the LED */
}

/**
 * @brief   UART0 Interrupt Handler
 */
void LPUART0_IRQHandler() {
    if (LPUART0->STAT & (1 << 21)) {  /* Check if receive buffer is full */
        char receivedData = LPUART0->DATA;

        if (receivedData == '1') {
            GPIOB->PCOR = 1 << LED_PIN;  /* Turn on LED if '1' is received */
        } else if (receivedData == '0') {
            GPIOB->PSOR = 1 << LED_PIN;  /* Turn off LED if '0' is received */
        }
    }
}

/**
 * @brief   Main function
 */
int main(void) {
    BOARD_InitBootClocks();   /* Initialize system clocks */
    InitUart0();              /* Initialize UART0 */
    InitLED();                /* Initialize the LED */

    /* Send an initial message to the computer */
    Uart0SentString("Hello from MKE16Z!\n");

    while (1) {
        Uart0SentString("Hello from MKE16Z!\n");  /* Send message every second */
        for (volatile uint32_t i = 0; i < 2400000; i++) {
            /* Delay for approximately 1 second (assuming 24MHz clock) */
        }
    }

    return 0;
}
