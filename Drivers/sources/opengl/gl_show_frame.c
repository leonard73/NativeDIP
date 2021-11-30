#include <GL/glut.h>
#include <stdio.h>
#include <sys/time.h>
// #include <bmp.h>
#include "driverInc.h"
#include "dipNativeInc.h"
uint32_t glFrameWidthSet;
uint32_t glFrameHeightSet;
uint32_t glFramePixelCount;
GLubyte *pixel_array=0;
GLint width,height;
float zoom_h_ratio =1;
float zoom_w_ratio =1;
camera_t * captured_p;
static int captured_id=0;
uint8_t rgb_bmp_data[1024*1024*3];
pipelineCTL global_pipeline_ctl;
framePipLineFunc  global_framePipLineFuncPtr;
filterParameter   global_frameFilterParam;
GLint record_width;
GLint record_height;
void init_filter_parameters()
{
    global_frameFilterParam.gammaCorrectParameters.GammaCorrect_power_level_inv = 2.2;
    global_frameFilterParam.gaussianSmoothParameters.GaussianSmooth_Padding_Mode=CONVOLUTION_PADDING_MODE_REPLICATE;
    global_frameFilterParam.gaussianSmoothParameters.GaussianSmooth_Sigma=1.3;
    global_frameFilterParam.gaussianSmoothParameters.GaussianSmooth_Kernel=5;
}
void update_zoom_ratio()
{
    zoom_w_ratio = (float)width/(float)glFrameWidthSet;
    zoom_h_ratio = (float)height/(float)glFrameHeightSet;
    record_width=width;
    record_height=height;
    #ifndef GL_DEBUG_ENABLE
    LOG_OPENGL("zoom_w_ratio = %f; zoom_h_ratio=%f;\n",zoom_w_ratio,zoom_h_ratio);
    #endif
}
void init_pixels()
{
    for(int i=0;i<glFramePixelCount;++i)
    {
        GLubyte pixel = i%255;
        pixel_array[i*3+0] = pixel;
        pixel_array[i*3+1] = 255-pixel;
        pixel_array[i*3+2] = pixel;
    }
}
void init(void)
{
    {
        record_width = glFrameWidthSet;
        record_height= glFrameHeightSet;
        glFramePixelCount = glFrameWidthSet * glFrameHeightSet;
    }
    pixel_array = (GLubyte*)malloc(glFramePixelCount*3*sizeof(uint8_t));
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
    init_pixels();
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    return;
}
void init_bmp(void)
{
    pixel_array = (GLubyte*)malloc(glFramePixelCount*3*sizeof(GLubyte));
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
   
    init_pixels();
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    return;
}
void menu_log()
{
    LOG_MENU(" *************************************%s","*************************************");
    LOG_MENU(" *************************************%s","*************************************");
    LOG_MENU(" %s   NativeDIP KeyBoard Control:     %s","******************","****************");
    LOG_MENU(" F1:    Original Frame                %s","                                     ");
    LOG_MENU(" F2:    [ISP]Gamma Correction         %s","                                     ");
    LOG_MENU("        a|A: add gamma_level_inv      %s","                                     ");
    LOG_MENU("        s|S: sub gamma_level_inv      %s","                                     ");
    LOG_MENU(" F3:    [ISP]Gaussian Smooth          %s","                                     ");
    LOG_MENU("        a|A: add kernel level step2   %s","                                     ");
    LOG_MENU("        s|S: sub kernel level step2   %s","                                     ");
    LOG_MENU("        q|Q: add gauss  sigma step.1  %s","                                     ");
    LOG_MENU("        w|W: sub gauss  sigma step.1  %s","                                     ");
    LOG_MENU(" *************************************%s","*************************************");
    LOG_MENU(" *************************************%s","*************************************");

}
void display(void)
{
    static int record_capture_id =0;
    static long record_timestamp =0;
    if(captured_id > record_capture_id)
    {
#ifdef DEBUG_LATENCY_ENABLE
    struct timeval tpstart,tpend;
    long timeuse_us;
    static long mean_latency_ms=0;
    gettimeofday(&tpstart,NULL); 
#endif
    glClear(GL_COLOR_BUFFER_BIT);
    glRasterPos2f(0,0);
    {
        frameRawData  frame_in;
        glShowDataRGB frame_out;
        frame_in.frameRawPtr = captured_p->head.start;
        frame_in.frameWidth=glFrameWidthSet;
        frame_in.frameHeight=glFrameHeightSet;
        frame_in.frameSize=glFramePixelCount*3;
        frame_in.piplineCtl= global_pipeline_ctl;
        frame_in.filterParam=global_frameFilterParam;
        global_framePipLineFuncPtr(&frame_in,&frame_out);
        glDrawPixels(frame_out.pixelWidth,frame_out.pixelHeight,GL_RGB,GL_UNSIGNED_BYTE,frame_out.rgb_data_p);
    }
    update_zoom_ratio();
    glPixelZoom(zoom_w_ratio,zoom_h_ratio);
    // glFlush();//GLUT_SINGLE
    glutSwapBuffers();
#ifdef DEBUG_LATENCY_ENABLE
    gettimeofday(&tpend,NULL); 
    timeuse_us=1000000*(tpend.tv_sec-tpstart.tv_sec) + tpend.tv_usec-tpstart.tv_usec; 
    long new_timestamp=1000000*(tpend.tv_sec)+ tpend.tv_usec;
    if(captured_id==1) {mean_latency_ms = timeuse_us;}
    mean_latency_ms = (mean_latency_ms + timeuse_us)>>1;
    LOG_OPENGL("idle run : %ld us; fps : %ld\n\n",mean_latency_ms,1000000/(new_timestamp-record_timestamp+1));
    record_timestamp=new_timestamp;
    record_capture_id=captured_id;
#endif
    }
    return;
}
void display_bmp(void)
{
    static int record_capture_id =0;
    static long record_timestamp =0;
#ifdef DEBUG_LATENCY_ENABLE
    struct timeval tpstart,tpend;
    long timeuse_us;
    static long mean_latency_ms=0;
    gettimeofday(&tpstart,NULL); 
#endif
    glClear(GL_COLOR_BUFFER_BIT);
    glRasterPos2f(-1.0f,-1.0f);
    {
        frameRawData  frame_in;
        glShowDataRGB frame_out;
        frame_in.frameRawPtr = rgb_bmp_data;
        frame_in.frameWidth=glFrameWidthSet;
        frame_in.frameHeight=glFrameHeightSet;
        frame_in.frameSize=glFramePixelCount*3;
        frame_in.piplineCtl= global_pipeline_ctl;
        frame_in.filterParam=global_frameFilterParam;
        global_framePipLineFuncPtr(&frame_in,&frame_out);
        glDrawPixels(frame_out.pixelWidth,frame_out.pixelHeight,GL_RGB,GL_UNSIGNED_BYTE,frame_out.rgb_data_p);
    }
    update_zoom_ratio();
    glPixelZoom(zoom_w_ratio,zoom_h_ratio);
    // glFlush();//GLUT_SINGLE
    glutSwapBuffers();
#ifdef DEBUG_LATENCY_ENABLE
    gettimeofday(&tpend,NULL); 
    timeuse_us=1000000*(tpend.tv_sec-tpstart.tv_sec) + tpend.tv_usec-tpstart.tv_usec; 
    long new_timestamp=1000000*(tpend.tv_sec)+ tpend.tv_usec;
    if(captured_id==1) {mean_latency_ms = timeuse_us;}
    mean_latency_ms = (mean_latency_ms + timeuse_us)>>1;
    LOG_OPENGL("idle run : %ld us; fps : %ld\n\n",mean_latency_ms,1000000/(new_timestamp-record_timestamp+1));
    record_timestamp=new_timestamp;
    record_capture_id=captured_id;
#endif
    return;
}
void reshape(int w, int h)
{
    glViewport(0,0,(GLsizei)w,(GLsizei)h);
    height = (GLint) h;
    width  = (GLint) w;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0,(GLdouble) w, 0.0,(GLdouble) h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    update_zoom_ratio();
    glPixelZoom(zoom_w_ratio,zoom_h_ratio);
}
void reshape_bmp(int w, int h)
{
    // glViewport(0,0,(GLsizei)w,(GLsizei)h);
    height = (GLint) h;
    width  = (GLint) w;
    // glMatrixMode(GL_PROJECTION);
    // glLoadIdentity();
    // gluOrtho2D(0.0,(GLdouble) w, 0.0,(GLdouble) h);
    // glMatrixMode(GL_MODELVIEW);
    // glLoadIdentity();
    update_zoom_ratio();
    glPixelZoom(zoom_w_ratio,zoom_h_ratio);
}
void idle(void)
{
    captured_p=uvc_camera_sdk_stream_captured_once();
    captured_id++;
	// LOG_OPENGL("ID[%d]: W=%d; H=%d; buf_length=%d; mean_pixel=%d\n",captured_id++,captured_p->width,captured_p->height,captured_p->head.length,meanOfBuffer(captured_p->head.start,captured_p->head.length));
    glutPostRedisplay();
    usleep(1000);
}
void idle_bmp(void)
{
    glutPostRedisplay();
    usleep(1000);
}
void free_gl_buf()
{
    if(!pixel_array)
    {
        free(pixel_array);
    }
}
void processSpecialKeys(int key, int x, int y) 
{
    switch(key) 
    {
        case GLUT_KEY_F1 :global_pipeline_ctl.step2_id=0                          ; break;
        case GLUT_KEY_F2 :global_pipeline_ctl.step2_id=ALGO_ID_RGB_GAMMA_CORRECT  ; break;
        case GLUT_KEY_F3 :global_pipeline_ctl.step2_id=ALGO_ID_RGB_GAUSSIAN_SMOOTH; break;
        default:                                                                    break;
    }
}
void processNormalKeys(unsigned char key,int x,int y)
{
    switch(global_pipeline_ctl.step2_id)
    {
        case ALGO_ID_RGB_GAMMA_CORRECT  : processNormalKeyCase_GammaCorrect(key,&(global_frameFilterParam.gammaCorrectParameters));break;
        case ALGO_ID_RGB_GAUSSIAN_SMOOTH: processNormalKeyCase_GaussSmooth(key,&(global_frameFilterParam.gaussianSmoothParameters));break;
        default:                          break;
    }
    
}

