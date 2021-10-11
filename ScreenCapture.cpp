#include "ScreenCapture.hpp"

ScreenCapture::ScreenCapture()
{
    display = XOpenDisplay(nullptr);
    screen = DefaultScreenOfDisplay(display);
    width = this->screen->width;
    height = this->screen->height;
    if(width%2)width -= width%2;
    if(height%2)height -= height%2;
}

cv::Mat ScreenCapture::GetImage()
{
    XImage* img = XGetImage(display, DefaultRootWindow(display), 0, 0, width, height, AllPlanes, ZPixmap);
    cv::Mat tmp(height, width, CV_8UC4,  img->data);
    return tmp;
}

ScreenCapture::~ScreenCapture()
{
    XCloseDisplay(display);
}
