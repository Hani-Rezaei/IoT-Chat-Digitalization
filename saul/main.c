/**
 * @file    main.c
 * @brief   
 * @details 
 *
 * @author  Mohammadjavad Esmaeili
 */

#include <stdio.h>
#include "shell.h"

// SAUL
#include "saul_utils.h"

// Log
#include "log.h"
#include <string.h>
#include <stdlib.h>

// LEDs
#include "led_utils.h"

// MQTT
#include "mqtt_utils.h"
#define MAIN_QUEUE_SIZE (8)
static volatile int message_ready = 0;  // Flag zur Synchronisation

static MQTTClient client;
static Network network;

static char saul_response[1024];
static char final_response[512];
static char saul_topic_to_publish[MAX_LEN_TOPIC];

static unsigned char buf[BUF_SIZE];
static unsigned char readbuf[BUF_SIZE];

static int topic_cnt = 0;
static char _topic_to_subscribe[MAX_TOPICS][MAX_LEN_TOPIC];

static int bme280_pub(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    enum QoS qos = QOS0;

    MQTTMessage message;
    message.qos = qos;
    message.retained = IS_RETAINED_MSG;

    message.payload = saul_response;
    printf("saul_response: %s\n",saul_response);

    message.payloadlen = strlen(message.payload);


    int rc;
    if ((rc = MQTTPublish(&client, saul_topic_to_publish, &message)) < 0) {
        printf("mqtt_example: Unable to publish (%d)\n", rc);
    }
    else {
        printf("mqtt_example: Message (%s) has been published to topic %s"
               "with QOS %d\n",
               (char *)message.payload, saul_topic_to_publish, (int)message.qos);
    }
    return rc;
}

static char *get_response(const char* topic, const char* request_unit) {
    
    static char escape_json_buffer[PUFFER_SIZE];  // Angemessene Puffergröße
    size_t json_size;
    // Reaktion auf spezifische Topics
    if (strncmp(topic, TOPIC_TEMPERATURE, strlen(TOPIC_TEMPERATURE)) == 0) {
        
        size_t topic_len = strlen(TOPIC_TEMPERATURE_TO_PUBLISH);
        strncpy(saul_topic_to_publish, TOPIC_TEMPERATURE_TO_PUBLISH, topic_len);
        saul_topic_to_publish[topic_len] = '\0'; // Null-Terminierung erzwingen
        
        int res = read_bme280_temperature (bme_280_name, request_unit, escape_json_buffer, &json_size);
        if (res < 0){
            printf("Fehler: %d\n", res);
        }
    }
    else if (strncmp(topic, TOPIC_HUMIDITY, strlen(TOPIC_HUMIDITY)) == 0) {

        size_t topic_len = strlen(TOPIC_HUMIDITY_TO_PUBLISH);
        strncpy(saul_topic_to_publish, TOPIC_HUMIDITY_TO_PUBLISH, topic_len);
        saul_topic_to_publish[topic_len] = '\0'; // Null-Terminierung erzwingen

        int res = read_bme280_humidity (bme_280_name, escape_json_buffer, &json_size);
        if (res < 0){
            printf("Fehler: %d\n", res);
        }
    } 
    else if (strncmp(topic, TOPIC_PRESSURE, strlen(TOPIC_PRESSURE)) == 0) {

        size_t topic_len = strlen(TOPIC_PRESSURE_TO_PUBLISH);
        strncpy(saul_topic_to_publish, TOPIC_PRESSURE_TO_PUBLISH, topic_len);
        saul_topic_to_publish[topic_len] = '\0'; // Null-Terminierung erzwingen

        int res = read_bme280_pressure (bme_280_name, request_unit, escape_json_buffer, &json_size);
        if (res < 0){
            printf("Fehler: %d\n", res);
        }
    }
    else {
        // Kein spezifisches Topic erkannt
        snprintf(escape_json_buffer, sizeof(escape_json_buffer), "No action for topic: %s", topic);
    }
    return escape_json_buffer;
}

// const char* parse_payload_for_unit(const char *payload)
// {
//     // Einfacher Parser für den Wert von "u"
//     char *key_start = strstr(payload, "\"u\": \"");
//     if (key_start) {
//         key_start += 6; // Um den Start von "u" zu überspringen
//         char *key_end = strchr(key_start, '"');
//         if (key_end) {
//             *key_end = '\0'; // Null-Terminierung
//             return key_start; // Rückgabe des gefundenen Wertes
//         }
//     }
//     return NULL; // Kein Wert gefunden
// }

