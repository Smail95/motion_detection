/*
 * Copyright (c) 2019-2020 AIDER Smail
 */
#include "mouvement_SIMD.h"
#include "vnrutil.h"

#include <stdio.h>

int SegmaDelta_step0(struct segma_t *param, struct image_t *image)
{
	int i, j;
	vuint8 vvmin = vmovq_n_u8(VMIN);
	
	for(i = 0; i < image->height; i++){
		for(j = 0; j < image->width; j++){
			param->Mt[i][j] = image->img[i][j];
			param->Vt[i][j] = vvmin;
		}
	}
	return 0;
}


int SegmaDelta_1step(struct segma_t *param, struct image_t *image)
{
	int i, j;
	vuint8 mul, c;
	vuint8 vn	= vmovq_n_u8(N);
	vuint8 vvmax	= vmovq_n_u8(VMAX);
	vuint8 vvmin	= vmovq_n_u8(VMIN);
	vuint8 vcdg 	= vmovq_n_u8(CODAGE);
	vuint8 vzero	= vmovq_n_u8(0);
	vuint8 vone 	= vmovq_n_u8(1);
	vuint8 v2 	= vmovq_n_u8(2);
	
	vuint8 **Mt = param->Mt;	
	vuint8 **Ot = param->Ot;
	vuint8 **Vt = param->Vt; 	
	vuint8 **Et = param->Et;
	vuint8 **It = image->img;
	
	/* step #1: Mt estimation */
	for(i = 0; i < image->height; i++){
		for(j = 0; j < image->width; j++){
				
			c = vcltq_u8(Mt[i][j], It[i][j]); // less than 
			Mt[i][j] = vbslq_u8(c, vaddq_u8(Mt[i][j], vone),  Mt[i][j]);
			c = vcgtq_u8(Mt[i][j], It[i][j]); // greater than 
			Mt[i][j] = vbslq_u8(c, vsubq_u8(Mt[i][j], vone),  Mt[i][j]);
		}
	}
	/* step #2: Ot computation */
	for(i = 0; i < image->height; i++){
		for(j = 0; j < image->width; j++){
			Ot[i][j] = vabdq_u8(Mt[i][j], It[i][j]);
		}
	}
	/* step #3: Vt update and clamping */
	for(i = 0; i < image->height; i++){
		for(j = 0; j < image->width; j++){
			
			mul = vmulq_u8(vn, Ot[i][j]);
			c   =  vcltq_u8(Vt[i][j], mul); // if(Vt[i][j] < N*Ot[i][j]) ++
			Vt[i][j] = vbslq_u8(c, vaddq_u8(Vt[i][j], vone), Vt[i][j]);
			c   =  vcgtq_u8(Vt[i][j], mul); // if(Vt[i][j] > N*Ot[i][j]) --
			Vt[i][j] = vbslq_u8(c, vsubq_u8(Vt[i][j], vone), Vt[i][j]);
			
			Vt[i][j] = vmaxq_u8(vminq_u8(Vt[i][j],vvmax), vvmin);
		}
	}
	/* step #4: Et estimation */
	for(i = 0; i < image->height; i++){
		for(j = 0; j < image->width; j++){
			
			c = vcltq_u8(Ot[i][j], Vt[i][j]); // less than
			Et[i][j] = vbslq_u8(c, vzero, vcdg);
		}
	}
	return 0;
}
