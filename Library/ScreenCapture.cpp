#include "ScreenCapture.hpp"

ScreenCapture::ScreenCapture()
{
    display = XOpenDisplay(nullptr);
    screen = DefaultScreenOfDisplay(display);
    width = screen->width - screen->width%2;
    height = screen->height -  screen->height%2;
    //if(width%2)width -= width%2;
    //if(height%2)height -= height%2;
    printf("Init Screen WxH %dx%d\n", width, height);
}

cv::Mat ScreenCapture::GetImage()
{
    //XImage* img = XGetImage(display, DefaultRootWindow(display), 0, 0, width, height, AllPlanes, ZPixmap);
    //cv::Mat tmp(height, width, CV_8UC4,  img->data);
    cv::Mat tmp(height, width, CV_8UC4, cv::Scalar(128,128,128));
    return tmp;
}

int ScreenCapture::getWidth(){ return width;}
int ScreenCapture::getHeight(){ return height;}

ScreenCapture::~ScreenCapture()
{
    XCloseDisplay(display);
}
