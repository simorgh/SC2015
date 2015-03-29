HOME    = $(PWD)
EXEC    = $(HOME)/main
SRC_SCL = ../simple-opencl
CC      = g++
SRC = main.c $(SRC_SCL)/simpleCL.c 
CFLAGS  = -Wall -Wextra -pedantic -O3

UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
INCL_P  = -I$(HOME)/inc -I/usr/local/cuda/include
LIBS   = -lm -lOpenCL -lrt
INCL_AMD = -I$(HOME)/inc -I/opt/AMDAPP/include 
LIBS_AMD = -L/opt/AMDAPP/lib/x86_64 $(LIBS)
CFLAGS_AMD  = $(CFLAGS) -DATI_OS_LINUX 
endif
ifeq ($(UNAME), Darwin)
INCL_P   = -I$(HOME)/inc
LIBS     = -framework OpenCL
INCL_AMD = $(INCL_P) 
LIBS_AMD = $(LIBS)
CFLAGS_AMD = $(CFLAGS)
endif

ifdef AMDAPPSDKROOT
INC_DIRS = "$(AMDAPPSDKROOT)include"
LIB_DIRS = "$(AMDAPPSDKROOT)lib\x86_64"
LIB = -lOpenCL
CFLAGS_AMD  = $(CFLAGS) -DATI_OS_LINUX 
endif
 

all:
	$(CC) $(CFLAGS) $(INCL_P) -c $(SRC)
	$(CC) $(CFLAGS) *.o -o $(EXEC) $(LIBS)
	#$(CC) $(CFLAGS_AMD) -I $(INC_DIRS) -c $(SRC)
	#$(CC) $(CFLAGS_AMD) *.o -o $(EXEC) -L $(LIB_DIRS) $(LIB)
	rm -f *.o

amd:
	$(CC) $(CFLAGS_AMD) -c $(SRC) $(INCL_AMD) 
	$(CC) $(CFLAGS_AMD) *.o -o $(EXEC) $(LIBS_AMD) $(LIB)
	rm -f *.o

clean:
	rm -f *.o $(EXEC)

cleanbin:
	rm -f snapshot* image*



