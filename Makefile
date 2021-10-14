all :
	exo1 exo2
	
display_com :
	g++ codec.cpp ScreenCapture.cpp fifo.cpp sender.cpp receiver.cpp screen_communication.cpp -lyuv -pthread -lrt -L/usr/lib/x86_64-linux-gnu/ -lavcodec -lavutil -lopencv_imgcodecs -I/usr/local/include/opencv4/opencv2/include/ `pkg-config --cflags --libs opencv` -lX11 -w -o screen_comm

display:
	g++ -L/usr/local/lib/ ScreenCapture.cpp codec.cpp  screen_save.cpp -lyuv -L/usr/lib/x86_64-linux-gnu/ -lavcodec -lavutil -lopencv_imgcodecs -I/usr/local/include/opencv4/opencv2/include/ `pkg-config --cflags --libs opencv` -lX11 -w -o screen_save