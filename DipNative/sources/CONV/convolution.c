#include "../../include/dipNativeInc.h"
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
void convolution_1d_u8_kfloat_zeroPadding(uint8_t * src,uint8_t *dst,uint32_t pixel_nb,float kernel1D[],uint32_t kernel_size)
{
    uint32_t k_h = kernel_size /2 ;
    for(uint32_t id  = 0 ; id < pixel_nb; ++id)
    {
        float sum = 0.0f, sum_k  = 0.0f;
        for(uint32_t k = id - k_h,kernel_id=0 ; k <=  id + k_h  ;k++,kernel_id++)
        {
            bool   isValInRange = (k>=0) && (k<pixel_nb);
            float  val = ( isValInRange ) ? (float)src[k] : 0.0f;
            sum   += kernel1D[kernel_id] * val;
            sum_k += (isValInRange) ? kernel1D[kernel_id] : 0.0f;
        }
        dst[id] = (uint8_t) ( sum / sum_k + 0.5f ) ;
    }
}
void convolution_1d_u8_kfloat_replicatePadding(uint8_t * src,uint8_t *dst,uint32_t pixel_nb,float kernel1D[],uint32_t kernel_size)
{
    uint32_t k_h = kernel_size /2 ;
    float sum_k  =  0.0f;
    for(uint32_t kernel_i = 0  ;  kernel_i  <   kernel_size; ++kernel_size)
    {
        sum_k += kernel1D[kernel_i];
    }
    float sum_k_inv = 1.0f / sum_k;
    for(uint32_t id  = k_h ; id < pixel_nb-k_h; ++id)
    {
        float sum = 0.0f, sum_k  = 0.0f;
        for(uint32_t k = id - k_h,kernel_id=0 ; k <=  id + k_h  ;k++,kernel_id++)
        {
            float  val = ( k<0 ) ? (float)src[0] : (( k>=pixel_nb ) ? (float)src[pixel_nb-1] : (float)src[k]);
            sum   += kernel1D[kernel_id] * val;
        }
        dst[id] = (uint8_t) ( sum * sum_k_inv + 0.5f ) ;
    }
    memcpy(&dst[0],&src[0],k_h * sizeof(uint8_t));
    memcpy(&dst[pixel_nb-k_h],&src[pixel_nb-k_h],k_h * sizeof(uint8_t));
}
void convolution_2d_u8_kfloat_replicatePadding(uint8_t * src,uint8_t *dst,uint32_t pixel_w,uint32_t pixel_h,uint32_t channel,float kernel2D[],uint32_t kernel_size)
{
    uint32_t k_h = kernel_size /2 ;
    float sum_k  =  0.0f;
    uint32_t pixel_total_width = channel  * pixel_w;
    for(uint32_t kernel_i = 0  ;  kernel_i  <   kernel_size*kernel_size; ++kernel_i)
    {
        sum_k += kernel2D[kernel_i];
    }
    float sum_k_inv = 1.0f / sum_k;
    //index offset
    uint32_t kernel_col_offset =  k_h*3;
    for(uint32_t id_row  = k_h ; id_row < pixel_h-k_h; ++id_row)
    {
        for(uint32_t id_col = kernel_col_offset; id_col < pixel_total_width-kernel_col_offset; ++id_col)
        {
            uint32_t center_index = id_row * pixel_total_width + id_col;
            float sum = 0.0f;uint32_t k_id=0;
            for(uint32_t h = id_row - k_h ; h <=id_row + k_h; ++ h)
            {
                for(uint32_t w = id_col - kernel_col_offset ; w<=id_col+kernel_col_offset;  w+=channel)
                {
                    uint32_t sub_index = h * pixel_total_width + w;
                    float val = (float)src[sub_index];
                    sum   += val*kernel2D[k_id++];
                }
            }
            // printf("sum = %f;center_index=%d;sum_k_inv=%f\n\n",sum,center_index,sum_k_inv);
            dst[center_index] = (uint8_t) ( sum * sum_k_inv + 0.5f ) ;
        }
        memcpy(&dst[id_row*pixel_total_width],&src[id_row*pixel_total_width],k_h * channel*sizeof(uint8_t));
        memcpy(&dst[(id_row+1)*pixel_total_width-k_h*channel],&src[(id_row+1)*pixel_total_width-k_h*channel],k_h *channel* sizeof(uint8_t));
    }
    memcpy(&dst[0],&src[0],k_h * pixel_total_width * sizeof(uint8_t));
    memcpy(&dst[(pixel_h-k_h)*pixel_total_width],&src[(pixel_h-k_h)*pixel_total_width],k_h * pixel_total_width * sizeof(uint8_t));
}
void convolution_2d_u8_kfloat_zeroPadding(uint8_t * src,uint8_t *dst,uint32_t pixel_w,uint32_t pixel_h,uint32_t channel,float kernel2D[],uint32_t kernel_size)
{
    uint32_t k_h = kernel_size /2 ;
    uint32_t pixel_total_width = channel  * pixel_w;
    for(uint32_t id_row  = 0 ; id_row < pixel_h; ++id_row)
    {
        for(uint32_t id_col = 0; id_col < pixel_total_width; ++id_col)
        {
            uint32_t center_index = id_row * pixel_total_width + id_col;
            float sum = 0.0f, sum_k  = 0.0f;
            uint32_t kernel_id=0;
            for(uint32_t h = id_row - k_h ; h <=id_row + k_h; ++ h)
            {
                for(uint32_t w = id_col - k_h ; w<=id_col+k_h;  w+=3)
                {
                    uint32_t sub_index = h * pixel_total_width + w;
                    bool  isValInRange = ((h>=0) && (h<pixel_h)) && ((w>=0) && (w<pixel_total_width));
                    float val = (isValInRange) ? ((float)src[sub_index]) : (0.0f) ;
                    sum   += val;
                    sum_k += (isValInRange) ? kernel2D[kernel_id] : 0.0f;
                    kernel_id++;
                }
            }
            dst[center_index] = (uint8_t) ( sum / sum_k + 0.5f ) ;
        }
    }
}
void convolution_1d_u8_kfloat(uint8_t * src,uint8_t *dst,uint32_t pixel_nb,float kernel1D[],uint32_t kernel_size,uint32_t padding_mode)
{
    switch(padding_mode)
    {
        case CONVOLUTION_PADDING_MODE_ZERO     :convolution_1d_u8_kfloat_zeroPadding(src,dst,pixel_nb,kernel1D,kernel_size)     ;break;
        case CONVOLUTION_PADDING_MODE_REPLICATE:convolution_1d_u8_kfloat_replicatePadding(src,dst,pixel_nb,kernel1D,kernel_size);break;
        default: break;
    }
}
void convolution_2d_u8_kfloat(uint8_t * src,uint8_t *dst,uint32_t pixel_w,uint32_t pixel_h,uint32_t channel,float kernel2D[],uint32_t kernel_size,uint32_t padding_mode)
{
    switch(padding_mode)
    {
        case CONVOLUTION_PADDING_MODE_ZERO     :convolution_2d_u8_kfloat_zeroPadding(src,dst,pixel_w,pixel_h,channel,kernel2D,kernel_size)     ;break;
        case CONVOLUTION_PADDING_MODE_REPLICATE:convolution_2d_u8_kfloat_replicatePadding(src,dst,pixel_w,pixel_h,channel,kernel2D,kernel_size);break;
        default: break;
    }
}