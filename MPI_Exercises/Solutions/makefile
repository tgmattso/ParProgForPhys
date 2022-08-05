# 

include ../make.def

EXES= pi_bsp$(EXE) hello_mpi$(EXE) ring$(EXE) trans$(EXE) ping$(EXE)

RINGOBJS = ring.$(OBJ) ring_naive.$(OBJ) ring_split.$(OBJ) \
           ring_sndrcv.$(OBJ) ring_persistent.$(OBJ)

TRANSOBJS = trans_isnd_ircv.$(OBJ)  trans_sendrcv.$(OBJ)  \
            trans_snd_rcv.$(OBJ) trans_utility.$(OBJ)  transpose.$(OBJ)

PINGOBJS  = ping_pong.$(OBJ) random.$(OBJ) time_stats.$(OBJ) wt_overhead.$(OBJ)

all: $(EXES)

pi_bsp$(EXE): pi_bsp.$(OBJ) 
	$(CLINKER) $(OPTFLAGS) -o pi_bsp pi_bsp.$(OBJ) $(LIBS)

hello_mpi$(EXE): hello_mpi.$(OBJ) 
	$(CLINKER) $(OPTFLAGS) -o hello_mpi hello_mpi.$(OBJ) $(LIBS)

ring$(EXE): $(RINGOBJS)
	$(CLINKER) $(OPTFLAGS) -o ring $(RINGOBJS) $(LIBS)

trans$(EXE): $(TRANSOBJS) trans_utility.h
	$(CLINKER) $(OPTFLAGS) -o trans $(TRANSOBJS) $(LIBS)

ping$(EXE): $(PINGOBJS) trans_utility.h
	$(CLINKER) $(OPTFLAGS) -o ping $(PINGOBJS) $(LIBS)

test: $(EXES)
	mpiexec -n 4 $(PRE)pi_bsp; \
        mpiexec -n 4 $(PRE)hello_mpi;  \
        mpiexec -n 4 $(PRE)ring;  \
        mpiexec -n 4 $(PRE)trans;  \
        mpiexec -n 2 $(PRE)ping

clean:
	$(RM) -f $(EXES) *.$(OBJ)

.SUFFIXES:
.SUFFIXES: .c .cpp .$(OBJ)

.c.$(OBJ):
	$(CC) $(CFLAGS) -c $<

.cpp.$(OBJ):
	$(CC) $(CFLAGS) -c $<
#test: $(EXES)
#	for i in $(EXES); do \
#            mpiexec -n 4 $(PRE)$$i; \
#        done
