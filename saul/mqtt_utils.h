#ifndef MQTT_UTILS_H
#define MQTT_UTILS_H

#include <string.h>
#include <stdbool.h>
#include "thread.h"
#include "mutex.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"

#ifndef BROKER_IPV6
#define BROKER_IPV6                   "2600:1f18:74ea:15df:f67c:5919:5bab:51f1"
#endif

// topic both_directions both 1 
#ifndef TOPIC_TO_SUBSCRIBE
#define TOPIC_TO_SUBSCRIBE              "awsiot_to_localgateway"
#endif

#ifndef TOPIC_TO_PUBLISH
#define TOPIC_TO_PUBLISH                "localgateway_to_awsiot"
#endif

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

#define BUF_SIZE                        1024
#define MQTT_VERSION_v311               4       /* MQTT v3.1.1 version is 4 */
#define COMMAND_TIMEOUT_MS              4000

#ifndef DEFAULT_MQTT_CLIENT_ID
#define DEFAULT_MQTT_CLIENT_ID          ""
#endif

#ifndef DEFAULT_MQTT_USER
#define DEFAULT_MQTT_USER               ""
#endif

#ifndef DEFAULT_MQTT_PWD
#define DEFAULT_MQTT_PWD                ""
#endif

/**
 * @brief Default MQTT port
 */
#define DEFAULT_MQTT_PORT               8080

/**
 * @brief Keepalive timeout in seconds
 */
#define DEFAULT_KEEPALIVE_SEC           10

#ifndef MAX_LEN_TOPIC
#define MAX_LEN_TOPIC                   100
#endif

#ifndef MAX_TOPICS
#define MAX_TOPICS                      4
#endif

#define IS_CLEAN_SESSION                1
#define IS_RETAINED_MSG                 0


#endif /* MQTT_UTILS_H */