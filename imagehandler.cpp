#include "imagehandler.h"
// Using Papert contour following algorithm
// http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.83.6509&rep=rep1&type=pdf
// Part 2, Figure 2

// check if pixel black
bool imageHandler::isPixelBlack(pixel pix){
    return (img.pixelColor(pix.gety(), pix.getx()).red() == 0 &&
            img.pixelColor(pix.gety(), pix.getx()).green() == 0 &&
            img.pixelColor(pix.gety(), pix.getx()).blue() == 0
            );
}

// Search for first black pixel line by line
pixel imageHandler::firstPixel(){
    for (int height = 0; height < img.height(); height++){
        for (int width = 0; width < img.width(); width++){
            if (isPixelBlack(pixel(height, width))){
                return pixel(height, width); // coordinates of a first pixel
            }
        }
    }
    throw emptyImage();
}

// left turn, based on previous direction
pixel imageHandler::turn_left(pixel current, unsigned int prev_dir){
    switch (prev_dir){
    case 0:
        return pixel(current.getx() + 1, current.gety());
    case 1:
        return pixel(current.getx(), current.gety() - 1);
    case 2:
        return pixel(current.getx() - 1, current.gety());
    case 3:
        return pixel(current.getx(), current.gety() + 1);
    default:
        throw wrongDirection(prev_dir);
    }
}

// right turn
pixel imageHandler::turn_right(pixel current,unsigned int prev_dir){
    switch (prev_dir){
    case 0:
        return pixel(current.getx() - 1, current.gety());
    case 1:
        return pixel(current.getx(), current.gety() + 1);
    case 2:
        return pixel(current.getx() + 1, current.gety());
    case 3:
        return pixel(current.getx(), current.gety() - 1);
    default:
        throw wrongDirection(prev_dir);
    }
}

int imageHandler::euclidian(pixel first, pixel second){
    return int(ceil(qSqrt(qPow(qFabs(first.getx() - second.getx()), 2) + qPow(qFabs(first.gety() - second.gety()), 2))));
}

void imageHandler::generateVectors(){
    pixel current  = start;
    for (int i = 1; i < points.size() - 1;){
        if ((euclidian(current, points[i + 1])) < delta){
            points.erase(points.begin() + i);
        }
        else {
            current = points[i];
            i++;
        }
    }


    for (int i = 0; i < points.size() - 1; i++){
        points[i] = points[i + 1] - points[i];
    }
    points.pop_back();
}

// Go through contour and generate chain code
// Approach based on vocabulary of possible moves
void imageHandler::readStringFromImage(){
    if (true == img.isGrayscale()){ // if grayscale
        start = firstPixel();
    } else throw notGrayscale();
    chaincode.clear();
    points.clear();
    points.push_back(start);
    unsigned int prev_dir = 1;
    pixel current = start;
    pixel last_black_pixel = current;
    // using Papert's algoritm for contour approach
    // while not reached first pixel
    do
    {
        // if in a figure cell
        if (isPixelBlack(current)){
            // check for possible moves during the approach
            // and add chain code
            if (current != last_black_pixel) {
                points.push_back(current);
            }
            last_black_pixel = current; // set new last black pixel
            current = turn_left(current, prev_dir); // turn left in approach
            prev_dir = (prev_dir + 1) % 4; // update previous direction
        } else { // if in a ground cell
            current = turn_right(current, prev_dir); // turn right in approach
            if (prev_dir == 0) prev_dir = 3; // update previous direction
            else prev_dir--;
        }
    }
    while (current != start);

    generateVectors();
}

void imageHandler::writeToImageFromString(QString filename){
    QImage img(height, width, QImage::Format_RGB888);
    QRgb black(qRgb(0, 0, 0));


    img.fill(qRgb(255, 255, 255)); // fill image with white


    pixel current = start;
    img.setPixel(current.gety(), current.getx(), black);

    QPainter p (&img);
    for (int counter = 0; counter < points.size(); counter++) { // go through the string
        pixel next = current + points[counter];

        p.drawLine(current.gety(), current.getx(), next.gety(), next.getx());
        current = next;
         // paint pixels black
    }
    p.drawLine(current.gety(), current.getx(), start.gety(), start.getx());
    img.save(filename);
}

QString imageHandler::writeChaincode(){
    QString rtn = "";
    for (int counter = 0; counter < chaincode.size(); counter++){
        rtn += QString::number(chaincode[counter]);
    }
    return rtn;
}

