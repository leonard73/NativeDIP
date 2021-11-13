#include <GL/glut.h>
#include <stdio.h>
#include <sys/time.h>
// #include <bmp.h>
#include "../../include/driverInc.h"
#define TEST_PIXEL_WIDTH  (640)
#define TEST_PIXEL_HEIGHT (480)
#define TEST_PIXEL_NUM    (TEST_PIXEL_WIDTH*TEST_PIXEL_HEIGHT)
GLubyte pixel_array[TEST_PIXEL_NUM*3];
GLint width=TEST_PIXEL_WIDTH,height=TEST_PIXEL_HEIGHT;
float zoom_h_ratio =1;
float zoom_w_ratio =0.5;
camera_t * captured_p;
static int captured_id=0;
/*********BMP********/
unsigned char * testRGB888;
void load_bmp_data()
{
    struct timeval tpstart,tpend;
    static long timeuse_us=0,times=1;
    testRGB888 = malloc(TEST_PIXEL_NUM*3);
    // bitmap_file_header bmpFileHeader;
    // bitmap_info_header bmpInfoHeader;
    // readBMP("test.bmp",&bmpFileHeader,&bmpInfoHeader,testRGB888);
    for(int i=0;i<TEST_PIXEL_NUM*3;i++)
    {
        pixel_array[i] = testRGB888[i];
    }
}
/*********BMP********/
void update_zoom_ratio()
{
    static GLint record_width=TEST_PIXEL_WIDTH;
    static GLint record_height=TEST_PIXEL_HEIGHT;
    zoom_w_ratio = (float)width/(float)record_width;
    zoom_h_ratio = (float)height/(float)record_height;
    // record_width=width;
    // record_height=height;
    #ifdef GL_DEBUG_ENABLE
    printf("zoom_w_ratio = %f; zoom_h_ratio=%f;\n",zoom_w_ratio,zoom_h_ratio);
    #endif
}
void init_pixels()
{
    for(int i=0;i<TEST_PIXEL_NUM;++i)
    {
        GLubyte pixel = i%255;
        pixel_array[i*3+0] = pixel;
        pixel_array[i*3+1] = 255-pixel;
        pixel_array[i*3+2] = pixel;
    }
}
void init(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
    init_pixels();
    load_bmp_data();
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
    loadStreamYuyv2RGBA((uint8_t*)pixel_array,captured_p->head.start,TEST_PIXEL_WIDTH,TEST_PIXEL_HEIGHT);
    // loadStreamY82RGBA(captured_p->head.start,TEST_PIXEL_WIDTH,TEST_PIXEL_HEIGHT);
    glDrawPixels(TEST_PIXEL_WIDTH,TEST_PIXEL_HEIGHT,GL_RGB,GL_UNSIGNED_BYTE,pixel_array);
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

void idle(void)
{

    captured_p=uvc_camera_sdk_stream_captured_once();
    captured_id++;
	// printf("ID[%d]: W=%d; H=%d; buf_length=%d; mean_pixel=%d\n",captured_id++,captured_p->width,captured_p->height,captured_p->head.length,meanOfBuffer(captured_p->head.start,captured_p->head.length));
    glutPostRedisplay();
    usleep(1000);
}
int start_gl_show_frame(int argc, char *argv[])
{
    int ret=uvc_camera_sdk_init("/dev/video0",TEST_PIXEL_WIDTH,TEST_PIXEL_HEIGHT,0);
	uvc_camera_sdk_stream_start(1000212);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(TEST_PIXEL_WIDTH/4, TEST_PIXEL_HEIGHT/4);
    glutCreateWindow("OpenGL HelloWorld");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(&idle);
    glutMainLoop();
    return 0;
}
int start_gl_show_bmp(int argc, char *argv[], char * load_bmp_path)
{
    int ret=uvc_camera_sdk_init("/dev/video0",TEST_PIXEL_WIDTH,TEST_PIXEL_HEIGHT,0);
	uvc_camera_sdk_stream_start(1000212);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(TEST_PIXEL_WIDTH/4, TEST_PIXEL_HEIGHT/4);
    glutCreateWindow("OpenGL HelloWorld");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(&idle);
    glutMainLoop();
    return 0;
}

