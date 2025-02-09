/**
 * @file    mqtt_utils.h
 * @brief   Header-Datei für die MQTT-Kommunikation und -Konfiguration.
 * 
 * Diese Datei enthält die Deklarationen von Konstanten und Makros, die 
 * für die MQTT-Kommunikation und -Konfiguration verwendet werden. Sie 
 * stellt Standardwerte für die Verbindung zum MQTT-Broker, die Definition 
 * von Topics und weitere MQTT-spezifische Parameter bereit.
 * 
 * Es werden sowohl die Verbindungsdetails als auch die Themen für den 
 * Datenaustausch definiert, um eine grundlegende MQTT-basierte Kommunikation 
 * zu ermöglichen.
 * 
 * @author  Mohammadjavad Esmaeili
 * @date    2025
 */

#ifndef MQTT_UTILS_H
#define MQTT_UTILS_H

#include <string.h>
#include <stdbool.h>
#include "thread.h"
#include "mutex.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"

#ifndef BROKER_IPV6
/** 
 * @brief IPv6-Adresse des MQTT-Brokers 
 */
#define BROKER_IPV6         "2600:1f18:74ea:15df:f67c:5919:5bab:51f1"
#endif

#ifndef TOPIC_TO_SUBSCRIBE
/** 
 * @brief Standard-Topic für Abonnements 
 */
#define TOPIC_TO_SUBSCRIBE   "awsiot_to_localgateway/#"
#endif

/** @name MQTT-Themen für Sensordaten 
 *  @{
 */
#ifndef TOPIC_TEMPERATURE
#define TOPIC_TEMPERATURE   "awsiot_to_localgateway/temperature"
#endif
#ifndef TOPIC_TEMPERATURE_TO_PUBLISH
#define TOPIC_TEMPERATURE_TO_PUBLISH   "localgateway_to_awsiot/temperature"
#endif

#ifndef TOPIC_HUMIDITY
#define TOPIC_HUMIDITY               "awsiot_to_localgateway/humidity"
#endif
#ifndef TOPIC_HUMIDITY_TO_PUBLISH
#define TOPIC_HUMIDITY_TO_PUBLISH    "localgateway_to_awsiot/humidity"
#endif

#ifndef TOPIC_PRESSURE
#define TOPIC_PRESSURE               "awsiot_to_localgateway/pressure"
#endif
#ifndef TOPIC_PRESSURE_TO_PUBLISH
#define TOPIC_PRESSURE_TO_PUBLISH    "localgateway_to_awsiot/pressure"
#endif
/** @} */

/** 
 * @brief Größe der Hauptnachrichtenwarteschlange 
 */
#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];
/** 
 * @brief Größe des Puffers für MQTT-Nachrichten 
 */
#define BUF_SIZE                        1024
/** 
 * @brief MQTT-Protokollversion (v3.1.1) 
 */
#define MQTT_VERSION_v311               4       /* MQTT v3.1.1 version is 4 */
/** 
 * @brief MQTT-Protokollversion (v3.1.1) 
 */
#define COMMAND_TIMEOUT_MS              4000

/** @name Standard-MQTT-Client-Anmeldedaten 
 *  @{
 */
#ifndef DEFAULT_MQTT_CLIENT_ID
#define DEFAULT_MQTT_CLIENT_ID          ""
#endif

#ifndef DEFAULT_MQTT_USER
#define DEFAULT_MQTT_USER               ""
#endif

#ifndef DEFAULT_MQTT_PWD
#define DEFAULT_MQTT_PWD                ""
#endif
/** @} */

/** 
 * @brief Standardport für die MQTT-Verbindung 
 */
#define DEFAULT_MQTT_PORT               8080

/** 
 * @brief Keepalive-Intervall in Sekunden 
 */
#define DEFAULT_KEEPALIVE_SEC           10

/** 
 * @brief Maximale Länge eines MQTT-Topics 
 */
#ifndef MAX_LEN_TOPIC
#define MAX_LEN_TOPIC                   100
#endif
/** 
 * @brief Maximale Anzahl der abonnierten Topics 
 */
#ifndef MAX_TOPICS
#define MAX_TOPICS                      4
#endif
/** 
 * @brief Angabe, ob eine neue Verbindung eine saubere Sitzung ist 
 */
#define IS_CLEAN_SESSION                1
/** 
 * @brief Angabe, ob MQTT-Nachrichten als "Retained Messages" gespeichert werden sollen 
 */
#define IS_RETAINED_MSG                 0

#endif /* MQTT_UTILS_H */