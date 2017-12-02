//
//  main.cpp
//  videolu deneme
//
//  Created by Yakup Görür on 10/07/16.
//  Copyright © 2016 Yakup Görür. All rights reserved.
//
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"

#include "list"
#include "Preprocessing.h"

using namespace cv;
using namespace std;


// a binary predicate implemented as a function:
bool same_integral_part (Point first, Point second)
{ return ( int(first.x)==int(second.x)||int(first.y)==int(second.y) ); }

// a binary predicate implemented as a class:
class is_near_class
{
public:
    bool operator() (Point first, Point second)
    { return (fabs(first.x-second.x)<1.0 || fabs(first.y-second.y)<1.0); }
} is_near_object;










int main(int, char**)
{
    
    
    VideoCapture cap("/users/yakup/TESTS/gridipod.m4v"); // Open the movie file
    //VideoCapture cap(0);  // open the default camera
    
    //cap.set(CV_CAP_PROP_FPS, 25);     //FPS setting but it does not work
    
    if(!cap.isOpened())  // Check if we succeeded
        return -1;
    
    
    int DETECT_R=0;       // Total detected lines.
    int DETECT_C=0;
    
    
    
    double previous_R,previous_C,t_C,t_R, deneme1,deneme2 = (double)getTickCount();     //Time
    
    deneme2=deneme1=previous_C=t_C=previous_R=t_R = ((double)getTickCount() - t_R)/getTickFrequency();
    
    
    int birlestirme=0;
    
    
    while(1)        //Video loop
    {
        
        Mat image;
        cap>>image;     //take a frame from video(cap)
        
        
        if(image.empty())   //Check if we succeeded
            break;
        
        
        int detected_R_N=0;     //detected mass center of rectangle in the selected area for only a frame
        int detected_R_P=0;
        int detected_C_P=0;
        int detected_C_N=0;
        
        
        int COLUMN=image.cols;
        int ROW=image.rows;
        
        int RowBound_N=ROW-30;
        int RowBound_P=ROW-60;
        int ColumnBound_N=COLUMN-20;
        int ColumnBound_P=COLUMN-40;
        
        Rect Row_P_window = Rect(0, RowBound_P, COLUMN, 100);       //fill with coordinates of entry window
        Rect Row_N_window = Rect(0, RowBound_N, COLUMN,100);      // I use this.
        
        Rect Column_P_window = Rect(ColumnBound_P,0 ,50 ,ROW);
        Rect Column_N_window = Rect(ColumnBound_N,0 ,50 ,ROW);
        
        
        Preprocessing(image);   //Preprocessing image.
        
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;
        
        
        findContours(image, contours,hierarchy,CV_RETR_TREE, 4); //finding the contours
        
        Mat drawing = Mat::zeros( image.size(), CV_8UC3 );       // The Contours' image
        /*
        rectangle(drawing, Row_P_window, Scalar(255,255,255));      //Our rectangle
        rectangle(drawing, Row_N_window, Scalar(200,200,255));     //Our rectangle
        rectangle(drawing, Column_P_window, Scalar(255,255,255));      //Our rectangle
        rectangle(drawing, Column_N_window, Scalar(200,200,255));     //Our rectangle
        */
        
        
        
        //list<RotatedRect> box;  //Minumun area of Rectangles
        
        
        for(int i = 0 ; i< contours.size(); i++ )
        {
            Scalar color= Scalar(255,255,255);
            drawContours( drawing, contours, i, color, 2, 8, hierarchy, 2, Point() );
            
            //box.push_back(minAreaRect(contours[i]));
            
        }
        
        
        vector<Moments> mu(contours.size() ); //Contors transfering to Moments
        
        int sizeofrectangle=0;
        for( int i = 0; i < contours.size(); i++ ){
            RotatedRect a= minAreaRect(contours[i]);    //Minumun area of Rectangles
            
            if(a.size.area()>1000){                     //decreasing the noise
                mu[sizeofrectangle] = moments( contours[i], false );
                sizeofrectangle++;
            }
        }
        
        
        
        //Mass center
        list<Point> mc( contours.size() );
        
        for( int i = 0; i < sizeofrectangle; i++ ){
            mc.push_back(Point( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ));   //finding the mass center
            
        }
        
        
        mc.unique(same_integral_part);
        mc.unique(is_near_object);
        //mc.unique();
        
        
        
        
        int masscenters=0;
        for(list<Point>::iterator it = mc.begin(); it!=mc.end(); it++, masscenters++){
            
            if(it->y >= RowBound_P && it->y <= RowBound_N){
                
                detected_R_P++; // how many mass centers are passing the our rectangle
                deneme1= (double)getTickCount() / getTickFrequency();
            }
            
            
            if(it->y >= RowBound_N){
                
                detected_R_N++; // how many mass centers are passing the our rectangle
                deneme2=(double)getTickCount()/getTickFrequency();
            }
            
            if(it->x >= ColumnBound_P && it->x <= ColumnBound_N){
                ++detected_C_P;
                
            }
            if(it->x >= ColumnBound_N){
                detected_C_N++;
                std::string COUNTER_S = std::to_string(detected_C_N);
                //putText(drawing, COUNTER_S , Point(350,350), 5, 10, Scalar(200,050,200));
                
                
            }
            
            
            
            
            
            circle(drawing, *it, 10, Scalar(255,255,255)); //draw circle for mass centers
            
            std::string s = std::to_string(masscenters);
            
            //putText(drawing, s , *it, 2, 2, Scalar(0,0,255));   //drawing MassCenters numbers
            
        }
        
        if(detected_R_P>2){
            t_R = ((double)getTickCount() - t_R)/getTickFrequency();
            
            if( ( ( deneme1 - deneme2 ) < 1 ) && ( ( deneme1-deneme2 )  > 0 ) ){
                cout<<"HEY"<<endl;
                --DETECT_R;;
                deneme2=100;
            }
            
            
            
            if( t_R - previous_R>5){       // set time bound.
                
                //waitKey(300);
                
                previous_R=t_R;
                
            }
            
        }
        
        
        
        
        if(detected_R_N>2){
            t_R = ((double)getTickCount() - t_R)/getTickFrequency();
            
            if(  ( ( deneme2 - deneme1 ) < 1 ) && ( ( deneme2 - deneme1 )  > 0 )){
                cout<<"HEY"<<endl;
                ++DETECT_R;
                deneme1=100;
            }
            if( t_R - previous_R>5){       // set time bound.
                //--DETECT_R;
                
                //waitKey(300);
                previous_R=t_R;
                
            }
            
        }
        
        if(detected_C_P>1){
            t_C = ((double)getTickCount() - t_C)/getTickFrequency();
            
            if( t_C - previous_C>5){       // set time bound.
                ++DETECT_C;
                
                //waitKey(300);
                
                previous_C=t_C;
                
            }
            
        }
        
        if(detected_C_N>1){
            t_C = ((double)getTickCount() - t_C)/getTickFrequency();
            
            if( t_C - previous_C>5){       // set time bound.
                --DETECT_C;
                
                //waitKey(300);
                
                previous_C=t_C;
                
            }
            
            
        }
        
        cout<<" asd: "<<deneme2-deneme1<<endl;
        
        if(deneme2-deneme1<=1 && deneme2-deneme1>0){
            cout<<"deneme2-deneme1"<<deneme2-deneme1<<endl;
        }
        
        
        std::string COUNTER_S = std::to_string(DETECT_R);
        putText(drawing, COUNTER_S , Point(300,350), 5, 10, Scalar(200,050,200));
        
        std::string COUNTER_S_C = std::to_string(DETECT_C);
        putText(drawing, COUNTER_S_C , Point(400,150), 5, 10, Scalar(50,200,200));
        
        
        //std::string TOTAL_MASS_CENTERS_S = std::to_string(masscenters);
        //putText(drawing, TOTAL_MASS_CENTERS_S , Point(150,150), 4, 5, Scalar(255,255,255));
        
        
        resize(drawing ,drawing,Size(800,600),CV_INTER_CUBIC);  //for only showing
        imshow("drawing", drawing);
        
        t_R = ((double)getTickCount() - t_R)/getTickFrequency();
        std::cout << "Times passed in seconds: " << t_R <<" ---/" <<int(t_R-previous_R) <<std::endl;
        //deneme1=(double)getTickCount()/getTickFrequency();
        //std::cout << "****** : "<<deneme1  <<std::endl;
        if(waitKey(100.49) >= 0) break;
        
    }
    
    
    
    
    cap.release();
    return 0;
}
