/*
 * Copyright (c) 2019-2020 AIDER Smail
 */
#include <stdio.h>
#include <stdlib.h>

#include "mouvement_SIMD.h"
#include "vnrutil.h"
#include "nrutil.h"

int main(int argc, char *argv)
{
	uint8** u8_img;
	int i, nrl, nrh, ncl, nch;
	int vnrl, vnrh, vncl, vnch;
	char filename[128] = "/home/aider/Desktop/projet_progpar/projet/car3/car_3000.pgm";
	char *in_path 	   = "/home/aider/Desktop/projet_progpar/projet/car3";
	char *out_path	   = "/home/aider/Desktop/projet_progpar/projet/out_segma";
	
	// load image_0 & get s&v indexes
	u8_img = LoadPGM_ui8matrix(filename, &nrl, &nrh, &ncl, &nch);
	s2v(nrl, nrh, ncl, nch, card_vuint8(), &vnrl, &vnrh, &vncl, &vnch);
	
	// alloc struct
	struct segma_t *param = malloc(sizeof(struct segma_t));
	param->Mt = vui8matrix(vnrl, vnrh, vncl, vnch);
	param->Ot = vui8matrix(vnrl, vnrh, vncl, vnch);
	param->Vt = vui8matrix(vnrl, vnrh, vncl, vnch);
	param->Et = vui8matrix(vnrl, vnrh, vncl, vnch);
	
	struct image_t *image = malloc(sizeof(struct image_t));
	//image->img = vui8matrix(vnrl, vnrh, vncl, vnch);
	image->height = vnrh+1;
	image->width  = vnch+1;
	
	// step #0
	//convert_ui8matrix_vui8matrix(u8_img, nrl, nrh, ncl, nch, image->img);
	image->img = (vuint8**)u8_img;
	SegmaDelta_step0(param, image);
	
	// step #2
	for(i = 1; i < 200; i++){
		// free
		free_ui8matrix(u8_img, nrl, nrh, ncl, nch);
		
		// in
		sprintf(filename, "%s/car_%d.pgm", in_path, i+3000);
		u8_img = LoadPGM_ui8matrix(filename, &nrl, &nrh, &ncl, &nch);
		image->img = (vuint8**)u8_img;
		
		// segdelta
		SegmaDelta_1step(param, image);	
		
		// out
		sprintf(filename, "%s/car_%d.pgm", out_path, i+3000);
		SavePGM_ui8matrix((uint8**)param->Et, nrl, nrh, ncl, nch, filename);
	}
	
	// free
	free_ui8matrix(u8_img, nrl, nrh, ncl, nch);
	free_vui8matrix(param->Mt,  vnrl, vnrh, vncl, vnch);
	free_vui8matrix(param->Ot,  vnrl, vnrh, vncl, vnch);
	free_vui8matrix(param->Vt,  vnrl, vnrh, vncl, vnch);
	free_vui8matrix(param->Et,  vnrl, vnrh, vncl, vnch);
	free(param);
	free(image);

	return 0;
}
