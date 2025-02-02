
/**
 * @file    led_utils.h
 * @brief   Header-Datei für die SAUL-Geräteverwaltung und Datenverarbeitung.
 * 
 * 
 * @author  Mohammadjavad Esmaeili
 */

#ifndef LED_UTILS_H
#define LED_UTILS_H


#include "clk.h"
#include "board.h"
#include "periph_conf.h"
#include "timex.h"
#include "ztimer.h"

int led_conf (const char* device_name, uint8_t is_on);


#endif  /* LED_UTILS_H */