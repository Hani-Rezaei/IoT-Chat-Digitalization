/**
 * @file  shell_command_utils.c
 * @brief MQTT Befehle für die Verbindung, Veröffentlichung und das Abonnieren von Themen.
 *
 * Dieser Code demonstriert die Verwendung von MQTT zur Verbindung mit einem Broker, 
 * zum Veröffentlichen von Nachrichten und zum Abonnieren von Themen. Der Code enthält
 * auch Befehlsfunktionen, die über eine Shell-Schnittstelle aufgerufen werden können.
 * 
 * @note Die Shell-Befehle sind hilfreich, um das Verhalten des MQTT-Clients zu debuggen.
 */

// #include <stdio.h>
// #include "shell.h"
// #include "mqtt_utils.h"

// static MQTTClient client;
// static Network network;
// static int topic_cnt = 0;
// static char _topic_to_subscribe[MAX_TOPICS][MAX_LEN_TOPIC];

// static unsigned get_qos(const char *str)
// {
//     int qos = atoi(str);

//     switch (qos) {
//     case 1:     return QOS1;
//     case 2:     return QOS2;
//     default:    return QOS0;
//     }
// }

// static void _on_msg_received(MessageData *data)
// {
//     printf("paho_mqtt_example: message received on topic"
//            " %.*s: %.*s\n",
//            (int)data->topicName->lenstring.len,
//            data->topicName->lenstring.data, (int)data->message->payloadlen,
//            (char *)data->message->payload);
// }

// static int _cmd_discon(int argc, char **argv)
// {
//     (void)argc;
//     (void)argv;

//     topic_cnt = 0;
//     int res = MQTTDisconnect(&client);
//     if (res < 0) {
//         printf("mqtt_example: Unable to disconnect\n");
//     }
//     else {
//         printf("mqtt_example: Disconnect successful\n");
//     }

//     NetworkDisconnect(&network);
//     return res;
// }

// static int _cmd_con(int argc, char **argv)
// {
//     if (argc < 2) {
//         printf(
//             "usage: %s <brokerip addr> [port] [clientID] [user] [password] "
//             "[keepalivetime]\n",
//             argv[0]);
//         return 1;
//     }

//     char *remote_ip = argv[1];

//     int ret = -1;

//     /* ensure client isn't connected in case of a new connection */
//     if (client.isconnected) {
//         printf("mqtt_example: client already connected, disconnecting it\n");
//         MQTTDisconnect(&client);
//         NetworkDisconnect(&network);
//     }

//     int port = DEFAULT_MQTT_PORT;
//     if (argc > 2) {
//         port = atoi(argv[2]);
//     }

//     MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
//     data.MQTTVersion = MQTT_VERSION_v311;

//     data.clientID.cstring = DEFAULT_MQTT_CLIENT_ID;
//     if (argc > 3) {
//         data.clientID.cstring = argv[3];
//     }

//     data.username.cstring = DEFAULT_MQTT_USER;
//     if (argc > 4) {
//         data.username.cstring = argv[4];
//     }

//     data.password.cstring = DEFAULT_MQTT_PWD;
//     if (argc > 5) {
//         data.password.cstring = argv[5];
//     }

//     data.keepAliveInterval = DEFAULT_KEEPALIVE_SEC;
//     if (argc > 6) {
//         data.keepAliveInterval = atoi(argv[6]);
//     }

//     data.cleansession = IS_CLEAN_SESSION;
//     data.willFlag = 0;

//     printf("mqtt_example: Connecting to MQTT Broker from %s %d\n",
//             remote_ip, port);
//     printf("mqtt_example: Trying to connect to %s, port: %d\n",
//             remote_ip, port);
//     ret = NetworkConnect(&network, remote_ip, port);
//     if (ret < 0) {
//         printf("mqtt_example: Unable to connect\n");
//         return ret;
//     }

