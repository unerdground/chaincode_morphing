#ifndef CUSTOM_EXCEPTIONS_H
#define CUSTOM_EXCEPTIONS_H
#include <QException>

class notGrayscale : public QException{
    virtual const char* what() const throw()
      {
        return "Image is not grayscale;";
      }
};

class unknownColor : public QException{
private:
    std::string error_msg;
public:
    unknownColor(int x, int y): error_msg("Unknown color at " + std::to_string(x) + ", " + std::to_string(y)){}

    virtual const char* what() const throw()
      {
        return error_msg.c_str();
      }
};

class wrongDirection : public QException{
private:
    std::string error_msg;
public:
    wrongDirection(unsigned int dir): error_msg("Unknown direction: " + std::to_string(dir)){}

    virtual const char* what() const throw()
      {
        return error_msg.c_str();
      }
};

class emptyImage : public QException{
    virtual const char* what() const throw()
      {
        return "Image contains no shapes;";
      }
};

class noImage : public QException{
private:
    std::string error_msg;
public:
    noImage(std::string filename): error_msg("Image not found: " + filename){}

    virtual const char* what() const throw()
      {
        return error_msg.c_str();
      }
};

class wrongApproach : public QException{
private:
    std::string error_msg;
public:
    wrongApproach(std::string queue): error_msg("Unknown approach: " + queue){}

    virtual const char* what() const throw()
      {
        return error_msg.c_str();
      }
};

class equalChainCode : public QException{
    virtual const char* what() const throw()
      {
        return "Shapes are equal;";
      }
};

#endif // CUSTOM_EXCEPTIONS_H
