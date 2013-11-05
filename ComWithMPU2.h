/* Those two files ComWithMPU2.c and ComWithMPU2.h need to be in sync on both aiMPU2 and aiCV
REVSION = 1 */

#ifndef SHAREDDATA2_H_
#define SHAREDDATA2_H_

#include "v4l2_od.h"

#ifdef __cplusplus
extern "C" {
#endif

struct structMPU2toCV {
	int command;
} structMPU2toCV;

struct structCVtoMPU2 {
	int type;
	float tripodvx;
	float tripodvy;
	struct v4l2_od_obj_desc face;
	float headbandx;
	float headbandy;
	float headbandarea;
} structCVtoMPU2;

extern char * FIFO_MPU2toCV[2];
extern char * FIFO_CVtoMPU2[2];

void InitComWithMPU2(int input);
void UninitComWithMPU2(int input);
void MPU2toCV_Send(int input, struct structMPU2toCV* data);
void MPU2toCV_Receive(int input, struct structMPU2toCV* data);
void CVtoMPU2_Send(int input, struct structCVtoMPU2* data);
void CVtoMPU2_Receive(int input, struct structCVtoMPU2* data);

#ifdef __cplusplus
}
#endif

#define COMMAND_PILOT_NONE 0
#define COMMAND_PILOT_FACE 1
#define COMMAND_PILOT_HEADBAND 2
#define COMMAND_PILOT_TRIPOD 4
#define TYPE_PILOT_NONE 0
#define TYPE_PILOT_FACE 1
#define TYPE_PILOT_HEADBAND 2
#define TYPE_PILOT_TRIPOD 4

#endif

