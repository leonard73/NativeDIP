#include "../../include/dipNativeInc.h"
#include <math.h>
#ifdef NEON_OPTIMIZATION_USED
#include <arm_neon.h>
uint8x8x4_t vTable[TABLE_SIZE>>5];
#endif
void generateTable(unsigned char * table,unsigned int size)
{
    for(unsigned int i=0;i<size ;i++)
    {
        table[i] = (uint8_t)round((pow(i/255.f, 1/2.2)*255.f));
        // printf("table[%3d]=%3d\n",i,table[i]);
    }
}
void GammaCorrection(unsigned char *src,unsigned char *dst,unsigned int pixel_nb,unsigned char * table)
{
    for(unsigned int i=0;i<pixel_nb;i++)
    {
        dst[i] = table[src[i]];
    }
}