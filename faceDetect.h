#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define OUTPUT_PRINTF 1
#define OUTPUT_VIDEO 2
#define OUTPUT_MEMORY 4

int faceDetectStart();
void faceDetect(uint8_t *dataIn, int flagsOutput, int width, int height);
int faceDetectStop();

#ifdef __cplusplus
}
#endif

