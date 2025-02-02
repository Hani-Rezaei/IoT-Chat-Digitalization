
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

    // Iteration über die Geräte in der Liste
    while (device != NULL) {
        // Gerätedetails ausgeben
        LOG_INFO("Gerät: %s, Klasse: %d\n", device->name, device->driver->type);

        // Gerätetyp überprüfen und Werte lesen
        if (strcmp(device->name, device_name) == 0)
        {
            LOG_INFO("%s\n", device->name);
            
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
            } else {
                LOG_INFO("'%s' erfolgreich eingeschaltet!\n", device->name);
            }
            
            int num_elements = saul_reg_read(device, &result);
            LOG_INFO("NACH number of elements: %d\n", num_elements);

            LOG_INFO("NACH DEM WRITE  Wert[0] = %d, Einheit = %d, Skalierung = %d\n",
                    result.val[0], result.unit, result.scale);
        }
        
        // Zum nächsten Gerät wechseln
        device = device->next;
        LOG_INFO("NEXT Gerät: %s, Klasse: %d\n", device->name, device->driver->type);
    }

    return 0; // Erfolgreicher Abschluss
}