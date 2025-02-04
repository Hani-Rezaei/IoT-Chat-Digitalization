
#include "led_utils.h"
#include "saul_utils.h"

int led_conf (const char* device_name, uint8_t is_on){

    // Findet das Gerät anhand des Namens
    saul_reg_t* device = saul_reg_find_name(device_name);

    // Nullprüfung für das gefundene Gerät
    if (device == NULL) {
        LOG_ERROR("Kein Gerät mit dem Namen '%s' gefunden\n", device_name);
        return -1; // Fehlercode zurückgeben
    }

    // Gerätetyp überprüfen und Werte lesen
    if (strcmp(device->name, device_name) == 0)
    {        
        phydat_t result;
        
        if (is_on == 1)
            result.val[0] = 255; 
        else
            result.val[0] = 0;
        
        result.unit = 0;
        result.scale = 0;

        int res = saul_reg_write(device, &result);
        if (res < 0) {
            LOG_ERROR("Fehler beim Einschalten von '%s'\n", device->name);
            return -2; // Fehlercode zurückgeben
        } else {
            LOG_INFO("'%s' erfolgreich ein/aus geschaltet!\n", device->name);
        }
    }        
    return 0; // Erfolgreicher Abschluss
}