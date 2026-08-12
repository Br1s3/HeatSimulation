CC = gcc
RM = rm -rf

CFLAGS += \
-Wall \
-Wextra \
-Wno-unused-function \
-lraylib \
-lGL \
-lm \
-lpthread \
-ldl \
-lrt \
-lX11 \

EXEC = HeatSimulation HeatSimulationPDE HeatSimulationPDECons

.PHONY: all clean

all: $(EXEC)

%: %.c
	$(CC) $< -o $@ $(CFLAGS)

clean:
	$(RM) $(EXEC)
