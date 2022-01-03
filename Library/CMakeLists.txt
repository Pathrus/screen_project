set(SOURCES codec.cpp fifo.cpp receiver.cpp ScreenCapture.cpp sender.cpp socket.cpp)
set(HEADERS codec.hpp fifo.hpp receiver.hpp ScreenCapture.hpp sender.hpp socket.hpp headers.h)

#set(CMAKE_PREFIX_PATH /usr/local/lib)

#add_library(screen STATIC ${SOURCES} ${HEADERS})
add_library(screen SHARED ${SOURCES} ${HEADERS})

find_library(LIBYUV libyuv.so)
message("-- LIBYUV = ${LIBYUV}")
find_package(X11 REQUIRED)

find_package(OpenCV 4.0.0 REQUIRED)
message("OpenCV version : ${OpenCV_VERSION}")
include_directories(${OpenCV_INCLUDE_DIRS})

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

#find_library(AVCODEC avcodec)
#find_library(AVCODEC NAMES libavcodec.so PATH /usr/lib/x86_64-linux-gnu)
find_library(AVCODEC libavcodec.so)
#find_library(AVCODEC libavcodec.so /usr/local/lib)
message("AVCODEC = ${AVCODEC}")

find_package(LibLZMA REQUIRED)
if (LibLZMA_FOUND)
    include_directories(${LibLZMA_INCLUDE_DIRS})
endif()
message(STATUS "LZMA_FOUND = ${LibLZMA_FOUND}")

find_package(ZLIB REQUIRED)
if (ZLIB_FOUND)
    include_directories(${ZLIB_INCLUDE_DIRS})
endif()
message(STATUS "ZLIB_FOUND = ${ZLIB_FOUND}")

find_library(AVUTIL avutil)
find_library(AVFORMAT avformat)

#find_package(FFmpeg REQUIRED)
#if (FFmpeg_FOUND)
#    message(STATUS "FFMPEG_FOUND= ${FFmpeg_FOUND}")
#endif ()


#find_package(LIBLZMA REQUIRED)
#find_library(LIBLZMA REQUIRED)

#target_link_libraries(screen ${LIBYUV} ${X11_LIBRARIES} ${OpenCV_LIBS}  ${AVUTIL} ${AVFORMAT} ${ZLIB_LIBRARIES} ${LIBLZMA_LIBRARIES} ${AVCODEC})
#target_link_libraries(screen ${LIBYUV} ${X11_LIBRARIES} ${OpenCV_LIBS} ${AVUTIL} ${AVFORMAT} ${AVCODEC} ${ZLIB_LIBRARIES} ${LIBLZMA_LIBRARIES})

target_link_libraries(screen ${LIBYUV} ${X11_LIBRARIES} ${OpenCV_LIBS} ${AVUTIL} ${AVFORMAT} ${AVCODEC})