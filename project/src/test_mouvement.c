/*
 * Copyright (c) 2019-2020 AIDER Smail
 */
#include <stdio.h>
#include <stdlib.h>

#include "mouvement.h"
#include "nrutil.h"


int main(int argc, char *argv)
{
	int i;
	int nrl, nrh, ncl, nch;
	char filename[128] = "/home/aider/Desktop/projet_progpar/projet/car3/car_3000.pgm";
	char *in_path 	   = "/home/aider/Desktop/projet_progpar/projet/car3";
	char *out_path	   = "/home/aider/Desktop/projet_progpar/projet/out_segma";
	
	// load image_0 alloc struct
	struct image_t *image = malloc(sizeof(struct image_t));
	image->img = LoadPGM_ui8matrix(filename, &nrl, &nrh, &ncl, &nch);
	image->height = nrh+1;
	image->width  = nch+1;
	
	struct segma_t *param = malloc(sizeof(struct segma_t));
	param->Mt = ui8matrix(nrl, nrh, ncl, nch);
	param->Ot = ui8matrix(nrl, nrh, ncl, nch);
	param->Vt = ui8matrix(nrl, nrh, ncl, nch);
	param->Et = ui8matrix(nrl, nrh, ncl, nch);

	// step #0
	SegmaDelta_step0(param, image);
	
	// step #2
	for(i = 1; i < 200; i++){
		// in
		sprintf(filename, "%s/car_%d.pgm", in_path, i+3000);
		free_ui8matrix(image->img, nrl, nrh, ncl, nch);
		
		// segdelta
		image->img = LoadPGM_ui8matrix(filename, &nrl, &nrh, &ncl, &nch);
		SegmaDelta_1step(param, image);	
		
		// out
		sprintf(filename, "%s/car_%d.pgm", out_path, i+3000);
		SavePGM_ui8matrix(param->Et, nrl, nrh, ncl, nch, filename);
	}
	
	// free
	free_ui8matrix(image->img, nrl, nrh, ncl, nch);
	free_ui8matrix(param->Mt, nrl, nrh, ncl, nch);
	free_ui8matrix(param->Ot, nrl, nrh, ncl, nch);
	free_ui8matrix(param->Vt, nrl, nrh, ncl, nch);
	free_ui8matrix(param->Et, nrl, nrh, ncl, nch);
	free(param);
	free(image);

	return 0;
}
