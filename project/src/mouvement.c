/*
 * Copyright (c) 2019-2020 AIDER Smail
 */
#include "mouvement.h"

#include <stdlib.h>
#include "nrutil.h"

#define max(a,b) (a > b ? a:b)
#define min(a,b) (a < b ? a:b)

int SegmaDelta_step0(struct segma_t *param, struct image_t *image)
{
	long i, j;
	
	for(i = 0; i < image->height; i++){
		for(j = 0; j < image->width; j++){
			param->Mt[i][j] = image->img[i][j];
			param->Vt[i][j] = VMIN;
		}
	}
	
	return 0;
}

int SegmaDelta_1step(struct segma_t *param, struct image_t *image)
{
	long i, j;
	uint8 **Mt = param->Mt;	
	uint8 **Ot = param->Ot;
	uint8 **Vt = param->Vt; 	
	uint8 **Et = param->Et;
	uint8 **It= image->img;
	
	/* step #1: Mt estimation */
	for(i = 0; i < image->height; i++){
		for(j = 0; j < image->width; j++){
			if     (Mt[i][j] < It[i][j]) 	Mt[i][j] = Mt[i][j] + 1;
			else if(Mt[i][j] > It[i][j]) 	Mt[i][j] = Mt[i][j] - 1;
			else 							Mt[i][j] = Mt[i][j];
		}
	}
	/* step #2: Ot computation */
	for(i = 0; i < image->height; i++)
		for(j = 0; j < image->width; j++)
			Ot[i][j] = abs(Mt[i][j] - It[i][j]);
			
	/* step #3: Vt update and clamping */
	for(i = 0; i < image->height; i++){
		for(j = 0; j < image->width; j++){
			if(Vt[i][j] < N * Ot[i][j])		 Vt[i][j] = Vt[i][j] + 1;
			else if(Vt[i][j] > N * Ot[i][j]) Vt[i][j] = Vt[i][j] - 1;
			else							 Vt[i][j] = Vt[i][j];
			Vt[i][j] = max(min(Vt[i][j], VMAX), VMIN);
		}
	}
	/* step #4: Et estimation */
	for(i = 0; i < image->height; i++){
		for(j = 0; j < image->width; j++){
			if(Ot[i][j] < Vt[i][j])		Et[i][j] = 0;
			else 						Et[i][j] = CODAGE;
		}
	}
	
	return 0;
}
