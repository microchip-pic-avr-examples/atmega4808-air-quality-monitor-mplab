#ifndef CONF_WIFI_M2M_PINS_H_INCLUDED
#define CONF_WIFI_M2M_PINS_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif
    
#include "../include/port.h"

/** Module Pin Settings */
#define CONF_WIFI_M2M_SPI_CS_PIN_set_dir            PA7_set_dir
#define CONF_WIFI_M2M_SPI_CS_PIN_set_level          PA7_set_level
#define CONF_WIFI_M2M_SPI_CS_PIN_set_isc            PA7_set_isc
#define CONF_WIFI_M2M_SPI_CS_PIN_set_pull_mode      PA7_set_pull_mode
#define CONF_WIFI_M2M_SPI_CS_PIN_set_inverted       PA7_set_inverted
#define CONF_WIFI_M2M_SPI_CS_PIN_get_level          PA7_get_level
#define CONF_WIFI_M2M_SPI_CS_PIN_toggle_level       PA7_toggle_level
    
#define CONF_WIFI_M2M_SPI_MOSI_PIN_set_dir          PA4_set_dir
#define CONF_WIFI_M2M_SPI_MOSI_PIN_set_level        PA4_set_level
#define CONF_WIFI_M2M_SPI_MOSI_PIN_set_isc          PA4_set_isc
#define CONF_WIFI_M2M_SPI_MOSI_PIN_set_pull_mode    PA4_set_pull_mode
#define CONF_WIFI_M2M_SPI_MOSI_PIN_set_inverted     PA4_set_inverted
#define CONF_WIFI_M2M_SPI_MOSI_PIN_get_level        PA4_get_level
#define CONF_WIFI_M2M_SPI_MOSI_PIN_toggle_level     PA4_toggle_level
    
#define CONF_WIFI_M2M_SPI_MISO_PIN_set_dir          PA5_set_dir
#define CONF_WIFI_M2M_SPI_MISO_PIN_set_level        PA5_set_level
#define CONF_WIFI_M2M_SPI_MISO_PIN_set_isc          PA5_set_isc
#define CONF_WIFI_M2M_SPI_MISO_PIN_set_pull_mode    PA5_set_pull_mode
#define CONF_WIFI_M2M_SPI_MISO_PIN_set_inverted     PA5_set_inverted
#define CONF_WIFI_M2M_SPI_MISO_PIN_get_level        PA5_get_level
#define CONF_WIFI_M2M_SPI_MISO_PIN_toggle_level     PA5_toggle_level
    
#define CONF_WIFI_M2M_SPI_SCK_PIN_set_dir           PA6_set_dir
#define CONF_WIFI_M2M_SPI_SCK_PIN_set_level         PA6_set_level
#define CONF_WIFI_M2M_SPI_SCK_PIN_set_isc           PA6_set_isc
#define CONF_WIFI_M2M_SPI_SCK_PIN_set_pull_mode     PA6_set_pull_mode
#define CONF_WIFI_M2M_SPI_SCK_PIN_set_inverted      PA6_set_inverted
#define CONF_WIFI_M2M_SPI_SCK_PIN_get_level         PA6_get_level
#define CONF_WIFI_M2M_SPI_SCK_PIN_toggle_level      PA6_toggle_level
    
static inline void PA4_set_pull_mode(const enum port_pull_mode pull_mode)
{
    PORTA_set_pin_pull_mode(4, pull_mode);
}

static inline void PA5_set_pull_mode(const enum port_pull_mode pull_mode)
{
    PORTA_set_pin_pull_mode(5, pull_mode);
}

static inline void PA6_set_pull_mode(const enum port_pull_mode pull_mode)
{
    PORTA_set_pin_pull_mode(6, pull_mode);
}

static inline void PA7_set_pull_mode(const enum port_pull_mode pull_mode)
{
    PORTA_set_pin_pull_mode(7, pull_mode);
}

static inline void PA4_set_dir(const enum port_dir dir)
{
    PORTA_set_pin_dir(4, dir);
}

static inline void PA5_set_dir(const enum port_dir dir)
{
    PORTA_set_pin_dir(5, dir);
}

static inline void PA6_set_dir(const enum port_dir dir)
{
    PORTA_set_pin_dir(6, dir);
}

static inline void PA7_set_dir(const enum port_dir dir)
{
    PORTA_set_pin_dir(7, dir);
}

static inline void PA4_set_isc(const PORT_ISC_t isc)
{
    PORTA_pin_set_isc(4, isc);
}

static inline void PA5_set_isc(const PORT_ISC_t isc)
{
    PORTA_pin_set_isc(5, isc);
}

static inline void PA6_set_isc(const PORT_ISC_t isc)
{
    PORTA_pin_set_isc(6, isc);
}

static inline void PA7_set_isc(const PORT_ISC_t isc)
{
    PORTA_pin_set_isc(7, isc);
}

static inline void PA4_set_inverted(const bool inverted)
{
    PORTA_pin_set_inverted(4, inverted);
}

static inline void PA5_set_inverted(const bool inverted)
{
    PORTA_pin_set_inverted(5, inverted);
}
static inline void PA6_set_inverted(const bool inverted)
{
    PORTA_pin_set_inverted(6, inverted);
}
static inline void PA7_set_inverted(const bool inverted)
{
    PORTA_pin_set_inverted(7, inverted);
}
static inline void PA4_set_level(const bool level)
{
    PORTA_set_pin_level(4, level);
}
static inline void PA5_set_level(const bool level)
{
    PORTA_set_pin_level(5, level);
}
static inline void PA6_set_level(const bool level)
{
    PORTA_set_pin_level(6, level);
}
static inline void PA7_set_level(const bool level)
{
    PORTA_set_pin_level(7, level);
}
static inline bool PA4_get_level()
{
    return PORTA_get_pin_level(4);
}
static inline bool PA5_get_level()
{
    return PORTA_get_pin_level(5);
}
static inline bool PA6_get_level()
{
    return PORTA_get_pin_level(6);
}
static inline bool PA7_get_level()
{
    return PORTA_get_pin_level(7);
}
static inline void PA4_toggle_level()
{
    PORTA_toggle_pin_level(4);
}
static inline void PA5_toggle_level()
{
    PORTA_toggle_pin_level(5);
}
static inline void PA6_toggle_level()
{
    PORTA_toggle_pin_level(6);
}
static inline void PA7_toggle_level()
{
    PORTA_toggle_pin_level(7);
}

#ifdef __cplusplus
}
#endif

#endif /* CONF_WIFI_M2M_PINS_H_INCLUDED */
