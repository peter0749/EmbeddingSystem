all: detect_face_lbp detect_lab9
detect_face_lbp: detect_face_lbp.cpp 
	g++ `pkg-config --cflags opencv` detect_face_lbp.cpp -o detect_face_lbp `pkg-config --libs opencv` -O2
detect_lab9: detect_lab9.cpp 
	g++ `pkg-config --cflags opencv` detect_lab9.cpp -o detect_lab9 `pkg-config --libs opencv` -O2
clean:
	rm -f detect_face_lbp detect_lab9

