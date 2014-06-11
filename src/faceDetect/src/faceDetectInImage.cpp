#include "/usr/local/include/opencv/cv.h"
#include "/usr/local/include/opencv/highgui.h"
#include "/usr/local/include/opencv2/core/core.hpp"
#include "/usr/local/include/opencv2/highgui/highgui.hpp"
#include <algorithm>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

#include "/usr/include/mysql_connection.h"
#include "/usr/include/cppconn/prepared_statement.h"
#include "/usr/include/cppconn/driver.h"
#include "/usr/include/cppconn/exception.h"
#include "/usr/include/cppconn/resultset.h"
#include "/usr/include/cppconn/statement.h"
#include "/usr/local/include/opencv2/core/types_c.h"
//#include <cstddef>

//using namespace cv;
using namespace std;

static CvMemStorage* storage_face = 0; //Memory Storage to Sore faces

static CvHaarClassifierCascade* cascade_face = 0;

IplImage* detect_and_draw(IplImage* image, int x_tag, int y_tag);
IplImage* crop(IplImage* src, CvRect roi);
void saveImageFace(IplImage* src, std::string path);
void getImageInfo();
int checkImageFace(CvRect r, int x, int y);
void initDetect(std::string load_path, std::string save_path, int x_tag, int y_tag);
std::string loadPath(std::string pid, std::string object);
std::string savePath(std::string pid, std::string object);
void LogDebug(std::string description, std::string content);
cv::Mat norm_0_255(const cv::Mat& src);
cv::Mat tan_triggs_preprocessing(cv::InputArray src,
        float alpha, float tau, float gamma, int sigma0,
        int sigma1);
//Haar cascade - if your openc CV is installed at location C:/OpenCV2.0/
const char* cascade_name_face = "faceDetect/src/dataTraining/haarcascades/haarcascade_frontalface_alt_tree.xml";
//const char* cascade_name_face = "src/dataTraining/haarcascades/haarcascade_frontalface_alt_tree.xml";

/////////////////////////////////////////////////////////////////////////////////

int main() {
    LogDebug("start:", "\n");
    getImageInfo();
    LogDebug("end", "\n\n\n");
    return 0;
}

////////////////////////////  Function To detect face //////////////////////////

void initDetect(std::string load_path, std::string save_path, long double x_tag, long double y_tag) {
    IplImage *image = 0, *face_image = NULL;
    char tab2[1024];
    strcpy(tab2, load_path.c_str());
    image = cvLoadImage(tab2, 1);
    if (!image) {
        cout << "Not load image: " << tab2 << endl;
        return;
    }
    remove(tab2);
    
    int x_real = (int) ((image->width * x_tag) / 100), y_real = (int) ((image->height * y_tag) / 100);

    cascade_face = (CvHaarClassifierCascade*) cvLoad(cascade_name_face, 0, 0, 0);

    if (!cascade_face) {
        printf("ERROR: Could not load classifier of face  cascade\n");
        return;
    }

    storage_face = cvCreateMemStorage(0);
    
    face_image = detect_and_draw(image, x_real, y_real);
    if (face_image != NULL) saveImageFace(face_image, save_path);
    
    // release resourses
    cvReleaseImage(&image);
    cvReleaseHaarClassifierCascade(&cascade_face);
    cvReleaseMemStorage(&storage_face);
    //cvDestroyWindow("result");
}

IplImage* detect_and_draw(IplImage* img, int x_tag, int y_tag) {

    double scale = 1;
    IplImage* result = NULL;
    // create a gray image for the input image
    IplImage* gray = cvCreateImage(cvSize(img->width, img->height), 8, 1);
    // Scale down the ie. make it small. This will increase the detection speed
    IplImage* small_img = cvCreateImage(cvSize(cvRound(img->width / scale), cvRound(img->height / scale)), 8, 1);

    int i;

    cvCvtColor(img, gray, CV_BGR2GRAY);

    cvResize(gray, small_img, CV_INTER_LINEAR);

    // Equalise contrast by eqalizing histogram of image
    cvEqualizeHist(small_img, small_img);

    cvClearMemStorage(storage_face);

    if (cascade_face) {
        // Detect object defined in Haar cascade. IN our case it is face
        CvSeq* faces = cvHaarDetectObjects(small_img, cascade_face, storage_face,
                1.1, 2, 0/*CV_HAAR_DO_CANNY_PRUNING*/,
                cvSize(30, 30));

        // Draw a rectagle around all detected face 
        for (i = 0; i < (faces ? faces->total : 0); i++) {
            CvRect r = *(CvRect*) cvGetSeqElem(faces, i);
            //cvRectangle(img, cvPoint(r.x*scale, r.y * scale), cvPoint((r.x + r.width) * scale, (r.y + r.height) * scale), CV_RGB(255, 0, 0), 3, 8, 0);
            if (checkImageFace(r, x_tag, y_tag)) {
                result = crop(img, r);
                cvReleaseImage(&gray);
                cvReleaseImage(&small_img);
                return result;
            }
        }
    }

    //cvShowImage("result", img);

    cvReleaseImage(&gray);
    cvReleaseImage(&small_img);
    return result;
}

