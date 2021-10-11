all :
	exo1 exo2
	
exo2 :
	g++ codec.cpp ScreenCapture.cpp fifo.cpp sender.cpp receiver.cpp exo2.cpp -pthread -lrt -L/usr/lib/x86_64-linux-gnu/ -lavcodec -lavutil -lopencv_imgcodecs -I/usr/local/include/opencv4/opencv2/include/ `pkg-config --cflags --libs opencv` -lX11 -w -o exo2

exo1:
	g++ exo1.cpp codec.cpp ScreenCapture.cpp -L/usr/lib/x86_64-linux-gnu/ -lavcodec -lavutil -lopencv_imgcodecs -I/usr/local/include/opencv4/opencv2/include/ `pkg-config --cflags --libs opencv` -lX11 -w -o exo1