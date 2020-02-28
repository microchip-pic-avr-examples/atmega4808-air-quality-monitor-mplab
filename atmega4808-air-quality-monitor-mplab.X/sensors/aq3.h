
#ifndef AQ3_H
#define	AQ3_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>
#include "../mcc_generated_files/include/twi0_master.h"

// CCS811 I2C addresses
#define CCS811_I2C_ADDRESS_1      (0x5A) 

/**
 * @brief 	CCS811 operation modes
 */


 #define    ccs811_mode_idle    (0x00) // Idle, low current mode
 #define    ccs811_mode_1s      (0x10)  // Constant Power mode, IAQ values every 1 s
 #define    ccs811_mode_10s     (0x20)  // Pulse Heating mode, IAQ values every 10 s
 #define    ccs811_mode_60s     (0x30)  // Low Power Pulse Heating, IAQ values every 60 s
 #define    ccs811_mode_250ms   (0x40)  // Constant Power mode, RAW data every 250 ms



void ccs811_InitSensor(void);
void ccs811_GetResults(uint16_t* iaq_eco2,uint16_t* iaq_tvoc);
void i2c_write4ByteRegister(twi0_address_t address, uint8_t reg, uint32_t data);


#ifdef	__cplusplus
}
#endif

#endif	/* AQ3_H */

