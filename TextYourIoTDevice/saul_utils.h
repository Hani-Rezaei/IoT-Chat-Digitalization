
/**
 * @file    saul_utils.h
 * @brief   Header-Datei für die Verwaltung und Verarbeitung von SAUL-Geräten.
 * 
 * Diese Datei enthält die Deklarationen von Funktionen und Variablen zur 
 * Verwaltung und Verarbeitung von SAUL-Geräten. Sie stellt Funktionen zum 
 * Lesen von Sensordaten und zur Umwandlung der Daten in das JSON-Format bereit. 
 * Zusätzlich werden globale Variablen für die Gerätenamen definiert.
 * 
 * @author  Mohammadjavad Esmaeili
 * @date    2025
 */
#ifndef SAUL_UTILS_H
#define SAUL_UTILS_H

#include <stdio.h>
#include "saul_reg.h"
#include "shell.h"
#include <math.h>
#include <stdlib.h>
// Log-Modul für Debugging und Fehlerausgabe
#include "log.h"

/** 
 * @brief Größe des Puffers für JSON-Ausgaben.
 */
#define PUFFER_SIZE 256
/** 
 * @brief Name des NRF_TEMP Geräts.
 * @note Diese Variable ist konstant und kann nicht verändert werden.
 */
extern const char* nrf_temp;
/** 
 * @brief Name des BME280 Sensors.
 * @note Diese Variable ist konstant und kann nicht verändert werden.
 */
extern const char* bme_280_name;
/**
 * @brief Liest die Temperatur vom BME280-Sensor und speichert das Ergebnis im JSON-Format.
 * 
 * @param[in]  device_name   Name des SAUL-Geräts (z. B. "bme280")
 * @param[in]  request_unit  Gewünschte Einheit für die Temperatur (z. B. "C" für Celsius)
 * @param[out] json_buffer   Puffer zum Speichern der JSON-formatierten Ausgabe
 * @param[out] json_size     Größe der generierten JSON-Daten
 * 
 * @return 0 bei Erfolg, negativer Fehlercode bei Fehlschlag.
 */
int read_bme280_temperature (const char*, const char* request_unit, char* json_buffer, size_t* json_size);
/**
 * @brief Liest den Luftdruck vom BME280-Sensor und speichert das Ergebnis im JSON-Format.
 * 
 * @param[in]  device_name   Name des SAUL-Geräts (z. B. "bme280")
 * @param[in]  request_unit  Gewünschte Einheit für den Luftdruck (z. B. "hPa")
 * @param[out] json_buffer   Puffer zum Speichern der JSON-formatierten Ausgabe
 * @param[out] json_size     Größe der generierten JSON-Daten
 * 
 * @return 0 bei Erfolg, negativer Fehlercode bei Fehlschlag.
 */
int read_bme280_pressure (const char* device_name, const char* request_unit, char* json_buffer, size_t* json_size);
/**
 * @brief Liest die Luftfeuchtigkeit vom BME280-Sensor und speichert das Ergebnis im JSON-Format.
 * 
 * @param[in]  device_name   Name des SAUL-Geräts (z. B. "bme280")
 * @param[out] json_buffer   Puffer zum Speichern der JSON-formatierten Ausgabe
 * @param[out] json_size     Größe der generierten JSON-Daten
 * 
 * @return 0 bei Erfolg, negativer Fehlercode bei Fehlschlag.
 */
int read_bme280_humidity (const char* device_name, char* json_buffer, size_t* json_size);

#endif  /* SAUL_UTILS_H */