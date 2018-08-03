#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <stdio.h>
#include <vector>
#include <iostream>

#include "objtracker.h"

using namespace cv;
using namespace std;

namespace tc = terraclear;

int main(int argc, char** argv) 
{
    //Config Options
    //Set Image Resolution..
    uint32_t cam_res_w = 1280;
    uint32_t cam_res_h = 960;

    // min square pixels off required object..
    uint32_t max_object_area = 500;
    
    Mat cam_mat;
    
    //Object Tracker..
    tc::objtracker objtrk;
    
    //Set range to "BLUE" Hue 
    objtrk._lowRange = Scalar(105,100,100);
    objtrk._highRange = Scalar(120,180,180);
            
    VideoCapture stream1(2);   //0 is the id of video device.0 if you have only one camera.
    stream1.set(3,cam_res_w);
    stream1.set(4,cam_res_h);
    
            
  
    if (!stream1.isOpened())
    { //check if video device has been initialised
        cout<< "Error loading video" << endl;
        return -1;
    }    
    
 //unconditional loop
    while (true)
    {
        stream1.read(cam_mat);
        if (!cam_mat.data)
            std::cout << "Image not loaded";
        else
        {
            
            
            //bounding boxes for tracked objects
             vector<Rect> tracked_objects = objtrk.findObjectBoundingBoxes(cam_mat);
             objtrk.mergeBoundingBoxes(tracked_objects);

             //Draw boxes on image.
             Point obj_box_center1 =  Point( cam_res_w / 2, cam_res_h);
             Point obj_box_center2 = obj_box_center1;
             
                         //check objects location relative to areas of interrest..
            for (auto obj_box : tracked_objects)
            {
                Point obj_box_tl = obj_box.tl();
                Point obj_box_br = obj_box.br();
                Scalar obj_box_color = Scalar(0x00, 0xff, 0x00);//object bounding box color
                Scalar obj_box_color2 = Scalar(0x00, 0x00, 0xff);//object bounding box color                //Make sure we cover only images of the min size.
                if ((obj_box.height * obj_box.width) > max_object_area)
                {
                    obj_box_center2 =  Point((obj_box_br.x - obj_box_tl.x)/2 + obj_box_tl.x, (obj_box_br.y - obj_box_tl.y)/2 + obj_box_tl.y);
                    rectangle(cam_mat, obj_box_tl, obj_box_br, obj_box_color, 2, 8, 0 );
                    line(cam_mat, obj_box_center1, obj_box_center2, obj_box_color2, 2, 8, 0);
                    obj_box_center1 = obj_box_center2;
                }
            }
             
            imshow("cam", cam_mat);
        }
        
        if (waitKey(30) >= 0)
        {
            vector<int> compression_params;
            compression_params.push_back(IMWRITE_PNG_COMPRESSION);
            compression_params.push_back(9);

#ifdef __linux__ 
            imwrite("/home/koos/Desktop/inrange.png", cam_mat, compression_params);
#elif __APPLE__
            imwrite("/Users/koos/Desktop/inrange.png", cam_mat, compression_params);
#else 
            cout << "NO IMAGE SAVED.." << endl;
#endif
            
            break;
        }
    }
    
    return 0;
}

