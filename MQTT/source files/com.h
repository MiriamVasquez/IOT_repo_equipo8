#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h> 
#include <errno.h>

#define SOF 		(0xAA)
#define PROP_OFFSET	(4)
#define CMDS_OFFSET (4)
#define FRAME_SIZE	(7)

typedef struct
{
	uint8_t bSOF;
	uint8_t bCMD;
	uint8_t bProperty;
	uint8_t bPayload[4];
}sFrame;

typedef enum 
{
	GET = 1,
	SET,
}eCMDs;


typedef enum
{
	ADC = 0,
	PUSHBOTTON,
	RED,
	GREEN,
	BLUE,
	RGB,
}eProperties;

int32_t dwfnReadFrame(sFrame * spFrameDest, int32_t dwConnection);
int32_t dwfnWriteFrame(sFrame * spFrameSrc, int32_t dwConnection);

