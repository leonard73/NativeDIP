#ifndef __DIP_NATIVE_INC_H__
#define __DIP_NATIVE_INC_H__
#include "ISP/yuv2rgb.h"
#include "ISP/gammaCorrect.h"
#include "ISP/gaussianSmooth.h"
#include "ISP/medianFilter.h"
#include "CONV/convolution.h"
#ifdef __DISABLE_LOG_DEBUG__
#define LOG_D(X,...) (void)0
#endif
#endif //__DIP_NATIVE_INC_H__