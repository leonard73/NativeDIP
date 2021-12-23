#include "../../include/dipNativeInc.h"
uint8_t find_median_1D_U8(uint8_t *startP,uint8_t kernel_size)
{
    uint8_t median_index = kernel_size >>  1;
    uint8_t *tbl_decreasing_order=(uint8_t*) malloc (median_index * sizeof(uint8_t));
    tbl_decreasing_order[0] = startP[0];
    for(uint8_t i = kernel_size-1 ; i >=median_index  ; --i)
    {
        for(uint8_t j  = 0; j < i ; j++)
        {
            // if(startP[j] > tbl_decreasing_order)
        }
    }
    free(tbl_decreasing_order);
}
void medianFilter1D_U8(uint8_t * src,uint8_t *dst,uint32_t num,uint8_t kernel_size)
{
    uint32_t  half_k = kernel_size >>1;
    for(uint32_t id=half_k;id<num-half_k;id++)
    {
        for(uint32_t k =  id  - half_k ; k <= id + half_k ; ++k )
        {
            
        }
    }
}