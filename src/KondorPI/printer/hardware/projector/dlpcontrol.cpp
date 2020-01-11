#include "dlpcontrol.h"

DLPcontrol::DLPcontrol()
{
    FramebufferInit();
	pWidth=0;
	pHeight=0;
}

DLPcontrol::~DLPcontrol()
{
    FreeFramebuffer();
}

void DLPcontrol::setWH(float w,float h)
{
	pWidth = w;
	pHeight = h;
}
void DLPcontrol::drallBlank()
{
    BlankDLP();
}

/**************************************************/
/*********************Private funcation*************/
/*******************operate the framebuffer********/
void DLPcontrol::PutPixel(U32 x,U32 y,U32 r,U32 g,U32 b)
{

#ifdef Q_OS_LINUX
    U8* pos= fbp + y*(2*vinfo.xres) +2*x;
    U16 c = ((r/8)*2048)+((g/4)*32)+(b/8);
    *(U16*)pos = c;
#endif
}

void DLPcontrol::BlankDLP()
{
#ifdef Q_OS_LINUX
    int x,y;
    for(y=0;y<vinfo.yres;y++)
        for(x=0;x<vinfo.xres;x++)
        {
            PutPixel(x,y,0,0,0);
        }
#endif
}

U8 DLPcontrol::FramebufferInit()
{
#ifdef Q_OS_LINUX
    long int screensize = 0;
    // Open the file for reading and writing
    fbfd = open("/dev/fb0", O_RDWR);
    if (fbfd == -1) {
      printf("Error: cannot open framebuffer device.\n");
      return 0;
    }
    printf("The framebuffer device was opened successfully.\n");

    // Get variable screen information
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo))
    {
        printf("Error reading variable information.\n");
        return 0;
    }
    printf("Original %dx%d, %dbpp\n",
            vinfo.xres, vinfo.yres,
            vinfo.bits_per_pixel );

    // Store for reset (copy vinfo to vinfo_orig)
    memcpy(&orig_vinfo, &vinfo, sizeof(struct fb_var_screeninfo));

    // Change variable info
    vinfo.bits_per_pixel = 16;
    if (ioctl(fbfd, FBIOPUT_VSCREENINFO, &vinfo))
    {
        printf("Error setting variable information.\n");
        return 0;
    }

    // Get fixed screen information
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo))
    {
        printf("Error reading fixed information.\n");
        return 0;
    }

    // map fb to user mem
    screensize = finfo.smem_len;
    printf("screensize= %d\n",screensize);
    fbp = (unsigned char*)mmap(0,
              screensize,
              PROT_READ | PROT_WRITE,
              MAP_SHARED,
              fbfd,
              0);

    if ((int)fbp == -1)
    {
        printf("Failed to mmap.\n");
        return 0;
    }
    else
    {
       return 1;
    }
#endif
}

void DLPcontrol::FreeFramebuffer()
{
#ifdef Q_OS_LINUX
      // cleanup
    // unmap fb file from memory
    long int screensize = finfo.smem_len;
    munmap(fbp, screensize);
    // reset the display mode
    if (ioctl(fbfd, FBIOPUT_VSCREENINFO, &orig_vinfo))
    {
        printf("Error re-setting variable information.\n");
    }
    // close fb file
    close(fbfd);
#endif
}
