/* 
 * File:   FaceDetect.h
 * Author: tabk30
 *
 * Created on April 11, 2014, 12:25 AM
 */

#ifndef __MAINFACEDETECT_H__
#define	__MAINFACEDETECT_H__

#include "/usr/local/include/opencv/cv.h"
#include "/usr/local/include/opencv/highgui.h"
#include "/usr/local/include/opencv2/core/core.hpp"
#include "/usr/local/include/opencv2/highgui/highgui.hpp"
#include <iostream>
#include <algorithm>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <opencv2/core/types_c.h>

using namespace std;
using namespace cv;
class FaceDetect {
public:
    FaceDetect();
    FaceDetect(const FaceDetect& orig);
    virtual ~FaceDetect();
    void initDetect(std::string load_path);
    IplImage * getImage();
    vector<IplImage> getFaceList();
    vector<IplImage> face_list;
    string convertInt(int number);
    IplImage* tagFaceRecognition(vector<int> tag_id);
private:
    CvMemStorage* storage_face; //Memory Storage to Sore faces
    CvHaarClassifierCascade* cascade_face;
    char* cascade_name_face ;
    IplImage* image_result;
    IplImage* image;
    CvSeq* faces;
    
    void detect_and_draw(IplImage* img);
    IplImage* crop(IplImage* src, CvRect roi);
    
};


#endif	/* FACEDETECT_H */

