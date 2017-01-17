#include <math.h>
#include "hmm_edit.h"
#include "nrutil.h"
static char rcsid[] = "$Id: viterbi.c,v 1.1 1999/05/06 05:25:37 kanungo Exp kanungo $";

#define VITHUGE  100000000000.0

void Viterbi(HMM *phmm, int T, int *O1, double *O2, double **delta, int **psi, 
	int *q, double *pprob)
{
	int 	i, j;	/* state indices */
	int  	t;	/* time index */	

	int	maxvalind;
	double	maxval, val;

	/* 1. Initialization  */
	
	for (i = 1; i <= phmm->N; i++) {
		delta[1][i] = phmm->pi[i] * ComputeEmission(phmm, i, 1, O1, O2);
		psi[1][i] = 0;
    //fprintf(stdout, "test %d %d %lf %lf %lf", i, O1[t], O2[t], phmm->pi[i], ComputeEmission(phmm, i, 1, O1, O2));
	}	

	/* 2. Recursion */
	
	for (t = 2; t <= T; t++) {
		for (j = 1; j <= phmm->N; j++) {
			maxval = 0.0; 
			maxvalind = 1;	
			for (i = 1; i <= phmm->N; i++) {
				val = delta[t-1][i]*(phmm->A[i][j]);
				if (val > maxval && val != 0.0) {
					maxval = val;	
					maxvalind = i;	
				}
			} 
			
			delta[t][j] = maxval*ComputeEmission(phmm, j, t, O1, O2);
      //fprintf(stdout, "test %d %d %lf %lf %lf", maxvalind, O1[t], O2[t], maxval, ComputeEmission(phmm, j, t, O1, O2));
			psi[t][j] = maxvalind; 
      //fprintf(stdout, "test %d %d ", maxvalind,psi[t][j]);

		}
	}

	/* 3. Termination */

	*pprob = 0.0;
	//q[T] = 1;
	for (i = 1; i <= phmm->N; i++) {
    if (delta[T][i] > *pprob && delta[T][i] != 0.0) {
			*pprob = delta[T][i];	
			q[T] = i;
		}
	}

	/* 4. Path (state sequence) backtracking */

	for (t = T - 1; t >= 1; t--)
		q[t] = psi[t+1][q[t+1]];

}
void ViterbiLog(HMM *phmm, int T, int *O1, double *O2, double **delta, int **psi,
        int *q, double *pprob)
{
        int     i, j;   /* state indices */
        int     t;      /* time index */
 
        int     maxvalind;
        double  maxval, val;
	double  **biot;

	/* 0. Preprocessing */

	for (i = 1; i <= phmm->N; i++) 
		phmm->pi[i] = log(phmm->pi[i]);
	for (i = 1; i <= phmm->N; i++) 
		for (j = 1; j <= phmm->N; j++) {
			phmm->A[i][j] = log(phmm->A[i][j]);
		}

	biot = dmatrix(1, phmm->N, 1, T);
	for (i = 1; i <= phmm->N; i++) 
		for (t = 1; t <= T; t++) {
			biot[i][t] = log(ComputeEmission(phmm, i, t, O1, O2));
		}
 
        /* 1. Initialization  */
 
        for (i = 1; i <= phmm->N; i++) {
                delta[1][i] = phmm->pi[i] + biot[i][1];
                psi[1][i] = 0;
        }
 
        /* 2. Recursion */
 
        for (t = 2; t <= T; t++) {
                for (j = 1; j <= phmm->N; j++) {
                        maxval = -VITHUGE;
                        maxvalind = 1;
                        for (i = 1; i <= phmm->N; i++) {
                                val = delta[t-1][i] + (phmm->A[i][j]);
                                if (val > maxval) {
                                        maxval = val;
                                        maxvalind = i;
                                }
                        }
 
                        delta[t][j] = maxval + biot[j][t]; 
                        psi[t][j] = maxvalind;
 
                }
        }
 
        /* 3. Termination */
 
        *pprob = -VITHUGE;
        q[T] = 1;
        for (i = 1; i <= phmm->N; i++) {
                if (delta[T][i] > *pprob) {
                        *pprob = delta[T][i];
                        q[T] = i;
                }
        }
 
 
	/* 4. Path (state sequence) backtracking */

	for (t = T - 1; t >= 1; t--)
		q[t] = psi[t+1][q[t+1]];

}
 