IplImage* crop(IplImage* src, CvRect roi) {
    // Must have dimensions of output image
    IplImage* cropped = cvCreateImage(cvSize(roi.width, roi.height), src->depth, src->nChannels);

    // Say what the source region is

    cvSetImageROI(src, roi);

    // Do the copy
    cvCopy(src, cropped);
    cvResetImageROI(src);

    return cropped;
}

void saveImageFace(IplImage* src, std::string path) {
    char tab2[1024];
    strcpy(tab2, path.c_str());
    cv::Mat origin(src);
    cv::Mat gs_rgb(origin.rows, origin.cols, CV_8UC1);
    cv::cvtColor(origin, gs_rgb, CV_RGB2GRAY);
    cv::Mat test = tan_triggs_preprocessing(gs_rgb, 0.1, 10.0, 0.2, 1, 2);
    imwrite(tab2, test);
}

int checkImageFace(CvRect r, int x, int y) {
    if ((r.x < x) && (x < r.x + r.width) && (r.y < y) && (y < r.y + r.height)) {
        return 1;
    }
    return 0;
}

void getImageInfo() {
    sql::Connection *con = NULL;
    try {
        sql::Driver *driver;
        /* Create a connection */
        driver = get_driver_instance();
        con = driver->connect("tcp://localhost:3306", "root", "root");
        /* Connect to the MySQL test database */
        con->setSchema("PhotoInfo");
    } catch (sql::SQLException &e) {
        std::cout << "# ERR: SQLException in " << __FILE__;
        std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cout << "# ERR: " << e.what();
        std::cout << " (MySQL error code: " << e.getErrorCode();
        std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
    }

    sql::ResultSet *res;
    sql::PreparedStatement *prep_stmt;
    // ...


    if (con)
        prep_stmt = con->prepareStatement("SELECT * FROM photo_tag");
    res = prep_stmt->executeQuery();
    while (res->next()) {
        initDetect(loadPath(res->getString(2), res->getString(4)), savePath(res->getString(2), res->getString(4)), res->getDouble("xcoord"), res->getDouble("ycoord"));
        //break;
    }

    delete res;

    return;
}

std::string loadPath(std::string pid, std::string object) {
    std::string result = "train/";
    result.append(object);
    result.append("/");
    result.append(pid);
    result.append(".jpg");
    return result;
}

std::string savePath(std::string pid, std::string object) {
    std::string result = "train/";
    result.append(object);
    //    char tab2[1024];
    //    strcpy(tab2, result.c_str());
    struct stat st = {0};
    if (stat(result.c_str(), &st) == -1) {
        mkdir(result.c_str(), 0755);
    }
    result.append("/");
    result.append(pid);
    result.append(".jpg");
    return result;
}

void LogDebug(std::string description, std::string content) {
    
    ofstream myfile("faceDetect/Debug/log.txt", std::ios::app);
    if (myfile.is_open()) {
        myfile << description;
        myfile << ": ";
        myfile << content;
        myfile << " \n";
        myfile.close();
    } else cout << "Unable to open file" << endl;
}

cv::Mat norm_0_255(const cv::Mat& src) {
    // Create and return normalized image:
    cv::Mat dst;
    switch (src.channels()) {
        case 1:
            normalize(src, dst, 0, 255, cv::NORM_MINMAX, CV_8UC1);
            break;
        case 3:
            normalize(src, dst, 0, 255, cv::NORM_MINMAX, CV_8UC3);
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

cv::Mat tan_triggs_preprocessing(cv::InputArray src,
        float alpha, float tau, float gamma, int sigma0,
        int sigma1) {

    // Convert to floating point:
    cv::Mat X = src.getMat();
    X.convertTo(X, CV_32FC1);
    // Start preprocessing:
    cv::Mat I;
    pow(X, gamma, I);
    // Calculate the DOG Image:
    {
        cv::Mat gaussian0, gaussian1;
        // Kernel Size:
        int kernel_sz0 = (3 * sigma0);
        int kernel_sz1 = (3 * sigma1);
        // Make them odd for OpenCV:
        kernel_sz0 += ((kernel_sz0 % 2) == 0) ? 1 : 0;
        kernel_sz1 += ((kernel_sz1 % 2) == 0) ? 1 : 0;
        cv::GaussianBlur(I, gaussian0, cv::Size(kernel_sz0, kernel_sz0), sigma0, sigma0, cv::BORDER_CONSTANT);
        cv::GaussianBlur(I, gaussian1, cv::Size(kernel_sz1, kernel_sz1), sigma1, sigma1, cv::BORDER_CONSTANT);
        cv::subtract(gaussian0, gaussian1, I);
    }

    {
        double meanI = 0.0;
        {
            cv::Mat tmp;
            pow(abs(I), alpha, tmp);
            meanI = cv::mean(tmp).val[0];

        }
        I = I / pow(meanI, 1.0 / alpha);
    }

    {
        double meanI = 0.0;
        {
            cv::Mat tmp;
            pow(min(abs(I), tau), alpha, tmp);
            meanI = cv::mean(tmp).val[0];
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
    return norm_0_255(I);
}
