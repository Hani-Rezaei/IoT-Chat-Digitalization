/**
 * @file    saul_utils.c
 * @brief   Funktionen zum Lesen und Verarbeiten von SAUL-Geräten und deren Werten.
 * @details Diese Datei enthält Funktionen, die mit der SAUL-Geräteliste interagieren und 
 *          Sensordaten auslesen. Die ausgelesenen Daten werden in JSON-Format konvertiert. 
 *          Es werden auch Geräteinformationen zu bestimmten Gerätetypen verarbeitet und geloggt.
 * 
 *          Die unterstützten Gerätetypen sind:
 *          - Temperaturgeräte (SAUL_SENSE_TEMP)
 *          - Druckgeräte (SAUL_SENSE_PRESS)
 *          - Feuchtigkeitsgeräte (SAUL_SENSE_HUM)
 * 
 *          Weitere Gerätetypen und deren Verarbeitung können in Zukunft hinzugefügt werden.
 * @author  Mohammadjavad Esmaeili
 */

#include "saul_utils.h"

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

const char* nrf_temp = "NRF_TEMP";
const char* bme_280_name = "bme280";

void escape_json(const char *input, char *output) {

    // *output++ = '"'; // Öffnendes äußeres Anführungszeichen hinzufügen
    while (*input) {
        if (*input == '"') {
            *output++ = '\\'; // Escape-Zeichen hinzufügen
        }
        *output++ = *input++;
    }
    //*output++ = '"'; // Schließendes äußeres Anführungszeichen hinzufügen
    *output = '\0'; // Null-Terminierung
}

int read_bme280_temperature (const char* device_name, const char* request_unit, char* escape_json_buffer, size_t* json_size){

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
        if (device->driver->type == SAUL_SENSE_TEMP)
        {
            LOG_INFO("Temperaturgerät gefunden: SAUL_SENSE_TEMP\n");
            
            // Initialisierung der Datenstruktur
            phydat_t result;
            int num_elements = saul_reg_read(device, &result);
            LOG_INFO("num_elements:%d\n",num_elements);
            // Überprüfen, ob das Lesen erfolgreich war
            if (num_elements <= 0) {
                LOG_ERROR("Fehler beim Lesen des Geräts '%s' (Klasse: %d)\n",
                        device->name, device->driver->type);
                return -1; // Fehlercode
            }

            // Verarbeitung der Rohdaten mit Skalierung
            for (int i = 0; i < num_elements; i++) {
                if (strcmp(request_unit, "\\u00b0C") == 0 && result.unit == UNIT_TEMP_C) {
                    LOG_INFO("Einheit ist bereits C, keine Konvertierung nötig.\n");
                } else if (strcmp(request_unit, "\\u00b0F") == 0 && result.unit == UNIT_TEMP_C) {
                    // Rohwert skalieren: tatsächlicher Wert = val * 10^scale
                    float scaled_value = result.val[i] * pow(10, result.scale);
                    // Celsius nach Fahrenheit konvertieren
                    scaled_value = (scaled_value * 9 / 5) + 32;
                    result.unit = UNIT_TEMP_F;
                    // Rückskalierung in die phydat-Struktur
                    result.val[i] = (int16_t)(scaled_value * 100); // In Integer konvertieren (z. B. für 2 Dezimalstellen)
                    result.scale = -2; // Neue Skalierung setzen
                    LOG_INFO("Konvertierung nach Fahrenheit durchgeführt: %.2f °F\n", scaled_value);
                } else {
                    LOG_ERROR("Angeforderte Einheit '%s' wird nicht unterstützt oder passt nicht zum Gerät.\n", request_unit);
                    return -3; // Fehlercode für ungültige Einheit
                }
            }

            // Ausgabe der gelesenen Werte
            LOG_INFO("NACH DEM KONVERTIEREN\n");
            LOG_INFO("Werte des Geräts '%s' (Klasse: %d):\n", device->name, device->driver->type);
            LOG_INFO("num_elements: %d\n", num_elements);
            for (int i = 0; i < num_elements; i++) {
                LOG_INFO("  Wert[%d] = %d, Einheit = %d, Skalierung = %d\n",
                        i, result.val[i], result.unit, result.scale);
            }

            // Schritt 1: Puffergröße ermitteln (maximale Größe)
            *json_size = phydat_to_json(&result, num_elements, NULL);  // json_buffer = NULL, nur Größe berechnen
            if (*json_size == 0) {
                LOG_ERROR("Fehler bei der Berechnung der JSON-Größe.\n");
                return -2; // Fehlercode: Ungültige JSON-Größe
            }
            // Schritt 2: Dynamischer Speicher für JSON-Daten
            char json_buffer[1024]; // Speicher für JSON-Puffer
            LOG_INFO("json_size: %zu\n", *json_size);

            phydat_to_json(&result, num_elements, json_buffer);  // JSON-Daten in den Puffer schreiben

            if (escape_json_buffer != NULL) {
                // escape_json(json_buffer, escape_json_buffer);
                strncpy(escape_json_buffer, json_buffer, *json_size);

                printf("JSON \": %s\n", escape_json_buffer); // Ausgabe der JSON-Daten
            } else {
                LOG_ERROR("Fehler: Ungültiger Puffer oder Größe\n");
                return -4; // Fehlercode
            }
            break; // Erfolgreicher Abschluss
        }
        // Zum nächsten Gerät wechseln
        device = device->next;
    }

    return 0; // Erfolgreicher Abschluss
}

