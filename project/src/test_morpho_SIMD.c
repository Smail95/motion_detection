/*
 * Copyright (c) 2019-2020 AIDER Smail
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "morpho_SIMD.h"
#include "vnrutil.h"
#include "nrutil.h"

int main(int argc, char *argv)
{
	
	uint8** u8_img;
	int nrl, nrh, ncl, nch, i, j;
	int vnrl, vnrh, vncl, vnch;
	char filename[128] = "/home/aider/Desktop/projet_progpar/projet/out_segma/car_3001.pgm";
	char *in_path	   = "/home/aider/Desktop/projet_progpar/projet/out_segma";
	char *out_path	   = "/home/aider/Desktop/projet_progpar/projet/out_morpho";
	
	
	// alloc struct
	struct image_t *in_img 	= malloc(sizeof(struct image_t));
	struct image_t *out_img = malloc(sizeof(struct image_t));
	
	// load image 0 & get s&v dim
	u8_img = LoadPGM_ui8matrix(filename, &nrl, &nrh, &ncl, &nch);
	s2v(nrl, nrh, ncl, nch, card_vuint8(), &vnrl, &vnrh, &vncl, &vnch);
	
	out_img->img 	= vui8matrix(vnrl, vnrh, vncl, vnch);
	in_img->height 	= vnrh+1;
	in_img->width  	= vnch+1;
	out_img->height	= vnrh+1;
	out_img->width 	= vnch+1;
	
	for(i = 1; i < 200; i++){
		// in 
		if(i > 1){
			sprintf(filename, "%s/car_%d.pgm", in_path, i+3000);
			u8_img = LoadPGM_ui8matrix(filename, &nrl, &nrh, &ncl, &nch);
		}
		in_img->img = (vuint8**)u8_img; 
		
		// morpho
		morph_erosion_3x3 (in_img, out_img);		
		morph_dilation_3x3(out_img, in_img);		
		morph_dilation_3x3(in_img, out_img);		
		morph_erosion_3x3 (out_img, in_img);
		
		// out
		sprintf(filename, "%s/car_%d.pgm", out_path, i+3000);
		SavePGM_ui8matrix((uint8**)in_img->img, nrl, nrh, ncl, nch, filename);
		
		// free
		free_ui8matrix(u8_img, nrl, nrh, ncl, nch);
	}
		
	// free
	free_vui8matrix(out_img->img, nrl, nrh, ncl, nch);
	free(in_img);
	free(out_img);

	return 0;
}
