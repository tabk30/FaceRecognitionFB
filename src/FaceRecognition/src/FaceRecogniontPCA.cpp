/* 
 * File:   FaceRecogniontPCA.cpp
 * Author: tabk30
 * 
 * Created on May 10, 2014, 3:42 PM
 */

#include "../lib/FaceRecogniontPCA.h"

FaceRecogniontPCA::FaceRecogniontPCA() {
    //load data
    this->LoadData();
    if (this->data.cols <= 0) {
    }
}

FaceRecogniontPCA::FaceRecogniontPCA(const FaceRecogniontPCA& orig) {

}

FaceRecogniontPCA::~FaceRecogniontPCA() {
}

vector<string> FaceRecogniontPCA::recognition(string path_image, string name) {
    this->file_name = name;
    FaceDetect * __face_detect = new FaceDetect();
    __face_detect->initDetect(path_image);

    this->face_detect = __face_detect->getImage();
    vector<IplImage> face_list = __face_detect->getFaceList();
    vector<string> result;
    vector<int> testVector;

    for (unsigned int i = 0; i < face_list.size(); i++) {
        IplImage temp = face_list.at(i);

        Mat image = Mat(&temp);
        string label_temp = this->recognitionAFace(image);
        if (label_temp.compare("") != 0) {
            if (this->checkExit(result, label_temp) == 0) {
                testVector.push_back(i);
                result.push_back(label_temp);
            }
        }

    }
    this->face_recognition = __face_detect->tagFaceRecognition(testVector);
    this->saveResult(result);
    return result;
}

Mat FaceRecogniontPCA::formatImagesForPCA(const vector<Mat> &data) {
    Mat dst(static_cast<int> (data.size()), data[0].rows * data[0].cols, CV_32F);
    for (unsigned int i = 0; i < data.size(); i++) {
        Mat image_row = data[i].clone().reshape(1, 1);
        Mat row_i = dst.row(i);
        image_row.convertTo(row_i, CV_32F);
    }

    return dst;
}

string FaceRecogniontPCA::recognitionAFace(Mat origin) {
    double leastDistSq = 1.0f;
    int __nearlest = 0;
    Mat coeffs, test = origin;
    test = this->synchronizationImage(test);
    vector<Mat> test_data;
    test_data.push_back(test);
    Mat test_compress = this->formatImagesForPCA(test_data);

    this->pca.project(test_compress.row(0), coeffs);
    for (int i = 0; i < this->data.rows; i++) {
        double euclidean = 0.0f;
        euclidean = cv::norm(this->data.row(i), coeffs, cv::NORM_L2);
        if (i == 0) {
            leastDistSq = euclidean;
            __nearlest = i;
        } else if (euclidean < leastDistSq) {
            leastDistSq = euclidean;
            __nearlest = i;
        }
    }
    return this->label_train.at(__nearlest);
}

void FaceRecogniontPCA::LoadData() {
    //Mat error(0, 0, DataType<float>::type), data;

    FileStorage fs("data/data.xml", FileStorage::READ);
    if (!fs.isOpened()) {
        cerr << "Failed to open " << "data/data.xml" << endl;
        return;
    }
    //Mat eigenvalues, eigenvectors, mean;
    fs["eigenvalues"] >> this->pca.eigenvalues;
    fs["eigenvectors"] >> this->pca.eigenvectors;
    fs["mean"] >> this->pca.mean;
    fs["train_data"] >> this->data;
    fs["label"] >> this->label_train;
    fs.release();

    return;
}

string FaceRecogniontPCA::findNearest(Mat input) {
    double leastDistSq = 1.0f;
    int __nearlest = 0;
    for (int i = 0; i < this->data.rows; i++) {
        double euclidean = 0.0f;
        euclidean = cv::norm(this->data.row(i), input, cv::NORM_L2);
        if (i == 0) {
            leastDistSq = euclidean;
            __nearlest = i;
        } else if (euclidean < leastDistSq) {
            leastDistSq = euclidean;
            __nearlest = i;
        }
    }

    return this->label_train.at(__nearlest);
}

Mat FaceRecogniontPCA::synchronizationImage(Mat image) {
    //Mat origin;
    //cvtColor(image, origin, CV_BGR2GRAY);
    Mat origin = this->tan_triggs_preprocessing(image);
    Mat smallerImage;
    resize(origin, smallerImage, Size(70, 70), 0, 0, INTER_AREA);

    return smallerImage;
}

