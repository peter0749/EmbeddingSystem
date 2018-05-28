all: open_img open_vid process_img process_vid
open_img: open_img.cpp cvheader.h 
	g++ `pkg-config --cflags opencv` open_img.cpp -o open_img `pkg-config --libs opencv` -g
process_img: process_img.cpp cvheader.h oCLAHE.o oCLAHE.h
	g++ `pkg-config --cflags opencv` process_img.cpp oCLAHE.o -o process_img `pkg-config --libs opencv` -g
open_vid: open_vid.cpp cvheader.h
	g++ `pkg-config --cflags opencv` open_vid.cpp -o open_vid `pkg-config --libs opencv` -g
process_vid: process_vid.cpp cvheader.h oCLAHE.o oCLAHE.h
	g++ `pkg-config --cflags opencv` process_vid.cpp oCLAHE.o -o process_vid `pkg-config --libs opencv` -g
oCLAHE.o: oCLAHE.cpp oCLAHE.h cvheader.h
	g++ -w -c `pkg-config --cflags opencv` oCLAHE.cpp -o oCLAHE.o `pkg-config --libs opencv` -g
clean:
	rm -f open_img open_vid oCLAHE.o process_img process_vid

