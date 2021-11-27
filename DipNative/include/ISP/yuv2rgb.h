#ifndef __YUV2RGB_H__
#define __YUV2RGB_H__
#include <stdint.h>
void loadStreamYuyv2RGBA(uint8_t * rgb,uint8_t* yuyv, uint32_t width, uint32_t height);
void loadStreamY82RGBA(uint8_t * rgb,uint8_t* yuyv, uint32_t width, uint32_t height);
void transStreamBGR2RGB(const uint8_t * bgr,uint8_t  * rgb, uint32_t pixel_w,uint32_t pixel_h);
void loadStreamYUYV2RGBA_GRAY(uint8_t * rgb,uint8_t* yuyv, uint32_t width, uint32_t height);
#endif //__YUV2RGB_H__