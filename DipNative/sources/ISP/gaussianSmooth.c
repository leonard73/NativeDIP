#include "../../include/dipNativeInc.h"
#include <math.h>
#define _GET_GAUSSIAN_K2D_(x,y,sigma2) ((1.0f/(2*M_PI*sigma2)) * powf(M_E,(x*x+y*y)/((-2.0f)*sigma2)) )
void generate_gaussian_kernel_2d(float *gaussianKernel_2D,uint32_t kernel_size,float sigma)
{
   int k_h =  kernel_size / 2;
   int id=0;
   for(int h  =  -k_h;  h <=  k_h ;  ++  h)
   {
       for(int w = -k_h; w <= k_h ; ++ w)
       {
           gaussianKernel_2D[id++] = _GET_GAUSSIAN_K2D_(w,h,sigma*sigma);
       }
   }
}
void gaussian_smooth_2d(uint8_t * src,uint8_t *dst,uint32_t pixel_w,uint32_t pixel_h,uint32_t channel,float kernel2D[],uint32_t kernel_size,uint32_t padding_mode)
{
   convolution_2d_u8_kfloat( src,dst, pixel_w, pixel_h, channel,kernel2D, kernel_size, padding_mode);
}