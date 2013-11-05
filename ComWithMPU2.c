/* Those two files ComWithMPU2.c and ComWithMPU2.h need to be in sync on both aiMPU2 and aiCV
REVSION = 1 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/poll.h>
#include <sys/mman.h>
#include <string.h>
#include <time.h>
#include "ComWithMPU2.h"

char * FIFO_MPU2toCV[2] = {"/tmp/aiMPU2toCV_0", "/tmp/aiMPU2toCV_1"};
char * FIFO_CVtoMPU2[2] = {"/tmp/aiCVtoMPU2_0", "/tmp/aiCVtoMPU2_1"};

void InitComWithMPU2(int input) {
	struct stat buffer;
	if (stat (FIFO_MPU2toCV[input], &buffer) != 0)
		mkfifo(FIFO_MPU2toCV[input], (mode_t)0600);
	if (stat (FIFO_CVtoMPU2[input], &buffer) != 0)
		mkfifo(FIFO_CVtoMPU2[input], (mode_t)0600);
}

void UninitComWithMPU2(int input) {
}

void MPU2toCV_Send(int input, struct structMPU2toCV* data) {
	int fdMPU2toCV = open(FIFO_MPU2toCV[input], O_WRONLY | O_NONBLOCK);
	if (fdMPU2toCV > 0) {
		write(fdMPU2toCV, (void *)data, sizeof(structMPU2toCV));
		close(fdMPU2toCV);
	}
}

void MPU2toCV_Receive(int input, struct structMPU2toCV* data) {
	int fdMPU2toCV = open(FIFO_MPU2toCV[input], O_RDONLY);
	char buffer[1024];
//This is a blocking call
	read(fdMPU2toCV, &buffer, 1024);
	memcpy(data, buffer, sizeof(structMPU2toCV));
	close(fdMPU2toCV);
}

void CVtoMPU2_Send(int input, struct structCVtoMPU2* data) {
	int fdCVtoMPU2 = open(FIFO_CVtoMPU2[input], O_WRONLY | O_NONBLOCK);
	if (fdCVtoMPU2 > 0) {
		write(fdCVtoMPU2, (void *)data, sizeof(structCVtoMPU2));
		close(fdCVtoMPU2);
	}
}

void CVtoMPU2_Receive(int input, struct structCVtoMPU2* data) {
//Not used as aiMPU2/MPU2 has its own implementation
	int fdCVtoMPU2 = open(FIFO_CVtoMPU2[input], O_RDONLY);
	char buffer[1024];
	read(fdCVtoMPU2, &buffer, 1024);
	memcpy(data, buffer, sizeof(structCVtoMPU2));
	close(fdCVtoMPU2);
}

