#include "slcreader.h"

SlcReader::SlcReader(QObject *parent) :
    QObject(parent)
{

    total_layers = 0;

    maxb=QVector2D(-99999,-99999);
    minb=QVector2D(99999,99999);
    returnpos=0;
    scalefactor=1.0;
}


void SlcReader::importSlc_firstPass(QString file)
{
    //here we will get the follow :
    //maxx,maxy,scalefactor,and returnpos
    quint8 readbyte;
    quint32 readfloat;
    float x;
    float y;
    float* pfloat;
    float layer_1_z = 0.0;
    float layer_2_z = 0.0;

    const int maxheadersize = 2048;
    int headersize = 0;

    QFile slcfile(file);
    total_layers = 0;
    if(!slcfile.open(QIODevice::ReadOnly))
    {
        //emit Sig_FileError();
        printf("Unable to open SLC file..\n");
        return;
    }
    QDataStream slcstream(&slcfile);
    QTextStream slctextstream(&slcfile);
    slcstream.setByteOrder(QDataStream::LittleEndian);
    QString headertext;
    QString fileunits;

    //Header Section:
    int unitit = 0;
    while(headersize <= maxheadersize)//readbyte != 0x1a && readbyte != 0x0d && readbyte != 0x0a)
    {
        slctextstream >> headertext;
        unitit++;
        if(headertext == "-UNIT")
        {
            slctextstream >> fileunits;
            break;
        }
        if(unitit >= 10)
        {
            break;
        }
    }

    slctextstream.device()->reset();

    while(headersize <= maxheadersize)//readbyte != 0x1a && readbyte != 0x0d && readbyte != 0x0a)
    {
        slcstream >> readbyte;
        headersize++;
        if(readbyte == 0x0d)
        {
            slcstream >> readbyte;
            headersize++;
            if(readbyte == 0x0a)
            {
                slcstream >>readbyte;
                headersize++;
                if(readbyte == 0x1a)
                {
                    break;
                }
            }
        }
    }

    //3D reserved section:
    //skip 256 bytes
    slcstream.skipRawData((256));

    //Sample table section:
    slcstream >> readbyte;
    int tableentries = (int)readbyte;
    qDebug() << "Table Entries: " << tableentries;

    for(int t = 0; t < tableentries; t++)
    {
        slcstream >> readfloat;
        slcstream >> readfloat;
        slcstream >> readfloat;
        slcstream >> readfloat;
    }

    //Contuour section:
    quint32 numboundries = 0;
    quint32 numvertices = 0;
    quint32 numgaps = 0;
    quint32 minzlevel = 0;

    if(fileunits == "INCH")
        scalefactor = 25.4;

    returnpos = slcstream.device()->pos();
    bool boundsdetermined = false;
    while(numboundries != 0xffffffff)
    {
        slcstream >> minzlevel;
        if(total_layers==1)
        {
            float* z_value;
            z_value = (float*)&minzlevel;
            layer_1_z = *z_value;
        }
        else if(total_layers==2)
        {
            float* z_value;
            z_value = (float*)&minzlevel;
            layer_2_z = *z_value;
        }


        slcstream >> numboundries;
        if(numboundries == 0xffffffff)
        {
            break;
        }
        total_layers++;
        //fill the new loops with vertices!
        for(unsigned b = 0; b < numboundries; b++)
        {
            slcstream >> numvertices;
            slcstream >> numgaps;
            for(uint v = 0; v < numvertices; v++)
            {
                slcstream >> readfloat;//x cord
                pfloat = (float*)&readfloat;
                x = *pfloat;

                slcstream >> readfloat;//y cord
                pfloat = (float*)&readfloat;
                y = *pfloat;

                if(x > maxb.x())
                    maxb.setX(x);
                if(y > maxb.y())
                    maxb.setY(y);
                if(x < minb.x())
                    minb.setX(x);
                if(y < minb.y())
                    minb.setY(y);

                boundsdetermined = true;
            }
        }
    }
    if(!boundsdetermined)
    {
        printf("Invalid File!\n");
        return;
    }
    thickness = layer_2_z - layer_1_z;
    qDebug()<<"this slc thickness is:"<<thickness;

    slcfile.close();
}
QImage SlcReader::importSlicesFromSlc(QString file, double pixelsizemicrons,int layersNu)
{
    QFile slcfile(file);
    if(!slcfile.open(QIODevice::ReadOnly))
    {
        //emit Sig_FileError();
        printf("Unable to open SLC file..\n");

    }
    QDataStream slcstream(&slcfile);
    slcstream.setByteOrder(QDataStream::LittleEndian);
    quint32 readfloat;
    quint32 numslices = 0;
    quint32 numboundries = 0;
    quint32 numvertices = 0;
    quint32 numgaps = 0;
    quint32 minzlevel = 0;

    maxb*=scalefactor;
    minb*=scalefactor;

    float x;
    float y;
    float* pfloat;
    double pixelsizemm;
    double layersizemm;

    pixelsizemm = pixelsizemicrons*0.001;
    layersizemm = pixelsizemicrons*0.001; //command line default
    slcstream.device()->seek(returnpos);
    numboundries = 0;
    QImage img((maxb - minb).x()/pixelsizemm,(maxb - minb).y()/pixelsizemm,QImage::Format_ARGB32);
    qDebug() << maxb << " " << minb;

    double x1;
    double y1;
    double x2;
    double y2;
    double Dot;
    double Cross;
    double diff;
    double totalAngle;
    numslices++;
    //SECOND PASS, actual formation.
    if(numboundries != 0xffffffff)
    {
        slcstream >> minzlevel;
//        float* z_value;
//        z_value = (float*)&minzlevel;
//        qDebug()<<"minizlevel:"<<*z_value;

        slcstream >> numboundries;

        printf("numslices == %d \n",numslices);
        QPainter painter(&img);
        painter.setCompositionMode(QPainter::CompositionMode_Exclusion);
        painter.setRenderHint(QPainter::Antialiasing,true);
        QBrush fillbrush(QColor(255,255,255));
        QBrush voidbrush(QColor(0,0,0));

        {
            img.fill(Qt::black);
        }

        for(unsigned b = 0; b < numboundries; b++)
        {
            slcstream >> numvertices;
            slcstream >> numgaps;
            QPointF* points = new QPointF[numvertices+1];

            totalAngle = 0;
            QPointF firstpoint;
            for(uint v = 0; v <= numvertices; v++)
            {
                if(v == numvertices)
                {
                    x = firstpoint.x();
                    y = firstpoint.y();
                }
                else
                {
                    slcstream >> readfloat;//x cord
                    pfloat = (float*)&readfloat;
                    x = *pfloat*scalefactor;

                    slcstream >> readfloat;//y cord
                    pfloat = (float*)&readfloat;
                    y = *pfloat*scalefactor;
                }
                if(v == 1)
                {
                    firstpoint = QPointF(x,y);
                }
                QVector2D cord(x,y);
                cord -= minb;
                cord *= (1/pixelsizemm);
                if(v >= 2)//we can use the point behind for angle counting
                {
                    x1 = points[v-1].x() - points[v-2].x();
                    y1 = points[v-1].y() - points[v-2].y();
                    x2 = cord.x() - points[v-1].x();
                    y2 = cord.y() - points[v-1].y();
                    QVector2D vec1(x1,y1); vec1.normalize();
                    QVector2D vec2(x2,y2); vec2.normalize();
                    x1 = vec1.x();
                    y1 = vec1.y();
                    x2 = vec2.x();
                    y2 = vec2.y();

                    Dot = (x1 * x2) + (y1 * y2);
                    Cross = (x1 * y2) - (y1 * x2);

                    //filter dot and cross
                    if(Dot > 1)
                        Dot = 1;
                    else if(Dot < -1)
                        Dot = -1;
                    if(Cross > 1)
                        Cross = 1;
                    else if(Cross < -1)
                        Cross = -1;

                    if(Cross >= 0)
                        diff = -acos(Dot);
                    else
                        diff = acos(Dot);

                    totalAngle = totalAngle + diff;
                }

                points[v] = QPointF(cord.x(),cord.y());
            }
            if(totalAngle > 0)
            {
                painter.setBrush(fillbrush);

            }
            else
            {
                painter.setBrush(fillbrush);

            }

            //Draw code here
            painter.drawPolygon(points,numvertices,Qt::WindingFill);

            delete[] points;
        }
        painter.end();
        img = img.mirrored(false,true);
        returnpos = slcstream.device()->pos();
        if(layersNu==1)
        {
            QImage s(img.size(),QImage::Format_ARGB32);
            s.fill(Qt::white);
            s.save("base.jpg","JPG",100);
        }

    }
    return img;
    slcfile.close();
}
