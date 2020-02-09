// -------------------
//  -- macro_neon.h --
// -------------------

#ifndef __MACRO_NEON_H__
#define __MACRO_NEON_H__

// --------------
// -- set zero --
// --------------

#define zero_vuint8(x)   x = vdupq_n_u8(0)
#define zero_vsint8(x)   x = vdupq_n_s8(0)
#define zero_vuint16(x)  x = vdupq_n_u16(0)
#define zero_vsint16(x)  x = vdupq_n_s16(0)
#define zero_vuint32(x)  x = vdupq_n_u32(0)
#define zero_vsint32(x)  x = vdupq_n_s32(0)
#define zero_vfloat16(x) x = vdupq_n_f16(0)
#define zero_vfloat32(x) x = vdupq_n_f32(0)
#define zero_vfloat64(x) x = vdupq_n_f64(0)

#define set1_vuint8(x)   vdupq_n_u8(x)
#define set1_vsint8(x)   vdupq_n_s8(x)
#define set1_vuint16(x)  vdupq_n_u16(x)
#define set1_vsint16(x)  vdupq_n_s16(x)
#define set1_vuint32(x)  vdupq_n_u32(x)
#define set1_vsint32(x)  vdupq_n_s32(x)
#define set1_vfloat16(x) vdupq_n_f16(x)
#define set1_vfloat32(x) vdupq_n_f32(x)
#define set1_vfloat64(x) vdupq_n_f64(x)

// ------------------------
// -- vec_load vec_store --
// ------------------------

#define vec_load1_f32(X, i)    vld1q_f32((float32*) &X[i])
#define vec_load2_f32(X, i, j) vld1q_f32((float32*) &X[i][j])

#define vec_store1_f32(X, i, x)    vst1q_f32((float32*)&X[i], x)
#define vec_store2_f32(X, i, j, x) vst1q_f32((float32*)&X[i][j], x)

// ------------------------
// -- vec_load vec_store --
// ------------------------

#define vec_div_f32(a, b)  a
#define vec_div3_f32(x)    x
#define vec_div5_f32(x)    x
#define vec_div7_f32(x)    x

// ----------------
// -- vec128_add --
// ----------------

#define vec_add2_f32(a,b) a
#define vec_add3_f32(a,b,c) a
#define vec_add4_f32(a,b,c,d) a
#define vec_add5_f32(a,b,c,d,e) a
#define vec_add6_f32(a,b,c,d,e,f) a
#define vec_add7_f32(a,b,c,d,e,f,g) a

#endif // __MACRO_NEON_H__