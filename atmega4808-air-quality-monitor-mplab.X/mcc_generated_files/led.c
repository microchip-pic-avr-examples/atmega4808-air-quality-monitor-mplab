/*
    \file   led.c

    \brief  Manage board LED's

    (c) 2018 Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip software and any
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party
    license terms applicable to your use of third party software (including open source software) that
    may accompany Microchip software.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS
    FOR A PARTICULAR PURPOSE.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS
    SOFTWARE.
*/

#include <stdbool.h>
#include "mcc.h"
#include "config/clock_config.h"

#include <util/delay.h>
#include "led.h"

#define LEDS_TEST_INTERVAL	50L
#define LED_ON_INTERVAL     200L
#define LEDS_HOLD_INTERVAL	2000L

static bool ledForDefaultCredentials = false;
static bool ledHeld = false;

static uint32_t yellow_task(void *payload);
static timerStruct_t yellow_timer = {yellow_task};

static uint32_t red_task(void *payload);
static timerStruct_t red_timer = {red_task};

static uint32_t defaultCredentials_task(void *payload);
static timerStruct_t defaultCredentials_timer = {defaultCredentials_task};

static uint32_t softAp_task(void *payload);
static timerStruct_t softAP_timer = {softAp_task};

static void testSequence (uint8_t ledState)
{
    if(LED_OFF == ledState){
        LED_BLUE_SetHigh();
        _delay_ms(LEDS_TEST_INTERVAL);
        LED_GREEN_SetHigh();
        _delay_ms(LEDS_TEST_INTERVAL);
        LED_YELLOW_SetHigh();
        _delay_ms(LEDS_TEST_INTERVAL);
        LED_RED_SetHigh();
        _delay_ms(LEDS_TEST_INTERVAL);
    } else {
        LED_BLUE_SetLow();
        _delay_ms(LEDS_TEST_INTERVAL);
        LED_GREEN_SetLow();
        _delay_ms(LEDS_TEST_INTERVAL);
        LED_YELLOW_SetLow();
        _delay_ms(LEDS_TEST_INTERVAL);
        LED_RED_SetLow();
        _delay_ms(LEDS_TEST_INTERVAL);
    }
}

void LED_test(void)
{
	testSequence(LED_ON);
	testSequence(LED_OFF);
}

static uint32_t yellow_task(void *payload)
{
   LED_YELLOW_SetHigh();
   ledHeld = false;
   return 0;
}

static uint32_t red_task(void *payload)
{
	LED_RED_SetHigh();
	return 0;	
}

static uint32_t softAp_task(void *payload)
{
    LED_BLUE_Toggle();
    return LED_ON_INTERVAL;
}

static uint32_t defaultCredentials_task(void *payload)
{
    LED_GREEN_Toggle();
    return LED_ON_INTERVAL;
}

void LED_flashYellow(void)
{
    if (ledHeld == false)
    {
        LED_YELLOW_SetLow();
        timeout_create(&yellow_timer,200);
    }

}

void LED_holdYellowOn(bool holdHigh)
{
    if (holdHigh == true)
    {
        LED_YELLOW_SetLow();
    }
    else
    {
        LED_YELLOW_SetHigh();
    }
    // Re-Use yellow_timer task
    ledHeld = true;
    timeout_create(&yellow_timer,LEDS_HOLD_INTERVAL);
}

void LED_flashRed(void)
{
   LED_RED_SetLow();
   timeout_create(&red_timer,LED_ON_INTERVAL);	
}

void LED_blinkingBlue(bool amBlinking)
{
    if (amBlinking == true)
    {
        timeout_create(&softAP_timer,LED_ON_INTERVAL);
    }
    else
    {
        timeout_delete(&softAP_timer);
    }
}

void LED_startBlinkingGreen(void)
{
    timeout_create(&defaultCredentials_timer,LED_ON_INTERVAL);
    ledForDefaultCredentials = true;
}

void LED_stopBlinkingGreen(void)
{
    if (ledForDefaultCredentials == true)
    {
        timeout_delete(&defaultCredentials_timer);
        ledForDefaultCredentials = false;
    }
}

bool LED_isBlinkingGreen (void)
{
    return ledForDefaultCredentials;
}