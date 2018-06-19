#include <iostream>
#include <ctime>
#include <sys/time.h>
#include <cstdio>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

/** Function Headers */
float detectAndDisplay( Mat frame );

/** Global variables */
String face_cascade_name = "lbpcascade_frontalface_improved.xml"; // 這裡放你的分類器描述檔 (*.xml)
CascadeClassifier face_cascade;
string window_name = "Capture - Face detection";
RNG rng(12345);

void initinalize_gpio(void) {
	/*
	* 開啟、初始化 GPIO port
	*/
	FILE *export_io = NULL;

	export_io = fopen("/sys/class/gpio/export", "w");
	fprintf(export_io, "4");
	if (export_io!=NULL) fclose(export_io);
	export_io = NULL;

	export_io = fopen("/sys/class/gpio/export", "w");
	fprintf(export_io, "5");
	if (export_io!=NULL) fclose(export_io);
	export_io = NULL;

	export_io = fopen("/sys/class/gpio/gpio4/direction", "w");
	fprintf(export_io, "out");
	if (export_io!=NULL) fclose(export_io);
	export_io = NULL;

	export_io = fopen("/sys/class/gpio/gpio5/direction", "w");
	fprintf(export_io, "out");
	if (export_io!=NULL) fclose(export_io);
	export_io = NULL;
}

void destruct_gpio(void) {
	/*
	* 關閉, unexport GPIO port
	*/
	FILE *export_io = NULL;

	export_io = fopen("/sys/class/gpio/gpio4/value", "w");
	fprintf(export_io, "0");
	if (export_io!=NULL) fclose(export_io);
	export_io = NULL;

	export_io = fopen("/sys/class/gpio/gpio5/value", "w");
	fprintf(export_io, "0");
	if (export_io!=NULL) fclose(export_io);
	export_io = NULL;

	export_io = fopen("/sys/class/gpio/unexport", "w");
	fprintf(export_io, "4");
	if (export_io!=NULL) fclose(export_io);
	export_io = NULL;

	export_io = fopen("/sys/class/gpio/unexport", "w");
	fprintf(export_io, "5");
	if (export_io!=NULL) fclose(export_io);
	export_io = NULL;
}

void indicate_bias(float bias) {
	/*
	* 判斷人臉在左 -> GPIO 4 亮 GPIO 5 暗
	* 判斷人臉在右 -> GPIO 4 暗 GPIO 5 亮
	* 其他情況全暗
	*/
	FILE *gpio4 = NULL; 
	FILE *gpio5 = NULL; 
	gpio4 = fopen("/sys/class/gpio/gpio4/value", "w");
	gpio5 = fopen("/sys/class/gpio/gpio5/value", "w");
	if (bias<0) {
		fprintf(gpio4, "1");
		fprintf(gpio5, "0");
	} else if (bias>0) {
		fprintf(gpio4, "0");
		fprintf(gpio5, "1");
	} else {
		fprintf(gpio4, "0");
		fprintf(gpio5, "0");
	}
	if (gpio4!=NULL) fclose(gpio4); gpio4=NULL;
	if (gpio5!=NULL) fclose(gpio5); gpio5=NULL;
}

/** @function main */
int main( int argc, const char** argv ) {
	CvCapture* capture;
	Mat frame;

	//-- 1. Load the cascades
	if( !face_cascade.load( face_cascade_name ) ) { 
		printf("--(!)Error loading\n"); 
		return -1; 
	}

	initinalize_gpio(); // 初始化 GPIO

	//-- 2. Read from webcam
	capture = cvCaptureFromCAM( 0 );
	if( capture ) {
		while( true ) {
			frame = cvarrToMat(cvQueryFrame( capture ));

			//-- 3. Apply the classifier to the frame
			if( !frame.empty() ) {
				float bias = detectAndDisplay( frame ); // 找人臉偏移 x 軸正中央的位移
				indicate_bias(bias); // 輸入偏移量 判斷左右
			} else {
				printf(" --(!) No captured frame -- Break!"); 
				break; 
			}

			if( waitKey(10)>=0 ) break;
		}
	}
	destruct_gpio(); // 關閉 GPIO
	return 0;
}

/** @function detectAndDisplay */
float detectAndDisplay( Mat frame ) {
    struct timeval tv1, tv2;
    struct timezone tz1, tz2;
    const double down_scale=1.5; // 進入分類器前 downscale 多少解析度
    static double sum = 0.0;
    static double num = 0.0;
	vector<Rect> faces;
	Mat frame_gray;

	cvtColor( frame, frame_gray, CV_BGR2GRAY ); // 轉灰階
	equalizeHist( frame_gray, frame_gray ); // Histogram Equlization 

	//-- Detect faces
    gettimeofday(&tv1, &tz1);
	face_cascade.detectMultiScale( frame_gray, faces, down_scale, 2, 0, Size(10, 10) ); // 找臉
    gettimeofday(&tv2, &tz2);
    double diff = 1.0e6 * (tv2.tv_sec-tv1.tv_sec) + (tv2.tv_usec - tv1.tv_usec);
    sum += diff;
    num += 1.0;
    
    cerr << frame.cols << "x" << frame.rows << "/" << down_scale << ", " << (sum/num) << endl; // 輸出找臉時間花費

	float mean_cx = 0;
	for( size_t i = 0; i < faces.size(); ++i ) {
		float cx = faces[i].x + faces[i].width*0.5;
		float cy = faces[i].y + faces[i].height*0.5;
		mean_cx += cx;
		Point center( cx, cy );
		ellipse( frame, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
	}
	if (faces.size()>0) {
		mean_cx /= (float) faces.size();
		Point p0( mean_cx, 0.0);
		Point p1( mean_cx, (float)frame.rows );
		line( frame, p0, p1, Scalar( 255, 0, 0 ), 5, CV_AA);
	}
	//-- Show what you got
	imshow( window_name, frame );
	return (faces.size()>0) ? mean_cx - 0.5 * frame.cols : 0; // 輸出偏移量
}