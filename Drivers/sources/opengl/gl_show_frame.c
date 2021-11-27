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
framePipLineFunc  global_framePipLineFuncPtr;
/*********BMP********/
// void load_bmp_data(BMP_DATA * bmp_d)
// {
//     struct timeval tpstart,tpend;
//     static long timeuse_us=0,times=1;
//     // bitmap_file_header bmpFileHeader;
//     // bitmap_info_header bmpInfoHeader;
//     // readBMP("test.bmp",&bmpFileHeader,&bmpInfoHeader,testRGB888);
//     for(int i=0;i<302*452;i++)
//     {
//         pixel_array[i*3+0] = (GLubyte)rgb_bmp_data[i*3+2];
//         pixel_array[i*3+1] = (GLubyte)rgb_bmp_data[i*3+1];
//         pixel_array[i*3+2] = (GLubyte)rgb_bmp_data[i*3+0];
//     }
// }
/*********BMP********/
GLint record_width;
GLint record_height;
void update_zoom_ratio()
{
    zoom_w_ratio = (float)width/(float)glFrameWidthSet;
    zoom_h_ratio = (float)height/(float)glFrameHeightSet;
    record_width=width;
    record_height=height;
    #ifndef GL_DEBUG_ENABLE
    printf("zoom_w_ratio = %f; zoom_h_ratio=%f;\n",zoom_w_ratio,zoom_h_ratio);
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
        global_framePipLineFuncPtr(&frame_in,&frame_out);
        glDrawPixels(frame_out.pixelWidth,frame_out.pixelHeight,GL_RGB,GL_UNSIGNED_BYTE,frame_out.rgb_data_p);
    }
    update_zoom_ratio();
    glPixelZoom(zoom_w_ratio,zoom_h_ratio);
    glFlush();
#ifdef DEBUG_LATENCY_ENABLE
    gettimeofday(&tpend,NULL); 
    timeuse_us=1000000*(tpend.tv_sec-tpstart.tv_sec) + tpend.tv_usec-tpstart.tv_usec; 
    long new_timestamp=1000000*(tpend.tv_sec)+ tpend.tv_usec;
    if(captured_id==1) {mean_latency_ms = timeuse_us;}
    mean_latency_ms = (mean_latency_ms + timeuse_us)>>1;
    printf("idle run : %ld us; fps : %ld\n\n",mean_latency_ms,1000000/(new_timestamp-record_timestamp+1));
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
        global_framePipLineFuncPtr(&frame_in,&frame_out);
        glDrawPixels(frame_out.pixelWidth,frame_out.pixelHeight,GL_RGB,GL_UNSIGNED_BYTE,frame_out.rgb_data_p);
    }
    update_zoom_ratio();
    glPixelZoom(zoom_w_ratio,zoom_h_ratio);
    glFlush();
#ifdef DEBUG_LATENCY_ENABLE
    gettimeofday(&tpend,NULL); 
    timeuse_us=1000000*(tpend.tv_sec-tpstart.tv_sec) + tpend.tv_usec-tpstart.tv_usec; 
    long new_timestamp=1000000*(tpend.tv_sec)+ tpend.tv_usec;
    if(captured_id==1) {mean_latency_ms = timeuse_us;}
    mean_latency_ms = (mean_latency_ms + timeuse_us)>>1;
    printf("idle run : %ld us; fps : %ld\n\n",mean_latency_ms,1000000/(new_timestamp-record_timestamp+1));
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
	// printf("ID[%d]: W=%d; H=%d; buf_length=%d; mean_pixel=%d\n",captured_id++,captured_p->width,captured_p->height,captured_p->head.length,meanOfBuffer(captured_p->head.start,captured_p->head.length));
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
int start_gl_show_frame(int argc, char *argv[],uint32_t pixelW,uint32_t pixelH,framePipLineFunc  piplineFuncP)
{
    global_framePipLineFuncPtr = piplineFuncP;
    glFrameWidthSet = pixelW;
    glFrameHeightSet = pixelH;
    int ret=uvc_camera_sdk_init("/dev/video0",glFrameWidthSet,glFrameHeightSet,0);
	uvc_camera_sdk_stream_start(1000212);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(glFrameWidthSet/4, glFrameHeightSet/4);
    glutCreateWindow("OpenGL UVC SHOW");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(&idle);
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
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(glFrameWidthSet, glFrameHeightSet);
    glutCreateWindow("OpenGL BMP SHOW");
    init_bmp();
    // load_bmp_data(&bmp_d);
    glutDisplayFunc(display_bmp);
    glutReshapeFunc(reshape_bmp);
    glutIdleFunc(&idle_bmp);
    glutMainLoop();
    // if(!rgb_bmp_data) free(rgb_bmp_data);
    // free_gl_buf();
    return 0;
}

