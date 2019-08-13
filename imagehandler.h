#ifndef IMAGEHANDLER_H
#define IMAGEHANDLER_H

#include <QtGui/QImage>
#include <QPainter>
#include <QtGui/QColor>
#include <QVector>
#include <QPair>
#include <iostream>
#include <QtMath>

#include "pixel.h"

class imageHandler
{
private:
    QImage img; // image to work with
    int height;
    int width;
    int delta = 13;
    QVector<int> chaincode; // chain code of image
    QVector<pixel> points;
    pixel start;
public:
    // check if black
    bool isPixelBlack(pixel pix);
    // find first black pixel in the image
    pixel firstPixel();

    QVector<pixel> getVectors(){
        return points;
    }

    // turn right or left during contour approach
    pixel turn_left(pixel current, unsigned int prev_dir);
    pixel turn_right(pixel current, unsigned int prev_dir);

    // generate chain code of img contour
    void readStringFromImage();
    // draw image from chain code
    void writeToImageFromString(QString filename);

    void generateVectors();
    int euclidian(pixel first, pixel second);
    // print codes to console
    QString writeChaincode();

    // return chain code
    QVector<int> getChaincode(){
        return chaincode;
    }
    // sets new chaincode
    void setChaincode(QVector<int> new_chaincode){
        this->chaincode = new_chaincode;
    }

    void setVectors(QVector<pixel> new_vector){
        points = new_vector;
    }

    // constructor
    imageHandler(QString filename){
        img.load(filename);
        // if image exists
        // get size and generate chain code
        if (not img.isNull()){
        width = img.width();
        height = img.height();
        } else throw noImage(filename.toLocal8Bit().constData());
    }

    imageHandler(int i_h, int i_w){ height = i_h; width = i_w;}
};

#endif // IMAGEHANDLER_H
