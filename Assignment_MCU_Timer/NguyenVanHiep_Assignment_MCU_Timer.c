#include "MKE16Z4.h"
/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */
#define GREEN_LED_PIN        (4)
#define BLUE_LED_PIN         (1)

/*
 * Variables
 */
uint32_t greenLedCounter = 0;
uint32_t blueLedCounter = 0;
uint8_t ledToggleFlag = 0;
uint8_t pitInterruptFlag = 0;

/*
 * @brief Function to initialize Green LED and Blue LED
 */
void InitializeLeds()
{
    /* Enable Clock */
    PCC->CLKCFG[PCC_PORTB_INDEX] |= (1U << 30);
    PCC->CLKCFG[PCC_PORTD_INDEX] |= (1U << 30);

    /* Configure MUX for PORT B & D */
    PORTB->PCR[GREEN_LED_PIN] |= (1U << 8);
    PORTD->PCR[BLUE_LED_PIN] |= (1U << 8);

    /* Set direction as output for LED green & blue */
    FGPIOB->PDDR |= (1U << GREEN_LED_PIN);
    FGPIOD->PDDR |= (1U << BLUE_LED_PIN);

    /* Turn off both LEDs initially */
    FGPIOB->PDOR |= (1U << GREEN_LED_PIN);
    FGPIOD->PDOR |= (1U << BLUE_LED_PIN);
}

/*
 * @brief Function to initialize the PIT
 */
void InitializePIT()
{
    /* Enable Clock for PIT */
    PCC->CLKCFG[PCC_LPIT0_INDEX] |= (0b011U << 24);
    PCC->CLKCFG[PCC_LPIT0_INDEX] |= (1U << 30);
    SCG->FIRCDIV |= (1U << 8);  /* DIV2 = 1 <=> Clock / 1 */

    /* Configure PIT */
    LPIT0->MCR = (1U << 0);  /* Enable Protocol clock to timers */

    /* Configure Timer Channel 0 as 32-bit periodic counter */
    LPIT0->CHANNEL[0].TCTRL = LPIT_TCTRL_MODE(0);  /* 0 << 2 -- 32-bit Periodic Counter */
    LPIT0->CHANNEL[0].TVAL = (DEFAULT_SYSTEM_CLOCK / 20U) - 1U;  /* 50ms */

    /* Enable interrupt for Timer Channel 0 */
    LPIT0->MIER |= (1U << 0);
    NVIC_EnableIRQ(LPIT0_IRQn);
    LPIT0->SETTEN |= (1U << 0);  /* Start Timer Channel 0 */
}

/*
 * @brief Function to turn on the Green LED
 */
void TurnOnGreenLED()
{
    FGPIOB->PDOR &= ~(1U << GREEN_LED_PIN);
    FGPIOD->PDOR |= (1U << BLUE_LED_PIN);
}

/*
 * @brief Function to turn on the Blue LED
 */
void TurnOnBlueLED()
{
    FGPIOD->PDOR &= ~(1U << BLUE_LED_PIN);
    FGPIOB->PDOR |= (1U << GREEN_LED_PIN);
}

/**
 * @brief Interrupt handling for LPI0
 */
void LPIT0_IRQHandler()
{
    LPIT0->MSR |= (1U << 0);  /* Clear the interrupt flag for Timer Channel 0 */
    pitInterruptFlag = 1;  /* Set the flag to indicate that the interrupt has occurred */
}

/*
 * @brief Function to control LED flashing:
 * Green LED on for 1 second and Blue LED on for 2.35 seconds
 */
void LEDControlHandler()
{
    if(ledToggleFlag == 0)  /* If flag = 0 -> Green LED on for 1s (50ms timer -> 20 times) */
    {
        TurnOnGreenLED();
        greenLedCounter++;
        if(greenLedCounter == 20U)  /* Run for 20 cycles (1 second) */
        {
            ledToggleFlag = 1;  /* Toggle flag to switch to Blue LED */
            greenLedCounter = 0;
        }
    }
    else  /* If flag = 1 -> Blue LED on for 2.35s (50ms timer -> 47 times) */
    {
        TurnOnBlueLED();
        blueLedCounter++;
        if(blueLedCounter == 47U)  /* Run for 47 cycles (2.35 seconds) */
        {
            ledToggleFlag = 0;  /* Toggle flag to switch to Green LED */
            blueLedCounter = 0;
        }
    }
}

/*
 * @brief   Application entry point.
 */
int main(void)
{
    InitializeLeds();
    InitializePIT();

    while(1)
    {
        if(pitInterruptFlag == 1)
        {
            pitInterruptFlag = 0;
            LEDControlHandler();
        }
        else
        {
            /* Do nothing */
        }
    }
}
