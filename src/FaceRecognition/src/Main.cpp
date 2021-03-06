#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <map>
#include "../lib/FaceRecogniontPCA.h"
using namespace cv;
using namespace std;

int main(int argc, const char *argv[]) {
    if(argc < 3){
         cout << "Not enough data" << endl;
         return 1;
    }
    FaceRecogniontPCA * pca_test = new FaceRecogniontPCA();
    string path(argv[1]);
    string name(argv[2]);
    pca_test->recognition(path, name);
}