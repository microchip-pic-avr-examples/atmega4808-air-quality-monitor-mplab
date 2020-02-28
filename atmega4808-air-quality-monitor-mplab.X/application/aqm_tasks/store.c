/******************************************************************************
 *  include files
 *****************************************************************************/
#include "store.h"
#include "sleep.h"
#include "../parameters.h"
#include "../../mcc_generated_files/drivers/timeout.h"
#include "../../mcc_generated_files/EEPROM3_driver.h"
#include "../../mcc_generated_files/include/nvmctrl.h"

/******************************************************************************
 *  enum
 *****************************************************************************/
typedef enum
{
    GET_WRITE_INDEX,
    WRITE_ONE_PARAMETER,
    UPDATE_WRITE_INDEX,
    WAITING
} store_task_state_t;

/******************************************************************************
 *  global variables, arrays
 *****************************************************************************/
timerStruct_t STORE_TasksTimer = {STORE_Task};

/******************************************************************************
 *  local variables, arrays
 *****************************************************************************/
static uint8_t parameterStoreCounter = AQI;
static unsigned long int eepromWriteIndex = 0;
static store_task_state_t state = GET_WRITE_INDEX;

/*******************************************************************************
 * @Name
 *    uint32_t STORE_Task(void *payload)
 * @Param
 *    payload address
 * @Returns
 *    next "time" to call this task
 * @Description
 *    Store data in external EEPROM
 *******************************************************************************/
uint32_t STORE_Task (void *payload)
{
    switch (state)
    {
    case GET_WRITE_INDEX:
//        AqmPrinter(DEBUG_AQM, "in GET_WRITE_INDEX store task");
        NVM_ReadEepromBlock (EXTERNAL_EEPROM_WRITE_INDEX_ADDRESS,
                             (uint8_t *) & eepromWriteIndex, LENGTH_OF_EEPROM_ADDRESS); //read(from internal eeprom) the address to which data has to be write in external eeprom  
        state = WRITE_ONE_PARAMETER;

        break;

    case WRITE_ONE_PARAMETER:
//        AqmPrinter (DEBUG_AQM, "in WRITE_SUBTASK store task: Writing 1 parameter");
        if (parameterStoreCounter < LAST_PARAMETER)
        {
            EEPROM3_WriteOneByte (eepromWriteIndex, (uint8_t) (*parameterTable[parameterStoreCounter].Parameter >> 8));
            eepromWriteIndex++;
            EEPROM3_WriteOneByte (eepromWriteIndex, (uint8_t) (*parameterTable[parameterStoreCounter].Parameter));
            eepromWriteIndex++;

            parameterStoreCounter++;
        }
        else
        {
            parameterStoreCounter = AQI;
            state = UPDATE_WRITE_INDEX;
        }

        break;

    case UPDATE_WRITE_INDEX:
//        AqmPrinter(DEBUG_AQM, "in UPDATE_WRITE_INDEX store task");
        AqmPrinter (INFO_AQM, "write index = %lu", eepromWriteIndex);
        if (eepromWriteIndex > LAST_EEPROM_ADDRESS) // if write address is more than last address
        {
            eepromWriteIndex = 0x00000000; //reset the eeprom write address
        }
        NVM_WriteEepromBlock (EXTERNAL_EEPROM_WRITE_INDEX_ADDRESS,
                              (uint8_t *) & eepromWriteIndex, LENGTH_OF_EEPROM_ADDRESS); // update write address in internal eeprom
        state = WAITING;
        runningTask &= ~STORE_TASK_MASK_BIT;

        break;

    case WAITING:
//        AqmPrinter(DEBUG_AQM, "in WAITING_SUBTASK store task");
        if (runningTask & STORE_TASK_MASK_BIT)
        {
            state = GET_WRITE_INDEX;
            wakeupSource &= ~(STORE_TASK_TIME_ARRIVED); // clear wake up source flag
        }

        break;
    }

    return STORE_TASK_INTERVAL;

}