/*
 * Copyright (c) 2019-2020 AIDER Smail
 */
#ifndef __MORPHO_H_
#define __MORPHO_H_

#include "mouvement.h"

#define SE		3
#define EROSION		0
#define DILATION	1	

void opening(struct image_t *in, struct image_t *out);
void closing(struct image_t *in, struct image_t *out);
void morph(struct image_t *in, struct image_t *out, int type);
void get_boundary(int x, int y, struct image_t *image, int *boundary);
int vec_min(int *boundary);
int vec_max(int *boundary);

#endif /* __MORPHO_H_ */
