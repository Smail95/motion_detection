/*
 * Copyright (c) 2019-2020 AIDER Smail
 */
#ifndef __MOUVEMENT_SIMD_H_
#define __MOUVEMENT_SIMD_H_

#include "vnrdef.h"

#define VMIN	1
#define VMAX	254
#define N	3
#define	CODAGE	255	/* codage logique(225)/binaire(1) */

struct segma_t {
	vuint8 **Mt;	/* image de fond */
	vuint8 **Ot;	/* image de différence */
	vuint8 **Vt; 	/* image de variance */
	vuint8 **Et;	/* image d'étiquettes */	
};

struct image_t {
	vuint8 **img;
	int height;
	int width;
};

int SegmaDelta_step0(struct segma_t *param, struct image_t *image);
int SegmaDelta_1step(struct segma_t *param, struct image_t *image);

#endif /*__MOUVEMENT_SIMD_H_*/