void parse_payload_for_unit(const char *payload, char *unit, size_t unit_size, char *chat_id, size_t chat_id_size) {
    // Suche nach "u" (Einheit)
    char *unit_start = strstr(payload, "\"u\": \"");
    if (unit_start) {
        unit_start += 6; // Überspringe `"u": "`
        char *unit_end = strchr(unit_start, '"');
        if (unit_end) {
            size_t length = unit_end - unit_start;
            if (length < unit_size) {
                strncpy(unit, unit_start, length);
                unit[length] = '\0';
            }
        }
    }

    // Suche nach "chat_id"
    char *chat_start = strstr(payload, "\"chat_id\": \"");
    if (chat_start) {
        chat_start += 12; // Überspringe `"chat_id": "`
        char *chat_end = strchr(chat_start, '"');
        if (chat_end) {
            size_t length = chat_end - chat_start;
            if (length < chat_id_size) {
                strncpy(chat_id, chat_start, length);
                chat_id[length] = '\0';
            }
        }
    }
}

static void _on_msg_received(MessageData *data)
{
    printf("_on_msg_received: message received on topic"
           " %.*s: %.*s\n",
           (int)data->topicName->lenstring.len,
           data->topicName->lenstring.data, (int)data->message->payloadlen,
           (char *)data->message->payload);

    // Zugriff auf das Topic
    const char *topic = data->topicName->lenstring.data;

    // Payload in einen Null-terminierten String umwandeln
    char payload[PUFFER_SIZE];
    if (data->message->payloadlen < sizeof(payload)) {
        strncpy(payload, (char *)data->message->payload, data->message->payloadlen);
        payload[data->message->payloadlen] = '\0'; // Null-Terminierung
    } else {
        fprintf(stderr, "Fehler: Payload ist zu groß, um verarbeitet zu werden.\n");
        return;
    }
    printf("Payload: %s\n", payload);    

    // Speicher für extrahierte Werte
    char unit[10];
    char chat_id[50];

    // Extrahiere Einheit und chat_id aus dem Payload
    parse_payload_for_unit(payload, unit, sizeof(unit), chat_id, sizeof(chat_id));

    if (unit[0] != '\0') {  // Prüfen, ob Einheit gefunden wurde

        const char* response = get_response(topic, unit);
        if (response) {

            // JSON mit `chat_id` hinzufügen
            snprintf(final_response, sizeof(final_response) - 1, "{ \"chat_id\": \"%s\", %s", chat_id, response + 1);
            final_response[sizeof(final_response) - 1] = '\0'; // Sicherheits-Null-Terminierung
            
            printf("Antwort generiert: %s\n", final_response);
            // Antwort in globalen Speicher kopieren
            snprintf(saul_response, sizeof(saul_response), "%s", final_response);

            message_ready = 1;  // Flag setzen
        } else {
            fprintf(stderr, "Fehler: Keine gültige Antwort generiert.\n");
            saul_response[0] = '\0'; // Leeren String setzen
        }
        // Antwort ausgeben (oder publizieren)
        printf("Response to publish: %s\n", saul_response);
    } else {
        fprintf(stderr, "Fehler: Schlüssel 'u' nicht gefunden.\n");
    }
}

static int _cmd_discon(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    int res = MQTTDisconnect(&client);
    if (res < 0) {
        printf("mqtt_example: Unable to disconnect\n");
    }
    else {
        printf("mqtt_example: Disconnect successful\n");
    }

    NetworkDisconnect(&network);
    return res;
}

