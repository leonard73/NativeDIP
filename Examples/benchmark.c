#include <driverInc.h>

void uvc_frame_display(int argc, char *argv[])
{
    start_gl_show_frame(argc,argv,640,480);
}
void bmp_frame_display(int argc, char *argv[])
{
    start_gl_show_bmp(argc, argv,"splash_red100.bmp",640,480);
}
int  main(int argc, char *argv[])
{
    switch(atoi(argv[1]))
    {
        case 1:bmp_frame_display(argc, argv);break;
        case 2:uvc_frame_display(argc, argv);break;
        default:break;
    }
}
