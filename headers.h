#ifndef HEADERS_H
#define HEADERS_H

#include "stdio.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <mutex>
#include <thread>

// for screen shot
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <signal.h>

#include "libyuv.h"
#include "libyuv/convert.h"

extern "C" {

}
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>



extern bool running;

#endif