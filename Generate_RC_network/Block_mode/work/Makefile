.SUFFIXES:
.SUFFIXES: .cc  .cpp  .o  .x

CC = g++
OPT =
DEBUG =
OTHER = -Wall -O3

# Use this CFLAGS definition if you need to see debug output
# CFLAGS = -c -g -Wno-deprecated -Wall -D_DEBUG
CFLAGS = -c -g -Wno-deprecated 

MODULE = run
SRCS = ../src/flp.cpp ../src/util.cpp ../src/temperature.cpp ../src/temperature_block.cpp ../src/rc_network.cpp ../src/npe.cpp ../src/shape.cpp ../src/RCutil.cpp ../src/temperature_grid.cpp ../src/main.cpp

OBJS = $(SRCS:../src/%.cpp=%.o)

include Makefile.defs
