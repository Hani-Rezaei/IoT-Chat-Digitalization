
#include "led_utils.h"
#include "saul_utils.h"

int led_set_state (const char* device_name, uint8_t is_on){

    // Findet das Gerät anhand des Namens
    saul_reg_t* device = saul_reg_find_name(device_name);
    if (!device) {
        LOG_ERROR("Kein Gerät mit dem Namen '%s' gefunden\n", device_name);
        return -1; // Fehler: Gerät nicht gefunden
    }

    // Daten für das Gerät setzen
    phydat_t result = {
        .val[0] = is_on ? 255 : 0,  // Setzt den Wert je nach is_on
        .unit = 0,
        .scale = 0
    };

    // Schreiben des neuen Werts ins Gerät
    if (saul_reg_write(device, &result) < 0) {
        LOG_ERROR("Fehler beim Setzen des Zustands für '%s'\n", device->name);
        return -2; // Fehler: Schreibvorgang fehlgeschlagen
    }

    LOG_INFO("Gerät '%s' erfolgreich auf %s geschaltet!\n", device->name, is_on ? "EIN" : "AUS");
    return 0; // Erfolg
}