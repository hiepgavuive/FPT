#include "MKE16Z4.h"

/* User Config */
#define LED_GREEN                  (4U)
#define LED_RED                    (5U)
#define LED_BLUE                   (1U)
#define SW2                        (3U)
#define SW3                        (2U)
#define SYSTICK_TIME_INTERRUPT     (0.0001) /* secs */
#define PWM_CYCLE                  (0.004)  /* secs */
#define PWM_MAX_DUTY               (50.0)   /* percent */
#define PWM_MIN_DUTY               (3.0)    /* percent */
#define PWM_TIME_OPERATION         (1.0)    /* secs */

#define PWM_CYCLE_BY_SYSTICK       (PWM_CYCLE / SYSTICK_TIME_INTERRUPT) /* secs */
#define PWM_DUTY_BY_SYSTICK(duty)  ((duty) * PWM_CYCLE_BY_SYSTICK / 100.0) /* number of SysTick interrupts to blink LED */
#define PWM_DUTY_PAYLOAD           (PWM_MAX_DUTY-PWM_MIN_DUTY)
#define PWM_STEP                   ((SYSTICK_TIME_INTERRUPT*1000) / (PWM_TIME_OPERATION*1000/PWM_DUTY_PAYLOAD))

/* Define state machine modes */
typedef enum
{
    MODE1 = 0,
    MODE2,
    MODE3,
    MAX_MODE
} APP_MODE;

APP_MODE currentMode = MODE1;

/* PWM direction */
typedef enum
{
    INCREASE = 0,
    DECREASE,
} DIRECTION_PWM;

void InitLED()
{
	/* GREEN */
    PCC->CLKCFG[PCC_PORTB_INDEX] |= 1 << 30;  /* Enable clock for GPIOB */
    PORTB->PCR[LED_GREEN] |= 1 << 8;  /* Enable GPIO */
    GPIOB->PDDR |= 1 << LED_GREEN;  /* Set direction as output */
    GPIOB->PDOR |= 1 << LED_GREEN;  /* Set output to 1 (turn off LED) */

    /* RED */
    PORTB->PCR[LED_RED] |= 1 << 8;  /*  */ Enable GPIO
    GPIOB->PDDR |= 1 << LED_RED;  /*  */ Set direction as output
    GPIOB->PDOR |= 1 << LED_RED;  /*  */ Set output to 1 (turn off LED)

    /* BLUE */
    PCC->CLKCFG[PCC_PORTD_INDEX] |= 1 << 30;  /* Enable clock for GPIOD */
    PORTD->PCR[LED_BLUE] |= 1 << 8;  /* Enable GPIO */
    GPIOD->PDDR |= 1 << LED_BLUE;  /* Set direction as output */
    GPIOD->PDOR |= 1 << LED_BLUE;  /* Set output to 1 (turn off LED) */
}

void InitSystick()
{
    SysTick->LOAD = SYSTICK_TIME_INTERRUPT / (1.0 / SystemCoreClock) - 1;
    SysTick->VAL = 0;
    SysTick->CTRL |= 1 << 2;  /* Use system clock */
    SysTick->CTRL |= 1 << 0;  /* Enable SysTick */
}

void InitSW()
{
    /* Enable clock for PORTD */
    PCC->CLKCFG[PCC_PORTD_INDEX] |= 1 << 30;

    /* SW2 */
    PORTD->PCR[SW2] = 1 << 8 | 1 << 1 | 1 << 0;  // Pull-up resistor, enable GPIO
    GPIOD->PDDR &= ~(1U << SW2);  // Input

    /* SW3 */
    PORTD->PCR[SW3] = 1 << 8 | 1 << 1 | 1 << 0;  // Pull-up resistor, enable GPIO
    GPIOD->PDDR &= ~(1U << SW3);  // Input
}

void InitInterrupt()
{
    /* Interrupt on falling edge for SW2 and SW3 */
    PORTD->PCR[SW2] |= PORT_PCR_IRQC(0x0A);
    PORTD->PCR[SW3] |= PORT_PCR_IRQC(0x0A);
    NVIC_EnableIRQ(PORTBCD_IRQn);  /* Enable interrupt in NVIC */
}

void SW2InterruptHandler()
{
    if (PORTD->ISFR & (1U << SW2))
    {
        /* Toggle LED RED */
        GPIOB->PTOR |= (1 << LED_RED);
        /* Clear the interrupt flag */
        PORTD->ISFR |= (1U << SW2);
    }
}

void SW3InterruptHandler()
{
    if (PORTD->ISFR & (1U << SW3))
    {
        /* Cycle through modes */
        currentMode = (currentMode + 1) % MAX_MODE;
        /* Clear the interrupt flag */
        PORTD->ISFR |= (1U << SW3);
    }
}

void PORTBCD_IRQHandler()
{
    SW2InterruptHandler();
    SW3InterruptHandler();
}

/* Mode 1: Turn on LED */
void Mode1()
{
    GPIOB->PCOR |= 1 << LED_GREEN;  // Turn on LED
}

/* Mode 2: Blink LED at 5 Hz */
void Mode2()
{
    static unsigned int countTime = 0;
    if (countTime >= 10000)
    {  /* Toggle every 0.1s (5 Hz) */
        GPIOB->PTOR |= 1 << LED_GREEN;  /* Toggle LED */
        countTime = 0;
    }
    countTime++;
}

/* Mode 3: Gradually brighten and dim LED */
void Mode3()
{
    static unsigned int systickInterruptCount = 0;
    static unsigned int timeClockCount = 0;  /* msecs */
    static float dutyPWM = 0.0;
    static DIRECTION_PWM directionPWM = INCREASE;

    if ((SysTick->CTRL & (1 << 16)))  /* Each SYSTICK_TIME_INTERRUPT */
    {
        /* Count from 0-TIME_OPERATION and reverse */
        if (directionPWM == DECREASE)
        {
            timeClockCount--;
            directionPWM = (timeClockCount <= 1) ? INCREASE : directionPWM;
        }
        else
        {
            timeClockCount++;
            directionPWM = (timeClockCount) > ((PWM_TIME_OPERATION * 1000) / (SYSTICK_TIME_INTERRUPT * 1000)) ? DECREASE : directionPWM;
        }

        /* Start control LED by simulate PWM */
        dutyPWM = (((float)timeClockCount * PWM_STEP) <= PWM_MIN_DUTY) ? PWM_MIN_DUTY : (float)timeClockCount * PWM_STEP;
        if (++systickInterruptCount <= PWM_DUTY_BY_SYSTICK(dutyPWM))
        {
            GPIOB->PCOR |= 1 << LED_GREEN;  /* Turn on LED */
        }
        else
        {
            GPIOB->PSOR |= 1 << LED_GREEN;  /* Turn off LED */
        }

        /* Use to fixed cycle = PWM_CYCLE_BY_SYSTICK */
        if (systickInterruptCount >= PWM_CYCLE_BY_SYSTICK)
        {
            systickInterruptCount = 0;
        }
    }
}

/*
 * @brief   Application entry point.
 */
int main(void)
{
    InitLED();
    InitSystick();
    InitSW();
    InitInterrupt();

    while (1)
    {
        switch (currentMode)
        {
            case MODE1:
                Mode1();
                break;
            case MODE2:
                Mode2();
                break;
            case MODE3:
                Mode3();
                break;
            default:
                break;
        }
    }
    return 0;
}
