/*
 * Copyright (c) 2019-2020 AIDER Smail
 */
#include "morpho_SIMD.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "vnrutil.h"

void morph_erosion_3x3(struct image_t *in, struct image_t *out)
{
	int i,j, k;
	int boundary[3*3*2]; 			// [[x,y],...];
	vuint8 m_left, m_middle, m_right;	// m(ax/in)_x
	vuint8 m_right1; 			// (card-1)middle_(1)right
	vuint8 left1_m;  			// (1)left_(card-1)middle
	vuint8 result; 
	
	for(i = 0; i < in->height; i++){	// row
		for(j = 0; j < in->width; j++){ // column
 			// get boundary indexes
			get_boundaryIdx(i, j, in, boundary, 3);
			
			// min boundary of vpixel[i][j]
			k=0;
			// border -1
			m_left = vminq_u8(in->img[boundary[k+0]][boundary[k+1]],
					  in->img[boundary[k+2]][boundary[k+3]]);
			m_left = vminq_u8(in->img[boundary[k+4]][boundary[k+5]], m_left);
			
			k=6;
			// border 0
			m_middle = vminq_u8(in->img[boundary[k+0]][boundary[k+1]],
					    in->img[boundary[k+2]][boundary[k+3]]);
			m_middle = vminq_u8(in->img[boundary[k+4]][boundary[k+5]], m_middle);
			
			k = 12;
			// border 12
			m_right = vminq_u8(in->img[boundary[k+0]][boundary[k+1]],
					   in->img[boundary[k+2]][boundary[k+3]]);
			m_right = vminq_u8(in->img[boundary[k+4]][boundary[k+5]], m_right);
			
			// shift left & right
			m_right1 = vextq_u8(m_middle, m_right, 1);
			left1_m  = vextq_u8(m_left, m_middle, 15);
			
			// max/min result
			result = vminq_u8(m_right1, m_middle);
			result = vminq_u8(left1_m , result);
			
			// set out pixel value
			out->img[i][j] = result;
		}
	}
}

void morph_dilation_3x3(struct image_t *in, struct image_t *out)
{
	int i,j, k;
	int boundary[3*3*2]; 			// [[x,y],...];
	vuint8 m_left, m_middle, m_right;	// m(ax/in)_x
	vuint8 m_right1; 			// (card-1)middle_(1)right
	vuint8 left1_m;  			// (1)left_(card-1)middle
	vuint8 result; 
	
	for(i = 0; i < in->height; i++){	// row
		for(j = 0; j < in->width; j++){ // column
 			// get boundary indexes
			get_boundaryIdx(i, j, in, boundary, 3);
			
			// min boundary of vpixel[i][j]
			k=0;
			// border -1
			m_left = vmaxq_u8(in->img[boundary[k+0]][boundary[k+1]],
					  in->img[boundary[k+2]][boundary[k+3]]);
			m_left = vmaxq_u8(in->img[boundary[k+4]][boundary[k+5]], m_left);
			
			k=6;
			// border 0
			m_middle = vmaxq_u8(in->img[boundary[k+0]][boundary[k+1]],
					    in->img[boundary[k+2]][boundary[k+3]]);
			m_middle = vmaxq_u8(in->img[boundary[k+4]][boundary[k+5]], m_middle);
		
			k=12;
			// border 1
			m_right = vmaxq_u8(in->img[boundary[k+0]][boundary[k+1]],
					   in->img[boundary[k+2]][boundary[k+3]]);
			m_right = vmaxq_u8(in->img[boundary[k+4]][boundary[k+5]], m_right);
			
			// shift left & right
			m_right1 = vextq_u8(m_middle, m_right, 1);
			left1_m  = vextq_u8(m_left, m_middle, 15);
			
			// max/min result
			result = vmaxq_u8(m_right1, m_middle);
			result = vmaxq_u8(left1_m , result);
			
			// set out pixel value
			out->img[i][j] = result;
		}
	}
}

