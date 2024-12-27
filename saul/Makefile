# name of your application
APPLICATION = saul_example

# If no BOARD is found in the environment, use this default:
BOARD ?= native

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../../RIOT

# we want to use SAUL:
USEMODULE += saul_default
# include the shell:
USEMODULE += shell_cmds_default
# additional modules for debugging:
USEMODULE += ps

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# BME280 connected via I2C
USEMODULE += bme280_i2c
# When using I2C, specify the default I2C device to use,
# and the BME280's address (see datasheet).
# The values below are the defaults:
CFLAGS += -DBMX280_PARAM_I2C_DEV="I2C_DEV(0)"
CFLAGS += -DBMX280_PARAM_I2C_ADDR=0x77

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

include $(RIOTBASE)/Makefile.include
