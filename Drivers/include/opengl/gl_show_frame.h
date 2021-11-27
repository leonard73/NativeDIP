#ifndef __GL_SHOW_FRAME_H__
#define __GL_SHOW_FRAME_H__
#include "../def/callbackDef.h"
int start_gl_show_frame(int argc, char *argv[],uint32_t pixelW,uint32_t pixelH,framePipLineFunc  piplineFuncP);
int start_gl_show_bmp(int argc, char *argv[], char * load_bmp_path,uint32_t pixelW,uint32_t pixelH,framePipLineFunc  piplineFuncP);
#endif //__GL_SHOW_FRAME_H__