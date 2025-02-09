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

/**
* @brief Liest Temperaturdaten von einem BME280-Sensor und konvertiert sie in die angeforderte Einheit.
*
* @param device_name Name des Geräts, von dem gelesen werden soll.
* @param request_unit Gewünschte Temperatureinheit (z. B. „\u00b0C“ oder „\u00b0F“).
* @param escape_json_buffer Puffer zum Speichern der resultierenden JSON-Daten.
* @param json_size Zeiger zum Speichern der Größe der generierten JSON-Daten.
*
* @return 0 bei Erfolg, negativer Fehlercode bei Fehler.
*/
int read_bme280_temperature (const char* device_name, const char* request_unit, char* escape_json_buffer, size_t* json_size){

    // Findet das Gerät anhand des Namens
    saul_reg_t* device = saul_reg_find_name(device_name);
    if (!device) {
        LOG_ERROR("Kein Gerät mit dem Namen '%s' gefunden\n", device_name);
        return -1; // Fehler: Gerät nicht gefunden
    }

    // Iteration über die Geräte in der Liste
    while (device) {
        // Gerätedetails ausgeben
        LOG_INFO("Gerät: %s, Klasse: %d\n", device->name, device->driver->type);

        // Überprüfe den Gerätetyp und lese die Werte
        if (device->driver->type == SAUL_SENSE_TEMP)
        {            
            phydat_t result;
            int num_elements = saul_reg_read(device, &result);
            if (num_elements <= 0) {
                LOG_ERROR("Fehler beim Lesen des Geräts '%s' (Klasse: %d)\n",
                        device->name, device->driver->type);
                return -2; // Fehlercode
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

            // Berechne die JSON-Größe
            *json_size = phydat_to_json(&result, num_elements, NULL);  // json_buffer = NULL, nur Größe berechnen
            if (*json_size == 0) {
                LOG_ERROR("Fehler bei der Berechnung der JSON-Größe.\n");
                return -4; // Fehlercode: Ungültige JSON-Größe
            }
            // JSON-Daten in den Puffer schreiben
            char json_buffer[PUFFER_SIZE]; // Speicher für JSON-Puffer
            phydat_to_json(&result, num_elements, json_buffer);  // JSON-Daten in den Puffer schreiben

            // JSON-Daten in den übergebenen Puffer kopieren
            if (escape_json_buffer != NULL) {
                strncpy(escape_json_buffer, json_buffer, *json_size);
                LOG_INFO("JSON \": %s\n", escape_json_buffer); // Ausgabe der JSON-Daten
                return 0; // Erfolgreicher Abschluss
            } else {
                LOG_ERROR("Fehler: Ungültiger Puffer oder Größe\n");
                return -5; // Fehlercode
            }
        }
        // Zum nächsten Gerät wechseln
        device = device->next;
    }
    return -1;
}

/**
* @brief Liest Luftfeuchtigkeit von einem BME280-Sensor
*
* @param device_name Name des Geräts, von dem gelesen werden soll.
* @param request_unit Gewünschte Luftfeuchtigkeit Einheit („%“).
* @param escape_json_buffer Puffer zum Speichern der resultierenden JSON-Daten.
* @param json_size Zeiger zum Speichern der Größe der generierten JSON-Daten.
*
* @return 0 bei Erfolg, negativer Fehlercode bei Fehler.
*/
int read_bme280_humidity (const char* device_name, char* escape_json_buffer, size_t* json_size){

    // Findet das Gerät anhand des Namens
    saul_reg_t* device = saul_reg_find_name(device_name);
    if (!device) {
        LOG_ERROR("Kein Gerät mit dem Namen '%s' gefunden\n", device_name);
        return -1; // Fehler: Gerät nicht gefunden
    }

    // Iteration über die Geräte in der Liste
    while (device) {
        // Gerätedetails ausgeben
        LOG_INFO("Gerät: %s, Klasse: %d\n", device->name, device->driver->type);

        // Gerätetyp überprüfen und Werte lesen
        if (device->driver->type == SAUL_SENSE_HUM)
        {            
            phydat_t result;
            int num_elements = saul_reg_read(device, &result);
            if (num_elements <= 0) {
                LOG_ERROR("Fehler beim Lesen des Geräts '%s' (Klasse: %d)\n",
                        device->name, device->driver->type);
                return -2; // Fehlercode
            }
            
            // Berechne die JSON-Größe
            *json_size = phydat_to_json(&result, num_elements, NULL);  // json_buffer = NULL, nur Größe berechnen
            if (*json_size == 0) {
                LOG_ERROR("Fehler bei der Berechnung der JSON-Größe.\n");
                return -3; // Fehlercode: Ungültige JSON-Größe
            }
            // JSON-Daten in den Puffer schreiben
            char json_buffer[PUFFER_SIZE]; // Speicher für JSON-Puffer
            phydat_to_json(&result, num_elements, json_buffer);  // JSON-Daten in den Puffer schreiben
            if (escape_json_buffer != NULL) {
                strncpy(escape_json_buffer, json_buffer, *json_size);
                LOG_INFO("JSON \": %s\n", escape_json_buffer); // Ausgabe der JSON-Daten
                return 0; // Erfolgreicher Abschluss
            } else {
                LOG_ERROR("Fehler: Ungültiger Puffer oder Größe\n");
                return -4; // Fehlercode
            }
        }
        // Zum nächsten Gerät wechseln
        device = device->next;
    }
    return -1;
}

/**
* @brief Liest Luftdruck von einem BME280-Sensor und konvertiert sie in die angeforderte Einheit.
*
* @param device_name Name des Geräts, von dem gelesen werden soll.
* @param request_unit Gewünschte Luftfeuchtigkeit Einheit („Pa“ oder „Bar“).
* @param escape_json_buffer Puffer zum Speichern der resultierenden JSON-Daten.
* @param json_size Zeiger zum Speichern der Größe der generierten JSON-Daten.
*
* @return 0 bei Erfolg, negativer Fehlercode bei Fehler.
*/
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
            phydat_t result;
            int num_elements = saul_reg_read(device, &result);
            if (num_elements <= 0) {
                LOG_ERROR("Fehler beim Lesen des Geräts '%s' (Klasse: %d)\n",
                        device->name, device->driver->type);
                return -2; // Fehlercode
            }
                     
            // Verarbeitung der Rohdaten mit Skalierung
            for (int i = 0; i < num_elements; i++) {
                if (strcmp(request_unit, "Pa") == 0 && result.unit == UNIT_PA) {
                    LOG_INFO("Einheit ist bereits °C, keine Konvertierung nötig.\n");
                } else if (strcmp(request_unit, "Bar") == 0 && result.unit == UNIT_PA) {
                    // Skalierung und Konvertierung
                    float normalized_value = result.val[i] * pow(10, result.scale); // Wert in Pascal
                    float bar_value = normalized_value / 100000.0f;
                    
                    // Vergewissern, dass der Wert in einem sinnvollen Bereich liegt
                    if (bar_value < -327.68 || bar_value > 327.67) {  // Anpassen der Grenzen basierend auf der Präzision
                        LOG_ERROR("Fehler: Der Wert in Bar überschreitet die Grenzen des zulässigen Bereichs: %.5f\n", bar_value);
                        return -3; // Fehlercode, wenn der Wert außerhalb des zulässigen Bereichs ist
                    }
                    result.val[i] = (int16_t)(bar_value * 100.0f);  // Umrechnung in Bar
                    result.unit = UNIT_BAR;                         // Einheit auf Bar setzen
                    result.scale = -2;                              // Neue Skalierung setzen, um Bar zu repräsentieren (Skalierung von 100)            
                    LOG_INFO("Konvertierung von Pa nach Bar durchgeführt: %.5f Bar\n", bar_value);
                } else {
                    LOG_ERROR("Angeforderte Einheit '%s' wird nicht unterstützt oder passt nicht zum Gerät.\n", request_unit);
                    return -4; // Fehlercode für ungültige Einheit
                }
            }

            // Berechne die JSON-Größe
            *json_size = phydat_to_json(&result, num_elements, NULL);  // json_buffer = NULL, nur Größe berechnen
            if (*json_size == 0) {
                LOG_ERROR("Fehler bei der Berechnung der JSON-Größe.\n");
                return -5; // Fehlercode: Ungültige JSON-Größe
            }
            // JSON-Daten in den Puffer schreiben
            char json_buffer[PUFFER_SIZE]; // Speicher für JSON-Puffer
            phydat_to_json(&result, num_elements, json_buffer);  // JSON-Daten in den Puffer schreiben
            if (escape_json_buffer != NULL) {
                strncpy(escape_json_buffer, json_buffer, *json_size);
                LOG_INFO("JSON \": %s\n", escape_json_buffer); // Ausgabe der JSON-Daten
                return 0; // Erfolgreicher Abschluss
            } else {
                LOG_ERROR("Fehler: Ungültiger Puffer oder Größe\n");
                return -6; // Fehlercode
            }
        }
        // Zum nächsten Gerät wechseln
        device = device->next;
    }
    return -1;
}
