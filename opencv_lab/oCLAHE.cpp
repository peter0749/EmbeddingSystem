#include "oCLAHE.h"
IplImage* oCLAHE(IplImage* img) {
    IplImage* v = cvCreateImage(cvGetSize(img), img->depth, 1); // load image
    cvCvtColor(img, v, CV_RGB2GRAY); // RGB -> gray scale
    int N = (v->width) * (v->height);
    int hist[256] = {0}; // histogram
    int cuthist[256] = {0}; // fix histogram
    double cdf[256] = {0};
    double clippedHist[256] = {0}, value=0;
    int x, y, pixel, m;
    int ex, limit;
    double a = 100;
    int s = 6;

    int gray=0;
    CvScalar Scalar1;
    for (int i=0; i<v->height; ++i) for (int j=0; j<v->width; ++j) {
        Scalar1 = cvGet2D(v, i, j);
        gray = (int) Scalar1.val[0]; // get gray value
        ++hist[gray];
    }
    limit = N/256 * (1+(a/100)*(s-1));

    ex = 0;
    for (int i=0; i<256; ++i) {
        cuthist[i] = hist[i];
        if (cuthist[i]>limit) {
            ex += cuthist[i] - limit;
            cuthist[i] = limit;
        }
    }

    m = ex/256;
    for (int i=0; i<256; ++i) {
        if (cuthist[i] < limit-m) {
            cuthist[i] += m;
            ex -= m;
        } else if (cuthist[i] < limit) {
            ex -= (limit - cuthist[i]);
            cuthist[i] = limit;
        }
    }

    for (int i=0; i<256; ++i) {
        if (cuthist[i] < limit) {
            cuthist[i] += 1;
            --ex;
        }
    }

    for (int i=0; i<256; ++i) 
        clippedHist[i] = (double) cuthist[i] / N;

    for (int i=0; i<256; ++i) for (int j=0; j<=i; ++j) {
        cdf[i] += clippedHist[j];
    }

    for (int i=0; i<v->height; ++i) for (int j=0; j<v->width; ++j) {
        Scalar1 = cvGet2D(v, i, j);
        pixel = (int) Scalar1.val[0];
        pixel = 255*cdf[pixel];
        cvSetReal2D(v, i, j, pixel);
    }
    return v;
}