//     printf("user:%s clientId:%s password:%s\n", data.username.cstring,
//              data.clientID.cstring, data.password.cstring);
//     ret = MQTTConnect(&client, &data);
//     if (ret < 0) {
//         printf("mqtt_example: Unable to connect client %d\n", ret);
//         _cmd_discon(0, NULL);
//         return ret;
//     }
//     else {
//         printf("mqtt_example: Connection successfully\n");
//     }

//     return (ret > 0) ? 0 : 1;
// }

// static int _cmd_pub(int argc, char **argv)
// {
//     enum QoS qos = QOS0;

//     if (argc < 3) {
//         printf("usage: %s <topic name> <string msg> [QoS level]\n",
//                argv[0]);
//         return 1;
//     }
//     if (argc == 4) {
//         qos = get_qos(argv[3]);
//     }
//     MQTTMessage message;
//     message.qos = qos;
//     message.retained = IS_RETAINED_MSG;
//     message.payload = argv[2];
//     message.payloadlen = strlen(message.payload);

//     int rc;
//     if ((rc = MQTTPublish(&client, argv[1], &message)) < 0) {
//         printf("mqtt_example: Unable to publish (%d)\n", rc);
//     }
//     else {
//         printf("mqtt_example: Message (%s) has been published to topic %s"
//                "with QOS %d\n",
//                (char *)message.payload, argv[1], (int)message.qos);
//     }

//     return rc;
// }

// static int _cmd_sub(int argc, char **argv)
// {
//     enum QoS qos = QOS0;

//     if (argc < 2) {
//         printf("usage: %s <topic name> [QoS level]\n", argv[0]);
//         return 1;
//     }

//     if (argc >= 3) {
//         qos = get_qos(argv[2]);
//     }

//     if (topic_cnt > MAX_TOPICS) {
//         printf("mqtt_example: Already subscribed to max %d topics,"
//                 "call 'unsub' command\n", topic_cnt);
//         return -1;
//     }

//     if (strlen(argv[1]) > MAX_LEN_TOPIC) {
//         printf("mqtt_example: Not subscribing, topic too long %s\n", argv[1]);
//         return -1;
//     }
//     strncpy(_topic_to_subscribe[topic_cnt], argv[1], strlen(argv[1]));

//     printf("mqtt_example: Subscribing to %s\n", _topic_to_subscribe[topic_cnt]);
//     int ret = MQTTSubscribe(&client,
//               _topic_to_subscribe[topic_cnt], qos, _on_msg_received);
//     if (ret < 0) {
//         printf("mqtt_example: Unable to subscribe to %s (%d)\n",
//                _topic_to_subscribe[topic_cnt], ret);
//         _cmd_discon(0, NULL);
//     }
//     else {
//         printf("mqtt_example: Now subscribed to %s, QOS %d\n",
//                argv[1], (int) qos);
//         topic_cnt++;
//     }
//     return ret;
// }

// static int _cmd_unsub(int argc, char **argv)
// {
//     if (argc < 2) {
//         printf("usage %s <topic name>\n", argv[0]);
//         return 1;
//     }

//     int ret = MQTTUnsubscribe(&client, argv[1]);

//     if (ret < 0) {
//         printf("mqtt_example: Unable to unsubscribe from topic: %s\n", argv[1]);
//         _cmd_discon(0, NULL);
//     }
//     else {
//         printf("mqtt_example: Unsubscribed from topic:%s\n", argv[1]);
//         topic_cnt--;
//     }
//     return ret;
// }

// int _cmd_handler(int argc, char **argv)
// {
//     (void)argc;
//     (void)argv;

//     return 0;
// }

// static const shell_command_t shell_commands[] =
// {
//     { "con",    "connect to MQTT broker",             _cmd_con    },
//     { "discon", "disconnect from the current broker", _cmd_discon },
//     { "pub",    "publish something",                  _cmd_pub    },
//     { "sub",    "subscribe topic",                    _cmd_sub    },
//     { "unsub",  "unsubscribe from topic",             _cmd_unsub  },
//     { "cmd_handler", "Zum Testen",                    _cmd_handler},
//     { NULL,     NULL,                                 NULL        }
// };