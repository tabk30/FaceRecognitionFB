/* 
 * File:   FaceRecogniontPCA.h
 * Author: tabk30
 *
 * Created on May 10, 2014, 3:42 PM
 */

#ifndef FACERECOGNIONTPCA_H
#define	FACERECOGNIONTPCA_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "PathGenerate.h"
#include "ImageData.h"

using namespace std;
using namespace cv;

class MyPCA {
public:
    MyPCA();
    MyPCA(const MyPCA& orig);
    virtual ~MyPCA();
    Mat formatImagesForPCA(const vector<Mat> &data);
    void SaveData(Mat eigenvalues, Mat eigenvectors, Mat mean, Mat train_compress,vector<string> label_train);
private:
};

#endif	/* FACERECOGNIONTPCA_H */

