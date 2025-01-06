# name of your application
APPLICATION = saul_example

# If no BOARD is found in the environment, use this default:
BOARD ?= native

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../../RIOT

# Add source files in subfolders manually
# SRC += main.c
# SRC += /src/saul_utils.c
# SRC += $(wildcard *.c saul_utils.c) 

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

include Makefile.saul
include Makefile.mqtt



# Alternative method to add files in subfolders using wildcards
# SRC += $(wildcard *.c folder/*.c folder/**/*.c)

include $(RIOTBASE)/Makefile.include

# Set a custom channel if needed
include $(RIOTMAKE)/default-radio-settings.inc.mk

