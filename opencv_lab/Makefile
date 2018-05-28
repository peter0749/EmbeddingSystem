all: open_img open_vid 
open_img: open_img.cpp cvheader.h
	g++ `pkg-config --cflags opencv` open_img.cpp -o open_img `pkg-config --libs opencv` -g
open_vid: open_vid.cpp cvheader.h
	g++ `pkg-config --cflags opencv` open_vid.cpp -o open_vid `pkg-config --libs opencv` -g
clean:
	rm -f open_img open_vid

