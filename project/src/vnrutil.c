/* ----------------- */
/* --- vnrutil.c --- */
/* ----------------- */

/*
 * Copyright (c) 2004-2006 Lionel Lacassagne
 */

#include "macro_neon.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <malloc.h> // posix_memalign ?

#include <arm_neon.h>

#include "nrdef.h"
#include "vnrdef.h"
#include "vnrutil.h"

#include <time.h>

/*unsigned long long read_time(void) {
 struct timespec t;
 clock_gettime(CLOCK_MONOTONIC, &t);
 return (unsigned long long) t.tv_nsec + 1000000000ull * (unsigned long long) t.tv_sec;
 }/**/

/* ------------------------------------------------- */
void s2v1D(int sj0, int sj1, int c, int *vj0, int *vj1)
/* ------------------------------------------------- */
{
    if(sj0%c) {
        *vj0 = (sj0-c)/c;
    } else {
        *vj0 = sj0/c;
    }
    *vj1 = sj1/c;
}
/* ------------------------------------------------- */
void v2m1D(int vj0, int vj1, int c, int *mj0, int *mj1)
/* ------------------------------------------------- */
{
    *mj0 = c * vj0;
    *mj1 = c * (vj1+1)-1;
}
/* ------------------------------------------------------------------------------------- */
void s2v(int si0, int si1, int sj0, int sj1, int c, int *vi0, int *vi1, int *vj0, int *vj1)
/* ------------------------------------------------------------------------------------- */
{
    // conversion indices scalaires en indices vectoriels
    *vi0 = si0;
    *vi1 = si1;
    
    s2v1D(sj0, sj1, c, vj0, vj1);
    
    /*if(sj0%c) {
     *vj0 = (sj0-c)/c;
     } else {
     *vj0 = sj0/c;
     }
     *vj1 = sj1/c;*/
}
/* ------------------------------------------------------------------------------------- */
void v2m(int vi0, int vi1, int vj0, int vj1, int c, int *mi0, int *mi1, int *mj0, int *mj1)
/* ------------------------------------------------------------------------------------- */
{
    // conversion indices vectoriels en indices scalaires pour malloc alignés
    *mi0 = vi0;
    *mi1 = vi1;
    
    /**mj0 = c * vj0;
     *mj1 = c * (vj1+1)-1;*/
    
    v2m1D(vj0, vj1, c, mj0, mj1);
}
// ------------------
int card_vuint8(void)
// ------------------
{
    return SIMD_CARD_VUINT8;
}
// ------------------
int card_vsint8(void)
// ------------------
{
    return SIMD_CARD_VSINT8;
}
// -------------------
int card_vuint16(void)
// -------------------
{
    return SIMD_CARD_VUINT16;
}
// -------------------
int card_vsint16(void)
// -------------------
{
    return SIMD_CARD_VSINT16;
}
// -------------------
int card_vuint32(void)
// -------------------
{
    return SIMD_CARD_VUINT32;
}
// -------------------
int card_vsint32(void)
// -------------------
{
    return SIMD_CARD_VSINT32;
}
// --------------------
int card_vfloat16(void)
// --------------------
{
    return SIMD_CARD_VFLOAT16;
}
// --------------------
int card_vfloat32(void)
// --------------------
{
    return SIMD_CARD_VFLOAT32;
}
// --------------------
int card_vfloat64(void)
// --------------------
{
    return SIMD_CARD_VFLOAT64;
}
// -----------------------------
void vnrerror(char error_text[])
// -----------------------------
/* Numerical Recipes standard error handler */
{
    fprintf(stderr,"Numerical Recipes run-time error...\n");
    fprintf(stderr,"%s\n",error_text);
    fprintf(stderr,"...now exiting to system...\n");
    exit(1);
}
// --------------------------------------------
void check_allocation(int ret, void* ptr, char* msg)
// --------------------------------------------
{
    //if(ret == EINVAL) { printf("allocation failure in %s: the value of the alignment parameter is not a power of two multiple of sizeof(void *)\n", msg); exit(-1);
    //if(ret == ENOMEM) { printf("allocation failure in %s: There is insufficient memory available with the requested alignment\n", msg); exit(-1); }
    if(ret != 0) { printf("allocation failure in %s: return code neq zero\n", msg); exit(-1); }
    //if (!ptr) { vnrerror("allocation failure in %s: NULL pointer return"); exit(-1); }
}
// -----------------------------------------------------
void check_posix_memalign(int ret, void* ptr, char* msg)
// -----------------------------------------------------
{
    if(ret != 0) { printf("allocation failure in %s: return code neq zero\n", msg); exit(-1); }
}
// --------------------------------------
void check_memalign(void* ptr, char* msg)
// --------------------------------------
{
    if(ptr == NULL) { printf("allocation failure in %s: return code neq zero\n", msg); exit(-1); }
}

//#define check_allocation(ret, ptr, msg) check_posix_memalign(ptr, msg)
#define check_allocation(ret, ptr, msg) check_memalign(ptr, msg)

