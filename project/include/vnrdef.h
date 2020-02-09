/* ---------------- */
/* --- vnrdef.h --- */
/* ---------------- */

/*
 * Copyright (c) 2013-2015 Lionel Lacassagne, all rights reserved https://www.lri.fr/~lacas
 * Laboratoire de Recherche en Informatique (LRI) http://www.lri.fr
 * University Paris Sud 11 http://www.u-psud.fr
 *
 * Copyright (c) 2015-2017 Lionel Lacassagne, all rights reserved http://www-soc.lip6.fr/~lacas
 * Laboratoire d'Informatique de Paris6 (LIP6) http://www.lip6.fr
 * University Pierre et Marie Curie http://www.upmc.fr
 *
 * Copyright (c) 2017-2019 Lionel Lacassagne, all rights reserved http://www-soc.lip6.fr/~lacas
 * Laboratoire d'Informatique de Paris6 (LIP6) http://www.lip6.fr
 * Sorbonne University http://www.sorbonne-universite.fr
 */

/*
 * History
 * Creation: 28/02/2013
 * Modification: 27/07/2017 add float16_t
 */

#ifndef __VNRDEF_H__
#define __VNRDEF_H__
//#pragma message ("include vdef.h for Neon")

#ifdef __cplusplus
#pragma message ("C++")
extern "C" {
#endif
    
#define ENABLE_NEON
#define myNeon
#define myArch_Neon

//#pragma message ("  define ENABLE_NEON")
//#pragma message ("  define myNeon")
//#pragma message ("  define myArch_Neon")

#include <arm_neon.h>
//#include <arm_fp16.h>

#define SIMD_ALIGNMENT 16
#define NEON_ALIGNMENT SIMD_ALIGNMENT

#define SIMD_CARD_VUINT8  16
#define SIMD_CARD_VSINT8  16

#define SIMD_CARD_VUINT16  8
#define SIMD_CARD_VSINT16  8

#define SIMD_CARD_VUINT32  4
#define SIMD_CARD_VSINT32  4

#define SIMD_CARD_VFLOAT16 8
#define SIMD_CARD_VFLOAT32 4
#define SIMD_CARD_VFLOAT64 2    
    
#define CARD_VUINT8 16
#define CARD_VSINT8 16
    
#define CARD_VUINT16 8
#define CARD_VSINT16 8
    
#define CARD_VUINT32 4
#define CARD_VSINT32 4
    
#define CARD_VFLOAT16 8
#define CARD_VFLOAT32 4
#define CARD_VFLOAT64 2

typedef int8x16_t vsint8;
typedef int16x8_t vsint16;
typedef int32x4_t vsint32;
typedef int64x2_t vsint64;

typedef uint8x16_t vuint8; //*
typedef uint16x8_t vuint16; 
typedef uint32x4_t vuint32;
typedef uint64x2_t vuint64;

typedef float16_t    float16; // only for ARM
typedef float16x8_t vfloat16;
typedef float32x4_t vfloat32;
typedef float64x2_t vfloat64;

//typedef float16x8_t vfloat16;
typedef float32x4_t vfloat;
typedef float32x4_t vfloat32;
typedef float64x2_t vfloat64;
    
typedef uint8x16_t vbool8;
typedef uint16x8_t vbool16;
typedef uint32x4_t vbool32;
// nothing for poly right now
//typedef poly8x16_t
//typedef poly16x8_t

   
#ifdef __cplusplus
}
#endif

#endif // __VNRDEF_H__
