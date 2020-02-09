/*
 * Copyright (c) 2019-2020 AIDER Smail
 */
#include "morpho.h"

#include <stdio.h>
#include <stdlib.h>
#include "nrutil.h"
#include "math.h"

void opening(struct image_t *in, struct image_t *out)
{
	struct image_t *tmp;
	int nrl, nrh, ncl, nch;
	
	nrl = 0; 
	nrh = in->height-1; 
	ncl = 0;
	nch = in->width-1;
	
	tmp 	    = malloc(sizeof(struct image_t));
	tmp->img    = ui8matrix(nrl, nrh, ncl, nch);
	tmp->height = in->height;
	tmp->width  = in->width;
	
	morph(in, tmp, DILATION);
	morph(tmp, out, EROSION);
	
	free_ui8matrix(tmp->img, nrl, nrh, ncl, nch);
	free(tmp);
}

void closing(struct image_t *in, struct image_t *out)
{
	struct image_t *tmp;
	int nrl, nrh, ncl, nch;
	
	nrl = 0; 
	nrh = in->height-1; 
	ncl = 0;
	nch = in->width-1;
	
	tmp 	    = malloc(sizeof(struct image_t));
	tmp->img    = ui8matrix(nrl, nrh, ncl, nch);
	tmp->height = in->height;
	tmp->width  = in->width;
	
	morph(in, tmp, EROSION);
	morph(tmp, out, DILATION);
	
	free_ui8matrix(tmp->img, nrl, nrh, ncl, nch);
	free(tmp);
}

void morph(struct image_t *in, struct image_t *out, int type)
{
	int i,j, val;
	int boundary[SE*SE]; // atmost se*se-1
	
	for(i = 0; i < in->height; i++){
		for(j = 0; j < in->width; j++){
			// get pixel neighborhood
			get_boundary(i, j, in, boundary);
			
			// get out pixel value
			if(type == EROSION)
				val = vec_min(boundary);
			if(type == DILATION) 	      
				val = vec_max(boundary);
			
			// set out pixel value
			if(val == -1)
				out->img[i][j] = in->img[i][j];
			else
				out->img[i][j] = val;
		}
	}
}

void get_boundary(int x, int y, struct image_t *image, int *boundary)
{
	int i, j, k = 0;
	int px, py;
	
	for(i = 0; i < SE; i++){
		for(j = 0; j < SE; j++){
			px = x + i - (SE/2);
			py = y + j - (SE/2);
			// ignore some neighborhood
			if((px < 0 || px >= image->height) ||
			   (py < 0 || py >= image->width)  ||
			   (px == x && py == y)) continue;

			boundary[k++] = image->img[px][py];
		}
	}
	boundary[k] = -1; // mark end
}

int vec_min(int *boundary)
{
	int i = 0;
	
	while(boundary[i] != -1){
		if(boundary[i] == 0)
			return 0;
		i++;
	}
	return -1;
}

int vec_max(int *boundary)
{
	int i = 0;
	
	while(boundary[i] != -1){
		if(boundary[i] == CODAGE)
			return CODAGE;
		i++;
	}
	return -1;
}
