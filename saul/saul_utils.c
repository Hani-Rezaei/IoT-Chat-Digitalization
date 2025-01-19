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

    *output++ = '"'; // Öffnendes äußeres Anführungszeichen hinzufügen
    while (*input) {
        if (*input == '"') {
            *output++ = '\\'; // Escape-Zeichen hinzufügen
        }
        *output++ = *input++;
    }
    *output++ = '"'; // Schließendes äußeres Anführungszeichen hinzufügen
    *output = '\0'; // Null-Terminierung
}

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
                LOG_INFO("Temperaturgerät gefunden: SAUL_SENSE_TEMP\n");
                break;

            case SAUL_SENSE_PRESS:
                LOG_INFO("Druckgerät gefunden: SAUL_SENSE_PRESS\n");
                break;

            case SAUL_SENSE_HUM:
                LOG_INFO("Feuchtigkeitsgerät gefunden: SAUL_SENSE_HUM\n");
                break;

            default:
                LOG_WARNING("Unbekannter Gerätetyp: %d\n", device->driver->type);
                device = device->next;
                continue; // Zum nächsten Gerät wechseln
        }
        
        // // JSON-Daten aus dem Gerät lesen
        // char* json_buffer = NULL;
        // int status = read_device_values(device, &json_buffer);
        // // Fehlerprüfung
        // if (status != 0) {
        //     LOG_ERROR("Fehler beim Lesen von Werten des Geräts '%s', Fehlercode: %d\n",
        //               device->name, status);
        // } else {
        //     // JSON-Daten verarbeiten
        //     LOG_INFO("JSON-Daten für Gerät '%s': %s\n", device->name, json_buffer);

        //     // JSON-Puffer freigeben
        //     free(json_buffer);
        // }
        // Zum nächsten Gerät wechseln
        device = device->next;
    }

    return 0; // Erfolgreicher Abschluss
}

int read_bme280_temperature (const char* device_name, char* escape_json_buffer, size_t* json_size){

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
            char json_buffer[1024];  // Angemessene Puffergröße
            LOG_INFO("json_size: %zu\n", *json_size);

            // Schritt 2: JSON in den übergebenen Puffer schreiben
            if (escape_json_buffer != NULL && *json_size > 0) {
                phydat_to_json(&result, num_elements, json_buffer);  // JSON-Daten in den Puffer schreiben
                escape_json(json_buffer, escape_json_buffer);
                printf("JSON ohne \": %s\n", json_buffer); // Ausgabe der JSON-Daten
                printf("JSON mit \": %s\n", escape_json_buffer); // Ausgabe der JSON-Daten
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

int read_bme280_humidity (const char* device_name, char* json_buffer, size_t* json_size){

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
            LOG_INFO("json_size: %zu\n", *json_size);

            // Schritt 2: JSON in den übergebenen Puffer schreiben
            if (json_buffer != NULL && *json_size > 0) {
                phydat_to_json(&result, num_elements, json_buffer);  // JSON-Daten in den Puffer schreiben
                printf("JSON: %s\n", json_buffer); // Ausgabe der JSON-Daten
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

int read_bme280_pressure (const char* device_name, char* json_buffer, size_t* json_size){

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
            LOG_INFO("Werte des Geräts '%s' (Klasse: %d):\n", device->name, device->driver->type);
            for (int i = 0; i < num_elements; i++) {
                LOG_INFO("  Wert[%d] = %d, Einheit = %d, Skalierung = %d\n",
                        i, result.val[i], result.unit, result.scale);
            }

            // Schritt 1: Puffergröße ermitteln (maximale Größe)
            *json_size = phydat_to_json(&result, num_elements, NULL);  // json_buffer = NULL, nur Größe berechnen
            LOG_INFO("json_size: %zu\n", *json_size);

            // Schritt 2: JSON in den übergebenen Puffer schreiben
            if (json_buffer != NULL && *json_size > 0) {
                phydat_to_json(&result, num_elements, json_buffer);  // JSON-Daten in den Puffer schreiben
                printf("JSON: %s\n", json_buffer); // Ausgabe der JSON-Daten
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
