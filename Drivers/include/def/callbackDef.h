#ifndef __CALL_BACK_DEF_H__
#define __CALL_BACK_DEF_H__
#include <stdint.h>
#define FRAME_RAW_FORMAT_YUV422_YUYV        (0x04220001)
#define FRAME_RAW_FORMAT_YUV422_YUVY        (0x04220002)
#define FRAME_RAW_FORMAT_YUV422_UYVY        (0x04220003)
#define FRAME_RAW_FORMAT_YUV420_YV12        (0x04200001)
#define FRAME_RAW_FORMAT_YUV420_YU12        (0x04200002)
#define FRAME_RAW_FORMAT_YUV420_NV12        (0x04200003)
#define FRAME_RAW_FORMAT_YUV420_NV21        (0x04200004)
typedef struct pipelineCTL_t
{
    uint32_t step0_id;
    uint32_t step1_id;
    uint32_t step2_id;
    uint32_t step3_id;
}pipelineCTL;
typedef struct frameRawData_t
{
    uint8_t *frameRawPtr;
    double   timestampInSeconds;
    uint32_t frameWidth;
    uint32_t frameHeight;
    uint32_t frameSize;
    uint32_t frameFormat;
    uint32_t frameIndex;
    pipelineCTL piplineCtl;
}frameRawData;
typedef struct glShowDataRGB_t
{
    uint8_t *rgb_data_p;
    double   timestampInSeconds;
    uint32_t pixelWidth;
    uint32_t pixelHeight;
    uint32_t dataSize;
    uint32_t frameIndex;
}glShowDataRGB;
typedef void (*framePipLineFunc)(frameRawData * frame_in,glShowDataRGB * frame_out); 
#endif //__CALL_BACK_DEF_H__