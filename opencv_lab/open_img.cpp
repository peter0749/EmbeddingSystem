#include "cvheader.h"

int main(const int argc, const char **argv) {
    if (argc<2) {
        cerr << "usage: " << argv[0] << " your_image" << endl;
        exit(1);
    }
    IplImage *pImage = NULL, *pSmallImage = NULL;
    const char *test = argv[1];
    const char *window_name_1 = "Original";
    const char *window_name_2 = "Resized";
    const int width = 250;
    const int height= 350;
    const int crop_x= 25;
    const int crop_y= 100;
    const int crop_w= 100;
    const int crop_h= 100;
    CvPoint vertexUL, vertexDR;
    CvScalar color;
    const int thickness = 5;

    // Read test.jpg image into pImage
    pImage = cvLoadImage(test); 
    if (!pImage) {
        cerr << "Cannot open " << argv[1] << endl;
        exit(2);
    }
    pSmallImage = cvCreateImage(cvSize(width,height), pImage->depth, pImage->nChannels);
    cvResize(pImage, pSmallImage, CV_INTER_AREA);
    vertexUL = cvPoint(crop_x, crop_y);
    vertexDR = cvPoint(min(crop_x+crop_w, width-1), min(crop_y+crop_h, height-1));

    color = CV_RGB(255,0,0);
    cvRectangle(pSmallImage, vertexUL, vertexDR, color, thickness, CV_AA, 0);
    // Create a named window
    cvNamedWindow(window_name_1, 1); 
    // Display the image
    cvShowImage(window_name_1, pImage);

    cvNamedWindow(window_name_2, 1); 
    cvShowImage(window_name_2, pSmallImage);

    // Wait user to close the window
    cvWaitKey(0);
    // Destroy opened window
    cvDestroyWindow(window_name_2);
    cvWaitKey(0);
    cvDestroyWindow(window_name_1);
    // Release resources
    cvReleaseImage(&pImage);
    cvReleaseImage(&pSmallImage);
    return 0;
}
