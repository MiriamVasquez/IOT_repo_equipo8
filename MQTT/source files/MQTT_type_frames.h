#ifndef MQTT_FRAMES_H
#define MQTT_FRAMES_H

#include <stdint.h>

// Estructura para CONNECT
typedef struct {
    uint8_t bFrameType;       // 0x10
    uint16_t wMsgLen;         // Longitud del mensaje
    uint16_t wProtNameLen;    // 0x0004
    char sProtName[5];        // String "MQTT"
    uint8_t bVersion;         // 0x04
    uint8_t bConnectFlags;    // 0x02
    uint16_t bKeepAlive;      // Tiempo de keep-alive
    uint16_t wClientIDlen;    // Longitud del Client ID
    char sClientID[100];      // Client ID
} sConnect;

typedef struct {
    uint8_t bFrameType;
    uint16_t wMsgLen;
    uint16_t wPacketID;
    uint16_t wTopicLen;
    char sTopic[100];
    uint8_t bQoS;
} sSubscribe;

typedef struct {
    uint8_t bFrameType;
    uint16_t wMsgLen;
    uint16_t wTopicLen;
    char sTopic[100];
    char sPayload[100];
} sPublish;

// Estructura para CONNACK
typedef struct {
    uint8_t bFrameType;       // 0x20
    uint8_t bMsgLen;          // Longitud del mensaje
    uint8_t bFlags;           // 0x00
    uint8_t bReturnStatus;    // 0x00 OK, 0x01 límite, 0x02 error
} sConnAck;

// Estructura para PINGREQ / PINGRESP
typedef struct {
    uint8_t bFrameType;       // 0xC0 Req, 0xD0 Resp
    uint8_t bMsgLen;          // 0x00
} sPingRe;

#endif // MQTT_FRAMES_H
