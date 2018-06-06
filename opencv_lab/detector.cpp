#include "cvheader.h"
#include "oCLAHE.h"
#include <cassert>

static CvMemStorage *storage=0;
static CvHaarClassifierCascade *cascade=0;
CvSeq* detect(IplImage *roi);
void draw(IplImage *img, CvSeq *ped);

CvSeq* detect(IplImage *roi) {
    CvSeq *ped;
    cvClearMemStorage(storage);
    cascade = (CvHaarClassifierCascade*) cvLoad("./998_763_14_35.xml", 0, 0, 0);
    if (cascade) 
        ped = cvHaarDetectObjects(roi, cascade, storage, 1.1, 2, 0, cvSize(14,35));
    return ped;
}

void draw (IplImage *img, CvSeq *ped) {
    double scale=1;
    for (int i=0; i<(ped?ped->total:0); ++i) {
        CvRect *r = (CvRect*)cvGetSeqElem(ped, i);
        cvRectangle(img, \
        cvPoint((int)((r->x)*scale), (int)((r->y)*scale)), \
        cvPoint((int)((r->x+r->width)*scale), (int)((r->y+r->height)*scale)), \
        CV_RGB(255,0,0), 2, 8, 0);
    }
    return;
}

int main(int argc, char **argv) {
    if (argc<3) return -1;
    const char *testvideo = argv[1];
    const char *outvideo = argv[2];
    storage = cvCreateMemStorage(0);
    CvSeq *ped;
    IplImage *pImage = NULL;
    CvCapture *pCapture = NULL;
    CvSize dst_size;
    IplImage *GImage = NULL;
    IplImage *pHistEq = NULL;
    int nFrmNum = 0;
    int fps, frameH, frameW, fourcc;
    CvVideoWriter *writer;
    pCapture = cvCaptureFromFile(testvideo);
    if (!pCapture) return -2;
    cvNamedWindow("WinPlayer", 1);

    while (pImage=cvQueryFrame(pCapture)) {
        ++nFrmNum;
        if (nFrmNum==1) {
            fps = cvGetCaptureProperty(pCapture, CV_CAP_PROP_FPS);
            frameH = cvGetCaptureProperty(pCapture, CV_CAP_PROP_FRAME_HEIGHT);
            frameW = cvGetCaptureProperty(pCapture, CV_CAP_PROP_FRAME_WIDTH);
            fourcc = cvGetCaptureProperty(pCapture, CV_CAP_PROP_FOURCC);
            int AviColor = 1;
            writer = cvCreateVideoWriter(outvideo, CV_FOURCC('M', 'J', 'P', 'G'), fps, cvSize(320, 240), AviColor);
        } else {
            GImage = cvCreateImage(cvSize(320,240), pImage->depth, pImage->nChannels);
            cvResize(pImage, GImage, CV_INTER_AREA);
            pHistEq = oCLAHE(GImage);
            ped = detect(pHistEq);
            draw(GImage, ped);
            cvShowImage("WinPlayer", GImage);
            assert(cvWriteFrame(writer, GImage)>0);
            if(GImage) cvReleaseImage(&GImage);
            if(pHistEq) cvReleaseImage(&pHistEq);
            if(cvWaitKey(10)>=0) break;
        }
    }
    cvReleaseCapture(&pCapture);
    cvReleaseVideoWriter(&writer);
    cvDestroyWindow("WinPlayer");
    return 0;
}
