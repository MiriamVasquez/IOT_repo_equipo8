#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdint.h>

#define MQTT_PORT 1883

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    uint8_t buffer[256];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket error");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(MQTT_PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind error");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 5) < 0) {
        perror("Listen error");
        exit(EXIT_FAILURE);
    }

    printf("MQTT Broker escuchando en puerto %d...\n", MQTT_PORT);

    while (1) {
        client_fd = accept(server_fd, (struct sockaddr *)&address, &addrlen);
        if (client_fd < 0) {
            perror("Accept error");
            continue;
        }

        printf("Cliente conectado\n");

        while (1) {
            int bytes = read(client_fd, buffer, sizeof(buffer));
            if (bytes <= 0) {
                printf("Cliente desconectado\n");
                break;
            }

            uint8_t packet_type = buffer[0];

            if (packet_type == 0x10) {
                printf("CONNECT frame recibido (%d bytes)\n", bytes);
                uint8_t connack[4] = {0x20, 0x02, 0x00, 0x00};
                write(client_fd, connack, sizeof(connack));
                printf("CONNACK enviado\n");
            } else if (packet_type == 0x82) {
                printf("SUBSCRIBE frame recibido (%d bytes)\n", bytes);
                uint8_t suback[5] = {0x90, 0x03, buffer[2], buffer[3], 0x00};
                write(client_fd, suback, sizeof(suback));
                printf("SUBACK enviado\n");
            } else if (packet_type == 0x30) {
                printf("PUBLISH frame recibido (%d bytes)\n", bytes);
                uint16_t topic_len = (buffer[2] << 8) | buffer[3];
                char topic[101] = {0};
                memcpy(topic, &buffer[4], topic_len);
                char payload[101] = {0};
                int payload_len = bytes - 4 - topic_len;
                memcpy(payload, &buffer[4 + topic_len], payload_len);
                printf("Mensaje publicado en topic '%s': %s\n", topic, payload);
            } else {
                printf("Frame desconocido: 0x%02X\n", packet_type);
            }
        }

        close(client_fd);
    }

    close(server_fd);
    return 0;
}
