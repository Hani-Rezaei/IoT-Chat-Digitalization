/**
* @file    main.c
* @brief   Beschreibung des Moduls oder der Datei.
* @details Ausführlichere Informationen, falls erforderlich.
* @author  Mohammadjavad Esmaeili
* @date    05.01.2025
*/

#include <stdio.h>
#include "saul_reg.h"
#include "shell.h"

#include "saul_utils.h"
//////////////////////////
///////// Log
#include "log.h"
#include <stdlib.h>

///////// LED
#include "clk.h"
#include "board.h"
#include "periph_conf.h"
#include "timex.h"
#include "ztimer.h"

/////////////////////////

// saul
// ID	Class		Name
// #0	SENSE_BTN	Button 1
// #1	SENSE_BTN	Button 2
// #2	SENSE_BTN	Button 3
// #3	SENSE_BTN	Button 4
// #4	ACT_DIMMER	LED 1
// #5	ACT_DIMMER	LED 2
// #6	ACT_DIMMER	LED 3
// #7	ACT_DIMMER	LED 4
// #8	SENSE_TEMP	NRF_TEMP
// #9	SENSE_TEMP	bme280
// #10	SENSE_PRESS	bme280
// #11	SENSE_HUM	bme280

/**
 * @brief Beschreibung der globalen Variable.
 */
const char* nrf_temp = "NRF_TEMP";
const char* bme_280_name = "bme280";

int read_device_values(saul_reg_t* saul_device) {
    // Nullprüfung des Geräts
    if (saul_device == NULL) {
        LOG_ERROR("Das übergebene Gerät ist NULL\n");
        return -1; // Fehlercode
    }

    // Initialisierung der Datenstruktur
    phydat_t result;
    int num_elements = saul_reg_read(saul_device, &result);
    LOG_INFO("number of elements: %d\n", num_elements);
    
    // Überprüfen, ob das Lesen erfolgreich war
    if (num_elements <= 0) {
        LOG_ERROR("Fehler beim Lesen des Geräts '%s' (Klasse: %d)\n",
                  saul_device->name, saul_device->driver->type);
        return -2; // Fehlercode
    }

    // Ausgabe der gelesenen Werte
    LOG_INFO("Werte des Geräts '%s' (Klasse: %d):\n", saul_device->name, saul_device->driver->type);
    for (int i = 0; i < num_elements; i++) {
        LOG_INFO("  Wert[%d] = %d, Einheit = %d, Skalierung = %d\n",
                 i, result.val[i], result.unit, result.scale);
    }

    // Schritt 1: Puffergröße ermitteln
    size_t json_size = phydat_to_json(&result, num_elements, NULL);  // json_buffer = NULL, nur Größe berechnen
    LOG_INFO("json_size: %d\n", json_size);

    // Schritt 2: Puffer zuweisen
    char* json_buffer = malloc(json_size);  // malloc gibt ausreichend Speicherplatz

    // Schritt 3: JSON in den Puffer schreiben
    if (json_buffer != NULL) {
        phydat_to_json(&result, 1, json_buffer);
        // Jetzt kann json_buffer den JSON-String enthalten
        printf("JSON: %s\n", json_buffer);
        
        // Nicht vergessen: den Speicher freizugeben
        free(json_buffer);
    } else {
        // Fehlerbehandlung, falls malloc fehlschlägt
        LOG_ERROR("Speicherzuweisung fehlgeschlagen\n");
    }

    return 0; // Erfolgreicher Abschluss
}

/**
 * @brief   Eine kurze Zusammenfassung der Funktion.
 * @details Ausführlichere Beschreibung, falls erforderlich.
 *
 * @param[in]  param1   Beschreibung des ersten Parameters.
 * @param[out] param2   Beschreibung des zweiten Parameters.
 * @return     Beschreibung des Rückgabewerts.
 */
int read_saul_reg_dev (const char* device_name){

    // Findet das Gerät anhand des Namens
    saul_reg_t* device = saul_reg_find_name(device_name);

    // Nullprüfung für das gefundene Gerät
    if (device == NULL) {
        LOG_ERROR("Kein Gerät mit dem Namen '%s' gefunden\n", device_name);
        return -1; // Fehlercode zurückgeben
    }

    // Iteration über die Geräte in der Liste
    while (device != NULL) {
        // Gerätedetails ausgeben
        LOG_INFO("Gerät: %s, Klasse: %d\n", device->name, device->driver->type);

        // Gerätetyp überprüfen und Werte lesen
        switch (device->driver->type) {
            case SAUL_SENSE_TEMP:
                LOG_INFO("Temperaturgerät gefunden\n");
                break;

            case SAUL_SENSE_PRESS:
                LOG_INFO("Druckgerät gefunden\n");
                break;

            case SAUL_SENSE_HUM:
                LOG_INFO("Feuchtigkeitsgerät gefunden\n");
                break;

            default:
                LOG_WARNING("Unbekannter Gerätetyp: %d\n", device->driver->type);
                device = device->next;
                continue; // Zum nächsten Gerät wechseln
        }

        // Gemeinsame Funktion für das Lesen und Verarbeiten von Werten
        if (read_device_values(device) != 0) {
            LOG_ERROR("Fehler beim Lesen von Werten des Geräts '%s'\n", device->name);
        }

        // Zum nächsten Gerät wechseln
        device = device->next;
    }

    return 0; // Erfolgreicher Abschluss
}