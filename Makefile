CFLAGS= -g
INCS=
# use the following line to "Purify" the code
#CC=purify gcc
CC=gcc
SRCS=BaumWelch_edit.c viterbi.c forward_edit.c backward_edit.c hmmutils_edit.c sequence_edit.c \
	genseq.c nrutil.c testvit.c esthmm_edit.c hmmrand_edit.c testfor.c const.c

all :	esthmm viterbi
	
esthmm: esthmm_edit.o BaumWelch_edit.o nrutil.o hmmutils_edit.o sequence_edit.o \
		forward_edit.o backward_edit.o hmmrand_edit.o const.o
	 $(CC) -o esthmm esthmm_edit.o BaumWelch_edit.o nrutil.o sequence_edit.o hmmutils_edit.o \
		forward_edit.o backward_edit.o hmmrand_edit.o const.o -lm
viterbi: runViterbi.o viterbi_edit.o nrutil.o hmmutils_edit.o sequence_edit.o forward_edit.o const.o
	 $(CC) -o viterbi runViterbi.o viterbi_edit.o nrutil.o sequence_edit.o forward_edit.o const.o\
		hmmutils_edit.o  hmmrand_edit.o -lm 
clean:
	rm *.o 
# DO NOT DELETE THIS LINE -- make depend depends on it.