// -------------------------------
vuint8* vui8vector(int nl, int nh)
// -------------------------------
{
    int ret = 0;
    vuint8 *v;
    
    //int ret = posix_memalign((void*)&v, NEON_ALIGNMENT, (nh-nl+1)*sizeof(vuint8));
    v = memalign(NEON_ALIGNMENT, (nh-nl+1)*sizeof(vuint8));
    check_allocation(ret, (void*) v, "vui8vector");
    
    return v-nl;
}
// -------------------------------
vsint8* vsi8vector(int nl, int nh)
// -------------------------------
{
    int ret = 0;
    vsint8 *v;
    
    //int ret = posix_memalign((void*)&v, NEON_ALIGNMENT, (nh-nl+1)*sizeof(vsint8));
    v = memalign(NEON_ALIGNMENT, (nh-nl+1)*sizeof(vsint8));
    check_allocation(ret, (void*) v, "vsi8vector");
    
    return v-nl;
}
// ---------------------------------
vuint16* vui16vector(int nl, int nh)
// ---------------------------------
{
    int ret = 0;
    vuint16 *v;
    
    //ret = posix_memalign((void*)&v, NEON_ALIGNMENT, (nh-nl+1)*sizeof(vuint16));
    v = memalign(NEON_ALIGNMENT, (nh-nl+1)*sizeof(vuint16));
    check_allocation(ret, (void*) v, "vui16vector");
    
    return v-nl;
}
// ---------------------------------
vsint16* vsi16vector(int nl, int nh)
// ---------------------------------
{
    int ret = 0;
    vsint16 *v;
    
    //ret = posix_memalign((void*)&v, NEON_ALIGNMENT, (nh-nl+1)*sizeof(vsint16));
    v = memalign(NEON_ALIGNMENT, (nh-nl+1)*sizeof(vsint16));
    check_allocation(ret, (void*) v, "vsi16vector");
    
    return v-nl;
}
// ---------------------------------
vuint32* vui32vector(int nl, int nh)
// ---------------------------------
{
    int ret = 0;
    vuint32 *v;
    
    //ret = posix_memalign((void*)&v, NEON_ALIGNMENT, (nh-nl+1)*sizeof(vuint32));
    v = memalign(NEON_ALIGNMENT, (nh-nl+1)*sizeof(vuint32));
    check_allocation(ret, (void*) v, "vui32vector");
    return v-nl;
}
// ---------------------------------
vsint32* vsi32vector(int nl, int nh)
// ---------------------------------
{
    int ret = 0;
    vsint32 *v;
    
    //ret = posix_memalign((void*)&v, NEON_ALIGNMENT, (nh-nl+1)*sizeof(vsint32));
    v = memalign(NEON_ALIGNMENT, (nh-nl+1)*sizeof(vsint32));
    check_allocation(ret, (void*) v, "vsi32vector");
    
    return v-nl;
}
// ---------------------------------
vfloat16* vf16vector(int nl, int nh)
// ---------------------------------
{
    int ret = 0;
    vfloat16 *v;
    
    //ret = posix_memalign((void*)&v, NEON_ALIGNMENT, (nh-nl+1)*sizeof(vfloat16));
    v = memalign(NEON_ALIGNMENT, (nh-nl+1)*sizeof(vfloat16));
    check_allocation(ret, (void*) v, "vf16vector");
    
    v -= nl;
    return v;
}
// ---------------------------------
vfloat32* vf32vector(int nl, int nh)
// ---------------------------------
{
    int ret = 0;
    vfloat32 *v;
    
    //ret = posix_memalign((void*)&v, NEON_ALIGNMENT, (nh-nl+1)*sizeof(vfloat32));
    v = memalign(NEON_ALIGNMENT, (nh-nl+1)*sizeof(vfloat32));
    check_allocation(ret, (void*) v, "vf32vector");
    
    v -= nl;
    return v;
}
// ---------------------------------
vfloat64* vf64vector(int nl, int nh)
// ---------------------------------
{
    int ret = 0;
    vfloat64 *v;
    
    //ret = posix_memalign((void*)&v, NEON_ALIGNMENT, (nh-nl+1)*sizeof(vfloat64));
    v = memalign(NEON_ALIGNMENT, (nh-nl+1)*sizeof(vfloat64));
    check_allocation(ret, (void*) v, "vf64vector");
    
    v -= nl;
    return v;
}
// --------------------------------------------
void free_vui8vector(vuint8 *v, int nl, int nh)
// --------------------------------------------
{
    free(v+nl);
}
// --------------------------------------------
void free_vsi8vector(vsint8 *v, int nl, int nh)
// --------------------------------------------
{
    free(v+nl);
}
// ----------------------------------------------
void free_vui16vector(vuint16 *v, int nl, int nh)
// ----------------------------------------------
{
    free(v+nl);
}
// ----------------------------------------------
void free_vsi16vector(vsint16 *v, int nl, int nh)
// ----------------------------------------------
{
    free(v+nl);
}
// ----------------------------------------------
void free_vui32vector(vuint32 *v, int nl, int nh)
// ----------------------------------------------
{
    free(v+nl);
}
// ----------------------------------------------
void free_vsi32vector(vsint32 *v, int nl, int nh)
// ----------------------------------------------
{
    free(v+nl);
}
// ------------------------------------------
void free_vfvector(vfloat *v, int nl, int nh)
// ------------------------------------------
{
    free(v+nl);
}
// ----------------------------------------------
void free_vf16vector(vfloat16 *v, int nl, int nh)
// ----------------------------------------------
{
    free(v+nl);
}
// ----------------------------------------------
void free_vf32vector(vfloat32 *v, int nl, int nh)
// ----------------------------------------------
{
    free(v+nl);
}
// ----------------------------------------------
void free_vf64vector(vfloat64 *v, int nl, int nh)
// ----------------------------------------------
{
    free(v+nl);
}
// -------------
// --- 2D ------
// -------------
// ----------------------------------------------------
vuint8 **vui8matrix(int nrl, int nrh, int ncl, int nch)
// ----------------------------------------------------
// allocate a vuint8 matrix with subscript range m[nrl..nrh][ncl..nch]
{
    int i, nrow=nrh-nrl+1,ncol=nch-ncl+1;
    vuint8 **m;
    int ret;
    
    //ret = posix_memalign((void**)&m, NEON_ALIGNMENT, (nrow)*sizeof(vuint8*));
    m = memalign(NEON_ALIGNMENT, nrow*sizeof(vuint8*));
    check_allocation(ret, (void*) m, "vui8matrix1");
    m -= nrl;
    
    //ret = posix_memalign((void*)&m[nrl], NEON_ALIGNMENT, (nrow*ncol)*sizeof(vuint8));
    m[nrl] = memalign(NEON_ALIGNMENT, (nrow*ncol)*sizeof(vuint8));
    check_allocation(ret, (void*) m[nrl], "vui8matrix2");
    m[nrl] -= ncl;
    
    for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;
    
    return m;
}
// ----------------------------------------------------
vsint8 **vsi8matrix(int nrl, int nrh, int ncl, int nch)
// ----------------------------------------------------
{
    int i, nrow=nrh-nrl+1,ncol=nch-ncl+1;
    vsint8 **m;
    int ret;
    
    //ret = posix_memalign((void**)&m, NEON_ALIGNMENT, (nrow)*sizeof(vsint8*));
    m = memalign(NEON_ALIGNMENT, nrow*sizeof(vsint8*));
    
    check_allocation(ret, (void*) m, "vsi8matrix1");
    m -= nrl;
    
    //ret = posix_memalign((void*)&m[nrl], NEON_ALIGNMENT, (nrow*ncol)*sizeof(vsint8));
    m[nrl] = memalign(NEON_ALIGNMENT, (nrow*ncol)*sizeof(vsint8));
    check_allocation(ret, (void*) m[nrl], "vsi8matrix2");
    m[nrl] -= ncl;
    
    for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;
    
    return m;
}
// ------------------------------------------------------
vuint16 **vui16matrix(int nrl, int nrh, int ncl, int nch)
// ------------------------------------------------------
{
    int i, nrow=nrh-nrl+1,ncol=nch-ncl+1;
    vuint16 **m;
    int ret;
    
    //ret = posix_memalign((void**)&m, NEON_ALIGNMENT, (nrow)*sizeof(vuint16*));
    m = memalign(NEON_ALIGNMENT, nrow*sizeof(vuint16*));
    check_allocation(ret, (void*) m, "vui16matrix1");
    m -= nrl;
    
    //ret = posix_memalign((void*)&m[nrl], NEON_ALIGNMENT, (nrow*ncol)*sizeof(vuint16));
    m[nrl] = memalign(NEON_ALIGNMENT, (nrow*ncol)*sizeof(vuint16));
    check_allocation(ret, (void*) m[nrl], "vui16matrix2");
    m[nrl] -= ncl;
    
    for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;
    
    return m;
}
// ------------------------------------------------------
vsint16 **vsi16matrix(int nrl, int nrh, int ncl, int nch)
// ------------------------------------------------------
{
    int i, nrow=nrh-nrl+1,ncol=nch-ncl+1;
    vsint16 **m;
    int ret;
    
    //ret = posix_memalign((void**)&m, NEON_ALIGNMENT, (nrow)*sizeof(vsint16*));
    m = memalign(NEON_ALIGNMENT, nrow*sizeof(vsint16*));
    check_allocation(ret, (void*) m, "vsi16matrix1");
    m -= nrl;
    
    //ret = posix_memalign((void*)&m[nrl], NEON_ALIGNMENT, (nrow*ncol)*sizeof(vsint16));
    m[nrl] = memalign(NEON_ALIGNMENT, (nrow*ncol)*sizeof(vsint16));
    check_allocation(ret, (void*) m[nrl], "vsi16matrix2");
    m[nrl] -= ncl;
    
    for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;
    
    return m;
}
// ------------------------------------------------------
vuint32 **vui32matrix(int nrl, int nrh, int ncl, int nch)
// ------------------------------------------------------
{
    int i, nrow=nrh-nrl+1,ncol=nch-ncl+1;
    vuint32 **m;
    int ret;
    
   // ret = posix_memalign((void**)&m, NEON_ALIGNMENT, (nrow)*sizeof(vuint32*));
    m = memalign(NEON_ALIGNMENT, nrow*sizeof(vuint32*));
    check_allocation(ret, (void*) m, "vui32matrix1");
    m -= nrl;
    
    //ret = posix_memalign((void*)&m[nrl], NEON_ALIGNMENT, (nrow*ncol)*sizeof(vuint32));
    m[nrl] = memalign(NEON_ALIGNMENT, (nrow*ncol)*sizeof(vuint32));
    check_allocation(ret, (void*) m[nrl], "vui32matrix2");
    m[nrl] -= ncl;
    
    for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;
    
    return m;
}
// ------------------------------------------------------
vsint32 **vsi32matrix(int nrl, int nrh, int ncl, int nch)
// ------------------------------------------------------
{
    int i, nrow=nrh-nrl+1,ncol=nch-ncl+1;
    vsint32 **m;
    int ret;
    
    //ret = posix_memalign((void**)&m, NEON_ALIGNMENT, (nrow)*sizeof(vsint32*));
    m = memalign(NEON_ALIGNMENT, nrow*sizeof(vsint32*));
    check_allocation(ret, (void*) m, "vsi32matrix1");
    m -= nrl;
    
    //ret = posix_memalign((void*)&m[nrl], NEON_ALIGNMENT, (nrow*ncol)*sizeof(vsint32));
    m[nrl] = memalign(NEON_ALIGNMENT, (nrow*ncol)*sizeof(vsint32));
    check_allocation(ret, (void*) m[nrl], "vsi32matrix2");
    m[nrl] -= ncl;
    
    for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;
    
    return m;
}
// ------------------------------------------------------
vfloat16 **vf16matrix(int nrl, int nrh, int ncl, int nch)
// ------------------------------------------------------
{
    int i, nrow=nrh-nrl+1,ncol=nch-ncl+1;
    vfloat16 **m;
    int ret;
    
    //ret = posix_memalign((void**)&m, NEON_ALIGNMENT, (nrow)*sizeof(vfloat16*));
    m = memalign(NEON_ALIGNMENT, nrow*sizeof(vfloat16*));
    check_allocation(ret, (void*) m, "vf16matrix1");
    m -= nrl;
    
    //ret = posix_memalign((void*)&m[nrl], NEON_ALIGNMENT, (nrow*ncol)*sizeof(vfloat16));
    m[nrl] = memalign(NEON_ALIGNMENT, (nrow*ncol)*sizeof(vfloat16));
    check_allocation(ret, (void*) m[nrl], "vf16matrix2");
    m[nrl] -= ncl;
    
    for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;
    
    return m;
}
// ------------------------------------------------------
vfloat32 **vf32matrix(int nrl, int nrh, int ncl, int nch)
// ------------------------------------------------------
{
    int i, nrow=nrh-nrl+1,ncol=nch-ncl+1;
    vfloat32 **m;
    int ret;
    
    //ret = posix_memalign((void**)&m, NEON_ALIGNMENT, (nrow)*sizeof(vfloat32*));
    m = memalign(NEON_ALIGNMENT, nrow*sizeof(vfloat32*));
    check_allocation(ret, (void*) m, "vf32matrix1");
    m -= nrl;
    
    //ret = posix_memalign((void*)&m[nrl], NEON_ALIGNMENT, (nrow*ncol)*sizeof(vfloat32));
    m[nrl] = memalign(NEON_ALIGNMENT, (nrow*ncol)*sizeof(vfloat32));
    check_allocation(ret, (void*) m[nrl], "vf32matrix2");
    m[nrl] -= ncl;
    
    for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;
    
    return m;
}
// ------------------------------------------------------
vfloat64 **vf64matrix(int nrl, int nrh, int ncl, int nch)
// ------------------------------------------------------
{
    int i, nrow=nrh-nrl+1,ncol=nch-ncl+1;
    vfloat64 **m;
    int ret;
    
    //ret = posix_memalign((void**)&m, NEON_ALIGNMENT, (nrow)*sizeof(vfloat64*));
    m = memalign(NEON_ALIGNMENT, nrow*sizeof(vfloat64*));
    check_allocation(ret, (void*) m, "vf64matrix1");
    m -= nrl;
    
    //ret = posix_memalign((void*)&m[nrl], NEON_ALIGNMENT, (nrow*ncol)*sizeof(vfloat64));
    m[nrl] = memalign(NEON_ALIGNMENT, (nrow*ncol)*sizeof(vfloat64));
    check_allocation(ret, (void*) m[nrl], "vf64matrix2");
    m[nrl] -= ncl;
    
    for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;
    
    return m;
}
// -----------------------------------------------------------------
void free_vui8matrix(vuint8 **m, int nrl, int nrh, int ncl, int nch)
// -----------------------------------------------------------------
{
    free(m[nrl]+ncl);
    free(m+nrl);
}
// -----------------------------------------------------------------
void free_vsi8matrix(vsint8 **m, int nrl, int nrh, int ncl, int nch)
// -----------------------------------------------------------------
{
    free(m[nrl]+ncl);
    free(m+nrl);
}
// -------------------------------------------------------------------
void free_vui16matrix(vuint16 **m, int nrl, int nrh, int ncl, int nch)
// -------------------------------------------------------------------
{
    free(m[nrl]+ncl);
    free(m+nrl);
}
// -------------------------------------------------------------------
void free_vsi16matrix(vsint16 **m, int nrl, int nrh, int ncl, int nch)
// -------------------------------------------------------------------
{
    free(m[nrl]+ncl);
    free(m+nrl);
}
// -------------------------------------------------------------------
void free_vui32matrix(vuint32 **m, int nrl, int nrh, int ncl, int nch)
// -------------------------------------------------------------------
{
    free(m[nrl]+ncl);
    free(m+nrl);
}
// ------------------------------------------------------------------
void free_vsi32matrix(vsint32 **m, int nrl, int nrh, int ncl, int nch)
// ------------------------------------------------------------------
{
    free(m[nrl]+ncl);
    free(m+nrl);
}
// -------------------------------------------------------------------
void free_vf16matrix(vfloat16 **m, int nrl, int nrh, int ncl, int nch)
// -------------------------------------------------------------------
{
    free(m[nrl]+ncl);
    free(m+nrl);
}
// -------------------------------------------------------------------
void free_vf32matrix(vfloat32 **m, int nrl, int nrh, int ncl, int nch)
// -------------------------------------------------------------------
{
    free(m[nrl]+ncl);
    free(m+nrl);
}
// -------------------------------------------------------------------
void free_vf64matrix(vfloat64 **m, int nrl, int nrh, int ncl, int nch)
// -------------------------------------------------------------------
{
    free(m[nrl]+ncl);
    free(m+nrl);
}
// ----------------------------------------------------
void display_vuint8(vuint8 x, char *format, char *name)
// ----------------------------------------------------
{
    vuint8 T[1];
    uint8 *p = (uint8*) T;
    
    vst1q_u8((uint8*)T, x);
    if(name) printf("%s", name);
    for(int i=0; i<CARD_VUINT8; i++) printf(format, p[i]);
    if(name) putchar('\n');
}
// ----------------------------------------------------
void display_vsint8(vsint8 x, char *format, char *name)
// ----------------------------------------------------
{
    vsint8 T[1];
    sint8 *p = (sint8*) T;
    
    vst1q_s8((sint8*)T, x);
    if(name) printf("%s", name);
    for(int i=0; i<CARD_VSINT8; i++) printf(format, p[i]);
    if(name) putchar('\n');
}
// ------------------------------------------------------
void display_vuint16(vuint16 x, char *format, char *name)
// ------------------------------------------------------
{
    int i;
    vuint16 T[1];
    uint16 *p = (uint16*) T;
    
    vst1q_u16((uint16*)T, x);
    if(name) printf("%s", name);
    for(i=0; i<CARD_VUINT16; i++) printf(format, p[i]);
    if(name) putchar('\n');
}
// ------------------------------------------------------
void display_vsint16(vsint16 x, char *format, char *name)
// ------------------------------------------------------
{
    int i;
    vsint16 T[1];
    sint16 *p = (sint16*) T;
    
    vst1q_s16((sint16*)T, x);
    if(name) printf("%s", name);
    for(i=0; i<CARD_VSINT16; i++) printf(format, p[i]);
    if(name) putchar('\n');
}
// ------------------------------------------------------
void display_vuint32(vuint32 x, char *format, char *name)
// ------------------------------------------------------
{
    int i;
    vuint32 T[1];
    uint32 *p = (uint32*) T;
    
    vst1q_u32((uint32*)T, x);
    if(name) printf("%s", name);
    for(i=0; i<CARD_VUINT32; i++) printf(format, p[i]);
    if(name) putchar('\n');
}
// ------------------------------------------------------
void display_vsint32(vsint32 x, char *format, char *name)
// ------------------------------------------------------
{
    int i;
    vsint32 T[1];
    sint32 *p = (sint32*) T;
    
    vst1q_s32((sint32*)T, x);
    if(name) printf("%s", name);
    for(i=0; i<CARD_VSINT32; i++) printf(format, p[i]);
    if(name) putchar('\n');
}
// --------------------------------------------------------
void display_vfloat32(vfloat32 x, char *format, char *name)
// --------------------------------------------------------
{
    int i;
    vfloat32 T[1];
    float32 *p = (float32*) T;
    
    vst1q_f32((float32*)T, x);
    if(name) printf("%s", name);
    for(i=0; i<CARD_VFLOAT32; i++) printf(format, p[i]);
    if(name) putchar('\n');
}
// --------------------------------------------------------
void display_vfloat64(vfloat64 x, char *format, char *name)
// --------------------------------------------------------
{
    int i;
    vfloat64 T[1];
    float64 *p = (float64*) T;
    
    vst1q_f64((float64*)T, x);
    if(name) printf("%s", name);
    for(i=0; i<CARD_VFLOAT64; i++) printf(format, p[i]);
    if(name) putchar('\n');
}
// --------------------
// --- 1D functions ---
// --------------------

