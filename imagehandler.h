#ifndef IMAGEHANDLER_H
#define IMAGEHANDLER_H

#include <QtGui/QImage>
#include <QtGui/QColor>
#include <QVector>
#include <QPair>
#include <iostream>

#include "pixel.h"

class imageHandler
{
private:
    QImage img; // image to work with
    int height;
    int width;
    QVector<int> chaincode; // chain code of image
    pixel start;
public:
    // check if black
    bool isPixelBlack(pixel pix);
    // find first black pixel in the image
    pixel firstPixel();

    // turn right or left during contour approach
    pixel turn_left(pixel current, unsigned int prev_dir);
    pixel turn_right(pixel current, unsigned int prev_dir);

    // generate chain code of img contour
    QVector<int> readStringFromImage();
    // draw image from chain code
    void writeToImageFromString(QString filename);

    // print codes to console
    void writeChaincode();

    // return chain code
    QVector<int> getChaincode(){
        return chaincode;
    }
    // sets new chaincode
    void setChaincode(QVector<int> new_chaincode){
        chaincode = new_chaincode;
    }

    // image size comparison
    bool isSizeEqual(const imageHandler& second){
        return (height == second.height && width == second.width);
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
};

#endif // IMAGEHANDLER_H