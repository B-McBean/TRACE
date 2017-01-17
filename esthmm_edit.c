#include <stdio.h> 
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "nrutil.h"
#include "hmm_edit.h"
#include <sys/types.h>
#include <unistd.h>

static char rcsid[] = "$Id: esthmm.c,v 1.1 1998/02/23 07:49:45 kanungo Exp kanungo $";

void Usage(char *name);

int main (int argc, char *argv[])
{
  
	int T;
  double *GC;
	HMM  	hmm;
	int	N;
	int	M;
	double 	**alpha; 
	double	**beta;
	double	**gamma;
	double	*O2; /* slop */
  int *O1; /* sequence, represented by number, 1=A, 2=C, 3=G, 4=T */
	//int	iflg=0, sflg=0, nflg=0, mflg=0, errflg =0, vflg=0;
	int	c;
	int	seed; /* seed for random number generator */
	//char	*hmminitfile;
	int	niter;
	double	logprobinit, logprobfinal;
	FILE	*fp;
 
  
	if (argc != 6) {
		Usage(argv[0]);
		exit (1);
	}
	/* read the observed sequence */
	fp = fopen(argv[1], "r");
	if (fp == NULL) {
		fprintf(stderr, "Error: File %s not found \n", argv[1]);
		exit (1);
	}
	ReadSequence(fp, &T, &GC, &O1); 
	fclose(fp);
 
  fp = fopen(argv[2], "r");
	if (fp == NULL) {
		fprintf(stderr, "Error: File %s not found \n", argv[2]);
		exit (1);
	}
	ReadSlop(fp, &T, &O2); 
	fclose(fp);
  
	/* initialize the hmm model */
  fp = fopen(argv[3], "r");
	if (fp == NULL) {
    fprintf(stderr, "Error: File %s not found \n", argv[3]);
   	exit (1);
	}
  
	ReadHMM(fp, &hmm);
  
	fclose(fp);
  
  sscanf(argv[4], "%d", &seed);
  if (&seed == NULL) {
    fprintf(stderr, "Error: seed %s not valid \n", argv[4]);
    fprintf(stdout, "T: %d \n", T);
    //fprintf(stdout, "D: %d \n", hmm.D);
    //fprintf(stdout, "N: %d \n", hmm.N);
    //fprintf(stdout, "GC: %lf \n", GC[1]);
   	exit (1);
	}
  
  InitHMMwithInput(&hmm, seed, GC);
  
  if (hmm.CG == NULL){
    printf("PROB");
  }
  PrintHMM(stdout, &hmm);
	/* allocate memory */
	alpha = dmatrix(1, T, 1, hmm.N);
	beta = dmatrix(1, T, 1, hmm.N);
	gamma = dmatrix(1, T, 1, hmm.N);
	/* call Baum Welch */
	BaumWelch(&hmm, T, O1, O2, alpha, beta, gamma, &niter, 
		&logprobinit, &logprobfinal);
  
/*
	if (vflg) {
		if (sflg) fprintf(stderr, "RandomSeed: %d\n", seed);
		fprintf(stderr, "Number of iterations: %d\n", niter);
		fprintf(stderr, "Log Prob(observation | init model): %E\n",
			logprobinit);	
		fprintf(stderr, "Log Prob(observation | estimated model): %E\n",
			logprobfinal);	
	}
*/

	/* print the answer */
	PrintHMM(stdout, &hmm);
  fp = fopen(argv[5], "w");
  if (fp == NULL) {
    fprintf(stderr, "Error: File %s not valid \n", argv[5]);
   	exit (1);
	}
  PrintHMM(fp, &hmm);
  
	/* free memory */
	free_ivector(O1, 1, T);
  //fprintf(stdout, "free1");
  free_dvector(O2, 1, T);
  //fprintf(stdout, "free2");
	free_dmatrix(alpha, 1, T, 1, hmm.N);
  //fprintf(stdout, "free3");
	free_dmatrix(beta, 1, T, 1, hmm.N);
  //fprintf(stdout, "free4");
	free_dmatrix(gamma, 1, T, 1, hmm.N);
  //fprintf(stdout, "free5");
	FreeHMM(&hmm);
}

void Usage(char *name)
{
	printf("Usage error. \n");
        /* printf("Usage1: %s [-v] -N <num_states> -M <num_symbols> <file.seq>\n", 
		name);
        printf("Usage2: %s [-v] -S <seed> -N <num_states> -M <num_symbols> <file.seq>\n", 
		name); */
        printf("Usage: ./esthmm <file.seq> <file.slop> <mod.hmm> <seed> <out.hmm>\n");
        
        printf("  seed - seed for random number genrator\n");
        printf("  mod.hmm - file with the initial model parameters\n");
        printf("  file.seq - file containing the obs. seqence\n");
        printf("  file.slop - file containing the obs. slop\n");
	//printf("  v - prints out number of iterations and log prob\n");
}
