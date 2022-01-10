#ifndef __CALL_BACK_DEF_H__
#define __CALL_BACK_DEF_H__
#include <stdint.h>
//algorithm controll
#define ALGO_ID_BMP_BGR2RGB                     (0x00000000)
#define ALGO_ID_BMP_RGB_RGB2GRAY                (0x00000001)
#define ALGO_ID_UVC_YUYV2RGB                    (0x00000002)
#define ALGO_ID_UVC_Y82RGBGRAY                  (0x00000007)
#define ALGO_ID_UVC_YUYV2RGBGRAY                (0x00000003)
#define ALGO_ID_RGB_GAMMA_CORRECT               (0x00000004)
#define ALGO_ID_RGB_GAUSSIAN_SMOOTH             (0x00000005)
#define ALGO_ID_RGB_MEDIAN_SMOOTH               (0x00000006)
#define ALGO_ID_RGB_FAST9_CORNER_DETECT         (0x00000007)
#define ALGO_ID_GRAY_APRITAG2                   (0x00000008)
#define FRAME_RAW_FORMAT_YUV422_YUYV            (0x04220001)
#define FRAME_RAW_FORMAT_YUV422_YUVY            (0x04220002)
#define FRAME_RAW_FORMAT_YUV422_UYVY            (0x04220003)
#define FRAME_RAW_FORMAT_YUV420_YV12            (0x04200001)
#define FRAME_RAW_FORMAT_YUV420_YU12            (0x04200002)
#define FRAME_RAW_FORMAT_YUV420_NV12            (0x04200003)
#define FRAME_RAW_FORMAT_YUV420_NV21            (0x04200004)
/***************************FRAME PIPELINE DEFINITIONS*********************/
typedef struct pipelineCTL_t
{
    uint32_t             step0_id;
    uint32_t             step1_id;
    uint32_t             step2_id;
    uint32_t             step3_id;
}pipelineCTL;
/***************************FILTER DEFINITIONS*****************************/
typedef struct gammaCorrectParam_t
{
    float                GammaCorrect_power_level_inv;
}gammaCorrectParam;
typedef struct gaussianSmoothParam_t
{
    uint32_t             GaussianSmooth_Kernel;
    float                GaussianSmooth_Sigma;
    uint32_t             GaussianSmooth_Padding_Mode;
}gaussianSmoothParam;
typedef struct medianSmoothParam_t
{
    uint32_t             MedianSmooth_Kernel;
    uint32_t             MedianSmooth_Padding_Mode;
}medianSmoothParam;
typedef struct filterParameter_t
{
    gammaCorrectParam    gammaCorrectParameters;
    gaussianSmoothParam  gaussianSmoothParameters;
    medianSmoothParam    medianSmoothParameters;
}filterParameter;
/**************************FRAME PIPELINE PARAMETERS*************************/
typedef struct frameRawData_t
{
    uint8_t *            frameRawPtr;
    double               timestampInSeconds;
    uint32_t             frameWidth;
    uint32_t             frameHeight;
    uint32_t             frameSize;
    uint32_t             frameFormat;
    uint32_t             frameIndex;
    pipelineCTL          piplineCtl;
    filterParameter      filterParam;
}frameRawData;
typedef struct glShowDataRGB_t
{
    uint8_t *            rgb_data_p;
    double               timestampInSeconds;
    uint32_t             pixelWidth;
    uint32_t             pixelHeight;
    uint32_t             dataSize;
    uint32_t             frameIndex;
}glShowDataRGB;
typedef void (*framePipLineFunc)(frameRawData * frame_in,glShowDataRGB * frame_out); 
#endif //__CALL_BACK_DEF_H__