#ifndef OBJTRACKER_H
#define OBJTRACKER_H

#include <opencv/cv.hpp>

#include <cstdlib>
#include <unistd.h>

#include <stdio.h>
#include <vector>
#include <iostream>

using namespace cv;
using namespace std;

namespace  terraclear
{
    class objtracker 
    {
        public:

            // hard coded color hue range for BLUE
            Scalar _lowRange = Scalar(107,80,80);
            Scalar _highRange = Scalar(116,220,220);

            objtracker();
            vector<Rect> findObjectBoundingBoxes(Mat imgsrc);
            void mergeBoundingBoxes(vector<Rect> &object_boxes);
        private:

            
    };
}

#endif /* OBJTRACKER_H */

