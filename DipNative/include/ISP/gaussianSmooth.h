#ifndef __GAUSSIAN_SMOOTH_H__
#define __GAUSSIAN_SMOOTH_H__
#include "../dipNativeInc.h"
#define GAUSSIAN_SMOOTH_OP_FIX_U32 (0)
#define GAUSSIAN_SMOOTH_OP_FIX_U16 (1)
#define GAUSSIAN_SMOOTH_OP_SINGLE  (2)
#define GAUSSIAN_SMOOTH_OP_DOUBLE  (4)
void generate_gaussian_kernel_2d_fixU16(uint16_t *gaussianKernel_2D,uint8_t imm_shr_bits,uint32_t kernel_size,float sigma);
void generate_gaussian_kernel_2d_fixU32(uint32_t *gaussianKernel_2D,uint8_t imm_shr_bits,uint32_t kernel_size,float sigma);
void generate_gaussian_kernel_2d(float gaussianKernel_2D[],uint32_t kernel_size,float sigma);
void gaussian_smooth_2d(uint8_t * src,uint8_t *dst,uint32_t pixel_w,uint32_t pixel_h,uint32_t channel,void * kernel2D,uint32_t kernel_size,uint32_t padding_mode,uint8_t imm_shr_bits);
#endif //__GAUSSIAN_SMOOTH_H__