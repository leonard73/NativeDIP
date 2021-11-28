#ifndef __CONVOLUTION_H__
#define __CONVOLUTION_H__
#include <stdlib.h>
#include <string.h>
#include "../dipNativeInc.h"
#define CONVOLUTION_PADDING_MODE_ZERO        (0)
#define CONVOLUTION_PADDING_MODE_REPLICATE   (1)
void convolution_1d_u8_kfloat(uint8_t * src,uint8_t *dst,uint32_t pixel_nb,float kernel1D[],uint32_t kernel_size,uint32_t padding_mode);
void convolution_2d_u8_kfloat(uint8_t * src,uint8_t *dst,uint32_t pixel_w,uint32_t pixel_h,uint32_t channel,float kernel2D[],uint32_t kernel_size,uint32_t padding_mode);
#endif //__CONVOLUTION_H__