int read_bme280_humidity (const char* device_name, char* escape_json_buffer, size_t* json_size){

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
        if (device->driver->type == SAUL_SENSE_HUM)
        {
            LOG_INFO("Temperaturgerät gefunden: SAUL_SENSE_TEMP\n");
            
            // Initialisierung der Datenstruktur
            phydat_t result;
            int num_elements = saul_reg_read(device, &result);
            LOG_INFO("number of elements: %d\n", num_elements);
            
            // Überprüfen, ob das Lesen erfolgreich war
            if (num_elements <= 0) {
                LOG_ERROR("Fehler beim Lesen des Geräts '%s' (Klasse: %d)\n",
                        device->name, device->driver->type);
                return -1; // Fehlercode
            }

            // Ausgabe der gelesenen Werte
            LOG_INFO("Werte des Geräts '%s' (Klasse: %d):\n", device->name, device->driver->type);
            for (int i = 0; i < num_elements; i++) {
                LOG_INFO("  Wert[%d] = %d, Einheit = %d, Skalierung = %d\n",
                        i, result.val[i], result.unit, result.scale);
            }
            
            // Schritt 1: Puffergröße ermitteln (maximale Größe)
            *json_size = phydat_to_json(&result, num_elements, NULL);  // json_buffer = NULL, nur Größe berechnen
            char json_buffer[512];  // Angemessene Puffergröße
            LOG_INFO("json_size: %zu\n", *json_size);

            // Schritt 2: JSON in den übergebenen Puffer schreiben
            if (escape_json_buffer != NULL && *json_size > 0) {
                phydat_to_json(&result, num_elements, json_buffer);  // JSON-Daten in den Puffer schreiben
                strncpy(escape_json_buffer, json_buffer, *json_size);
                printf("JSON: %s\n", escape_json_buffer); // Ausgabe der JSON-Daten
            } else {
                LOG_ERROR("Fehler: Ungültiger Puffer oder Größe\n");
                return -2; // Fehlercode
            }
            return 0; // Erfolgreicher Abschluss
        }
        else
            // Zum nächsten Gerät wechseln
            device = device->next;
    }

    return 0; // Erfolgreicher Abschluss
}

