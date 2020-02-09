/*
 * Copyright (c) 2019-2020 AIDER Smail
 */
#include <stdio.h>
#include <stdlib.h>

#include "morpho.h"
#include "nrutil.h"


int main(int argc, char *argv)
{
	int i, j;
	int nrl, nrh, ncl, nch;
	char filename[128] = "/home/aider/Desktop/projet_progpar/projet/out_segma/car_3001.pgm";
	char *in_path	   = "/home/aider/Desktop/projet_progpar/projet/out_segma";
	char *out_path	   = "/home/aider/Desktop/projet_progpar/projet/out_morpho";
	
	// alloc 	
	struct image_t *in_img 	= malloc(sizeof(struct image_t));
	struct image_t *out_img = malloc(sizeof(struct image_t));
	
	// load image 0 & get dim
	in_img->img = LoadPGM_ui8matrix(filename, &nrl, &nrh, &ncl, &nch);
	
	out_img->img 	= ui8matrix(nrl, nrh, ncl, nch);
	in_img->height 	= nrh+1;
	in_img->width  	= nch+1;
	out_img->height	= nrh+1;
	out_img->width 	= nch+1;
	
	for(i = 1; i < 200; i++){
		// in 
		if(i>1){
			sprintf(filename, "%s/car_%d.pgm", in_path, i+3000);
			in_img->img = LoadPGM_ui8matrix(filename, &nrl, &nrh, &ncl, &nch);
		}
		// morpho
		morph(in_img, out_img, EROSION);
		morph(out_img, in_img, DILATION);
		morph(in_img, out_img, DILATION);
		morph(out_img, in_img, EROSION);
		
		// out
		sprintf(filename, "%s/car_%d.pgm", out_path, i+3000);
		SavePGM_ui8matrix(in_img->img, nrl, nrh, ncl, nch, filename);
		
		// free
		free_ui8matrix(in_img->img, nrl, nrh, ncl, nch);
	}
		
	// free
	free_ui8matrix(out_img->img, nrl, nrh, ncl, nch);
	free(in_img);
	free(out_img);

	return 0;
}