// --------------------------------------------------------------------------
void display_vui8vector(vuint8 *vX, int j0, int j1, char *format, char *name)
// --------------------------------------------------------------------------
{
    int j;
    vuint8 x;
    
    if(name != NULL) printf("%s", name);
    
    for(j=j0; j<=j1; j++) {
        x = vld1q_u8((uint8*)&vX[j]);
        display_vuint8(x, format, NULL);
    }
    putchar('\n');
}
// --------------------------------------------------------------------------
void display_vsi8vector(vsint8 *vX, int j0, int j1, char *format, char *name)
// --------------------------------------------------------------------------
{
    int j;
    vsint8 x;
    
    if(name != NULL) printf("%s\n", name);
    
    for(j=j0; j<=j1; j++) {
        x = vld1q_s8((sint8*)&vX[j]);
        display_vsint8(x, format, NULL);
    }
    putchar('\n');
}
// ----------------------------------------------------------------------------
void display_vui16vector(vuint16 *vX, int j0, int j1, char *format, char *name)
// ----------------------------------------------------------------------------
{
    int j;
    vuint16 x;
    
    if(name != NULL) printf("%s", name);
    
    for(j=j0; j<=j1; j++) {
        x = vld1q_u16((uint16*)&vX[j]);
        display_vuint16(x, format, NULL);
    }
    putchar('\n');
}
// ----------------------------------------------------------------------------
void display_vsi16vector(vsint16 *vX, int j0, int j1, char *format, char *name)
// ----------------------------------------------------------------------------
{
    int j;
    vsint16 x;
    
    if(name != NULL) printf("%s", name);
    
    for(j=j0; j<=j1; j++) {
        x = vld1q_s16((sint16*)&vX[j]);
        display_vsint16(x, format, NULL);
    }
    putchar('\n');
}
// ----------------------------------------------------------------------------
void display_vui32vector(vuint32 *vX, int j0, int j1, char *format, char *name)
// ----------------------------------------------------------------------------
{
    int j;
    vuint32 x;
    
    if(name != NULL) printf("%s", name);
    
    for(j=j0; j<=j1; j++) {
        x = vld1q_u32((uint32*)&vX[j]);
        display_vuint32(x, format, NULL);
    }
    putchar('\n');
}
// ----------------------------------------------------------------------------
void display_vsi32vector(vsint32 *vX, int j0, int j1, char *format, char *name)
// ----------------------------------------------------------------------------
{
    int j;
    vsint32 x;
    
    if(name != NULL) printf("%s", name);
    
    for(j=j0; j<=j1; j++) {
        x = vld1q_s32((sint32*)&vX[j]);
        display_vsint32(x, format, NULL);
    }
    putchar('\n');
}
// ----------------------------------------------------------------------------
void display_vf32vector(vfloat32 *vX, int j0, int j1, char *format, char *name)
// ----------------------------------------------------------------------------
{
    int j;
    vfloat32 x;
    
    if(name != NULL) printf("%s", name);
    
    for(j=j0; j<=j1; j++) {
        x = vld1q_f32((float32*)&vX[j]);
        display_vfloat32(x, format, NULL);
    }
    putchar('\n');
}

