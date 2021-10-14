#ifndef SCREENCAPTURE_HPP
#define SCREENCAPTURE_HPP

#include "headers.h"

class ScreenCapture
{
    public:
        ScreenCapture();
        cv::Mat GetImage();
        ~ScreenCapture();

        int getWidth();
        int getHeight();

    public :
        int width, height;
        //int height;
        Display* display;
        Screen* screen;
};

#endif