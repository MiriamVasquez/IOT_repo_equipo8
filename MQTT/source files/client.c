#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdint.h>
#include "MQTT_type_frames.h"

#define MQTT_PORT 1883
#define MQTT_SERVER "192.168.192.128"
#define MQTT_LOCAL_HOST "127.0.0.1"


int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return 1;
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(MQTT_PORT);
    server.sin_addr.s_addr = inet_addr(MQTT_LOCAL_HOST);

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Connection failed");
        return 1;
    }

char ProtName[] = "MQTT";
char ClientID[] = "Controller";
sConnect connectFrame;
connectFrame.bFrameType = 0x10;
connectFrame.wMsgLen = (2 + strlen(ProtName)) + 1 + 1 + 2 + (2 + strlen(ClientID));
connectFrame.wProtNameLen = strlen(ProtName);
strcpy(connectFrame.sProtName, ProtName);
connectFrame.bVersion = 0x04;
connectFrame.bConnectFlags = 0x02;
connectFrame.bKeepAlive = 60;
connectFrame.wClientIDlen = strlen(ClientID);
strcpy(connectFrame.sClientID, ClientID);

    // Enviar frame
    size_t msgSize = connectFrame.wMsgLen + 2;
    write(sock, &connectFrame, msgSize);
    printf("CONNECT frame enviado (%ld bytes)\n", msgSize);

    // Esperar CONNACK (opcional)
    uint8_t response[4];
    int bytes = read(sock, response, sizeof(response));
    if (bytes > 0 && response[0] == 0x20) {
        printf("CONNACK recibido\n");
    }
////////////////Subcribe///////////////////////////
sSubscribe subscribeFrame;
char topic_suscribe[] = "test/topic";

subscribeFrame.bFrameType = 0x82;
subscribeFrame.wPacketID = 1;
subscribeFrame.wTopicLen = strlen(topic_suscribe);
strcpy(subscribeFrame.sTopic, topic_suscribe);
subscribeFrame.bQoS = 0;

// Calcular longitud del mensaje MQTT (sin incluir bFrameType ni wMsgLen)
subscribeFrame.wMsgLen = 2 + 2 + strlen(topic_suscribe) + 1;

// Enviar frame
size_t subscribeSize = 1 + 2 + 2 + 2 + strlen(topic_suscribe) + 1;
write(sock, &subscribeFrame, subscribeSize);

/////////////////////// Publish ////////////////////////////
sPublish publishFrame;
char topic_publish[] = "test/topic";
char payload[] = "Hola desde cliente";

publishFrame.bFrameType = 0x30;
publishFrame.wTopicLen = strlen(topic_publish);
strcpy(publishFrame.sTopic, topic_publish);
strcpy(publishFrame.sPayload, payload);

// Longitud del mensaje MQTT (sin incluir bFrameType ni wMsgLen)
publishFrame.wMsgLen = 2 + strlen(topic_publish) + strlen(payload);

// Enviar frame
size_t publishSize = 1 + 2 + 2 + strlen(topic_publish) + strlen(payload);
write(sock, &publishFrame, publishSize);
printf("PUBLISH frame enviado (%ld bytes)\n", publishSize);

sleep(1);
    close(sock);
    return 0;
}
