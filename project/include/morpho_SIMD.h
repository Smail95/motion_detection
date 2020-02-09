/*
 * Copyright (c) 2019-2020 AIDER Smail
 */
#ifndef __MORPHO_SIMD_H_
#define __MORPHO_SIMD_H_

#include "mouvement_SIMD.h"

void opening(struct image_t *in, struct image_t *out);
void closing(struct image_t *in, struct image_t *out);
void morph_erosion_3x3(struct image_t *in, struct image_t *out);
void morph_erosion_5x5(struct image_t *in, struct image_t *out);
void morph_dilation_3x3(struct image_t *in, struct image_t *out);
void morph_dilation_5x5(struct image_t *in, struct image_t *out);
void get_boundaryIdx(int x, int y, struct image_t *image, int *boundary, int SE);

#endif /* __MORPHO_SIMD_H_ */