int start_gl_show_frame(int argc, char *argv[],uint32_t pixelW,uint32_t pixelH,framePipLineFunc  piplineFuncP,char device_path[20])
{
    global_framePipLineFuncPtr = piplineFuncP;
    glFrameWidthSet = pixelW;
    glFrameHeightSet = pixelH;
    int ret=uvc_camera_sdk_init(device_path,glFrameWidthSet,glFrameHeightSet,1);
	uvc_camera_sdk_stream_start(1000212);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(glFrameWidthSet/2, glFrameHeightSet/2);
    glutCreateWindow("OpenGL UVC SHOW");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(&idle);
    glutSpecialUpFunc(processSpecialKeys);
    glutKeyboardUpFunc(processNormalKeys);
    init_filter_parameters();
    menu_log();
    glutMainLoop();
    free_gl_buf();
    return 0;
}    

int start_gl_show_bmp(int argc, char *argv[], char * load_bmp_path,uint32_t pixelW,uint32_t pixelH,framePipLineFunc  piplineFuncP)
{
    global_framePipLineFuncPtr = piplineFuncP;
    BMP_DATA bmp_d;
    bmp_d.pixel_raw_array=rgb_bmp_data;
    read_bmp_file(load_bmp_path,&bmp_d);
    glFrameWidthSet = bmp_d.bmp_header_struct.bmp_info_struct.bmpInfo_width;
    glFrameHeightSet = bmp_d.bmp_header_struct.bmp_info_struct.bmpInfo_height;
    glFramePixelCount = glFrameWidthSet * glFrameHeightSet;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(glFrameWidthSet, glFrameHeightSet);
    glutCreateWindow("OpenGL BMP SHOW");
    init_bmp();
    // load_bmp_data(&bmp_d);
    glutDisplayFunc(display_bmp);
    glutReshapeFunc(reshape_bmp);
    glutIdleFunc(&idle_bmp);
    glutSpecialUpFunc(processSpecialKeys);
    glutKeyboardUpFunc(processNormalKeys);
    init_filter_parameters();
    menu_log();
    glutMainLoop();
    if(!rgb_bmp_data) free(rgb_bmp_data);
    return 0;
}

