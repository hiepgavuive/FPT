/*
 * Copyright 2016-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    MKE16Z4_Project.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "MKE16Z4.h"

/**
  * @brief  PWM Initialization Function
  * @details
  *  This function configures the PWM on the MKE16Z4 microcontroller.
  *  It enables the clock for the necessary peripherals, sets up the GPIO pin
  *  as the PWM output, configures the TPM2 module for PWM generation with a
  *  frequency of 100Hz, and initializes the PWM mode with edge-aligned
  *  configuration.
  */
void init_pwm(void)
{
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;  // Enable clock for Port B
    PORTB->PCR[19] = PORT_PCR_MUX(3);    // Set PTB19 as TPM2_CH0 (PWM output)

    SIM->SCGC6 |= SIM_SCGC6_TPM2_MASK;   // Enable clock for TPM2
    SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);   // Use MCGFLLCLK as the timer clock source

    TPM2->SC = TPM_SC_CMOD(1) | TPM_SC_PS(4); // Set TPM source and prescaler
    TPM2->MOD = 48000;                    // Set period for 100Hz PWM (assuming 48MHz clock)
    TPM2->CONTROLS[0].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK; // Edge-aligned PWM
}

/**
  * @brief  Set PWM Duty Cycle
  * @param  dutyCycle: Duty cycle percentage (0-100%)
  * @details
  *  This function sets the PWM duty cycle. The duty cycle determines the
  *  percentage of time the PWM signal stays high within a single period.
  *  A duty cycle of 0% turns the LED off, while 100% turns it fully on.
  */
void set_pwm_duty_cycle(uint8_t dutyCycle)
{
    TPM2->CONTROLS[0].CnV = (dutyCycle * TPM2->MOD) / 100;
}

/**
  * @brief  Millisecond Delay Function
  * @param  ms: Number of milliseconds to delay
  * @details
  *  This function creates a delay for the specified number of milliseconds
  *  using the SysTick timer. It is used to create time intervals in the
  *  LED control functions.
  */
void delay_ms(uint32_t ms)
{
    SysTick->LOAD = (48000L * ms) - 1;
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;
    while ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0) {}
    SysTick->CTRL = 0;
}

/**
  * @brief  Breathing LED Mode
  * @details
  *  This function implements the breathing LED effect. The LED gradually
  *  increases in brightness to a maximum level, then gradually decreases
  *  back to off. The entire cycle takes 4 seconds.
  */
void mode1_breathing_led(void)
{
    for (int i = 0; i < 100; i++)
    {
        set_pwm_duty_cycle(i);   // Gradually increase brightness
        delay_ms(20);            // Adjust delay for smoothness
    }
    for (int i = 100; i > 0; i--)
    {
        set_pwm_duty_cycle(i);   // Gradually decrease brightness
        delay_ms(20);
    }
}

/**
  * @brief  Toggle LED Mode
  * @details
  *  This function toggles the LED on and off at a frequency of 1Hz. The
  *  LED stays on for 500ms and then off for 500ms.
  */
void mode2_toggle_led(void)
{
    GPIOB->PDOR ^= (1 << 19);  // Toggle LED connected to PTB19
    delay_ms(500);             // 1 Hz frequency (500ms on, 500ms off)
}

/**
  * @brief  Main Function
  * @details
  *  The main function initializes the PWM and then enters an infinite loop
  *  where it alternates between the breathing LED mode and the toggle LED
  *  mode. Each mode runs for 8 seconds before switching to the other.
  */
int main(void)
{
    init_pwm();  // Initialize PWM
    while (1)
    {
        for (int i = 0; i < 8; i++)
        {
            mode1_breathing_led();  // Run Mode 1 for 8 seconds
        }
        for (int i = 0; i < 8; i++)
        {
            mode2_toggle_led();     // Run Mode 2 for 8 seconds
        }
    }
    return 0;
}

