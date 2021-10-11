# image ubuntu 20.04 avec opencv et ffmpeg
FROM borda/docker_python-opencv-ffmpeg

ARG DEBIAN_FRONTEND=noninteractive

ENV DISPLAY=:0
ARG LD_LIBRARY_PATH=/lib/x86_64-linux-gnu/

# Use bash shell
RUN rm /bin/sh && ln -s /bin/bash /bin/sh

# Update apt and install dependances
RUN apt-get update && apt-get install  -y \
git \
cmake \
build-essential \
g++ \
pkg-config

RUN ln -s /usr/local/include/opencv4 /usr/include/opencv4

WORKDIR exos
ENV PKG_CONFIG_PATH=/exos/

RUN apt-get install libavcodec-dev libavformat-dev -y
RUN apt-get install x11-apps -y

COPY ./headers.h ./*.cpp ./*.hpp ./Makefile ./opencv.pc ./

RUN make exo1 exo2
