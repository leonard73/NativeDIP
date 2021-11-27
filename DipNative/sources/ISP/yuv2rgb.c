#include "../../include/dipNativeInc.h"
#ifdef NEON_OPTIMIZATION_USED
#include <arm_neon.h>
#endif
static inline int minmax(int min, int v, int max)
{
	return (v<min)?min:((max<v)?max:v);
}
void loadStreamYuyv2RGBA(uint8_t * rgb,uint8_t* yuyv, uint32_t width, uint32_t height)
{
	int i = 0, j = 0;
	int32_t y0 = 0, y1 = 0, u = 0,  v = 0;
	int index = 0;
    for (i = 0; i < height; ++i)
	{
		for (j = 0; j < width; j += 2)
		{
			index = i * width + j;
			y0 = yuyv[index*2 + 0] << 8;
			u = yuyv[index*2 + 1] - 128;
			y1 = yuyv[index*2 + 2] << 8;
			v = yuyv[index*2 + 3] - 128;
			
			rgb[index*3 + 0] = minmax(0, (y0 + 359*v)>>8, 255);
			rgb[index*3 + 1] = minmax(0, (y0 + 88*v - 183*u)>>8, 255);
			rgb[index*3 + 2] = minmax(0, (y0 + 454*u)>>8, 255);
			rgb[index*3 + 3] = minmax(0, (y1 + 359*v)>>8, 255);
			rgb[index*3 + 4] = minmax(0, (y1 + 88*v - 183*u)>>8, 255);
			rgb[index*3 + 5] = minmax(0, (y1 + 454*u)>>8, 255);
		}
	}
}
void loadStreamYuyv2RGBA_Neon_I32(uint8_t * rgb,uint8_t* yuyv, uint32_t width, uint32_t height)
{
	int i = 0, j = 0;
	int32_t y0 = 0, y1 = 0, u = 0,  v = 0;
	int index = 0;
    for (i = 0; i < height; ++i)
	{
		for (j = 0; j < width; j += 8)
		{
			index = i * width + j;
			y0 = yuyv[index*2 + 0] << 8;
			u = yuyv[index*2 + 1] - 128;
			y1 = yuyv[index*2 + 2] << 8;
			v = yuyv[index*2 + 3] - 128;
			
			rgb[index*3 + 0] = minmax(0, (y0 + 359*v)>>8, 255);
			rgb[index*3 + 1] = minmax(0, (y0 + 88*v - 183*u)>>8, 255);
			rgb[index*3 + 2] = minmax(0, (y0 + 454*u)>>8, 255);
			rgb[index*3 + 3] = minmax(0, (y1 + 359*v)>>8, 255);
			rgb[index*3 + 4] = minmax(0, (y1 + 88*v - 183*u)>>8, 255);
			rgb[index*3 + 5] = minmax(0, (y1 + 454*u)>>8, 255);
		}
	}
}
void loadStreamY82RGBA(uint8_t * rgb,uint8_t* yuyv, uint32_t width, uint32_t height)
{
	int i = 0, j = 0;
	int32_t y = 0;
	int index = 0;
    for (i = 0; i < height; ++i)
	{
		for (j = 0; j < width; j +=1)
		{
			index = i * width + j;
			y = yuyv[index];			
			rgb[index*3 + 0] = minmax(0, y, 255);
			rgb[index*3 + 1] = minmax(0, y, 255);
			rgb[index*3 + 2] = minmax(0, y, 255);
		}
	}
}
void loadStreamYUYV2RGBA_GRAY(uint8_t * rgb,uint8_t* yuyv, uint32_t width, uint32_t height)
{
	int i = 0, j = 0;
	int32_t y0 = 0, y1 = 0;
	int index = 0;
    for (i = 0; i < height; ++i)
	{
		for (j = 0; j < width; j += 2)
		{
			index = i * width + j;
			y0 = yuyv[index*2 + 0] ;
			y1 = yuyv[index*2 + 2] ;
			
			rgb[index*3 + 0] = y0;
			rgb[index*3 + 1] = y0;
			rgb[index*3 + 2] = y0;
			rgb[index*3 + 3] = y1;
			rgb[index*3 + 4] = y1;
			rgb[index*3 + 5] = y1;
		}
	}
}
void transStreamBGR2RGB(const uint8_t * bgr,uint8_t  * rgb, uint32_t pixel_w,uint32_t pixel_h)
{
	uint32_t index_count = pixel_w * pixel_h * 3;
	for(uint32_t id = 0 ; id < index_count ; id  += 3)
	{
		rgb[id + 0] = bgr[id + 2];
		rgb[id + 1] = bgr[id + 1];
		rgb[id + 2] = bgr[id + 0];
	}
}