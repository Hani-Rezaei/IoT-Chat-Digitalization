
#include <stdio.h>
#include "saul_reg.h"
#include "shell.h"

//////////////////////////
// #include "led.h"
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

const char* nrf_temp = "NRF_TEMP";
const char* bme_280_name = "bme280";

saul_reg_t* find_saul_reg_dev_with_name (const char* dev_name){
    saul_reg_t* saul_reg_dev = saul_reg_find_name(dev_name);
    if (saul_reg_dev != NULL)
        return saul_reg_dev;
    
    return NULL;
}

// saul_reg_t* find_saul_reg_with_name_type (const char* sensor_name){
//     saul_reg_t* saul_bme280 = saul_reg_find_name(sensor_name);
//     if (saul_bme280 != NULL)
//         return saul_bme280;
    
//     return NULL;
// }

int read_saul_reg_dev (saul_reg_t* dev, phydat_t* result){

    saul_reg_t* device = saul_reg_find_name(bme_280_name);

    while (device != NULL) {
        // Klasse und Name des aktuellen Geräts prüfen
        printf("Gerät: %s, Klasse: %d\n", device->name, device->driver->type);

        if (device->driver->type == SAUL_SENSE_TEMP) {
            printf("Temperaturgerät gefunden:\n");
            read_and_print_device_values(device);
        } else if (device->driver->type == SAUL_SENSE_PRESS) {
            printf("Druckgerät gefunden:\n");
            read_and_print_device_values(device);
        } else if (device->driver->type == SAUL_SENSE_HUM) {
            printf("Feuchtigkeitsgerät gefunden:\n");
            read_and_print_device_values(device);
        } 
        
        // Zum nächsten Gerät wechseln
        device = device->next;
    }
}

void read_and_print_device_values(saul_reg_t* saul_device) {
    if (saul_device == NULL) {
        printf("Gerät ist NULL!\n");
        return;
    }

    phydat_t result;
    int num_elements = saul_reg_read(saul_device, &result);
    if (num_elements > 0) {
        for (int i = 0; i < num_elements; i++) {
            printf("Wert[%d]: %d, Einheit: %d, Skalierung: %d\n", i, result.val[i], result.unit, result.scale);
        }
    } else {
        printf("Fehler beim Lesen von Gerät: %s\n", saul_device->name);
    }
}

int cmd_handler(int argc, char **argv)
{
    /* ... */
    // saul_reg_t* saul_bme280 = find_saul_reg_with_name(bme_280_name);
    
    // //saul_bme280 = saul_reg_find_type_and_name(9,bme_280_name);

    // if (saul_bme280 != NULL)
    // {
    //     printf("saul gefunden!\n");
    //     phydat_t result;
    //     int element_number_saul_reg_read = read_saul_reg(saul_bme280, &result);
    //     if (element_number_saul_reg_read >= 0)
    //     {
    //         printf("element_number_saul_reg_read = %d\n", element_number_saul_reg_read);
    //         // for (int i = 0; i < (int)PHYDAT_DIM; i++) {
    //         //     if (i < element_number_saul_reg_read) { // Nur gültige Werte ausgeben
    //         //         printf("Wert[%d]: %d, Einheit: %d, Skalierung: %d\n",
    //         //             i, result.val[i], result.unit, result.scale);
    //         //     }
    //         // }
    //         for (int i = 0; i < 3; i++) {
    //            printf("Wert[%d]: %d, Einheit: %d, Skalierung: %d\n",
    //            i, result.val[i], result.unit, result.scale);
    //         }
    //     }
    //     else{
    //         printf("element_number_saul_reg_read  < 0 = %d\n", element_number_saul_reg_read);
    //         return -1;
    //     }
    // }
    // else
    // {
    //     printf("saul NICHT gefunden");
    //     return -1;
    // }   



    (void)argc;
    (void)argv;

    return 0;
}

// static const shell_command_t commands[] = {
//     //    { "command name", "command description", cmd_handler }, 
//     { "cmd_handler", NULL, cmd_handler }
// };

int main(void)
{
    puts("Welcome to RIOT!\n");
    puts("Type `help` for help, type `saul` to see all SAUL devices\n");

    printf("This application runs on %s\n", RIOT_BOARD);


    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(commands, line_buf, SHELL_DEFAULT_BUFSIZE);
    // shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