int read_bme280_pressure (const char* device_name, const char* request_unit, char* escape_json_buffer, size_t* json_size){

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
        if (device->driver->type == SAUL_SENSE_PRESS)
        {
            LOG_INFO("Temperaturgerät gefunden: SAUL_SENSE_TEMP\n");
            
            // Initialisierung der Datenstruktur
            phydat_t result;
            int num_elements = saul_reg_read(device, &result);
            LOG_INFO("number of elements: %d\n", num_elements);
            
            // Überprüfen, ob das Lesen erfolgreich war
            if (num_elements <= 0) {
                LOG_ERROR("Fehler beim Lesen des Geräts '%s' (Klasse: %d)\n",
                        device->name, device->driver->type);
                return -1; // Fehlercode
            }

            // Ausgabe der gelesenen Werte
            LOG_INFO("VOR DEM KONVERTIEREN\n");
            LOG_INFO("Werte des Geräts '%s' (Klasse: %d):\n", device->name, device->driver->type);
            LOG_INFO("num_elements: %d\n", num_elements);
            for (int i = 0; i < num_elements; i++) {
                LOG_INFO("  Wert[%d] = %d, Einheit = %d, Skalierung = %d\n",
                        i, result.val[i], result.unit, result.scale);
            }
            
            // Verarbeitung der Rohdaten mit Skalierung
            for (int i = 0; i < num_elements; i++) {
                if (strcmp(request_unit, "Pa") == 0 && result.unit == UNIT_PA) {
                    LOG_INFO("Einheit ist bereits °C, keine Konvertierung nötig.\n");
                } else if (strcmp(request_unit, "Bar") == 0 && result.unit == UNIT_PA) {
                    // Skalierung und Konvertierung
                    float normalized_value = result.val[i] * pow(10, result.scale); // Wert in Pascal
                    float bar_value = normalized_value / 100000.0f;
                    
                    // Debugging der Zwischenschritte
                    printf("Rohwert in Pascal: %f\n", normalized_value);
                    printf("Wert in Bar: %f\n", bar_value);

                    // Vergewissern, dass der Wert in einem sinnvollen Bereich liegt
                    if (bar_value < -327.68 || bar_value > 327.67) {  // Anpassen der Grenzen basierend auf der Präzision
                        LOG_ERROR("Fehler: Der Wert in Bar überschreitet die Grenzen des zulässigen Bereichs: %.5f\n", bar_value);
                        return -1; // Fehlercode, wenn der Wert außerhalb des zulässigen Bereichs ist
                    }

                    result.val[i] = (int16_t)(bar_value * 100.0f);  // Umrechnung in Bar
                    result.unit = UNIT_BAR;                         // Einheit auf Bar setzen
                    result.scale = -2;                              // Neue Skalierung setzen, um Bar zu repräsentieren (Skalierung von 100)            
                    LOG_INFO("Konvertierung von Pa nach Bar durchgeführt: %.5f Bar\n", bar_value);
                } else {
                    LOG_ERROR("Angeforderte Einheit '%s' wird nicht unterstützt oder passt nicht zum Gerät.\n", request_unit);
                    return -3; // Fehlercode für ungültige Einheit
                }
            }

            // Ausgabe der gelesenen Werte
            LOG_INFO("NACH DEM KONVERTIEREN\n");
            LOG_INFO("Werte des Geräts '%s' (Klasse: %d):\n", device->name, device->driver->type);
            for (int i = 0; i < num_elements; i++) {
                LOG_INFO("  Wert[%d] = %d, Einheit = %d, Skalierung = %d\n",
                        i, result.val[i], result.unit, result.scale);
            }

            // Schritt 1: Puffergröße ermitteln (maximale Größe)
            *json_size = phydat_to_json(&result, num_elements, NULL);  // json_buffer = NULL, nur Größe berechnen
            char json_buffer[512];  // Angemessene Puffergröße
            LOG_INFO("json_size: %zu\n", *json_size);

            // Schritt 2: JSON in den übergebenen Puffer schreiben
            if (escape_json_buffer != NULL && *json_size > 0) {
                phydat_to_json(&result, num_elements, json_buffer);  // JSON-Daten in den Puffer schreiben
                strncpy(escape_json_buffer, json_buffer, *json_size);
                printf("JSON: %s\n", escape_json_buffer); // Ausgabe der JSON-Daten
            } else {
                LOG_ERROR("Fehler: Ungültiger Puffer oder Größe\n");
                return -2; // Fehlercode
            }
            return 0; // Erfolgreicher Abschluss
        }
        else
            // Zum nächsten Gerät wechseln
            device = device->next;
    }

    return 0; // Erfolgreicher Abschluss
}
