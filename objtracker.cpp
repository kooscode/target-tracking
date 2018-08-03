#include "objtracker.h"

namespace  terraclear
{
    objtracker::objtracker() 
    {
    }

    vector<Rect> objtracker::findObjectBoundingBoxes(Mat imgsrc)
    {


        //ret vector
        vector<Rect> ret_vect;

        Mat mat_filtered;

        //blur Image a bit first.
        cv::blur(imgsrc, mat_filtered, Size(20,20));

        /// Transform it to HSV color space
        cvtColor(mat_filtered, mat_filtered, COLOR_BGR2HSV);

        //find all objects in range (Thresholding)
        cv::inRange(mat_filtered, _lowRange, _highRange, mat_filtered);

        //Vector for all contours.
        vector<vector<Point>> contours;
        findContours(mat_filtered, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

        //create bounding boxes from contours
        for (auto contour : contours)
        {
            ret_vect.push_back( boundingRect(Mat(contour)));
        }

        return ret_vect;
    }

   
    void objtracker::mergeBoundingBoxes(vector<Rect> &object_boxes)
    {
        //Group neighbouring and overlapping rectangles
        vector<Rect> object_boxes_copy(object_boxes);
        object_boxes.insert(object_boxes.end(), object_boxes_copy.begin(), object_boxes_copy.end());
        groupRectangles(object_boxes, 1, 1);
     }
    
}