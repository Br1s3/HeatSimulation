#!/bin/bash

set -xe

gcc HeatSimulation.c -Wall -Wextra -Wno-unused-function -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o HeatSimulation

gcc HeatSimulationPDE.c -Wall -Wextra -Wno-unused-function -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o HeatSimulationPDE

gcc HeatSimulationPDECons.c -Wall -Wextra -Wno-unused-function -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o HeatSimulationPDECons


# ./HeatSimulationODECons
# ./HeatSimulationODE
# ./HeatSimulation
