#
#  USAGE:
#     make          ... to build the programs
#     make test     ... to run the default test case
#
#  NOTE:
#     summationCpp won't build unless the C utilityFunctions is
#     compiled with the C++ compiler. 
#

CC          = gcc-14
CPP         = g++-14
CLINKER     = $(CC)
CPPLINKER   = $(CPP)
#OPTFLAGS   = -std=c11 -fopenmp -O3
OPTFLAGS    = -O0 -fopenmp -Wall -frounding-math
LIBS        = -lm
CFLAGS	  = $(OPTFLAGS)

EXES = roundingC roundingCpp  summation  summationCpp

SUMOBJS    = summation.o    UtilityFunctions.o
SUMOBJSCpp = summationCpp.o UtilityFunctions.o

all: $(EXES)

roundingC: roundingC.o
	$(CLINKER) $(OPTFLAGS) -o roundingC roundingC.o $(LIBS)

roundingCpp: roundingCpp.o
	$(CPPLINKER) $(OPTFLAGS) -o roundingCpp roundingCpp.o $(LIBS)

summation: $(SUMOBJS)
	$(CLINKER) $(OPTFLAGS) -o summation $(SUMOBJS)  $(LIBS)

summationCpp: summationCpp.o
	$(CPPLINKER) $(OPTFLAGS) -o summationCpp  \
		summationCpp.o UtilityFunctions.c $(LIBS)

test: $(EXES)
	./roundingC
	./roundingCpp
	./summation
	./summationCpp


clean:
	rm -rf $(EXES) *.o

.SUFFIXES:
.SUFFIXES: .c .cc .o

.c.o:
	$(CC) $(CFLAGS) -c $<

.cc.o:
	$(CPP) $(CFLAGS) -c $<

