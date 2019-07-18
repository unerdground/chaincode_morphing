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
    for (int width = 0; width < img.width(); width++){
        for (int height = 0; height < img.height(); height++){
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

// Go through contour and generate chain code
// Approach based on vocabulary of possible moves
QVector<int> imageHandler::readStringFromImage(){
    if (true == img.isGrayscale()){ // if grayscale
        start = firstPixel();
    } else throw notGrayscale();
    chaincode.clear();
    unsigned int prev_dir = 1;
    pixel current = start;
    pixel last_black_pixel = current;
    std::string queue = "";

    // using Papert's algoritm for contour approach
    // while not reached first pixel
    do
    {
        // if in a figure cell
        if (isPixelBlack(current)){
            // check for possible moves during the approach
            // and add chain code
            if (current != last_black_pixel) {
                queue += std::to_string(prev_dir);
                if (queue == "0") chaincode.push_back(0);
                else if (queue == "1") chaincode.push_back(1);
                else if (queue == "2") chaincode.push_back(2);
                else if (queue == "3") chaincode.push_back(3);

                else if (queue == "210") chaincode.push_back(1);
                else if (queue == "103") chaincode.push_back(0);
                else if (queue == "321") chaincode.push_back(2);
                else if (queue == "032") chaincode.push_back(3);

                else if (queue == "03") {chaincode.push_back(3);
                    chaincode.push_back(0);}
                else if (queue == "01") {chaincode.push_back(1);
                    chaincode.push_back(0);}
                else if (queue == "21") {chaincode.push_back(1);
                    chaincode.push_back(2);}
                else if (queue == "23") {chaincode.push_back(3);
                    chaincode.push_back(2);}
                else if (queue == "10") {chaincode.push_back(0);
                    chaincode.push_back(1);}
                else if (queue == "13") {chaincode.push_back(3);
                    chaincode.push_back(1);}
                else if (queue == "32") {chaincode.push_back(2);
                    chaincode.push_back(3);}
                else if (queue == "30") {chaincode.push_back(0);
                    chaincode.push_back(3);}
                else throw wrongApproach(queue);
            }
            queue = ""; // reset queue
            last_black_pixel = current; // set new last black pixel
            current = turn_left(current, prev_dir); // turn left in approach
            prev_dir = (prev_dir + 1) % 4; // update previous direction
        } else { // if in a ground cell
            queue += std::to_string(prev_dir); // add move to queue
            current = turn_right(current, prev_dir); // turn right in approach
            if (prev_dir == 0) prev_dir = 3; // update previous direction
            else prev_dir--;
        }
    }
    while (current != start);

    return chaincode;
}

void imageHandler::writeToImageFromString(QString filename){
    QImage img(height, width, QImage::Format_RGB888);
    QRgb black(qRgb(0, 0, 0));


    img.fill(qRgb(255, 255, 255)); // fill image with white

    pixel current = start;
    img.setPixel(current.gety(), current.getx(), black);

    for (int counter = 0; counter < chaincode.size(); counter++) { // go through the string
        if (chaincode[counter] == 0){
            current.sety(current.gety() + 1);
        } else if (chaincode[counter] == 1){
            current.setx(current.getx() + 1);
        } else if (chaincode[counter] == 2){
            current.sety(current.gety() - 1);
        } else if (chaincode[counter] == 3){
            current.setx(current.getx() - 1);
        }
        img.setPixel(current.gety(), current.getx(), black);
         // paint pixels black
    }

    img.save(filename);
}

void imageHandler::writeChaincode(){
    for (int counter = 0; counter < chaincode.size(); counter++){
        std::cout << chaincode[counter] << "; ";
    }
}

