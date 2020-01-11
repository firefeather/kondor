#ifndef DLPCONTROL_H
#define DLPCONTROL_H

#include <QObject>
#include <QImage>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>


#ifdef Q_OS_LINUX
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#endif

typedef unsigned char U8;
typedef unsigned short U16;
typedef unsigned int U32;

class DLPcontrol
{
public:
    DLPcontrol();
    ~DLPcontrol();
    void drallBlank();
	void setWH(float,float);
	void FreeFramebuffer();
	void PutPixel(U32 x,U32 y,U32 r,U32 g,U32 b);
private:
    //for framebuffer
    void BlankDLP();
    U8 FramebufferInit();
    

//    CrushedPrintJob *mpCPJ;
    QImage mCurImage;

    //for get the framebuffer
    int fbfd ;
	float pWidth;
	float pHeight;
#ifdef Q_OS_LINUX
    struct fb_var_screeninfo orig_vinfo;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
#endif
    U8 *fbp;        //fbp was the framebuffer which need to write
};

#endif // DLPCONTROL_H