/* get boundary index of the pixel(l;c), column per column */
void get_boundaryIdx(int l, int c, struct image_t *image, int *boundary, int SE)
{
	int i, j, k = 0;
	int pl, pc;
	vuint8 cond;
	vsint8 zero	= vmovq_n_s8(0);
	vuint8 height 	= vmovq_n_u8(image->height);
	vuint8 width 	= vmovq_n_u8(image->width);
	vuint8 vl	= vmovq_n_u8(l);
	vuint8 vc	= vmovq_n_u8(c);

	for(i = 0; i < SE; i++){	 // column
		for(j = 0; j < SE; j++){ // row
			pl = l + j - (SE/2);
			pc = c + i - (SE/2);
			
			vuint8 vupl = vmovq_n_u8(pl);
			vuint8 vupc = vmovq_n_u8(pc);
			vsint8 vspl = vmovq_n_s8(pl);
			vsint8 vspc = vmovq_n_s8(pc);
			
			cond = vcltq_s8(vspl, zero);	// if(pl < 0) pl = l;
			pl = vgetq_lane_u8(vbslq_u8(cond, vl, vupl), 0);
				
			cond = vcltq_s8(vspc, zero);	// if(pc < 0) pc = c;
			pc = vgetq_lane_u8(vbslq_u8(cond, vc, vupc), 0);
			
			cond = vcgeq_u8(vupl, height); // if(pl >= image->height) pl = l
			pl = vgetq_lane_u8(vbslq_u8(cond, vl, vupl), 0);
			
			cond = vcgeq_u8(vupc, width); // if(pc >= image->width) pc = c
			pc = vgetq_lane_u8(vbslq_u8(cond, vc, vupc), 0);
			
			boundary[k++] = pl;
			boundary[k++] = pc;
		}
	}
}

void morph_erosion_5x5(struct image_t *in, struct image_t *out)
{
	int i,j, k;
	int boundary[5*5*2]; 			// [[x,y],...];
	vuint8 m_left, m_middle, m_right;	// m(ax/in)_x
	vuint8 m_lleft, m_rright;		
	vuint8 m_right1, m_rright1;		// (card-1)middle_(1)right
	vuint8 left1_m, lleft1_m;		// (1)left_(card-1)middle
	vuint8 result; 
	
	for(i = 0; i < in->height; i++){	// row
		for(j = 0; j < in->width; j++){ // column
 			// get boundary indexes
			get_boundaryIdx(i, j, in, boundary, 5);
			
			// min boundary of vpixel[i][j]
			k = 0;
			// border -2
			m_lleft = vminq_u8(in->img[boundary[k+0]][boundary[k+1]],
					   in->img[boundary[k+2]][boundary[k+3]]);	
			m_lleft = vminq_u8(in->img[boundary[k+4]][boundary[k+5]], m_lleft);
			m_lleft = vminq_u8(in->img[boundary[k+6]][boundary[k+7]], m_lleft);
			m_lleft = vminq_u8(in->img[boundary[k+8]][boundary[k+9]], m_lleft);
			// border -1
			m_left = vminq_u8(in->img[boundary[k+10]][boundary[k+11]],
					  in->img[boundary[k+12]][boundary[k+13]]);	
			m_left = vminq_u8(in->img[boundary[k+14]][boundary[k+15]], m_left);
			m_left = vminq_u8(in->img[boundary[k+16]][boundary[k+17]], m_left);
			m_left = vminq_u8(in->img[boundary[k+18]][boundary[k+19]], m_left);
			
			k = 20;
			// border 0
			m_middle = vminq_u8(in->img[boundary[k+0]][boundary[k+1]],
					    in->img[boundary[k+2]][boundary[k+3]]);
			m_middle = vminq_u8(in->img[boundary[k+4]][boundary[k+5]], m_middle);
			m_middle = vminq_u8(in->img[boundary[k+6]][boundary[k+7]], m_middle);
			m_middle = vminq_u8(in->img[boundary[k+8]][boundary[k+9]], m_middle);
			
			k = 30;
			// border +1
			m_right = vminq_u8(in->img[boundary[k+0]][boundary[k+1]],
					   in->img[boundary[k+2]][boundary[k+3]]);
			m_right = vminq_u8(in->img[boundary[k+4]][boundary[k+5]], m_right);
			m_right = vminq_u8(in->img[boundary[k+6]][boundary[k+7]], m_right);
			m_right = vminq_u8(in->img[boundary[k+8]][boundary[k+9]], m_right);
			// border +2
			m_rright = vminq_u8(in->img[boundary[k+10]][boundary[k+11]],
					    in->img[boundary[k+12]][boundary[k+13]]);	
			m_rright = vminq_u8(in->img[boundary[k+14]][boundary[k+15]], m_rright);
			m_rright = vminq_u8(in->img[boundary[k+16]][boundary[k+17]], m_rright);
			m_rright = vminq_u8(in->img[boundary[k+18]][boundary[k+19]], m_rright);
			
			// shift left & right
			m_rright1 = vextq_u8(m_middle, m_rright, 1);
			m_right1  = vextq_u8(m_middle, m_right,  1);

			lleft1_m = vextq_u8(m_lleft, m_middle, 15);
			left1_m  = vextq_u8(m_left, m_middle, 15);
			
			// min all vector 
			m_left  = vminq_u8(left1_m, lleft1_m);
			m_right = vminq_u8(m_right1, m_rright1);
			
			result  = vminq_u8(m_left, m_right);
			result  = vminq_u8(m_middle, result);
			
			// set out pixel value
			out->img[i][j] = result;
		}
	}
}

