
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
 * @brief Liest die Werte eines SAUL-Geräts und gibt sie als JSON zurück.
 *
 * Diese Funktion liest die Sensordaten eines SAUL-Geräts aus und konvertiert diese in ein 
 * JSON-Format. Die JSON-Daten werden in einem dynamisch allokierten Puffer gespeichert, dessen 
 * Adresse an den Aufrufer übergeben wird. Der Aufrufer ist dafür verantwortlich, den Speicher 
 * nach der Verwendung freizugeben.
 *
 * @param[in] saul_device  Zeiger auf das SAUL-Gerät, dessen Werte ausgelesen werden sollen.
 * @param[out] json_buffer Zeiger auf einen Zeiger, der die Adresse des allokierten Puffers 
 *                         enthält, der die JSON-Daten speichern wird.
 *
 * @return int Rückgabewert, der den Status der Operation angibt:
 *         - 0 bei Erfolg
 *         - Negativer Wert bei einem Fehler (z.B. beim Lesen des Geräts oder bei der 
 *           Speicherzuweisung)
 *
 * @note Der Aufrufer sollte sicherstellen, dass der zurückgegebene `json_buffer` nach 
 *       der Verwendung mit `free(json_buffer)` freigegeben wird.
 */
int read_device_values(saul_reg_t*, char**);

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

#endif  /* SAUL_UTILS_H */