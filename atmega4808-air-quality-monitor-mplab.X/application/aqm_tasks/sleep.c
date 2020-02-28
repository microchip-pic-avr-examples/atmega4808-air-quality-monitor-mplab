/******************************************************************************
 *  include files
 *****************************************************************************/
#include "sleep.h"
#include "display.h"
#include "store.h"
#include "send.h"
#include "../parameters.h"
#include "../../mcc_generated_files/drivers/timeout.h"
#include "../../mcc_generated_files/config/clock_config.h"
#include "../../mcc_generated_files/include/ccp.h"
#include <util/delay.h>
#include <avr/sleep.h>

/******************************************************************************
 *  Macros definitions
 *****************************************************************************/
#define SEND_TASK_TIME    (2)  
#define READ_TASK_TIME    (2)                          //number should be (n /2 ) for n sec   eg. fot 10 sec, 10/2 = 5
#define STORE_TASK_TIME   (30)  

#define _NOP() do { __asm__ __volatile__ ("nop"); } while (0)

/******************************************************************************
 *  global variables, arrays
 *****************************************************************************/
volatile uint8_t wakeupSource = 0; // to store wake up source
volatile uint8_t runningTask = ALL_TASK_MASK_BIT; // to store task status

timerStruct_t SLEEP_TasksTimer = {SLEEP_Task};

/******************************************************************************
 *  local variables, arrays
 *****************************************************************************/
static uint8_t PitIntCounter_forReadTask = RESET_COUNTER;
static uint8_t PitIntCounter_forStoreTask = RESET_COUNTER;
static uint8_t PitIntCounter_forSendTask = RESET_COUNTER;

/*******************************************************************************
 * @Name
 *    uint32_t SLEEP_Task(void *payload) 
 * @Param
 *    payload address
 * @Returns
 *    next "time" to call this task
 * @Description
 *    Go to sleep and wake up from sleep
 *******************************************************************************/
uint32_t SLEEP_Task (void *payload)
{
    if (runningTask == 0) //perform sleep task if all task are completed 
    {
        AqmPrinter (INFO_AQM, "wakeup source = %u", wakeupSource);
        AqmPrinter (TASK_AQM, "sleeping");
        _delay_ms (50);
        ccp_write_io((void*)&(CLKCTRL.MCLKCTRLA),0x01);    //changing clock source 
        while(!(CLKCTRL.MCLKSTATUS & 0b00100000));
        
        while (wakeupSource == 0) //sleep again if no valid wake up source 
        {
            sleep_cpu ();
            _NOP();
            _NOP();
        }
        
        ccp_write_io ((void*) &(CLKCTRL.MCLKCTRLA), 0x00);   //changing clock source 
        while (!(CLKCTRL.MCLKSTATUS & 0b00010000));
        AqmPrinter (TASK_AQM, "waking up");
        AqmPrinter (INFO_AQM, "wakeup source = %u", wakeupSource);
        

    }

    return SLEEP_TASK_INTERVAL;

}

/*******************************************************************************
 * @Name
 *    void PitISR(void)
 * @Param
 *    none
 * @Returns
 *    none
 * @Description
 *    PIT interrupt callback function
 *    enables task according to number of interrupt (time)
 *******************************************************************************/
void PitISR (void)
{
    static uint8_t firstPitInterrupt = YES;

    if (firstPitInterrupt == YES)
    {
        firstPitInterrupt = NO;
        //need to ignore first PIT interrupt   
    }
    else
    {
        //increment counters
        PitIntCounter_forReadTask++;
        PitIntCounter_forStoreTask++;
        PitIntCounter_forSendTask++;

        if (PitIntCounter_forSendTask >= SEND_TASK_TIME) // if send time has arrived (8 sec completed)
        {
            PitIntCounter_forSendTask = RESET_COUNTER;
            wakeupSource |= SEND_TASK_TIME_ARRIVED;
            runningTask |= SEND_TASK_MASK_BIT;
        }

        if (PitIntCounter_forReadTask >= READ_TASK_TIME) // if read time has arrived  (15 min completed)
        {
            PitIntCounter_forReadTask = RESET_COUNTER;
            wakeupSource |= READ_TASK_TIME_ARRIVED;
            runningTask |= READ_TASK_MASK_BIT;
        }

        if (PitIntCounter_forStoreTask >= STORE_TASK_TIME) // if store time has arrived (1 hour completed)
        {
            PitIntCounter_forStoreTask = RESET_COUNTER;
            wakeupSource |= STORE_TASK_TIME_ARRIVED;
            runningTask |= STORE_TASK_MASK_BIT;
        }
    }
}



