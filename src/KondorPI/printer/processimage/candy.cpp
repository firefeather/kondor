#include "candy.h"

Candy::Candy(QObject *parent) :
    QObject(parent)
{
}

//返回图片的边缘用于显示
QImage Candy::createToverMap3(QImage src)
{
    int width = src.width();
    int height = src.height();
    QRgb *pixels = (QRgb *)src.scanLine(0);
    double dBlobVal;
    QImage back(width,height,QImage::Format_ARGB32);
    back.fill(QColor(255,0,0));
    QRgb *backPic = (QRgb *)back.scanLine(0);
    QRgb thread = qRgb(200,200,200);
    QRgb zero = qRgb(0,0,0);
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            dBlobVal = 0;
            if((pixels[x + y*width])>=thread)
            { //we only care about set pixels...
                ///////////////////
                // R1 pattern
                //(-1,-1)
                if(!(x<1       ||        y<1)) if((pixels[(x - 1) + (y - 1)*width])==zero) dBlobVal += 4.285122492;
                //( 0,-1)
                if(!(                    y<1)) if((pixels[(x    ) + (y - 1)*width])==zero) dBlobVal += 3.030039172;
                //( 1,-1)
                if(!(x>=width-1||        y<1)) if((pixels[(x + 1) + (y - 1)*width])==zero) dBlobVal += 4.285122492;

                //(-1,0)
                if(!(x<1                    )) if((pixels[(x - 1) + (y    )*width])==zero) dBlobVal += 3.030039172;
                //( 1,0)
                if(!(x>=width-1             )) if((pixels[(x + 1) + (y    )*width])==zero) dBlobVal += 3.030039172;

                //(-1, 1)
                if(!(x<1       ||y>=height-1)) if((pixels[(x - 1) + (y + 1)*width])==zero) dBlobVal += 4.285122492;
                //( 0, 1)
                if(!(            y>=height-1)) if((pixels[(x    ) + (y + 1)*width])==zero) dBlobVal += 3.030039172;
                //( 1, 1)
                if(!(x>=width-1||y>=height-1)) if((pixels[(x + 1) + (y + 1)*width])==zero) dBlobVal += 4.285122492;

                ///////////////////////
                // R2
                //(-1,-2)
                if(!(x<1       ||        y<2)) if((pixels[(x - 1) + (y - 2)*width])==zero) dBlobVal += 6.775373563;
                //( 0,-2)
                if(!(                    y<2)) if((pixels[(x    ) + (y - 2)*width])==zero) dBlobVal += 6.060078344;
                //( 1,-2)
                if(!(x>=width-1||        y<2)) if((pixels[(x + 1) + (y - 2)*width])==zero) dBlobVal += 6.775373563;


                //(-2,-1)
                if(!(x<2       ||       y <1)) if((pixels[(x - 2) + (y - 1)*width])==zero) dBlobVal += 6.775373563;
                //(-2,0)
                if(!(x<2                    )) if((pixels[(x - 2) + (y    )*width])==zero) dBlobVal += 6.060078344;
                //(-2,1)
                if(!(x<2       ||y>=height-1)) if((pixels[(x - 2) + (y + 1)*width])==zero) dBlobVal += 6.775373563;

                //(2,-1)
                if(!(x>=width-2||       y <1)) if((pixels[(x + 2) + (y - 1)*width])==zero) dBlobVal += 6.775373563;
                //(2,0)
                if(!(x>=width-2             )) if((pixels[(x + 2) + (y    )*width])==zero) dBlobVal += 6.060078344;
                //(2,1)
                if(!(x>=width-2||y>=height-1)) if((pixels[(x + 2) + (y + 1)*width])==zero) dBlobVal += 6.775373563;


                //(-1, 2)
                if(!(x<1       ||y>=height-2)) if((pixels[(x - 1) + (y + 2)*width])==zero) dBlobVal += 6.775373563;
                //( 0, 2)
                if(!(            y>=height-2)) if((pixels[(x    ) + (y + 2)*width])==zero) dBlobVal += 6.060078344;
                //( 1, 2)
                if(!(x>=width-1||y>=height-2)) if((pixels[(x + 1) + (y + 2)*width])==zero) dBlobVal += 6.775373563;

                //(-2,-2)
                if(!(x<2       ||        y<2)) if((pixels[(x - 2) + (y - 2)*width])==zero) dBlobVal += 8.570244983;
                //( 2,-2)
                if(!(x>=width-2||        y<2)) if((pixels[(x + 2) + (y - 2)*width])==zero) dBlobVal += 8.570244983;
                //(-2, 2)
                if(!(x<2       ||y>=height-2)) if((pixels[(x - 2) + (y + 2)*width])==zero) dBlobVal += 8.570244983;
                //( 2, 2)
                if(!(x>=width-2||y>=height-2)) if((pixels[(x + 2) + (y + 2)*width])==zero) dBlobVal += 8.570244983;

                ////////////////////////////////
                // R3
                //(-1,-3)
                if(!(x<1       ||        y<3)) if((pixels[(x - 1) + (y - 3)*width])==zero) dBlobVal += 9.581825183;
                //( 0,-3)
                if(!(                    y<3)) if((pixels[(x    ) + (y - 3)*width])==zero) dBlobVal += 9.090117516;
                //( 1,-3)
                if(!(x>=width-1||        y<3)) if((pixels[(x + 1) + (y - 3)*width])==zero) dBlobVal += 9.581825183;


                //(-3,-1)
                if(!(x<3       ||       y <1)) if((pixels[(x - 3) + (y - 1)*width])==zero) dBlobVal += 9.581825183;
                //(-3,0)
                if(!(x<3                    )) if((pixels[(x - 3) + (y    )*width])==zero) dBlobVal += 9.090117516;
                //(-3,1)
                if(!(x<3       ||y>=height-1)) if((pixels[(x - 3) + (y + 1)*width])==zero) dBlobVal += 9.581825183;

                //(3,-1)
                if(!(x>=width-3||       y <1)) if((pixels[(x + 3) + (y - 1)*width])==zero) dBlobVal += 9.581825183;
                //(3,0)
                if(!(x>=width-3             )) if((pixels[(x + 3) + (y    )*width])==zero) dBlobVal += 9.090117516;
                //(3,1)
                if(!(x>=width-3||y>=height-1)) if((pixels[(x + 3) + (y + 1)*width])==zero) dBlobVal += 9.581825183;


                //(-1, 3)
                if(!(x<1       ||y>=height-3)) if((pixels[(x - 1) + (y + 3)*width])==zero) dBlobVal += 9.581825183;
                //( 0, 2)
                if(!(            y>=height-3)) if((pixels[(x    ) + (y + 3)*width])==zero) dBlobVal += 9.090117516;
                //( 1, 2)
                if(!(x>=width-1||y>=height-3)) if((pixels[(x + 1) + (y + 3)*width])==zero) dBlobVal += 9.581825183;

                if(dBlobVal > 5.0)
                {
                    backPic[x + y*width] = qRgb(255,255,255);

                }
                else
                {
                    backPic[x + y*width] = qRgb(0,0,0);
                }
            }
            else
            {
                backPic[x + y*width] = qRgb(0,0,0);
            }
        }
    }

    return back;
}
