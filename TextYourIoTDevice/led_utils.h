
/**
 * @file    led_utils.h
 * @brief   Header-Datei für die SAUL-Geräteverwaltung und LED-Steuerung.
 * 
 * Diese Datei enthält die Deklarationen für die Steuerung von LED-Geräten,
 * die mit dem SAUL-Register (Sensor, Actuator, and User Logic) verwaltet werden.
 * Sie ermöglicht das Ein- und Ausschalten von LEDs über ihre Gerätenamen.
 * 
 * @author  Mohammadjavad Esmaeili
 * @date    2025
 */

#ifndef LED_UTILS_H
#define LED_UTILS_H


#include "clk.h"
#include "board.h"
#include "periph_conf.h"
#include "timex.h"
#include "ztimer.h"

/**
 * @brief   Setzt den Zustand einer LED basierend auf ihrem Gerätenamen.
 * 
 * Diese Funktion sucht ein Gerät anhand des übergebenen Namens im SAUL-Register
 * und setzt dessen Zustand auf AN oder AUS, abhängig vom Parameter `is_on`.
 * 
 * @param[in]  device_name  Name des LED-Geräts im SAUL-Register.
 * @param[in]  is_on        Status der LED: 1 für AN, 0 für AUS.
 * 
 * @return 0 bei Erfolg, -1 wenn das Gerät nicht gefunden wurde, -2 falls das Setzen des Zustands fehlschlägt.
 */

int led_set_state (const char* device_name, uint8_t is_on);


#endif  /* LED_UTILS_H */