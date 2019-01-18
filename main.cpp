/*
# Auto Red Eye Remover

Lei Mao

## Introduction
This is a simple automatic red eye remover implemented using OpenCV C++. It uses a naive red eye detection and correction algorithm. 
More complicated algorithms could be found in literatures. 

## Usage
./main <img_filepath> [threshold]

Threshold value is optional. But sometimes red eye removal will fail or the image after removal looks abnormal due to that the default threshold is not appropriate.
Try a value between 0 to 255 to see whether an optimal removal could be achieved. 
The algorithm uses eye detection algorithm to first detect eyes. If eye detection fails on the provided image, no red eye removal will happen. 
*/

#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <boost/filesystem.hpp>

#include <sstream>
#include <iostream>
#include <string>

#include "utils.h"

using namespace std;
using namespace cv;
namespace fs = boost::filesystem;

int main(int argc, char * argv[])
{
    // Use Boost library to extract file and system information.
    string img_filepath(argv[1]);
    string img_basename = fs::basename(img_filepath);
    string img_extension = fs::extension(img_filepath);
    string img_fixed_basename = img_basename + "_fixed";
    string img_dir = fs::path(img_filepath).parent_path().string();
    string img_fixed_filepath = (fs::path(img_dir) / fs::path(img_fixed_basename + img_extension)).string();

    // Read image.
    Mat img = imread(img_filepath, IMREAD_COLOR);

    int red_eye_threshold = 180;
    if (argv[2])
    {
        istringstream ss(argv[2]);
        if (!(ss >> red_eye_threshold))
        {
            std::cerr << "Invalid red eye threshold value: " << argv[2] << endl;
        }
        else if (!ss.eof())
        {
            std::cerr << "Invalid red eye threshold value: " << argv[2] << endl;
        }
    }
    if ((red_eye_threshold < 0) || (red_eye_threshold > 255))
    {
        cout << "Red eye threshold value not between 0 and 255. Use default value 180." << endl;
        red_eye_threshold = 180;
    }

    Mat img_fixed = remove_red_eye(img, red_eye_threshold);

    // Write fixed image to hard drive.
    imwrite(img_fixed_filepath, img_fixed);

    // Display image.
    namedWindow("Corrected Image", WINDOW_NORMAL);
    imshow("Corrected Image", img_fixed);
    waitKey(0);

    return 0;
}