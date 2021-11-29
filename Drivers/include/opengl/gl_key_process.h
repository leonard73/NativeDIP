#ifndef __GL_KEY_PROCESS_H__
#define __GL_KEY_PROCESS_H__
#ifdef __DISABLE_LOG_KEYBOARD_RESPONSE__
#define LOG_KEYBOARD_RESPONSE(X,...) (void)0
#else
#define LOG_KEYBOARD_RESPONSE(X,...) fflush(stdout);printf("%s[Key Res]: " #X " \r","                                                    ",__VA_ARGS__)
#endif
void processNormalKeyCase_GammaCorrect(unsigned char key,gammaCorrectParam * gammaParam_p);
void processNormalKeyCase_GaussSmooth(unsigned char key,gaussianSmoothParam * gaussParam_p);
#endif  //__GL_KEY_PROCESS_H__