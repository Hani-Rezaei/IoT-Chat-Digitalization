
#ifndef SAUL_UTILS_H
#define SAUL_UTILS_H

#include <stdio.h>
#include "saul_reg.h"
#include "shell.h"

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

extern const char* nrf_temp;
extern const char* bme_280_name;

int read_device_values(saul_reg_t* );
int read_saul_reg_dev (const char* );

#endif  /* SAUL_UTILS_H */