// -------------------
// --- 2D function ---
// -------------------

// ------------------------------------------------------------------------------------------
void display_vui8matrix(vuint8 **vX, int i0, int i1, int j0, int j1, char *format, char *name)
// ------------------------------------------------------------------------------------------
{
    int i, j;
    vuint8 x;
    
    if(name != NULL) printf("%s", name);
    
    for(i=i0; i<=i1; i++) {
        for(j=j0; j<=j1; j++) {
            x = vld1q_u8((uint8*)&vX[i][j]);
            display_vuint8(x, format, NULL);
        }
        putchar('\n');
    }
    putchar('\n');
}
// -------------------------------------------------------------------------------------------
void display_vsi8matrix(vsint8 **vX, int i0, int i1, int j0, int j1, char *format, char *name)
// -------------------------------------------------------------------------------------------
{
    int i, j;
    vsint8 x;
    
    if(name != NULL) printf("%s", name);
    
    for(i=i0; i<=i1; i++) {
        for(j=j0; j<=j1; j++) {
            x = vld1q_s8((sint8*)&vX[i][j]);
            display_vsint8(x, format, NULL);
        }
        putchar('\n');
    }
    putchar('\n');
}
// ---------------------------------------------------------------------------------------------
void display_vui16matrix(vuint16 **vX, int i0, int i1, int j0, int j1, char *format, char *name)
// ---------------------------------------------------------------------------------------------
{
    int i, j;
    vuint16 x;
    
    if(name != NULL) printf("%s", name);
    
    for(i=i0; i<=i1; i++) {
        for(j=j0; j<=j1; j++) {
            x = vld1q_u16((uint16*)&vX[i][j]);
            display_vuint16(x, format, NULL);
        }
        putchar('\n');
    }
    putchar('\n');
}
// ---------------------------------------------------------------------------------------------
void display_vsi16matrix(vsint16 **vX, int i0, int i1, int j0, int j1, char *format, char *name)
// ---------------------------------------------------------------------------------------------
{
    int i, j;
    vsint16 x;
    
    if(name != NULL) printf("%s", name);
    
    for(i=i0; i<=i1; i++) {
        for(j=j0; j<=j1; j++) {
            x = vld1q_s16((sint16*)&vX[i][j]);
            display_vsint16(x, format, NULL);
        }
        putchar('\n');
    }
    putchar('\n');
}
// ---------------------------------------------------------------------------------------------
void display_vui32matrix(vuint32 **vX, int i0, int i1, int j0, int j1, char *format, char *name)
// ---------------------------------------------------------------------------------------------
{
    int i, j;
    vuint32 x;
    
    if(name != NULL) printf("%s", name);
    
    for(i=i0; i<=i1; i++) {
        for(j=j0; j<=j1; j++) {
            x = vld1q_u32((uint32*)&vX[i][j]);
            display_vuint32(x, format, NULL);
        }
        putchar('\n');
    }
    putchar('\n');
}
// ---------------------------------------------------------------------------------------------
void display_vsi32matrix(vsint32 **vX, int i0, int i1, int j0, int j1, char *format, char *name)
// ---------------------------------------------------------------------------------------------
{
    int i, j;
    vsint32 x;
    
    if(name != NULL) printf("%s", name);
    
    for(i=i0; i<=i1; i++) {
        for(j=j0; j<=j1; j++) {
            x = vld1q_s32((sint32*)&vX[i][j]);
            display_vsint32(x, format, NULL);
        }
        putchar('\n');
    }
    putchar('\n');
}
// ---------------------------------------------------------------------------------------------
void display_vf32matrix(vfloat32 **vX, int i0, int i1, int j0, int j1, char *format, char *name)
// ---------------------------------------------------------------------------------------------
{
    int i, j;
    vfloat32 x;
    
    if(name != NULL) printf("%s", name);
    
    for(i=i0; i<=i1; i++) {
        for(j=j0; j<=j1; j++) {
            x = vld1q_f32((float32*)&vX[i][j]);
            display_vfloat32(x, format, NULL);
        }
        putchar('\n');
    }
    putchar('\n');
}
// ------------------
// --- 0D Altivec ---
// ------------------

// ------------------------
vuint8 init_vuint8(uint8 x)
// ------------------------
{
    /*vuint8 T[1];
     uint8 *p = (uint8*) T;
     
     p[0]  = p[1]  = p[2]  = p[3]  = x;
     p[4]  = p[5]  = p[6]  = p[7]  = x;
     p[8]  = p[9]  = p[10] = p[11] = x;
     p[12] = p[13] = p[14] = p[15] = x;
     
     return T[0];*/
    
    return set1_vuint8(x);
}
// ------------------------
vsint8 init_vsint8(sint8 x)
// ------------------------
{
    /*vsint8 T[1];
     sint8 *p = (sint8*) T;
     
     p[0]  = p[1]  = p[2]  = p[3]  = x;
     p[4]  = p[5]  = p[6]  = p[7]  = x;
     p[8]  = p[9]  = p[10] = p[11] = x;
     p[12] = p[13] = p[14] = p[15] = x;
     
     return T[0];*/
    
    return set1_vsint8(x);
}
// ---------------------------
vuint16 init_vuint16(uint16 x)
// ---------------------------
{
    /*vuint16 T[1];
     uint16 *p = (uint16*) T;
     
     p[0] = p[1] = p[2] = p[3] = x;
     p[4] = p[5] = p[6] = p[7] = x;
     
     return T[0];*/
    
    return set1_vuint16(x);
}
// ---------------------------
vsint16 init_vsint16(sint16 x)
// ---------------------------
{
    /*vsint16 T[1];
     sint16 *p = (sint16*) T;
     
     p[0] = p[1] = p[2] = p[3] = x;
     p[4] = p[5] = p[6] = p[7] = x;
     
     return T[0];*/
    
    return set1_vsint16(x);
}
// ---------------------------
vuint32 init_vuint32(uint32 x)
// ---------------------------
{
    /*vuint32 T[1];
     uint32 *p = (uint32*) T;
     
     p[0] = p[1] = p[2] = p[3] = x;
     
     return T[0];*/
    
    return set1_vuint32(x);
}
// ---------------------------
vsint32 init_vsint32(sint32 x)
// ---------------------------
{
    /*vsint32 T[1];
     sint32 *p = (sint32*) T;
     
     p[0] = p[1] = p[2] = p[3] = x;
     
     return T[0];*/
    
    return set1_vsint32(x);
}
// ------------------------------
vfloat16 init_vfloat16(float16 x)
// ------------------------------
{
    vfloat16 T[1];
    float16 *p = (float16*) T;
    
    for(int i=0; i<CARD_VFLOAT16; i++) {
        p[i] = x;
    }
    
    return T[0];
    
    // vdupq_n_f16 is unknown for gcc 5.3.1
    //return set1_vfloat16(x);
    //return vdupq_n_f16(0);
}
// ------------------------------
vfloat32 init_vfloat32(float32 x)
// ------------------------------
{
    /*vfloat32 T[1];
     float32 *p = (float32*) T;
     
     p[0] = p[1] = p[2] = p[3] = x;
     
     return T[0];*/
    
    return set1_vfloat32(x);
}
// ------------------------------
vfloat64 init_vfloat64(float64 x)
// ------------------------------
{
    return set1_vfloat64(x);
}
// -------------------------------------------
vuint8 init_vuint8_param(uint8 x0, uint8 step)
// -------------------------------------------
{
    int i;
    vuint8 T[1];
    uint8 *p = (uint8*) T;
    
    for(i=0; i<CARD_VUINT8; i++) {
        p[i] = x0;
        x0 += step;
    }
    return T[0];
}
// -------------------------------------------
vsint8 init_vsint8_param(sint8 x0, sint8 step)
// -------------------------------------------
{
    int i;
    vsint8 T[1];
    sint8 *p = (sint8*) T;
    
    for(i=0; i<CARD_VSINT8; i++) {
        p[i] = x0;
        x0 += step;
    }
    return T[0];
}
// -----------------------------------------------
vuint16 init_vuint16_param(uint16 x0, uint16 step)
// -----------------------------------------------
{
    int i;
    vuint16 T[1];
    uint16 *p = (uint16*) T;
    
    for(i=0; i<CARD_VUINT16; i++) {
        p[i] = x0;
        x0 += step;
    }
    return T[0];
}
// -----------------------------------------------
vsint16 init_vsint16_param(sint16 x0, sint16 step)
// -----------------------------------------------
{
    int i;
    vsint16 T[1];
    sint16 *p = (sint16*) T;
    
    for(i=0; i<CARD_VSINT16; i++) {
        p[i] = x0;
        x0 += step;
    }
    return T[0];
}
// -----------------------------------------------
vuint32 init_vuint32_param(uint32 x0, uint32 step)
// -----------------------------------------------
{
    int i;
    vuint32 T[1];
    uint32 *p = (uint32*) T;
    
    for(i=0; i<CARD_VUINT32; i++) {
        p[i] = x0;
        x0 += step;
    }
    return T[0];
}
// -----------------------------------------------
vsint32 init_vsint32_param(sint32 x0, sint32 step)
// -----------------------------------------------
{
    int i;
    vsint32 T[1];
    sint32 *p = (sint32*) T;
    
    for(i=0; i<CARD_VSINT32; i++) {
        p[i] = x0;
        x0 += step;
    }
    return T[0];
}
// ---------------------------------------------------
vfloat16 init_vfloat16_param(float16 x0, float16 step)
// ---------------------------------------------------
{
    int i;
    vfloat16 T[1];
    float16 *p = (float16*) T;
    
    for(i=0; i<CARD_VFLOAT16; i++) {
        p[i] = x0;
        x0 += step;
    }
    return T[0];
}
// ---------------------------------------------------
vfloat32 init_vfloat32_param(float32 x0, float32 step)
// ---------------------------------------------------
{
    int i;
    vfloat32 T[1];
    float32 *p = (float32*) T;
    
    for(i=0; i<CARD_VFLOAT32; i++) {
        p[i] = x0;
        x0 += step;
    }
    return T[0];
}
// ---------------------------------------------------
vfloat64 init_vfloat64_param(float64 x0, float64 step)
// ---------------------------------------------------
{
    int i;
    vfloat64 T[1];
    float64 *p = (float64*) T;
    
    for(i=0; i<CARD_VFLOAT64; i++) {
        p[i] = x0;
        x0 += step;
    }
    return T[0];
}
// -------------------
// --- 1D function ---
// -------------------

