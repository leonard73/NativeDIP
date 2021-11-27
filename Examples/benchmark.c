#include <driverInc.h>
#define DATA_SPLASH_BMP_PATH "Data/splash.bmp"
uint32_t algo_id  = 0;
#define ALGO_ID_BMP_BGR2RGB                  (0x00000000)
#define ALGO_ID_BMP_RGB_RGB2GRAY             (0x00000001)
#define ALGO_ID_UVC_YUYV2RGB                 (0x00000002)
#define ALGO_ID_UVC_YUYV_SHOW_GRAY           (0x00000003)
#define MAX_OUTPUT_DATA_SIZE (1024*1024*32)
uint8_t OUTPUT_MEM[MAX_OUTPUT_DATA_SIZE];
void frame_piplineFunc(frameRawData * frame_in,glShowDataRGB * frame_out); 
void frame_pipline_step0_init(frameRawData * frame_in,glShowDataRGB * frame_out);
void frame_pipline_step1_run_rgb_show_directy(frameRawData * frame_in,glShowDataRGB * frame_out);
void frame_pipline_step1_run_yuyv2rgb(frameRawData * frame_in,glShowDataRGB * frame_out);
void frame_pipline_step1_run_yuyv_rgb_gray(frameRawData * frame_in,glShowDataRGB * frame_out);
void frame_piplineFunc(frameRawData * frame_in,glShowDataRGB * frame_out)
{
    printf("call frame_piplineFunc\n");
    frame_pipline_step0_init(frame_in,frame_out);
    switch(algo_id)
    {
        case  ALGO_ID_BMP_BGR2RGB        :frame_pipline_step1_run_rgb_show_directy(frame_in,frame_out);break;
        case  ALGO_ID_UVC_YUYV2RGB       :frame_pipline_step1_run_yuyv2rgb(frame_in,frame_out);break;
        case  ALGO_ID_UVC_YUYV_SHOW_GRAY :frame_pipline_step1_run_yuyv_rgb_gray(frame_in,frame_out);break;
        default:break;
    }
    // loadStreamY82RGBA(captured_p->head.start,glFrameWidthSet,glFrameHeightSet);
    
}
int  main(int argc, char *argv[])
{
    switch(atoi(argv[2]))
    {
        case 0:algo_id=ALGO_ID_BMP_BGR2RGB;break;
        case 1:algo_id=ALGO_ID_BMP_RGB_RGB2GRAY;break;
        case 2:algo_id=ALGO_ID_UVC_YUYV2RGB;break;
        case 3:algo_id=ALGO_ID_UVC_YUYV_SHOW_GRAY;break;
        default:break;
    }
    switch(atoi(argv[1]))
    {
        case 1:bmp_frame_display(argc, argv);break;
        case 2:uvc_frame_display(argc, argv);break;
        default:break;
    }
}
void frame_pipline_step0_init(frameRawData * frame_in,glShowDataRGB * frame_out)
{
    frame_out->pixelWidth  = frame_in->frameWidth;
    frame_out->pixelHeight = frame_in->frameHeight;
    frame_out->dataSize    = frame_in->frameSize;
    frame_out->rgb_data_p  = &OUTPUT_MEM[0];
    printf("frame_out->pixelWidth=%d\n",frame_out->pixelWidth);
    printf("frame_out->pixelHeight=%d\n",frame_out->pixelHeight);
    printf("frame_out->dataSize=%d\n",frame_out->dataSize);   
}
void frame_pipline_step1_run_rgb_show_directy(frameRawData * frame_in,glShowDataRGB * frame_out)
{
    frame_out->pixelWidth  = frame_in->frameWidth;
    frame_out->pixelHeight = frame_in->frameHeight;
    frame_out->dataSize    = frame_out->pixelWidth*frame_out->pixelHeight*3;
    memcpy(frame_out->rgb_data_p,frame_in->frameRawPtr,frame_out->dataSize);
    transStreamBGR2RGB(frame_in->frameRawPtr,frame_out->rgb_data_p, frame_in->frameWidth,frame_in->frameHeight);
}
void frame_pipline_step1_run_yuyv2rgb(frameRawData * frame_in,glShowDataRGB * frame_out)
{
    frame_out->pixelWidth  = frame_in->frameWidth;
    frame_out->pixelHeight = frame_in->frameHeight;
    frame_out->dataSize    = frame_out->pixelWidth*frame_out->pixelHeight*3;
    loadStreamYuyv2RGBA((uint8_t*)frame_out->rgb_data_p,frame_in->frameRawPtr,frame_in->frameWidth,frame_in->frameHeight);
}
void frame_pipline_step1_run_yuyv_rgb_gray(frameRawData * frame_in,glShowDataRGB * frame_out)
{
    frame_out->pixelWidth  = frame_in->frameWidth;
    frame_out->pixelHeight = frame_in->frameHeight;
    frame_out->dataSize    = frame_out->pixelWidth*frame_out->pixelHeight*3;
    loadStreamYUYV2RGBA_GRAY((uint8_t*)frame_out->rgb_data_p,frame_in->frameRawPtr,frame_in->frameWidth,frame_in->frameHeight);   
}
void uvc_frame_display(int argc, char *argv[])
{
    start_gl_show_frame(argc,argv,640,480,&frame_piplineFunc);
}
void bmp_frame_display(int argc, char *argv[])
{
    start_gl_show_bmp(argc, argv,DATA_SPLASH_BMP_PATH,640,480,&frame_piplineFunc);
}