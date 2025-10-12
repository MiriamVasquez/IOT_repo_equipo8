#include "com.h"

int32_t dwfnReadFrame(sFrame * spFrameDest, int32_t dwConnection)
{
	uint8_t* bpFramePtr;
	//uint8_t bSize = FRAME_SIZE-1;
	bpFramePtr = (uint8_t*)malloc(FRAME_SIZE);
	if( (read( dwConnection, bpFramePtr, FRAME_SIZE) ) != FRAME_SIZE )
	{
		printf("Reading Error\n\r");
		free(bpFramePtr);
		return -1;
	}
	else
	{
		//bpFramePtr++;
		/*while(bSize--)
		{
			bpFramePtr[bSize] = ntohs(bpFramePtr[bSize]);
		}*/
		memcpy((uint8_t*) spFrameDest, (uint8_t*) bpFramePtr, sizeof(FRAME_SIZE));
		//bpFramePtr--;
		free(bpFramePtr);
		return 0;
	}
}

int32_t dwfnWriteFrame(sFrame * spFrameSrc, int32_t dwConnection)
{
	uint8_t* bpFramePtr;
	bpFramePtr = (uint8_t*)malloc(FRAME_SIZE);
	//*bpFramePtr = SOF;
	//bpFramePtr++;
	memcpy(bpFramePtr,spFrameSrc,sizeof(FRAME_SIZE));
	if((write(dwConnection,bpFramePtr,FRAME_SIZE)) != FRAME_SIZE)
	{
		printf("Writing Error\n\r");
		//bpFramePtr--;
		free(bpFramePtr);	
		return -1;
	}
	else
	{
		//bpFramePtr--;
		free(bpFramePtr);
		return 0;
	}

}
