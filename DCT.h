//
// Created by Ghady Youssef on 19/04/2024.
//

#include <bits/stdc++.h>
#include "BinaryStdIn.h"
#include "BinaryStdOut.h"
#include <opencv2/opencv.hpp>

#define pi 3.141592653589793238462643

#ifndef COMPRESSION_LIBRARY_DCT_H
#define COMPRESSION_LIBRARY_DCT_H

using namespace std;
using namespace cv;

cv::Mat padImageToMultipleOfBlockSize(const cv::Mat& inputImage, int block_size) {
    int rows = inputImage.rows;
    int cols = inputImage.cols;

    // Calculate new dimensions that are multiples of block_size
    int paddedRows = ((rows + block_size - 1) / block_size) * block_size;
    int paddedCols = ((cols + block_size - 1) / block_size) * block_size;

    // Create output image with padded dimensions
    cv::Mat paddedImage(paddedRows, paddedCols, inputImage.type(), cv::Scalar(0));

    // Copy input image to the top-left corner of the padded image
    inputImage.copyTo(paddedImage(cv::Rect(0, 0, cols, rows)));

    return paddedImage;
}

void dctCompression(const string & path) {
        const int BLOCK_SIZE = 8;

        // Define Q50 quantization matrix
        cv::Mat Q50 = (cv::Mat_<double>(8, 8) <<
        16,  11,  10,  16,  24,  40,  51,  61,
        12,  12,  14,  19,  26,  58,  60,  55,
        14,  13,  16,  24,  40,  57,  69,  56,
        14,  17,  22,  29,  51,  87,  80,  62,
        18,  22,  37,  56,  68, 109, 103,  77,
        24,  35,  55,  64,  81, 104, 113,  92,
        49,  64,  78,  87, 103, 121, 120, 101,
        72,  92,  95,  98, 112, 100, 103,  99);

        Mat Q10 = (cv::Mat_<double>(8, 8) <<
        80,  60,  50,  80, 120, 200, 255, 255,
        55,  60,  70,  95, 130, 255, 255, 255,
        70,  65,  80, 120, 200, 255, 255, 255,
        70,  85, 110, 145, 255, 255, 255, 255,
        90, 110, 185, 255, 255, 255, 255, 255,
        120, 175, 255, 255, 255, 255, 255, 255,
        245, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255
        );

        Mat image = imread(path, IMREAD_GRAYSCALE);

        image = padImageToMultipleOfBlockSize(image, BLOCK_SIZE);

        Mat compressedImage = image.clone();

        // Compute the DCT matrix
        Mat T(BLOCK_SIZE, BLOCK_SIZE, CV_64FC1);

        for(int i = 0; i < BLOCK_SIZE; ++i) {
            for(int j = 0; j < BLOCK_SIZE; ++j) {
                if(i == 0) {
                    T.at<double>(i, j) = 1.0 / sqrt(BLOCK_SIZE);
                } else{
                    T.at<double>(i, j) = sqrt(2.0/BLOCK_SIZE) * cos(((2.0 * j + 1) * i * pi) / (2.0 * BLOCK_SIZE));
                }
            }
        }

        Mat Tprime(BLOCK_SIZE, BLOCK_SIZE, CV_64F);

        invert(T, Tprime); // invert or transpose is the same here

        for(int y = 0; y < image.rows; y += 8) {
            for(int x = 0; x < image.cols; x += 8) {
                cv::Rect blockRect(x, y, BLOCK_SIZE, BLOCK_SIZE);

                cv::Mat M = image(blockRect);
                M.convertTo(M, CV_64FC1);
                M = M - 128;

                M.convertTo(M, CV_64FC1);

                Mat D = T * M * Tprime;

                Mat C(BLOCK_SIZE, BLOCK_SIZE, CV_64FC1);

                for(int i =0 ;i<BLOCK_SIZE; ++i) {
                    for(int j =0 ;j<BLOCK_SIZE; ++j) {
                        auto temp =  round(D.at<double>(i, j)/Q10.at<double>(i, j));
                        C.at<double>(i,j) = temp;
                    }
                }

                // decompression
                Mat R(BLOCK_SIZE,BLOCK_SIZE,CV_64FC1 );

                for(int i =0 ;i<BLOCK_SIZE; ++i) {
                    for(int j =0 ;j<BLOCK_SIZE; ++j) {
                        R.at<double>(i,j) = Q10.at<double>(i, j) * C.at<double>(i, j);
                    }
                }

                Mat N = Tprime * R * T;

                for(int i =0; i<BLOCK_SIZE; ++i) {
                    for(int j =0; j<BLOCK_SIZE; ++j) {
                        N.at<double>(i,j) = round(N.at<double>(i,j));
                    }
                }

                N = N + 128; //unshift

                N.copyTo(compressedImage(blockRect));
            }
        }

        cv::imshow("Original Image", image);
        cv::imshow("Compressed and Decompressed Image", compressedImage);
        cv::waitKey(0);
};
#endif //COMPRESSION_LIBRARY_DCT_H
