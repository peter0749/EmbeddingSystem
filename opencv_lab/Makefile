all: open_img open_vid process_img process_vid detector detector_webcam detect_face_lbp optical_flow detect_lab9
open_img: open_img.cpp cvheader.h 
	g++ `pkg-config --cflags opencv` open_img.cpp -o open_img `pkg-config --libs opencv` -O2
process_img: process_img.cpp cvheader.h oCLAHE.o oCLAHE.h
	g++ `pkg-config --cflags opencv` process_img.cpp oCLAHE.o -o process_img `pkg-config --libs opencv` -O2
open_vid: open_vid.cpp cvheader.h
	g++ `pkg-config --cflags opencv` open_vid.cpp -o open_vid `pkg-config --libs opencv` -O2
process_vid: process_vid.cpp cvheader.h oCLAHE.o oCLAHE.h
	g++ `pkg-config --cflags opencv` process_vid.cpp oCLAHE.o -o process_vid `pkg-config --libs opencv` -O2
detector: detector.cpp cvheader.h oCLAHE.o oCLAHE.h
	g++ `pkg-config --cflags opencv` detector.cpp oCLAHE.o -o detector `pkg-config --libs opencv` -O2
detector_webcam: detector_webcam.cpp cvheader.h oCLAHE.o oCLAHE.h
	g++ `pkg-config --cflags opencv` detector_webcam.cpp oCLAHE.o -o detector_webcam `pkg-config --libs opencv` -O2
detect_face_lbp: detect_face_lbp.cpp cvheader.h
	g++ `pkg-config --cflags opencv` detect_face_lbp.cpp -o detect_face_lbp `pkg-config --libs opencv` -O2
detect_lab9: detect_lab9.cpp cvheader.h
	g++ `pkg-config --cflags opencv` detect_lab9.cpp -o detect_lab9 `pkg-config --libs opencv` -O2
optical_flow: optical_flow.cpp 
	g++ `pkg-config --cflags opencv` optical_flow.cpp -o optical_flow `pkg-config --libs opencv` -O2
oCLAHE.o: oCLAHE.cpp oCLAHE.h cvheader.h
	g++ -w -c `pkg-config --cflags opencv` oCLAHE.cpp -o oCLAHE.o `pkg-config --libs opencv` -O2
clean:
	rm -f open_img open_vid oCLAHE.o process_img process_vid detector detector_webcam detect_face_lbp optical_flow detect_lab9

