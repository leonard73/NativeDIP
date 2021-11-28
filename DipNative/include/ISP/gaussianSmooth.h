#ifndef __GAUSSIAN_SMOOTH_H__
#define __GAUSSIAN_SMOOTH_H__
#include "../dipNativeInc.h"
void generate_gaussian_kernel_2d(float gaussianKernel_2D[],uint32_t kernel_size,float sigma);
void gaussian_smooth_2d(uint8_t * src,uint8_t *dst,uint32_t pixel_w,uint32_t pixel_h,uint32_t channel,float kernel2D[],uint32_t kernel_size,uint32_t padding_mode);
#endif //__GAUSSIAN_SMOOTH_H__