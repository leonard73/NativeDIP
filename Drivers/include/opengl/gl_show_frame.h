#ifndef __GL_SHOW_FRAME_H__
#define __GL_SHOW_FRAME_H__
#include "../def/callbackDef.h"
#ifdef __DISABLE_LOG_DEBUG__
#define LOG_OPENGL(X,...) (void)0
#else
#define LOG_OPENGL(X,...) printf("[Debug]: " #X " \n",__VA_ARGS__)
#endif
#ifdef __DISABLE_LOG_MENU__
#define LOG_MENU(X,...) (void)0
#else
#define LOG_MENU(X,...) printf("[Menu]: " #X " \n",__VA_ARGS__)
#endif
int start_gl_show_frame(int argc, char *argv[],uint32_t pixelW,uint32_t pixelH,framePipLineFunc  piplineFuncP,char device_path[20]);
int start_gl_show_bmp(int argc, char *argv[], char * load_bmp_path,uint32_t pixelW,uint32_t pixelH,framePipLineFunc  piplineFuncP);
#endif //__GL_SHOW_FRAME_H__