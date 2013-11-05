ARM_CC := gcc
ARM_AR := ar
ifeq ($(shell uname -m),armv7l)
NEON := -mfpu=neon -DARM
endif
ARM_CFLAGS = $(NEON) $(CFLAGS) -Iexp_headers -Iexp_headers/ref -I. -I/usr/include/arm-linux-gnueabihf `pkg-config opencv --cflags`
ARM_LDFLAGS = -L. -lUncannyCV -lssl -lcrypto -lzip -lz -lrt -ldl -lm -lpthread `pkg-config opencv --libs`

OBJs := ./main.o \
	ComWithMPU2.o \
	uv_v4l_camera_utils.o \
	faceDetect.o \
	helpers.o

.PHONY : clean

all: gpp_exec
clean: gpp_clean

gpp_exec: $(OBJs) $(OBJ_Rs)
ifneq ($(shell uname -m),armv7l)
	@find . -name "*.o" -exec rm -f {} \;
else
	$(ARM_CC) $(LDFLAGS) $(OBJs) $(ARM_LDFLAGS) -o aicv
endif

%.o : %.c*
	$(ARM_CC) $< -o $@ -c $(ARM_CFLAGS)

gpp_clean:
	@find . -name "*.o" -exec rm -f {} \;
	@rm -f aicv

