# Name der Anwendung
APPLICATION = TextYourIoTDevice

# Standard-Board setzen, falls keine spezifische Auswahl getroffen wurde
BOARD ?= nrf52840dk

# Absoluter Pfad zum RIOT-Basisverzeichnis
RIOTBASE ?= $(CURDIR)/../../RIOT

# Entwicklungsmodus aktivieren (nützliche Debugging-Hilfen und Sicherheitschecks)
# Setze auf 0 für eine produktionsnahe Umgebung mit weniger Sicherheitsprüfungen
DEVELHELP ?= 1

# Unterdrücke Standardausgaben des Compilers (auf 0 setzen, um alle Aufrufe anzuzeigen)
QUIET ?= 1

# Einbinden der SAUL-spezifischen Makefile-Konfiguration
include Makefile.saul

# MQTT-Support einbinden
include Makefile.mqtt

# Einbinden der RIOT-spezifischen Build-Konfiguration
include $(RIOTBASE)/Makefile.include

# Falls nötig, benutzerdefinierte Funkkanaleinstellungen laden
include $(RIOTMAKE)/default-radio-settings.inc.mk

