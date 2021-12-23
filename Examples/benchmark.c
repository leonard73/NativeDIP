#include <driverInc.h>
#include <dipNativeInc.h>
#include <sys/time.h>
#ifndef LOG_D
#define LOG_D(X,...) printf("[Debug]: " #X " \n",##__VA_ARGS__)
#endif
#ifndef __DISABLE_PIPELINE_LATENCY__
#define LOG_PIPELINE_LATENCY(name,latency_us)             fflush(stdout);printf("[%s]: Run %6ld US;",name,latency_us)
#define LOG_PIPELINE_LATENCY_END(name,latency_us)         fflush(stdout);printf("[%s]: Run %6ld US;\r",name,latency_us)
#else
#define LOG_PIPELINE_LATENCY(name,latency_us)             (void)0
#define LOG_PIPELINE_LATENCY_END(name,latency_us)         (void)0
#endif
#define DATA_SPLASH_BMP_PATH "Data/splash.bmp"
uint32_t algo_id  = 0, step_2_id=0;
#define MAX_OUTPUT_DATA_SIZE                    (1024*1024*16)
uint8_t OUTPUT_MEM[MAX_OUTPUT_DATA_SIZE];
uint8_t OUTPUT_MEM2[MAX_OUTPUT_DATA_SIZE];
void uvc_frame_display(int argc, char *argv[]);
void bmp_frame_display(int argc, char *argv[]);
void frame_piplineFunc(frameRawData * frame_in,glShowDataRGB * frame_out); 
void frame_pipline_step0_init(frameRawData * frame_in,glShowDataRGB * frame_out);
void frame_pipline_step1_run_bgr2rgb(frameRawData * frame_in,glShowDataRGB * frame_out);
void frame_pipline_step1_run_bgr_rgb_gray(frameRawData * frame_in,glShowDataRGB * frame_out);
void frame_pipline_step1_run_yuyv2rgb(frameRawData * frame_in,glShowDataRGB * frame_out);
void frame_pipline_step1_run_yuyv_rgb_gray(frameRawData * frame_in,glShowDataRGB * frame_out);
void frame_pipline_step1_run_y8_rgb_gray(frameRawData * frame_in,glShowDataRGB * frame_out);
void frame_pipline_step2_run_gamma_correct(frameRawData * frame_in,glShowDataRGB * frame_out);
void frame_pipline_step2_run_gaussian_smooth(frameRawData * frame_in,glShowDataRGB * frame_out);
void frame_pipline_step3_run_fast9_corner_detector(frameRawData * frame_in,glShowDataRGB * frame_out);
void frame_piplineFunc(frameRawData * frame_in,glShowDataRGB * frame_out)
{
    struct timeval tpstart,tpend;
    long timeuse_us;
    static long mean_latency_ms=0;
    gettimeofday(&tpstart,NULL); 
    LOG_D("call frame_piplineFunc\n");
    frame_pipline_step0_init(frame_in,frame_out);
    switch(algo_id)
    {
        case  ALGO_ID_BMP_BGR2RGB        :frame_pipline_step1_run_bgr2rgb(frame_in,frame_out);       break;
        case  ALGO_ID_BMP_RGB_RGB2GRAY   :frame_pipline_step1_run_bgr_rgb_gray(frame_in,frame_out);  break;
        case  ALGO_ID_UVC_YUYV2RGB       :frame_pipline_step1_run_yuyv2rgb(frame_in,frame_out);      break;
        case  ALGO_ID_UVC_YUYV2RGBGRAY   :frame_pipline_step1_run_yuyv_rgb_gray(frame_in,frame_out); break;
        case  ALGO_ID_UVC_Y82RGBGRAY     :frame_pipline_step1_run_y8_rgb_gray(frame_in,frame_out); break;
        default                          :                                                           break;
    }
    gettimeofday(&tpend,NULL); 
    timeuse_us=1000000*(tpend.tv_sec-tpstart.tv_sec) + tpend.tv_usec-tpstart.tv_usec; 
    LOG_PIPELINE_LATENCY("Format",timeuse_us);
    gettimeofday(&tpstart,NULL);
    switch(frame_in->piplineCtl.step2_id)
    {
        case ALGO_ID_RGB_GAMMA_CORRECT   :frame_pipline_step2_run_gamma_correct(frame_in,frame_out)  ; break;
        case ALGO_ID_RGB_GAUSSIAN_SMOOTH :frame_pipline_step2_run_gaussian_smooth(frame_in,frame_out); break;
        default                          :                                                             break;
    }
    frame_in->piplineCtl.step3_id = ALGO_ID_RGB_FAST9_CORNER_DETECT;
    switch(frame_in->piplineCtl.step3_id)
    {
        case ALGO_ID_RGB_FAST9_CORNER_DETECT    :frame_pipline_step3_run_fast9_corner_detector(frame_in,frame_out)  ;   break;
        default                                 :                                                                       break;
    }
    gettimeofday(&tpend,NULL); 
    timeuse_us=1000000*(tpend.tv_sec-tpstart.tv_sec) + tpend.tv_usec-tpstart.tv_usec; 
    LOG_PIPELINE_LATENCY_END("ISP",timeuse_us);
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
        case 7  : algo_id=ALGO_ID_UVC_Y82RGBGRAY;        break;
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
void frame_pipline_step1_run_y8_rgb_gray(frameRawData * frame_in,glShowDataRGB * frame_out)
{
    frame_out->pixelWidth  = frame_in->frameWidth;
    frame_out->pixelHeight = frame_in->frameHeight;
    frame_out->dataSize    = frame_out->pixelWidth*frame_out->pixelHeight*3;
    loadStreamY82RGBA((uint8_t*)frame_out->rgb_data_p,frame_in->frameRawPtr,frame_in->frameWidth,frame_in->frameHeight); 
    LOG_D("Run loadStreamYUYV2RGBA_GRAY OK %s", " ");    
}
void frame_pipline_step2_run_gamma_correct(frameRawData * frame_in,glShowDataRGB * frame_out)
{
    uint8_t table_gamma[256];
    generateTable(table_gamma,256,frame_in->filterParam.gammaCorrectParameters.GammaCorrect_power_level_inv);
    GammaCorrection((uint8_t*)frame_out->rgb_data_p,(uint8_t*)frame_out->rgb_data_p,frame_out->dataSize,table_gamma);  
    frame_out->pixelHeight *= 2;
    frame_out->dataSize    = frame_out->pixelWidth*frame_out->pixelHeight*3;
    LOG_D("Run GammaCorrection OK %s", " ");
}
void frame_pipline_step2_run_gaussian_smooth(frameRawData * frame_in,glShowDataRGB * frame_out)
{
    uint32_t kernel_size = frame_in->filterParam.gaussianSmoothParameters.GaussianSmooth_Kernel;
    float    sigma       = frame_in->filterParam.gaussianSmoothParameters.GaussianSmooth_Sigma;
    uint32_t paddingMode = frame_in->filterParam.gaussianSmoothParameters.GaussianSmooth_Padding_Mode;
    uint32_t width       = frame_in->frameWidth;
    uint32_t height      = frame_in->frameHeight;
    uint8_t  *src        = frame_out->rgb_data_p;
    uint8_t  *dst        = &OUTPUT_MEM2[0];
    #if 0
    float *gaussian_kernel_tbl = 0;
    gaussian_kernel_tbl=(float*)  malloc (kernel_size*kernel_size*sizeof(float));  
    generate_gaussian_kernel_2d(gaussian_kernel_tbl,kernel_size,sigma);
    gaussian_smooth_2d(src,dst,width,height,3,gaussian_kernel_tbl,kernel_size,paddingMode,0);
    if(!gaussian_kernel_tbl) {free(gaussian_kernel_tbl);}
    #else
    uint16_t *gaussian_kernel_tbl = 0;
    gaussian_kernel_tbl = (uint16_t*) malloc(kernel_size*kernel_size*sizeof(uint32_t)); 
    generate_gaussian_kernel_2d_fixU16(gaussian_kernel_tbl,8,kernel_size,sigma);   
    gaussian_smooth_2d(src,dst,width,height,3,gaussian_kernel_tbl,kernel_size,paddingMode,8);
    if(!gaussian_kernel_tbl) {free(gaussian_kernel_tbl);}   
    #endif
}
void frame_pipline_step3_run_fast9_corner_detector(frameRawData * frame_in,glShowDataRGB * frame_out)
{
    uint32_t static threshould=100;
    uint32_t width       = frame_in->frameWidth;
    uint32_t height      = frame_in->frameHeight;
    uint8_t  *src        = frame_out->rgb_data_p;
    uint8_t  *dst        = frame_out->rgb_data_p;
    uint32_t  width_rgb = width*3;
    uint32_t  coord_count=0;
    // memcpy(dst,src,height*width*3);
    for(uint32_t i=7;i<height-7;i++)
    {
        for(uint32_t j=7;j<width-7;j++)
        {
            uint32_t index_center  = i*width*3+j*3;
            uint32_t index_left    = index_center-3*1;
            uint32_t index_right   = index_center+3*1;
            uint32_t index_up      = index_center-1*width*3;
            uint32_t index_down    = index_center+1*width*3;
            int16_t  pixel_left    = (int16_t)src[index_left]+(int16_t)src[index_left+1]+(int16_t)src[index_left+2];
            int16_t  pixel_right   = (int16_t)src[index_right]+(int16_t)src[index_right+1]+(int16_t)src[index_right+2];
            int16_t  pixel_up      = (int16_t)src[index_up]+(int16_t)src[index_up+1]+(int16_t)src[index_up+2];
            int16_t  pixel_down    = (int16_t)src[index_down]+(int16_t)src[index_down+1]+(int16_t)src[index_down+2];
            int16_t  pixel_center  = (int16_t)src[index_center]+(int16_t)src[index_center+1]+(int16_t)src[index_center+2];
            int16_t  Ix            = (pixel_right - pixel_left)/3;
            int16_t  Iy            = (pixel_up - pixel_down)/3;
            int32_t  Ix2           = Ix*Ix;
            int32_t  Iy2           = Iy*Iy;
            int32_t  Ixy           = Ix*Iy;
            int32_t  DET           = Ix2 * Iy2 - Ixy * Ixy;
            int32_t  TRACE         = Ix2 + Iy2;
            int32_t  RESPONSE      = ((DET<<7)  -  (7 * TRACE*TRACE))>>7;
            if(RESPONSE >(256*256*256*100*threshould))
            {
                coord_count++;
                dst[index_center] = 220;            dst[index_center+1] = 0;            dst[index_center+2] = 0;
                dst[index_center+9-width_rgb*1] = 220;  dst[index_center+9-width_rgb*1+1] = 0;  dst[index_center+9-width_rgb*1+2] = 0;
                dst[index_center+9] = 220;          dst[index_center+9+1] = 0;          dst[index_center+9+2] = 0;
                dst[index_center+9+width_rgb*1] = 220;  dst[index_center+9+width_rgb*1+1] = 0;  dst[index_center+9+width_rgb*1+2] = 0;
                dst[index_center+6-width_rgb*2] = 220;  dst[index_center+6-width_rgb*2+1] = 0;  dst[index_center+6-width_rgb*2+2] = 0;
                dst[index_center+6+width_rgb*2] = 220;  dst[index_center+6+width_rgb*2+1] = 0;  dst[index_center+6+width_rgb*2+2] = 0;
                dst[index_center+3-width_rgb*3] = 220;  dst[index_center+3-width_rgb*3+1] = 0;  dst[index_center+3-width_rgb*3+2] = 0;
                dst[index_center+3+width_rgb*3] = 220;  dst[index_center+3+width_rgb*3+1] = 0;  dst[index_center+3+width_rgb*3+2] = 0;
                dst[index_center-3-width_rgb*3] = 220;  dst[index_center-3-width_rgb*3+1] = 0;  dst[index_center-3-width_rgb*3+2] = 0;
                dst[index_center-3+width_rgb*3] = 220;  dst[index_center-3+width_rgb*3+1] = 0;  dst[index_center-3+width_rgb*3+2] = 0;
                dst[index_center-9-width_rgb*1] = 220;  dst[index_center-9-width_rgb*1+1] = 0;  dst[index_center-9-width_rgb*1+2] = 0;
                dst[index_center-9] = 220;          dst[index_center-9+1] = 0;          dst[index_center-9+2] = 0;
                dst[index_center-9+width_rgb*1] = 220;  dst[index_center-9+width_rgb*1+1] = 0;  dst[index_center-9+width_rgb*1+2] = 0;
                dst[index_center-6-width_rgb*2] = 220;  dst[index_center-6-width_rgb*2+1] = 0;  dst[index_center-6-width_rgb*2+2] = 0;
                dst[index_center-6+width_rgb*2] = 220;  dst[index_center-6+width_rgb*2+1] = 0;  dst[index_center-6+width_rgb*2+2] = 0;
                dst[index_center+width_rgb*3] = 220;  dst[index_center+width_rgb*3+1] = 0;  dst[index_center+width_rgb*3+2] = 0;
                dst[index_center-width_rgb*3] = 220;  dst[index_center-width_rgb*3+1] = 0;  dst[index_center-width_rgb*3+2] = 0;
            }

        }
    }
    if(coord_count > 1000 ) 
    {
        threshould ++;
    }
    if(coord_count < 50 ) 
    {
        threshould --;
    }
    if(threshould > 127 )
    {
        threshould =  127;
    }
    if(threshould <  10)
    {
        threshould =  10;
    }
    printf("threshould =  %d;  coord_count=%d \n",threshould,coord_count);
}
void uvc_frame_display(int argc, char *argv[])
{
    start_gl_show_frame(argc,argv,1280,480,&frame_piplineFunc,"/dev/video0");
}
void bmp_frame_display(int argc, char *argv[])
{
    start_gl_show_bmp(argc, argv,DATA_SPLASH_BMP_PATH,640,480,&frame_piplineFunc);
}