Mat FaceRecogniontPCA::norm_0_255(const Mat& src) {
    // Create and return normalized image:
    Mat dst;
    switch (src.channels()) {
        case 1:
            cv::normalize(src, dst, 0, 255, NORM_MINMAX, CV_8UC1);
            break;
        case 3:
            cv::normalize(src, dst, 0, 255, NORM_MINMAX, CV_8UC3);
            break;
        default:
            src.copyTo(dst);
            break;
    }
    return dst;
}

//
// Calculates the TanTriggs Preprocessing as described in:
//
// Tan, X., and Triggs, B. "Enhanced local texture feature sets for face
// recognition under difficult lighting conditions.". IEEE Transactions
// on Image Processing 19 (2010), 1635–650.
//
// Default parameters are taken from the paper.
//

Mat FaceRecogniontPCA::tan_triggs_preprocessing(InputArray src,
        float alpha = 0.1, float tau = 10.0, float gamma = 0.2, int sigma0 = 1,
        int sigma1 = 2) {

    // Convert to floating point:
    Mat X = src.getMat();
    X.convertTo(X, CV_32FC1);
    // Start preprocessing:
    Mat I;
    pow(X, gamma, I);
    // Calculate the DOG Image:
    {
        Mat gaussian0, gaussian1;
        // Kernel Size:
        int kernel_sz0 = (3 * sigma0);
        int kernel_sz1 = (3 * sigma1);
        // Make them odd for OpenCV:
        kernel_sz0 += ((kernel_sz0 % 2) == 0) ? 1 : 0;
        kernel_sz1 += ((kernel_sz1 % 2) == 0) ? 1 : 0;
        GaussianBlur(I, gaussian0, Size(kernel_sz0, kernel_sz0), sigma0, sigma0, BORDER_CONSTANT);
        GaussianBlur(I, gaussian1, Size(kernel_sz1, kernel_sz1), sigma1, sigma1, BORDER_CONSTANT);
        subtract(gaussian0, gaussian1, I);
    }

    {
        double meanI = 0.0;
        {
            Mat tmp;
            pow(abs(I), alpha, tmp);
            meanI = mean(tmp).val[0];

        }
        I = I / pow(meanI, 1.0 / alpha);
    }

    {
        double meanI = 0.0;
        {
            Mat tmp;
            pow(min(abs(I), tau), alpha, tmp);
            meanI = mean(tmp).val[0];
        }
        I = I / pow(meanI, 1.0 / alpha);
    }

    // Squash into the tanh:
    {
        for (int r = 0; r < I.rows; r++) {
            for (int c = 0; c < I.cols; c++) {
                I.at<float>(r, c) = tanh(I.at<float>(r, c) / tau);
            }
        }
        I = tau * I;
    }
    return this->norm_0_255(I);
}

Mat FaceRecogniontPCA::tan_triggs_preprocessing(Mat src) {
    Mat origin(src);
    cv::Mat gs_rgb(origin.rows, origin.cols, CV_8UC1);
    cv::cvtColor(origin, gs_rgb, CV_RGB2GRAY);
    return tan_triggs_preprocessing(gs_rgb, 0.1F, 10.0F, 0.2F, 1, 2);
}

int FaceRecogniontPCA::checkExit(vector<string> result, string label_temp) {
    for (unsigned int i = 0; i < result.size(); i++) {
        if (label_temp.compare(result.at(i)) == 0) {
            return 1;
        }
    }
    return 0;
}

void FaceRecogniontPCA::saveResult(vector<string> result) {
    //save detect image:
    string detect_name = "images/";
    detect_name.append(this->file_name);
    detect_name.append("_detection.jpg");
    count << "detect: " << detect_name << endl;
    char name_1[1024];
    strcpy(name_1, detect_name.c_str());
    Mat face_detect(this->face_detect);
    imwrite(name_1, face_detect);

    //save recognition image
    string recogn_name = "images/";
    recogn_name.append(this->file_name);
    recogn_name.append("_recognition.jpg");
    count << "recogntition: " << recogn_name << endl;
    char name_2[1024];
    strcpy(name_2, recogn_name.c_str());
    Mat face_recogn(this->face_recognition);
    imwrite(name_2, face_recogn);

    //save result:
    string result_name = "images/";
    result_name.append(this->file_name);
    result_name.append("_result.txt");
    char name_3[1024];
    strcpy(name_3, detect_name.c_str());
    ofstream myfile(name_3, std::ios::);
    if (myfile.is_open()) {
        for (unsigned int i = 0; i < result.size(); i++) {
            myfile << result.at(i);
            myfile << "\n";
            myfile.close();
        }
    } else cout << "Unable to open file" << endl;
    
}