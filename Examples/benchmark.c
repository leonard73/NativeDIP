#include <driverInc.h>
#include <dipNativeInc.h>
#ifndef LOG_D
#define LOG_D(X,...) printf("[Debug]: " #X " \n",##__VA_ARGS__)
#endif
#define DATA_SPLASH_BMP_PATH "Data/splash.bmp"
uint32_t algo_id  = 0, step_2_id=0;
#define ALGO_ID_BMP_BGR2RGB                     (0x00000000)
#define ALGO_ID_BMP_RGB_RGB2GRAY                (0x00000001)
#define ALGO_ID_UVC_YUYV2RGB                    (0x00000002)
#define ALGO_ID_UVC_YUYV2RGBGRAY                (0x00000003)
#define ALGO_ID_RGB_GAMMA_CORRECT               (0x00000004)
#define MAX_OUTPUT_DATA_SIZE                    (1024*1024*32)
uint8_t OUTPUT_MEM[MAX_OUTPUT_DATA_SIZE];
void frame_piplineFunc(frameRawData * frame_in,glShowDataRGB * frame_out); 
void frame_pipline_step0_init(frameRawData * frame_in,glShowDataRGB * frame_out);
void frame_pipline_step1_run_bgr2rgb(frameRawData * frame_in,glShowDataRGB * frame_out);
void frame_pipline_step1_run_bgr_rgb_gray(frameRawData * frame_in,glShowDataRGB * frame_out);
void frame_pipline_step1_run_yuyv2rgb(frameRawData * frame_in,glShowDataRGB * frame_out);
void frame_pipline_step1_run_yuyv_rgb_gray(frameRawData * frame_in,glShowDataRGB * frame_out);
void frame_pipline_step2_run_gamma_correct(frameRawData * frame_in,glShowDataRGB * frame_out);
void frame_piplineFunc(frameRawData * frame_in,glShowDataRGB * frame_out)
{
    LOG_D("call frame_piplineFunc\n");
    frame_pipline_step0_init(frame_in,frame_out);
    switch(algo_id)
    {
        case  ALGO_ID_BMP_BGR2RGB        :frame_pipline_step1_run_bgr2rgb(frame_in,frame_out);       break;
        case  ALGO_ID_BMP_RGB_RGB2GRAY   :frame_pipline_step1_run_bgr_rgb_gray(frame_in,frame_out);  break;
        case  ALGO_ID_UVC_YUYV2RGB       :frame_pipline_step1_run_yuyv2rgb(frame_in,frame_out);      break;
        case  ALGO_ID_UVC_YUYV2RGBGRAY   :frame_pipline_step1_run_yuyv_rgb_gray(frame_in,frame_out); break;
        default                          :                                                           break;
    }
    switch(frame_in->piplineCtl.step2_id)
    {
        case ALGO_ID_RGB_GAMMA_CORRECT   :frame_pipline_step2_run_gamma_correct(frame_in,frame_out); break;
        default                          :                                                           break;
    }
}
int  main(int argc, char *argv[])
{
    step_2_id = atoi(argv[3]);
    switch(atoi(argv[2]))
    {
        case 0  : algo_id=ALGO_ID_BMP_BGR2RGB;           break;
        case 1  : algo_id=ALGO_ID_BMP_RGB_RGB2GRAY;      break;
        case 2  : algo_id=ALGO_ID_UVC_YUYV2RGB;          break;
        case 3  : algo_id=ALGO_ID_UVC_YUYV2RGBGRAY;      break;
        default :                                        break;
    }
    switch(atoi(argv[1]))
    {
        case 1  : bmp_frame_display(argc, argv);         break;
        case 2  : uvc_frame_display(argc, argv);         break;
        default :                                        break;
    }
}
void frame_pipline_step0_init(frameRawData * frame_in,glShowDataRGB * frame_out)
{
    frame_out->pixelWidth  = frame_in->frameWidth;
    frame_out->pixelHeight = frame_in->frameHeight;
    frame_out->dataSize    = frame_in->frameSize;
    frame_out->rgb_data_p  = &OUTPUT_MEM[0];
    #if 0
    LOG_D("frame_out->pixelWidth=%d\n",frame_out->pixelWidth);
    LOG_D("frame_out->pixelHeight=%d\n",frame_out->pixelHeight);
    LOG_D("frame_out->dataSize=%d\n",frame_out->dataSize);   
    #endif
    LOG_D("Run frame_pipline_step0_init OK %s", " ");
}
void frame_pipline_step1_run_bgr2rgb(frameRawData * frame_in,glShowDataRGB * frame_out)
{
    frame_out->pixelWidth  = frame_in->frameWidth;
    frame_out->pixelHeight = frame_in->frameHeight;
    frame_out->dataSize    = frame_out->pixelWidth*frame_out->pixelHeight*3;
    memcpy(frame_out->rgb_data_p,frame_in->frameRawPtr,frame_out->dataSize);
    transStreamBGR2RGB(frame_in->frameRawPtr,frame_out->rgb_data_p, frame_in->frameWidth,frame_in->frameHeight);
    LOG_D("Run transStreamBGR2RGB OK %s", " ");
}
void frame_pipline_step1_run_bgr_rgb_gray(frameRawData * frame_in,glShowDataRGB * frame_out)
{
    frame_out->pixelWidth  = frame_in->frameWidth;
    frame_out->pixelHeight = frame_in->frameHeight;
    frame_out->dataSize    = frame_out->pixelWidth*frame_out->pixelHeight*3;
    memcpy(frame_out->rgb_data_p,frame_in->frameRawPtr,frame_out->dataSize);
    transStreamBGR2RGB_GRAY(frame_in->frameRawPtr,frame_out->rgb_data_p, frame_in->frameWidth,frame_in->frameHeight);
    LOG_D("Run transStreamBGR2RGB_GRAY OK %s", " ");
}
void frame_pipline_step1_run_yuyv2rgb(frameRawData * frame_in,glShowDataRGB * frame_out)
{
    frame_out->pixelWidth  = frame_in->frameWidth;
    frame_out->pixelHeight = frame_in->frameHeight;
    frame_out->dataSize    = frame_out->pixelWidth*frame_out->pixelHeight*3;
    loadStreamYuyv2RGBA((uint8_t*)frame_out->rgb_data_p,frame_in->frameRawPtr,frame_in->frameWidth,frame_in->frameHeight);
    LOG_D("Run loadStreamYuyv2RGBA OK %s", " ");
}
void frame_pipline_step1_run_yuyv_rgb_gray(frameRawData * frame_in,glShowDataRGB * frame_out)
{
    frame_out->pixelWidth  = frame_in->frameWidth;
    frame_out->pixelHeight = frame_in->frameHeight;
    frame_out->dataSize    = frame_out->pixelWidth*frame_out->pixelHeight*3;
    loadStreamYUYV2RGBA_GRAY((uint8_t*)frame_out->rgb_data_p,frame_in->frameRawPtr,frame_in->frameWidth,frame_in->frameHeight); 
    LOG_D("Run loadStreamYUYV2RGBA_GRAY OK %s", " ");
}
void frame_pipline_step2_run_gamma_correct(frameRawData * frame_in,glShowDataRGB * frame_out)
{
    uint8_t table_gamma[256];
    generateTable(table_gamma,256);
    GammaCorrection((uint8_t*)frame_out->rgb_data_p,(uint8_t*)frame_out->rgb_data_p,frame_out->dataSize,table_gamma);  
    frame_out->pixelHeight *= 2;
    frame_out->dataSize    = frame_out->pixelWidth*frame_out->pixelHeight*3;
    LOG_D("Run GammaCorrection OK %s", " ");
}
void uvc_frame_display(int argc, char *argv[])
{
    start_gl_show_frame(argc,argv,2560,960,&frame_piplineFunc);
}
void bmp_frame_display(int argc, char *argv[])
{
    start_gl_show_bmp(argc, argv,DATA_SPLASH_BMP_PATH,640,480,&frame_piplineFunc);
}