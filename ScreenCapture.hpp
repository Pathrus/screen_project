#ifndef SCREENCAPTURE_HPP
#define SCREENCAPTURE_HPP

#include "headers.h"

class ScreenCapture
{
    public:
        ScreenCapture();
        cv::Mat GetImage();
        ~ScreenCapture();

    public :
        int width;
        int height;
        Display* display;
        Screen* screen;
};

#endif