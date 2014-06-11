/*
 * Copyright (c) 2012. Philipp Wagner <bytefish[at]gmx[dot]de>.
 * Released to public domain under terms of the BSD Simplified license.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of the organization nor the names of its contributors
 *     may be used to endorse or promote products derived from this software
 *     without specific prior written permission.
 *
 *   See <http://www.opensource.org/licenses/bsd-license>
 */

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
    if(argc != 1){
         cout << "Not enough data" << endl;
         return 1;
    }
    FaceRecogniontPCA * pca_test = new FaceRecogniontPCA();
    string path(argv[0]);
    string name(argv[1]);
    pca_test->recognition(path, name);
}