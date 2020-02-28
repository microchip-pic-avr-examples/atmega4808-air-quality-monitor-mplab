
#ifndef AQI_CALCULATION_H
#define	AQI_CALCULATION_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>

uint16_t CalculateAveragePM(void);
uint16_t CalculateAQI (uint16_t averagePm);


#ifdef	__cplusplus
}
#endif

#endif	/* AQI_CALCULATION_H */