void morph_dilation_5x5(struct image_t *in, struct image_t *out)
{
	int i,j, k;
	int boundary[5*5*2]; 			// [[x,y],...];
	vuint8 m_left, m_middle, m_right;	// m(ax/in)_x
	vuint8 m_lleft, m_rright;		
	vuint8 m_right1, m_rright1;		// (card-1)middle_(1)right
	vuint8 left1_m, lleft1_m;		// (1)left_(card-1)middle
	vuint8 result; 
	
	for(i = 0; i < in->height; i++){	// row
		for(j = 0; j < in->width; j++){ // column
 			// get boundary indexes
			get_boundaryIdx(i, j, in, boundary, 5);
			
			// min boundary of vpixel[i][j]
			k = 0;
			// border -2
			m_lleft = vmaxq_u8(in->img[boundary[k+0]][boundary[k+1]],
					   in->img[boundary[k+2]][boundary[k+3]]);	
			m_lleft = vmaxq_u8(in->img[boundary[k+4]][boundary[k+5]], m_lleft);
			m_lleft = vmaxq_u8(in->img[boundary[k+6]][boundary[k+7]], m_lleft);
			m_lleft = vmaxq_u8(in->img[boundary[k+8]][boundary[k+9]], m_lleft);
			// border -1
			m_left = vmaxq_u8(in->img[boundary[k+10]][boundary[k+11]],
					  in->img[boundary[k+12]][boundary[k+13]]);	
			m_left = vmaxq_u8(in->img[boundary[k+14]][boundary[k+15]], m_left);
			m_left = vmaxq_u8(in->img[boundary[k+16]][boundary[k+17]], m_left);
			m_left = vmaxq_u8(in->img[boundary[k+18]][boundary[k+19]], m_left);
			
			k = 20;
			// border 0
			m_middle = vmaxq_u8(in->img[boundary[k+0]][boundary[k+1]],
					    in->img[boundary[k+2]][boundary[k+3]]);
			m_middle = vmaxq_u8(in->img[boundary[k+4]][boundary[k+5]], m_middle);
			m_middle = vmaxq_u8(in->img[boundary[k+6]][boundary[k+7]], m_middle);
			m_middle = vmaxq_u8(in->img[boundary[k+8]][boundary[k+9]], m_middle);
			
			k = 30;
			// border +1
			m_right = vmaxq_u8(in->img[boundary[k+0]][boundary[k+1]],
					   in->img[boundary[k+2]][boundary[k+3]]);
			m_right = vmaxq_u8(in->img[boundary[k+4]][boundary[k+5]], m_right);
			m_right = vmaxq_u8(in->img[boundary[k+6]][boundary[k+7]], m_right);
			m_right = vmaxq_u8(in->img[boundary[k+8]][boundary[k+9]], m_right);
			// border +2
			m_rright = vmaxq_u8(in->img[boundary[k+10]][boundary[k+11]],
					    in->img[boundary[k+12]][boundary[k+13]]);	
			m_rright = vmaxq_u8(in->img[boundary[k+14]][boundary[k+15]], m_rright);
			m_rright = vmaxq_u8(in->img[boundary[k+16]][boundary[k+17]], m_rright);
			m_rright = vmaxq_u8(in->img[boundary[k+18]][boundary[k+19]], m_rright);
			
			// shift left & right
			m_rright1 = vextq_u8(m_middle, m_rright, 1);
			m_right1  = vextq_u8(m_middle, m_right,  1);

			lleft1_m = vextq_u8(m_lleft, m_middle, 15);
			left1_m  = vextq_u8(m_left, m_middle, 15);
			
			// min all vector 
			m_left  = vmaxq_u8(left1_m, lleft1_m);
			m_right = vmaxq_u8(m_right1, m_rright1);
			
			result  = vmaxq_u8(m_left, m_right);
			result  = vmaxq_u8(m_middle, result);
			
			// set out pixel value
			out->img[i][j] = result;
		}
	}
}
