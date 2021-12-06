#ifndef __CONVOLUTION_H__
#define __CONVOLUTION_H__
#include <stdlib.h>
#include <string.h>
#include "../dipNativeInc.h"
#define CONVOLUTION_PADDING_MODE_ZERO        (0)
#define CONVOLUTION_PADDING_MODE_REPLICATE   (1)
#define CONVOLUTION_OP_MODE_FIX_U32          (2)
#define CONVOLUTION_OP_MODE_FIX_U16          (3)
#define CONVOLUTION_OP_MODE_SINGLE           (4)
#define CONVOLUTION_OP_MODE_DOUBLE           (5)
void convolution_1d_u8_kfloat(uint8_t * src,uint8_t *dst,uint32_t pixel_nb,float kernel1D[],uint32_t kernel_size,uint32_t padding_mode);
void convolution_2d_u8_kfloat(uint8_t * src,uint8_t *dst,uint32_t pixel_w,uint32_t pixel_h,uint32_t channel,float kernel2D[],uint32_t kernel_size,uint32_t padding_mode);
void convolution_2d_u8_u32_imm(uint8_t * src,uint8_t *dst,uint32_t pixel_w,uint32_t pixel_h,uint32_t channel,uint32_t kernel2D[],uint32_t kernel_size,uint32_t padding_mode,uint8_t  immediate_shr_bits);
void convolution_2d_u8_u16_imm(uint8_t * src,uint8_t *dst,uint32_t pixel_w,uint32_t pixel_h,uint32_t channel,uint16_t kernel2D[],uint32_t kernel_size,uint32_t padding_mode,uint8_t  immediate_shr_bits);
#endif //__CONVOLUTION_H__