// --------------------------------------------
void zero_vsi8vector(vsint8 *v, int j0, int j1)
// --------------------------------------------
{
    int j;
    vsint8 z = vdupq_n_s8(0);
    
    for(j=j0; j<=j1; j++) {
        vst1q_s8((sint8*)&v[j], z);
    }
}
// --------------------------------------------
void zero_vui8vector(vuint8 *v, int j0, int j1)
// --------------------------------------------
{
    int j;
    vuint8 z = vdupq_n_u8(0);
    
    for(j=j0; j<=j1; j++) {
        vst1q_u8((uint8*)&v[j], z);
    }
}
// ----------------------------------------------
void zero_vsi16vector(vsint16 *v, int j0, int j1)
// ----------------------------------------------
{
    int j;
    vsint16 z = vdupq_n_s16(0);
    
    for(j=j0; j<=j1; j++) {
        vst1q_s16((sint16*)&v[j], z);
    }
}
// ----------------------------------------------
void zero_vui16vector(vuint16 *v, int j0, int j1)
// ----------------------------------------------
{
    int j;
    vuint16 z = vdupq_n_u16(0);
    
    for(j=j0; j<=j1; j++) {
        vst1q_u16((uint16*)&v[j], z);
    }
}
// ----------------------------------------------
void zero_vsi32vector(vsint32 *v, int j0, int j1)
// ----------------------------------------------
{
    int j;
    vsint32 z = vdupq_n_s32(0);
    
    for(j=j0; j<=j1; j++) {
        vst1q_s32((sint32*)&v[j], z);
    }
}
// ----------------------------------------------
void zero_vui32vector(vuint32 *v, int j0, int j1)
// ----------------------------------------------
{
    int j;
    vuint32 z = vdupq_n_u32(0);
    
    for(j=j0; j<=j1; j++) {
        vst1q_u32((sint32*)&v[j], z);
    }
}
// ----------------------------------------------
void zero_vf16vector(vfloat16 *v, int j0, int j1)
// ----------------------------------------------
{
    int j;
    vfloat16 z = init_vfloat16(0);
    
    for(j=j0; j<=j1; j++) {
        vst1q_f16((float16*)&v[j], z);
    }
}
// ----------------------------------------------
void zero_vf32vector(vfloat32 *v, int j0, int j1)
// ----------------------------------------------
{
    int j;
    vfloat32 z = vdupq_n_f32(0);
    
    for(j=j0; j<=j1; j++) {
        vst1q_f32((float32*)&v[j], z);
    }
}
// ----------------------------------------------
void zero_vf64vector(vfloat64 *v, int j0, int j1)
// ----------------------------------------------
{
    int j;
    vfloat64 z = vdupq_n_f64(0);
    
    for(j=j0; j<=j1; j++) {
        vst1q_f64((float64*)&v[j], z);
    }
}
// ------------------------------------------------------
void dup_vui8vector(vuint8 *X, int j0, int j1, vuint8 *Y)
// ------------------------------------------------------
{
    int j;
    vuint8 x;
    
    for(j=j0; j<=j1; j++) {
        x = vld1q_u8((uint8*)&X[j]);
        vst1q_u8((uint8*)&Y[j], x);
    }
}
// ------------------------------------------------------
void dup_vsi8vector(vsint8 *X, int j0, int j1, vsint8 *Y)
// ------------------------------------------------------
{
    int j;
    vsint8 x;
    
    for(j=j0; j<=j1; j++) {
        x = vld1q_s8((sint8*)&X[j]);
        vst1q_s8((sint8*)&Y[j], x);
    }
}
// ---------------------------------------------------------
void dup_vui16vector(vuint16 *X, int j0, int j1, vuint16 *Y)
// ---------------------------------------------------------
{
    int j;
    vuint16 x;
    
    for(j=j0; j<=j1; j++) {
        x = vld1q_u16((uint16*)&X[j]);
        vst1q_u16((uint16*)&Y[j], x);
    }
}
// ---------------------------------------------------------
void dup_vsi16vector(vsint16 *X, int j0, int j1, vsint16 *Y)
// ---------------------------------------------------------
{
    int j;
    vsint16 x;
    
    for(j=j0; j<=j1; j++) {
        x = vld1q_s16((sint16*)&X[j]);
        vst1q_s16((sint16*)&Y[j], x);
    }
}
// ---------------------------------------------------------
void dup_vui32vector(vuint32 *X, int j0, int j1, vuint32 *Y)
// ---------------------------------------------------------
{
    int j;
    vuint32 x;
    
    for(j=j0; j<=j1; j++) {
        x = vld1q_u32((uint32*)&X[j]);
        vst1q_u32((uint32*)&Y[j], x);
    }
}
// ---------------------------------------------------------
void dup_vsi32vector(vsint32 *X, int j0, int j1, vsint32 *Y)
// ---------------------------------------------------------
{
    int j;
    vsint32 x;
    
    for(j=j0; j<=j1; j++) {
        x = vld1q_s32((sint32*)&X[j]);
        vst1q_s32((sint32*)&Y[j], x);
    }
}
// ----------------------------------------------------------
void dup_vf16vector(vfloat16 *X, int j0, int j1, vfloat16 *Y)
// ----------------------------------------------------------
{
    int j;
    vfloat16 x;
    
    for(j=j0; j<=j1; j++) {
        x = vld1q_f16((float16*)&X[j]);
        vst1q_f16((float16*)&Y[j], x);
    }
}
// ----------------------------------------------------------
void dup_vf32vector(vfloat32 *X, int j0, int j1, vfloat32 *Y)
// ----------------------------------------------------------
{
    int j;
    vfloat32 x;
    
    for(j=j0; j<=j1; j++) {
        x = vld1q_f32((float32*)&X[j]);
        vst1q_f32((float32*)&Y[j], x);
    }
}
// ----------------------------------------------------------
void dup_vf64vector(vfloat64 *X, int j0, int j1, vfloat64 *Y)
// ----------------------------------------------------------
{
    int j;
    vfloat64 x;
    
    for(j=j0; j<=j1; j++) {
        x = vld1q_f64((float64*)&X[j]);
        vst1q_f64((float64*)&Y[j], x);
    }
}
// -------------------------------------------------------------------------
void init_vsi8vector_param(vsint8 *v, int j0, int j1, sint8 x0, sint8 xstep)
// -------------------------------------------------------------------------
{
    int j;
    vsint8 vx = init_vsint8_param(x0, xstep);
    vsint8 vxstep = init_vsint8(CARD_VSINT8 * xstep);
    
    for(j=j0; j<=j1; j++) {
        vst1q_s8((sint8*)&v[j], vx);
        vx = vaddq_s8(vx, vxstep);
    }
}
// -------------------------------------------------------------------------
void init_vui8vector_param(vuint8 *v, int j0, int j1, uint8 x0, uint8 xstep)
// -------------------------------------------------------------------------
{
    int j;
    vuint8 vx = init_vuint8_param(x0, xstep);
    vuint8 vxstep = init_vuint8(CARD_VUINT8 * xstep);
    
    for(j=j0; j<=j1; j++) {
        vst1q_u8((uint8*)&v[j], vx);
        vx = vaddq_u8(vx, vxstep);
    }
}
// -----------------------------------------------------------------------------
void init_vsi16vector_param(vsint16 *v, int j0, int j1, sint16 x0, sint16 xstep)
// -----------------------------------------------------------------------------
{
    int j;
    vsint16 vx = init_vsint16_param(x0, xstep);
    vsint16 vxstep = init_vsint16(CARD_VSINT16 * xstep);
    
    for(j=j0; j<=j1; j++) {
        vst1q_s16((sint16*)&v[j], vx);
        vx = vaddq_s16(vx, vxstep);
    }
}
// -----------------------------------------------------------------------------
void init_vui16vector_param(vuint16 *v, int j0, int j1, uint16 x0, uint16 xstep)
// -----------------------------------------------------------------------------
{
    int j;
    vuint16 vx = init_vuint16_param(x0, xstep);
    vuint16 vxstep = init_vuint16(CARD_VUINT16 * xstep);
    
    for(j=j0; j<=j1; j++) {
        vst1q_u16((uint16*)&v[j], vx);
        vx = vaddq_u16(vx, vxstep);
    }
}
// -----------------------------------------------------------------------------
void init_vsi32vector_param(vsint32 *v, int j0, int j1, sint32 x0, sint32 xstep)
// -----------------------------------------------------------------------------
{
    int j;
    vsint32 vx = init_vsint32_param(x0, xstep);
    vsint32 vxstep = init_vsint32(CARD_VSINT32 * xstep);
    
    for(j=j0; j<=j1; j++) {
        vst1q_s32((sint32*)&v[j], vx);
        vx = vaddq_s32(vx, vxstep);
    }
}
// -----------------------------------------------------------------------------
void init_vui32vector_param(vuint32 *v, int j0, int j1, uint32 x0, uint32 xstep)
// -----------------------------------------------------------------------------
{
    int j;
    vuint32 vx = init_vuint32_param(x0, xstep);
    vuint32 vxstep = init_vuint32(CARD_VUINT32 * xstep);
    
    for(j=j0; j<=j1; j++) {
        vst1q_u32((uint32*)&v[j], vx);
        vx = vaddq_u32(vx, vxstep);
    }
}
// -------------------------------------------------------------------------------
void init_vf16vector_param(vfloat16 *v, int j0, int j1, float16 x0, float16 xstep)
// -------------------------------------------------------------------------------
{
    int j;
    vfloat16 vx = init_vfloat16_param(x0, xstep);
    vfloat16 vxstep = init_vfloat16(CARD_VFLOAT16 * xstep);
  
    puts("[init_vf16vector_param]: vaddq_f16 INCONNU");
    exit(-1);
    
    for(j=j0; j<=j1; j++) {
        vst1q_f16((float16*)&v[j], vx);
//#pragma message("vaddq_f16 INCONNU")
        //vx = vaddq_f16(vx, vxstep);
    }
}
// -------------------------------------------------------------------------------
void init_vf32vector_param(vfloat32 *v, int j0, int j1, float32 x0, float32 xstep)
// -------------------------------------------------------------------------------
{
    int j;
    vfloat32 vx = init_vfloat32_param(x0, xstep);
    vfloat32 vxstep = init_vfloat32(CARD_VFLOAT32 * xstep);
    
    for(j=j0; j<=j1; j++) {
        vst1q_f32((float32*)&v[j], vx);
        vx = vaddq_f32(vx, vxstep);
    }
}
// -------------------------------------------------------------------------------
void init_vf64vector_param(vfloat64 *v, int j0, int j1, float64 x0, float64 xstep)
// -------------------------------------------------------------------------------
{
    int j;
    vfloat64 vx = init_vfloat64_param(x0, xstep);
    vfloat64 vxstep = init_vfloat64(CARD_VFLOAT64 * xstep);
    
    for(j=j0; j<=j1; j++) {
        vst1q_f64((float64*)&v[j], vx);
        vx = vaddq_f64(vx, vxstep);
    }
}

