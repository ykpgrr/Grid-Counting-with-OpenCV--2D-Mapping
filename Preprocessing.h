//
//  Preprocessing.h
//  Sudoku 2.2.testf
//
//  Created by Yakup Görür on 05/07/16.
//  Copyright © 2016 Yakup Görür. All rights reserved.
//

#ifndef Preprocessing_h

#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"


using namespace cv;
using namespace std;

void Preprocessing(Mat &image){
    
    
    
    Size Ga_Size(5,5);
    GaussianBlur(image, image, Ga_Size,0);
    
    cvtColor(image, image, CV_BGR2GRAY);
    
    Mat kernel;
    Size ksize(20,20);
    kernel=getStructuringElement(MORPH_ELLIPSE, ksize);
    
    
    morphologyEx(image, image, MORPH_CLOSE, kernel);
    
    adaptiveThreshold(image, image, 255, 0, 1, 15, 5); // 15*15 substract 2
    
    
    
    
    
    //morphological opening (removes small objects from the foreground)
    
    /*
    erode(image, image, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    dilate( image, image, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    
    //morphological closing (removes small holes from the foreground)
    dilate( image, image, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    erode(image, image, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    
     */
    
    morphologyEx(image, image, MORPH_OPEN, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
    morphologyEx(image, image, MORPH_CLOSE, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
    
    
    
    bitwise_not(image, image);
    
    
}




#endif /* Preprocessing_h */
