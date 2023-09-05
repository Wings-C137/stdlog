# Compilers
CC = gcc
CPLUS = g++

# Target binary executable programs.
TARGET1=stdlog_demo
# Library
LIB1= -lpthread -lrt -ldl
#LIB2 =
# Flags
CFLAGS = -fPIC -O3 -Wall -pthread -g -std=c++11 -m64
#DEFINE =
PWD = $(shell pwd)
INCLUDE1 = -I$(PWD)/include

SRC1 = $(wildcard ./src/*.cpp)
OBJ1 = $(patsubst %.cpp,%.o,$(SRC1))

All:$(TARGET1)

$(TARGET1):$(OBJ1)
	$(CPLUS) $^ -o $@ $(LIB1) $(INCLUDE1)
%.o:%.cpp
	$(CPLUS) $(CFLAGS) $(INCLUDE1) $(INCLUDE2) -c $< -o $@

.PHONY:

clean:
	-rm -f $(OBJ1)
	-rm -f $(TARGET1)
