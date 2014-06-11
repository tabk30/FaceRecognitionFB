/* 
 * File:   FaceRecogniontPCA.h
 * Author: tabk30
 *
 * Created on May 10, 2014, 3:42 PM
 */

#ifndef __MAINFACERECOGNIONTPCA_H__
#define	__MAINFACERECOGNIONTPCA_H__

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "../lib/FaceDetect.h"

using namespace std;
using namespace cv;

class FaceRecogniontPCA {
public:
    FaceRecogniontPCA();
    FaceRecogniontPCA(const FaceRecogniontPCA& orig);
    virtual ~FaceRecogniontPCA();
    vector<string> recognition(string path_name, string name);
    string recognitionAFace(Mat origin);
    void LoadData();
    string findNearest(Mat input);
    Mat synchronizationImage(Mat image);
    string getLabel(string label);
    Mat norm_0_255(const Mat& src);
    Mat tan_triggs_preprocessing(InputArray src,
            float alpha, float tau , float gamma, int sigma0,
            int sigma1);
    Mat tan_triggs_preprocessing(Mat src);
    int checkExit(vector<string> result, string label_temp);
    void saveResult(vector<string> result);
private:
    Mat formatImagesForPCA(const vector<Mat> &data);
    Mat data;
    PCA pca;
    vector<string> label_train;
    double distance;
    IplImage * face_detect;
    IplImage * face_recognition;
    
    //data save:
    string file_name;
};

#endif	/* FACERECOGNIONTPCA_H */