// -------------------
// --- 2D function ---
// -------------------

// ------------------------------------------------------------------------
void dup_vui8matrix(vuint8 **X, int i0, int i1, int j0, int j1, vuint8 **Y)
// ------------------------------------------------------------------------
{
    int i, j;
    
    vuint8 x;
    
    for(i=i0; i<=i1; i++) {
        for(j=j0; j<=j1; j++) {
            x = vld1q_u8((uint8*)&X[i][j]);
            vst1q_u8((uint8*)&Y[i][j], x);
        }
    }
}
// ------------------------------------------------------------------------
void dup_vsi8matrix(vsint8 **X, int i0, int i1, int j0, int j1, vsint8 **Y)
// ------------------------------------------------------------------------
{
    int i, j;
    
    vsint8 x;
    
    for(i=i0; i<=i1; i++) {
        for(j=j0; j<=j1; j++) {
            x = vld1q_s8((sint8*)&X[i][j]);
            vst1q_s8((sint8*)&Y[i][j], x);
        }
    }
}
// ---------------------------------------------------------------------------
void dup_vui16matrix(vuint16 **X, int i0, int i1, int j0, int j1, vuint16 **Y)
// ---------------------------------------------------------------------------
{
    int i, j;
    
    vuint16 x;
    
    for(i=i0; i<=i1; i++) {
        for(j=j0; j<=j1; j++) {
            x = vld1q_u16((uint16*)&X[i][j]);
            vst1q_u16((uint16*)&Y[i][j], x);
        }
    }
}
// ---------------------------------------------------------------------------
void dup_vsi16matrix(vsint16 **X, int i0, int i1, int j0, int j1, vsint16 **Y)
// ---------------------------------------------------------------------------
{
    int i, j;
    
    vsint16 x;
    
    for(i=i0; i<=i1; i++) {
        for(j=j0; j<=j1; j++) {
            x = vld1q_s16((sint16*)&X[i][j]);
            vst1q_s16((sint16*)&Y[i][j], x);
        }
    }
}
// ---------------------------------------------------------------------------
void dup_vui32matrix(vuint32 **X, int i0, int i1, int j0, int j1, vuint32 **Y)
// ---------------------------------------------------------------------------
{
    int i, j;
    
    vuint32 x;
    
    for(i=i0; i<=i1; i++) {
        for(j=j0; j<=j1; j++) {
            x = vld1q_u32((uint32*)&X[i][j]);
            vst1q_u32((uint32*)&Y[i][j], x);
        }
    }
}
// ---------------------------------------------------------------------------
void dup_vsi32matrix(vsint32 **X, int i0, int i1, int j0, int j1, vsint32 **Y)
// ---------------------------------------------------------------------------
{
    int i, j;
    
    vsint32 x;
    
    for(i=i0; i<=i1; i++) {
        for(j=j0; j<=j1; j++) {
            x = vld1q_s32((sint32*)&X[i][j]);
            vst1q_s32((sint32*)&Y[i][j], x);
        }
    }
}
// ----------------------------------------------------------------------------
void dup_vf16matrix(vfloat16 **X, int i0, int i1, int j0, int j1, vfloat16 **Y)
// ----------------------------------------------------------------------------
{
    int i, j;
    
    vfloat16 x;
    
    for(i=i0; i<=i1; i++) {
        for(j=j0; j<=j1; j++) {
            x = vld1q_f16((float16*)&X[i][j]);
            vst1q_f16((float16*)&Y[i][j], x);
        }
    }
}
// ----------------------------------------------------------------------------
void dup_vf32matrix(vfloat32 **X, int i0, int i1, int j0, int j1, vfloat32 **Y)
// ----------------------------------------------------------------------------
{
    int i, j;
    
    vfloat32 x;
    
    for(i=i0; i<=i1; i++) {
        for(j=j0; j<=j1; j++) {
            x = vld1q_f32((float32*)&X[i][j]);
            vst1q_f32((float32*)&Y[i][j], x);
        }
    }
}
// ----------------------------------------------------------------------------
void dup_vf64matrix(vfloat64 **X, int i0, int i1, int j0, int j1, vfloat64 **Y)
// ----------------------------------------------------------------------------
{
    int i, j;
    
    vfloat64 x;
    
    for(i=i0; i<=i1; i++) {
        for(j=j0; j<=j1; j++) {
            x = vld1q_f64((float64*)&X[i][j]);
            vst1q_f64((float64*)&Y[i][j], x);
        }
    }
}
// -------------------------------------------------------------------------------------------------------
void init_vui8matrix_param(vuint8 **m, int i0, int i1, int j0, int j1, uint8 x0, uint8 xstep, uint8 ystep)
// -------------------------------------------------------------------------------------------------------
{
    int i, j;
    vuint8 vxstep = init_vuint8(CARD_VUINT8 * xstep);
    vuint8 vystep = init_vuint8(ystep);
    vuint8 vx, vx0 = init_vuint8_param(x0, xstep);
    
    for(i=i0; i<=i1; i++) {
        vx = vx0;
        for(j=j0; j<=j1; j++) {
            vst1q_u8((uint8*)&m[i][j], vx);
            vx = vaddq_u8(vx, vxstep);
        }
        vx0 = vaddq_u8(vx0, vystep);
    }
}
// -------------------------------------------------------------------------------------------------------
void init_vsi8matrix_param(vsint8 **m, int i0, int i1, int j0, int j1, sint8 x0, sint8 xstep, sint8 ystep)
// -------------------------------------------------------------------------------------------------------
{
    int i, j;
    vsint8 vxstep = init_vsint8(CARD_VSINT8 * xstep);
    vsint8 vystep = init_vsint8(ystep);
    vsint8 vx, vx0 = init_vsint8_param(x0, xstep);
    
    for(i=i0; i<=i1; i++) {
        vx = vx0;
        for(j=j0; j<=j1; j++) {
            vst1q_s8((sint8*)&m[i][j], vx);
            vx = vaddq_s8(vx, vxstep);
        }
        vx0 = vaddq_s8(vx0, vystep);
    }
}
// ------------------------------------------------------------------------------------------------------------
void init_vui16matrix_param(vuint16 **m, int i0, int i1, int j0, int j1, uint16 x0, uint16 xstep, uint16 ystep)
// ------------------------------------------------------------------------------------------------------------
{
    int i, j;
    vuint16 vx, vx0 = init_vuint16_param(x0, xstep);
    vuint16 vxstep = init_vuint16(CARD_VUINT16 * xstep);
    vuint16 vystep = init_vuint16(ystep);
    
    for(i=i0; i<=i1; i++) {
        vx = vx0;
        for(j=j0; j<=j1; j++) {
            vst1q_u16((uint16*)&m[i][j], vx);
            vx = vaddq_u16(vx, vxstep);
        }
        vx0 = vaddq_u16(vx0, vystep);
    }
}
// ------------------------------------------------------------------------------------------------------------
void init_vsi16matrix_param(vsint16 **m, int i0, int i1, int j0, int j1, sint16 x0, sint16 xstep, sint16 ystep)
// ------------------------------------------------------------------------------------------------------------
{
    int i, j;
    vsint16 vx, vx0 = init_vsint16_param(x0, xstep);
    vsint16 vxstep = init_vsint16(CARD_VSINT16 * xstep);
    vsint16 vystep = init_vsint16(ystep);
    
    for(i=i0; i<=i1; i++) {
        vx = vx0;
        for(j=j0; j<=j1; j++) {
            vst1q_s16((sint16*)&m[i][j], vx);
            vx = vaddq_s16(vx, vxstep);
        }
        vx0 = vaddq_s16(vx0, vystep);
    }
}
// ---------------------------------------------------------------------------------------------------------------
void init_vui32matrix_param(vuint32 **m, int i0, int i1, int j0, int j1, uint32  x0, uint32  xstep, uint32  ystep)
// ---------------------------------------------------------------------------------------------------------------
{
    int i, j;
    vuint32 vx, vx0 = init_vuint32_param(x0, xstep);
    vuint32 vxstep = init_vuint32(CARD_VUINT32 * xstep);
    vuint32 vystep = init_vuint32(ystep);
    
    for(i=i0; i<=i1; i++) {
        vx = vx0;
        for(j=j0; j<=j1; j++) {
            vst1q_u32((uint32*)&m[i][j], vx);
            vx = vaddq_u32(vx, vxstep);
        }
        vx0 = vaddq_u32(vx0, vystep);
    }
}
// ---------------------------------------------------------------------------------------------------------------
void init_vsi32matrix_param(vsint32 **m, int i0, int i1, int j0, int j1, sint32  x0, sint32  xstep, sint32  ystep)
// ---------------------------------------------------------------------------------------------------------------
{
    int i, j;
    vsint32 vx, vx0 = init_vsint32_param(x0, xstep);
    vsint32 vxstep  = init_vsint32(CARD_VSINT32 * xstep);
    vsint32 vystep  = init_vsint32(ystep);
    
    for(i=i0; i<=i1; i++) {
        vx = vx0;
        for(j=j0; j<=j1; j++) {
            vst1q_s32((sint32*)&m[i][j], vx);
            vx = vaddq_s32(vx, vxstep);
        }
        vx0 = vaddq_s32(vx0, vystep);
    }
}
// ---------------------------------------------------------------------------------------------------------------
void init_vf16matrix_param(vfloat16 **m, int i0, int i1, int j0, int j1, float16 x0, float16 xstep, float16 ystep)
// ---------------------------------------------------------------------------------------------------------------
{
    int i, j;
    vfloat16 vx, vx0 = init_vfloat16_param(x0, xstep);
    vfloat16 vxstep = init_vfloat16(CARD_VFLOAT16 * xstep);
    vfloat16 vystep = init_vfloat16(ystep);
    
    puts("[init_vf16matrix_param]: vaddq_f16 INCONNU");
    exit(-1);
    
    for(i=i0; i<=i1; i++) {
        vx = vx0;
        for(j=j0; j<=j1; j++) {
            vst1q_f16((float16*)&m[i][j], vx);
//#pragma message("[init_vf16matrix_param]: vaddq_f16 INCONNU")
            //vx = vaddq_f16(vx, vxstep);
        }
        //vx0 = vaddq_f16(vx0, vystep);
    }
}
// ---------------------------------------------------------------------------------------------------------------
void init_vf32matrix_param(vfloat32 **m, int i0, int i1, int j0, int j1, float32 x0, float32 xstep, float32 ystep)
// ---------------------------------------------------------------------------------------------------------------
{
    int i, j;
    vfloat32 vx, vx0 = init_vfloat32_param(x0, xstep);
    vfloat32 vxstep = init_vfloat32(CARD_VFLOAT32 * xstep);
    vfloat32 vystep = init_vfloat32(ystep);
    
    for(i=i0; i<=i1; i++) {
        vx = vx0;
        for(j=j0; j<=j1; j++) {
            vst1q_f32((float32*)&m[i][j], vx);
            vx = vaddq_f32(vx, vxstep);
        }
        vx0 = vaddq_f32(vx0, vystep);
    }
}
// ---------------------------------------------------------------------------------------------------------------
void init_vf64matrix_param(vfloat64 **m, int i0, int i1, int j0, int j1, float64 x0, float64 xstep, float64 ystep)
// ---------------------------------------------------------------------------------------------------------------
{
    int i, j;
    vfloat64 vx, vx0 = init_vfloat64_param(x0, xstep);
    vfloat64 vxstep = init_vfloat64(CARD_VFLOAT64 * xstep);
    vfloat64 vystep = init_vfloat64(ystep);
    
    for(i=i0; i<=i1; i++) {
        vx = vx0;
        for(j=j0; j<=j1; j++) {
            vst1q_f64((float64*)&m[i][j], vx);
            vx = vaddq_f64(vx, vxstep);
        }
        vx0 = vaddq_f64(vx0, vystep);
    }
}
// -------------------------------------------------------------
void zero_vsi8matrix(vsint8 **m, int i0, int i1, int j0, int j1)
// -------------------------------------------------------------
{
    int i, j;
    vsint8 z = init_vsint8(0);
    
    for(i=i0; i<=i1; i++) {
        for(j=j0; j<=j1; j++) {
            vst1q_s8((sint8*)&m[i][j], z);
        }
    }
}
// -------------------------------------------------------------
void zero_vui8matrix(vuint8 **m, int i0, int i1, int j0, int j1)
// -------------------------------------------------------------
{
    int i, j;
    vuint8 z = init_vuint8(0);
    
    for(i=i0; i<=i1; i++) {
        for(j=j0; j<=j1; j++) {
            vst1q_u8((uint8*)&m[i][j], z);
        }
    }
}
// ---------------------------------------------------------------
void zero_vsi16matrix(vsint16 **m, int i0, int i1, int j0, int j1)
// ---------------------------------------------------------------
{
    int i, j;
    vsint16 z = init_vsint16(0);
    
    for(i=i0; i<=i1; i++) {
        for(j=j0; j<=j1; j++) {
            vst1q_s16((sint16*)&m[i][j], z);
        }
    }
}
// ---------------------------------------------------------------
void zero_vui16matrix(vuint16 **m, int i0, int i1, int j0, int j1)
// ---------------------------------------------------------------
{
    int i, j;
    vuint16 z = init_vuint16(0);
    
    for(i=i0; i<=i1; i++) {
        for(j=j0; j<=j1; j++) {
            vst1q_u16((uint16*)&m[i][j], z);
        }
    }
}
// ---------------------------------------------------------------
void zero_vsi32matrix(vsint32 **m, int i0, int i1, int j0, int j1)
// ---------------------------------------------------------------
{
    int i, j;
    vsint32 z = init_vsint32(0);
    
    for(i=i0; i<=i1; i++) {
        for(j=j0; j<=j1; j++) {
            vst1q_s32((sint32*)&m[i][j], z);
        }
    }
}
// ---------------------------------------------------------------
void zero_vui32matrix(vuint32 **m, int i0, int i1, int j0, int j1)
// ---------------------------------------------------------------
{
    int i, j;
    vuint32 z = init_vuint32(0);
    
    for(i=i0; i<=i1; i++) {
        for(j=j0; j<=j1; j++) {
            vst1q_u32((uint32*)&m[i][j], z);
        }
    }
}
// ---------------------------------------------------------------
void zero_vf16matrix(vfloat16 **m, int i0, int i1, int j0, int j1)
// ---------------------------------------------------------------
{
    int i, j;
    vfloat16 z = init_vfloat16(0);
    
    for(i=i0; i<=i1; i++) {
        for(j=j0; j<=j1; j++) {
            vst1q_f16((float16*)&m[i][j], z);
        }
    }
}
// ---------------------------------------------------------------
void zero_vf32matrix(vfloat32 **m, int i0, int i1, int j0, int j1)
// ---------------------------------------------------------------
{
    int i, j;
    vfloat32 z = init_vfloat32(0);
    
    for(i=i0; i<=i1; i++) {
        for(j=j0; j<=j1; j++) {
            vst1q_f32((float32*)&m[i][j], z);
        }
    }
}
// ---------------------------------------------------------------
void zero_vf64matrix(vfloat64 **m, int i0, int i1, int j0, int j1)
// ---------------------------------------------------------------
{
    int i, j;
    vfloat64 z = init_vfloat64(0);
    
    for(i=i0; i<=i1; i++) {
        for(j=j0; j<=j1; j++) {
            vst1q_f64((float64*)&m[i][j], z);
        }
    }
}
// --------------------------------------
// --- convertion vector => scalar ------
// --------------------------------------
// ---------------------------------------------------------------------------------------
void convert_vui8matrix_ui8matrix(vuint8 **vX, int i0, int i1, int j0, int j1, uint8 **sX)
// ---------------------------------------------------------------------------------------
{
    int i, j;
    vuint8 *vXi;
    uint8 *sXi, *p;
    
    for(i=i0; i<=i1; i++) { 
        vXi = vX[i];
        sXi = sX[i];
        p = (uint8*) &vX[i][j0/CARD_VUINT8]; p -= j0;  // Beware of pointer arithmetic
        for(j=j0; j<=j1; j++) {
            sXi[j] = p[j];
        }
    }
}
// ---------------------------------------------------------------------------------------
void convert_ui8matrix_vui8matrix(uint8 **sX, int i0, int i1, int j0, int j1, vuint8 **vX)
// ---------------------------------------------------------------------------------------
{
    int i, j;
    vuint8 *vXi;
    uint8 *sXi, *p;

    // adress computation indexes are in 'scalar mode': a div is required
    for(i=i0; i<=i1; i++) {
        vXi = vX[i];
        sXi = sX[i];
        //p = (uint8*) &vXi[j0];
        p = (uint8*) &vX[i][j0/CARD_VUINT8]; p -= j0;  // Beware of pointer arithmetic
        for(j=j0; j<=j1; j++) {
            p[j] = sXi[j];
        }
    }
}
// -------------------------------------------------------------------------------------------
void convert_vui16matrix_ui16matrix(vuint16 **vX, int i0, int i1, int j0, int j1, uint16 **sX)
// -------------------------------------------------------------------------------------------
{
    int i, j;
    vuint16 *vXi;
    uint16 *sXi, *p;
    
    for(i=i0; i<=i1; i++) {
        vXi = vX[i];
        sXi = sX[i];
        p = (uint16*) &vX[i][j0/CARD_VUINT16]; p -= j0;  // Beware of pointer arithmetic
        for(j=j0; j<=j1; j++) {
            sXi[j] = p[j];
        }
    }
}
// -------------------------------------------------------------------------------------------
void convert_ui16matrix_vui16matrix(uint16 **sX, int i0, int i1, int j0, int j1, vuint16 **vX)
// -------------------------------------------------------------------------------------------
{
    int i, j;
    vuint16 *vXi;
    uint16 *sXi, *p;
    
    // adress computation indexes are in 'scalar mode': a div is required
    for(i=i0; i<=i1; i++) {
        vXi = vX[i];
        sXi = sX[i];
        //p = (uint16*) &vXi[j0];
        p = (uint16*) &vX[i][j0/CARD_VSINT16]; p -= j0;  // Beware of pointer arithmetic
        for(j=j0; j<=j1; j++) {
            p[j] = sXi[j];
        }
    }
}
// -------------------------------------------------------------------------------------------
void convert_vui32matrix_ui32matrix(vuint32 **vX, int i0, int i1, int j0, int j1, uint32 **sX)
// -------------------------------------------------------------------------------------------
{
    int i, j;
    vuint32 *vXi;
    uint32 *sXi, *p;
    //uint32 t0, t1, t2, t3, t4, t5, t6, t7;
    
    for(i=i0; i<=i1; i++) {
        vXi = vX[i];
        sXi = sX[i];
        //p = (uint32*) &vXi[j0];
        p = (uint32*) &vX[i][j0/CARD_VUINT32]; p -= j0;  // Beware of pointer arithmetic
        for(j=j0; j<=j1; j++) {
            sXi[j] = p[j];
        }
    }
}
// -------------------------------------------------------------------------------------------
void convert_ui32matrix_vui32matrix(uint32 **sX, int i0, int i1, int j0, int j1, vuint32 **vX)
// -------------------------------------------------------------------------------------------
{
    int i, j;
    vuint32 *vXi;
    uint32 *sXi, *p;
    
    for(i=i0; i<=i1; i++) {
        vXi = vX[i];
        sXi = sX[i];
        //p = (uint32*) &vXi[j0];
        p = (uint32*) &vX[i][j0/CARD_VSINT32]; p -= j0;  // Beware of pointer arithmetic
        for(j=j0; j<=j1; j++) {
            p[j] = sXi[j];
        }
    }
}
// -------------------------------------------------------------------------------------------
void convert_f32matrix_vf32matrix(float32 **sX, int i0, int i1, int j0, int j1, vfloat32 **vX)
// -------------------------------------------------------------------------------------------
{
    int i, j;
    vfloat32 *vXi;
    float32 *sXi, *p;
    
    for(i=i0; i<=i1; i++) {
        vXi = vX[i];
        sXi = sX[i];
        //p = (float32*) &vXi[j0];
        p = (float32*) &vX[i][j0/CARD_VFLOAT32]; p -= j0;  // Beware of pointer arithmetic
        for(j=j0; j<=j1; j++) {
            p[j] = sXi[j];
        }
    }
}
// -------------------------------------------------------------------------------------------
void convert_vf32matrix_f32matrix(vfloat32 **vX, int i0, int i1, int j0, int j1, float32 **sX)
// -------------------------------------------------------------------------------------------
{
    int i, j;
    vfloat32 *vXi;
    float32 f, *sXi, *p;
    
    for(i=i0; i<=i1; i++) {
        vXi = vX[i];
        sXi = sX[i];
        //p = (float32*) &vXi[j0];
        p = (float32*) &vX[i][j0/CARD_VFLOAT32]; p -= j0;  // Beware of pointer arithmetic
        for(j=j0; j<=j1; j++) {
            f = p[j];
            sXi[j] = f;
        }
    }
}
// -----------------------------------------------------------------------------------------------
void convert_vui8matrix_vf32matrix(vuint8 **X, int si0, int si1, int sj0, int sj1, vfloat32 **vY)
// -----------------------------------------------------------------------------------------------
{
    puts("convert_vui8matrix_vf32matrix: not implemented");
    exit(-1);
}
