#include "cvheader.h"
#include "oCLAHE.h"

int main(const int argc, const char **argv) {
    if (argc<2) {
        cerr << "usage: " << argv[0] << " your_image" << endl;
        exit(1);
    }
    const char *test = argv[1];
    const char *window_name_1 = "Original";
    const char *window_name_2 = "Resized";
    const char *window_name_3 = "EQ";
    const float crop_x= 0.01;
    const float crop_y= 0.35;
    const float crop_w= 0.98;
    const float crop_h= 0.45;
    const float zoom_ratio = 0.7;
    const CvScalar color = CV_RGB(255,0,0);
    const int thickness = 5;
    IplImage *pImage = NULL, *pSmallImage = NULL, *pEQ = NULL;
    CvCapture *pCapture = NULL;
    CvPoint vertexUL, vertexDR;

    // Read from a video file
    pCapture = cvCaptureFromFile(test);
    if (!pCapture) {
        cerr << "Cannot open " << argv[1] << endl;
        exit(2);
    }
    
    // const int pCapture_frame_n = cvGetCaptureProperty(pCapture, CV_CAP_PROP_POS_FRAME_COUNT);
    // const int pCapture_width = cvGetCaptureProperty(pCapture, CV_CAP_PROP_FRAME_WIDTH); // assume that all frames have same shape
    // const int pCapture_height = cvGetCaptureProperty(pCapture, CV_CAP_PROP_FRAME_HEIGHT);
    // const int new_width  = (int) ceil(pCapture_width  * zoom_ratio);
    // const int new_height = (int) ceil(pCapture_height * zoom_ratio);

    cvNamedWindow(window_name_1, 1); 
    cvNamedWindow(window_name_2, 1); 
    cvNamedWindow(window_name_3, 1); 
    while ((pImage = cvQueryFrame(pCapture)) != NULL) {
        int new_width   = (int) ceil(pImage->width  * zoom_ratio); // dynamic adjust screen size
        int new_height  = (int) ceil(pImage->height * zoom_ratio);
        int crop_x0_int = (int) (new_width * crop_x);
        int crop_x1_int = min((int) ceil(crop_x0_int + new_width * crop_w), new_width-1);
        int crop_y0_int = (int) (new_height * crop_y);
        int crop_y1_int = min((int) ceil(crop_y0_int + new_height * crop_h), new_height-1);

        pSmallImage = cvCreateImage(cvSize(new_width,new_height), pImage->depth, pImage->nChannels);
        cvResize(pImage, pSmallImage, CV_INTER_AREA);

        pEQ = oCLAHE(pSmallImage);

        vertexUL = cvPoint(crop_x0_int, crop_y0_int);
        vertexDR = cvPoint(crop_x1_int, crop_y1_int);

        cvRectangle(pSmallImage, vertexUL, vertexDR, color, thickness, CV_AA, 0);

        cvShowImage(window_name_1, pImage);
        cvShowImage(window_name_2, pSmallImage);
        cvShowImage(window_name_3, pEQ);

        if (pEQ!=NULL) { 
            cvReleaseImage(&pEQ);
            pEQ = NULL;
        }
        if (cvWaitKey(10)>=0) break; // Long press to break
    }

    cvDestroyWindow(window_name_3);
    cvDestroyWindow(window_name_2);
    cvDestroyWindow(window_name_1);
    // Release resources
    if (pSmallImage!=NULL) cvReleaseImage(&pSmallImage);
    if (pCapture!=NULL) cvReleaseCapture(&pCapture);
    pEQ = pImage = pSmallImage = NULL;
    pCapture = NULL;
    return 0;
}
