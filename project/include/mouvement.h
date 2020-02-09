/*
 * Copyright (c) 2019-2020 AIDER Smail
 */
#ifndef __MOUVEMENT_H_
#define __MOUVEMENT_H_

#include "nrdef.h"

#define VMIN	1
#define VMAX	254
#define N	3
#define	CODAGE	255	/* codage logique(225)/binaire(1) */

struct segma_t {
	uint8 **Mt;	/* image de fond */
	uint8 **Ot;	/* image de différence */
	uint8 **Vt; 	/* image de variance */
	uint8 **Et;	/* image d'étiquettes */	
};

struct image_t {
	uint8 **img;
	long height;
	long width;
};


int SegmaDelta_step0(struct segma_t *param, struct image_t *image);
int SegmaDelta_1step(struct segma_t *param, struct image_t *image);


#endif /*__MOUVEMENT_H_*/

