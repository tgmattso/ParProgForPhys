#
#  USAGE:
#     make          ... to build the programs
#     make test     ... to run the default test case
#

CC          = gcc-13
CPP         = g++-13
CLINKER     = $(CC)
CPPLINKER   = $(CPP)
#OPTFLAGS   = -std=c11 -fopenmp -O3
OPTFLAGS    = -O0 -fopenmp -Wall -frounding-math
LIBS        = -lm
CFLAGS	  = $(OPTFLAGS)

SUMOBJS = summationWithKahan.o UtilityFunctions.o

EXES = summation

all: $(EXES)

summation: $(SUMOBJS) UtilityFunctions.h
	$(CLINKER) $(OPTFLAGS) -o summation $(SUMOBJS)  $(LIBS)


test: $(EXES)
	./summation

clean:
	rm -rf $(EXES) *.o

.SUFFIXES:
.SUFFIXES: .c .cc .o

.c.o:
	$(CC) $(CFLAGS) -c $<

.cc.o:
	$(CC) $(CFLAGS) -c $<