static int cmd_con(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    char *remote_ip = BROKER_IPV6;
    int ret = -1;

    /* ensure client isn't connected in case of a new connection */
    if (client.isconnected) {
        printf("mqtt_example: client already connected, disconnecting it\n");
        MQTTDisconnect(&client);
        NetworkDisconnect(&network);
    }

    int port = DEFAULT_MQTT_PORT;
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = MQTT_VERSION_v311;

    data.clientID.cstring = DEFAULT_MQTT_CLIENT_ID;
    data.username.cstring = DEFAULT_MQTT_USER;
    data.password.cstring = DEFAULT_MQTT_PWD;
    data.keepAliveInterval = DEFAULT_KEEPALIVE_SEC;
    data.cleansession = IS_CLEAN_SESSION;
    data.willFlag = 0;

    printf("mqtt_example: Connecting to MQTT Broker from %s %d\n",
            remote_ip, port);
    printf("mqtt_example: Trying to connect to %s, port: %d\n",
            remote_ip, port);
    ret = NetworkConnect(&network, remote_ip, port);
    if (ret < 0) {
        printf("mqtt_example: Unable to connect\n");
        return ret;
    }

    printf("user:%s clientId:%s password:%s\n", data.username.cstring,
             data.clientID.cstring, data.password.cstring);
    ret = MQTTConnect(&client, &data);
    if (ret < 0) {
        printf("mqtt_example: Unable to connect client %d\n", ret);
        _cmd_discon(0, NULL);
        return ret;
    }
    else {
        printf("mqtt_example: Connection successfully\n");
    }

    return (ret > 0) ? 0 : 1;
}

static int cmd_sub(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    
    enum QoS qos = QOS0;

    if (topic_cnt > MAX_TOPICS) {
        printf("mqtt_example: Already subscribed to max %d topics,"
                "call 'unsub' command\n", topic_cnt);
        return -1;
    }

    if (strlen(TOPIC_TO_SUBSCRIBE) > MAX_LEN_TOPIC) {
        printf("mqtt_example: Not subscribing, topic too long %s\n", TOPIC_TO_SUBSCRIBE);
        return -1;
    }
    strncpy(_topic_to_subscribe[topic_cnt], TOPIC_TO_SUBSCRIBE, strlen(TOPIC_TO_SUBSCRIBE) + 1);

    printf("mqtt_example: Subscribing to %s\n", _topic_to_subscribe[topic_cnt]);
    int ret = MQTTSubscribe(&client,
              _topic_to_subscribe[topic_cnt], qos, _on_msg_received);
    if (ret < 0) {
        printf("mqtt_example: Unable to subscribe to %s (%d)\n",
               _topic_to_subscribe[topic_cnt], ret);
        _cmd_discon(0, NULL);
    }
    else {
        printf("mqtt_example: Now subscribed to %s, QOS %d\n",
               TOPIC_TO_SUBSCRIBE, (int) qos);
        topic_cnt++;
    }
    return ret;
}

/**
 * Event-Loop: Verarbeitet Nachrichten aus der Queue und veröffentlicht diese.
 */
static void custom_event_loop(void)
{
    while (1) {
        if (message_ready) {
            led_set_state("LED 4", 1);
            printf("saul_topic_to_publish = %s\n", saul_topic_to_publish);

            bme280_pub(0, NULL);       
            message_ready = 0;  // Flag zurücksetzen
            led_set_state("LED 4", 0);
        }
    }
}

int main(void)
{
    puts("Welcome to RIOT!\n");

    printf("This application runs on %s\n", RIOT_BOARD);

    if (IS_USED(MODULE_GNRC_ICMPV6_ECHO)) {
        msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    }
#ifdef MODULE_LWIP
    /* let LWIP initialize */
    ztimer_sleep(ZTIMER_MSEC, 1 * MS_PER_SEC);
#endif

    NetworkInit(&network);

    MQTTClientInit(&client, &network, COMMAND_TIMEOUT_MS, buf, BUF_SIZE,
                   readbuf,
                   BUF_SIZE);

    MQTTStartTask(&client);

    led_set_state("LED 1", 1);

    // Verbindung zum Broker herstellen
    if (cmd_con(0, NULL) < 0) {
        fprintf(stderr, "Fehler: Verbindung zum Broker fehlgeschlagen.\n");
        return -1;
    }
    led_set_state("LED 2", 1);

    // Automatisches Abonnieren eines Themas
    if (cmd_sub(0, NULL) < 0) {
        fprintf(stderr, "Fehler: Abonnieren des Themas fehlgeschlagen.\n");
        return -1;
    }
    led_set_state("LED 3", 1);

    // if (MQTTSubscribe(&client, _topic_to_subscribe[0], QOS0, _on_msg_received) < 0) {
    // if (MQTTSubscribe(&client, TOPIC_TO_SUBSCRIBE, QOS0, _on_msg_received) < 0) {
    //     fprintf(stderr, "Fehler: Thema konnte nicht abonniert werden.\n");
    //     return -1;
    // }

    // Event-Loop starten
    custom_event_loop();
    

    // char line_buf[SHELL_DEFAULT_BUFSIZE];
    // shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}