#include "../../include/dipNativeInc.h"
#include <arm_neon.h>
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