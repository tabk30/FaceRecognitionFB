/* 
 * File:   MyPCA.cpp
 * Author: tabk30
 * 
 * Created on May 10, 2014, 3:42 PM
 */

#include "../lib/MyPCA.h"

MyPCA::MyPCA() {
    //load data
    ImageData * image_process = new ImageData(100, 100);
    PathGenerate * path = new PathGenerate("train", "Info/train.txt");
    path->generate();
    delete path;

    vector<Mat> db;
    vector<string> label_train;
    image_process->loadImage("PCAPerform/Info/train.txt", db, label_train);
    Mat train = this->formatImagesForPCA(db);
    ///////////////////////////////////////////////////////////////////


    //int component = 50;
    PCA pca(train, cv::Mat(), CV_PCA_DATA_AS_ROW, 0.95);
    Mat train_compress;
    train_compress.create(train.rows, db.size(), train.type());
    for (int i = 0; i < train_compress.rows; i++) {
        Mat vec = train.row(i), coeffs = train_compress.row(i);
        pca.project(vec, coeffs);
    }
    this->SaveData(pca.eigenvalues, pca.eigenvectors, pca.mean, train_compress, label_train);
}

MyPCA::MyPCA(const MyPCA& orig) {

}

MyPCA::~MyPCA() {
}

Mat MyPCA::formatImagesForPCA(const vector<Mat> &data) {
    Mat dst(static_cast<int> (data.size()), data[0].rows * data[0].cols, CV_32F);
    for (unsigned int i = 0; i < data.size(); i++) {
        Mat image_row = data[i].clone().reshape(1, 1);
        Mat row_i = dst.row(i);
        image_row.convertTo(row_i, CV_32F);
    }

    return dst;
}

void MyPCA::SaveData(Mat eigenvalues, Mat eigenvectors, Mat mean, Mat train_compress, vector<string> label_train) {
    FileStorage fs("data/data.xml", FileStorage::WRITE);

    if (!fs.isOpened()) {
        cerr << "Failed to open " << "data/data.xml" << endl;
        //help(av);
        return;
    }
    fs << "eigenvalues" << eigenvalues.clone();
    fs << "eigenvectors" << eigenvectors.clone();
    fs << "mean" << mean.clone();
    fs << "train_data" << train_compress.clone();
    fs << "label" << label_train;
    fs.release();
}