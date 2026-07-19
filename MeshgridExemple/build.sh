#!/bin/bash

set -xe

gcc main.c -Wall -Wextra -Wno-unused-function -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o main

gcc main2.c -Wall -Wextra -Wno-unused-function -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o main2

./main2

./main
