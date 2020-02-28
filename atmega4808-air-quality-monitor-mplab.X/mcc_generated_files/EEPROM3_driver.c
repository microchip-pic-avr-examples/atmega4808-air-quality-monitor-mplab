/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

#include "EEPROM3_driver.h"
#include "mcc.h"

#define PAGESIZE    256
#define MIN(a,b) (a<b?a:b)

uint16_t EEPROM_DEVICE_ADDRESS = (0xA8 >> 1);

typedef struct
{
    uint8_t *data;
    uint16_t *dataSize;
} block_t;
    
static uint16_t eeprom_flipAddress(uint16_t address)
{
    return (address << 8) | (address >> 8);
}

static uint8_t getMSBAddress(uint32_t address)
{
    uint8_t deviceAddress;
    deviceAddress = (address & 0xF0000) >> 16;
    return (deviceAddress);
}

static void setDeviceAddress(uint8_t MSBAddress)
{
    switch(MSBAddress)
    {
        case 0x0:
            EEPROM_DEVICE_ADDRESS = (0xA8 >> 1);
            break;
        case 0x1:
            EEPROM_DEVICE_ADDRESS = (0xAA >> 1);
            break;
        case 0x2:
            EEPROM_DEVICE_ADDRESS = (0xAC >> 1);
            break;
        case 0x3:
            EEPROM_DEVICE_ADDRESS = (0xAE >> 1);
            break;
        default:
            break;
        
    }
}

static void resetDeviceAddress()
{
    switch(EEPROM_DEVICE_ADDRESS)
    {
        case 0x54:
            EEPROM_DEVICE_ADDRESS = (0xAA >> 1);
            break;
        case 0x55:
            EEPROM_DEVICE_ADDRESS = (0xAC >> 1);
            break;
        case 0x56:
            EEPROM_DEVICE_ADDRESS = (0xAE >> 1);
            break;
        case 0x57:
            EEPROM_DEVICE_ADDRESS = (0xA8 >> 1);
            break;
        default:
            break;
        
    }
}

static twi0_operations_t write1ByteHandler(void *p)
{
    I2C0_SetBuffer(p,1);
    I2C0_SetDataCompleteCallback(NULL,NULL);
    return I2C0_CONTINUE;
}

static twi0_operations_t read1ByteHandler(void *p)
{
    I2C0_SetBuffer(p,1);
    I2C0_SetDataCompleteCallback(NULL,NULL);
    return I2C0_RESTART_READ;
}

void EEPROM3_WriteOneByte(uint32_t address, uint8_t data)
{
    uint8_t MSBAddress = getMSBAddress(address);
    setDeviceAddress(MSBAddress);
    while(!I2C0_Open(EEPROM_DEVICE_ADDRESS)); // sit here until we get the bus..
    address = eeprom_flipAddress(address); 
    I2C0_SetDataCompleteCallback(write1ByteHandler,&data);
    I2C0_SetBuffer(&address,2);
    I2C0_SetAddressNackCallback(I2C0_SetRestartWriteCallback,NULL); //NACK polling?
    I2C0_MasterWrite();
    while(I2C0_BUSY == I2C0_Close()); // sit here until finished.
//    NOP();
}

uint8_t EEPROM3_ReadOneByte(uint32_t address)
{
    uint8_t readData = 0;
    uint8_t MSBAddress = getMSBAddress(address);
    setDeviceAddress(MSBAddress);
    while(!I2C0_Open(EEPROM_DEVICE_ADDRESS)); // sit here until we get the bus..
    address = eeprom_flipAddress(address); 
    I2C0_SetDataCompleteCallback(read1ByteHandler,&readData);
    I2C0_SetBuffer(&address,2);
    I2C0_SetAddressNackCallback(I2C0_SetRestartWriteCallback,NULL); //NACK polling?
    I2C0_MasterWrite();
    while(I2C0_BUSY == I2C0_Close()); // sit here until finished.
    return readData;
}

static twi0_operations_t writeBlockHandler(void *payload)
{
    block_t *pgData = payload;
    I2C0_SetBuffer(pgData->data,*pgData->dataSize);
    I2C0_SetDataCompleteCallback(NULL,NULL);
    return I2C0_CONTINUE;
}

static void EEPROM3_WritePage(uint16_t address, uint8_t* data, uint16_t count)
{
    while(!I2C0_Open(EEPROM_DEVICE_ADDRESS)); // sit here until we get the bus..
    address = eeprom_flipAddress(address); 

    block_t blk = {0};
    blk.data = data;
    blk.dataSize = &count;

    I2C0_SetDataCompleteCallback(writeBlockHandler,&blk);
    I2C0_SetBuffer(&address,2);
    I2C0_SetAddressNackCallback(I2C0_SetRestartWriteCallback,NULL); //NACK polling?
    I2C0_MasterWrite();
    while(I2C0_BUSY == I2C0_Close()); // sit here until finished.
//    NOP();

}

void EEPROM3_WriteBlock(uint32_t address, void *dataBlock, uint16_t dataBlockByteCount)
{   
    uint8_t MSBAddress = getMSBAddress(address);
    setDeviceAddress(MSBAddress);
    uint16_t wordAddress = (address & 0x00FFFF);
    uint16_t pageAddress = wordAddress & 0xFF00;
    uint32_t byte2write = PAGESIZE - (wordAddress - pageAddress);
    uint8_t *block = dataBlock;
    do
    {
        byte2write = MIN(byte2write,dataBlockByteCount);
        EEPROM3_WritePage(wordAddress,block,byte2write);
        wordAddress = wordAddress + byte2write;
        if(wordAddress == 0){
            resetDeviceAddress();
        }
        block = block + byte2write;
        dataBlockByteCount = dataBlockByteCount - byte2write;
        byte2write = PAGESIZE;
		_delay_ms(5);
		_delay_ms(5);
    } while(dataBlockByteCount);
}

static twi0_operations_t readBlockHandler(void *payload)
{
    block_t *pgData = payload;
    I2C0_SetBuffer(pgData->data,*pgData->dataSize);
    I2C0_SetDataCompleteCallback(NULL,NULL);
    return I2C0_RESTART_READ;
}

void EEPROM3_ReadBlock(uint32_t address, void* data, uint16_t count)
{
    uint8_t MSBAddress = getMSBAddress(address);
    setDeviceAddress(MSBAddress);
    uint16_t wordAddress = (address & 0x00FFFF);
    while(!I2C0_Open(EEPROM_DEVICE_ADDRESS)); // sit here until we get the bus..
    wordAddress = eeprom_flipAddress(wordAddress); 
    block_t blk = {0};
    blk.data = data;
    blk.dataSize = &count;
    I2C0_SetDataCompleteCallback(readBlockHandler,&blk);
    I2C0_SetBuffer(&wordAddress,2);
    I2C0_SetAddressNackCallback(I2C0_SetRestartWriteCallback,NULL); //NACK polling?
    I2C0_MasterWrite();
    while(I2C0_BUSY == I2C0_Close()); // sit here until finished.
    
}