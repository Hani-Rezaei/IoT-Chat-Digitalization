
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

/**
 * @brief Liest und verarbeitet alle Geräte eines bestimmten Typs aus einer SAUL-Geräteliste.
 *
 * Diese Funktion sucht nach einem Gerät mit dem angegebenen Namen, iteriert über alle 
 * Geräte in der SAUL-Geräteliste und liest die zugehörigen Werte aus. Wenn das Gerät erfolgreich 
 * gelesen wird, werden die Werte im JSON-Format zurückgegeben und geloggt.
 * Für jedes Gerät wird der entsprechende Gerätetyp überprüft, und je nach Gerätetyp erfolgt 
 * eine spezifische Verarbeitung.
 *
 * @param[in] device_name Der Name des Geräts, das in der SAUL-Datenbank gefunden und verarbeitet 
 *                        werden soll.
 *
 * @return int Rückgabewert, der den Status der Operation angibt:
 *         - 0 bei Erfolg
 *         - -1 wenn kein Gerät mit dem angegebenen Namen gefunden wurde
 *         - Negativer Wert bei Fehlern während des Lesens oder der Verarbeitung der Geräte
 *
 * @note Die Funktion verarbeitet die Geräte der SAUL-Geräteliste in einer Schleife. 
 *       Der Aufrufer kann die Rückgabewerte verwenden, um Fehler zu diagnostizieren und 
 *       gegebenenfalls Maßnahmen zu ergreifen.
 */
int read_saul_reg_dev (const char*);

int read_bme280_temperature (const char*, char* json_buffer, size_t* json_size);
int read_bme280_pressure (const char* device_name, char* json_buffer, size_t* json_size);
int read_bme280_humidity (const char* device_name, char* json_buffer, size_t* json_size);

#endif  /* SAUL_UTILS_H */