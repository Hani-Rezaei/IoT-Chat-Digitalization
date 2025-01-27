
/**
 * @file    saul_utils.h
 * @brief   Header-Datei für die SAUL-Geräteverwaltung und Datenverarbeitung.
 * 
 * Diese Datei enthält die Deklarationen von Funktionen und Variablen, die zur 
 * Verwaltung und Verarbeitung von SAUL-Geräten verwendet werden. Sie stellt 
 * Funktionen zum Lesen von Sensordaten und zur Umwandlung der Daten in das 
 * JSON-Format bereit. Die Datei enthält außerdem Deklarationen für globale 
 * Variablen, die die Namen spezifischer SAUL-Geräte enthalten.
 * 
 * @author  Mohammadjavad Esmaeili
 */

#ifndef SAUL_UTILS_H
#define SAUL_UTILS_H

#include <stdio.h>
#include "saul_reg.h"
#include "shell.h"
#include <math.h>

// Log
#include "log.h"
#include <stdlib.h>

/**
 * @brief Name des NRF_TEMP Geräts         
 * @note Diese Variablen sind konstant und kann nicht verändert werden.
 */
extern const char* nrf_temp;

/**
 * @brief Name des BME_280 Sensors         
 * @note Diese Variablen sind konstant und kann nicht verändert werden.
 */
extern const char* bme_280_name;

int read_bme280_temperature (const char*, const char* request_unit, char* json_buffer, size_t* json_size);
int read_bme280_pressure (const char* device_name, const char* request_unit, char* json_buffer, size_t* json_size);
int read_bme280_humidity (const char* device_name, char* json_buffer, size_t* json_size);

void escape_json(const char *input, char *output);


#endif  /* SAUL_UTILS_H */