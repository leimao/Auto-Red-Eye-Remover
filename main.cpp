/*
# Auto Red Eye Remover

Lei Mao

## Introduction
This is a simple automatic red eye remover implemented using OpenCV C++. It uses a naive red eye detection and correction algorithm. 
More complicated algorithms could be found in literatures. 

## Usage
./main img_filepath [threshold]

Threshold value is optional. But sometimes red eye removal will fail or the image after removal looks abnormal due to that the default threshold is not appropriate.
Try a value between 0 to 255 to see whether an optimal removal could be achieved. 
The algorithm uses eye detection algorithm to first detect eyes. If eye detection fails on the provided image, no red eye removal will happen. 

## Reference
https://www.learnopencv.com/automatic-red-eye-remover-using-opencv-cpp-python/
http://www.ijeert.org/pdf/v2-i2/10.pdf
*/

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <boost/filesystem.hpp>

#include <sstream>
#include <iostream>
#include <string>

using namespace std;
using namespace cv;
namespace fs = boost::filesystem;

void fillHoles(Mat &mask)
{
    Mat mask_floodfill = mask.clone();
    // Fill the non-hole regions with 255, only the hole regions remain 0.
    floodFill(mask_floodfill, Point(0,0), Scalar(255));
    Mat mask_inverse;
    // Now only the hole regions are 255, the rest of the regions are 0.
    bitwise_not(mask_floodfill, mask_inverse);
    // Fill the hole regions with 255 to the original mask.
    mask = (mask_inverse | mask);
}

int main(int argc, char * argv[])
{
    // Use Boost library to extract file and system information.
    string img_filepath(argv[1]);
    string img_basename = fs::basename(img_filepath);
    string img_extension = fs::extension(img_filepath);
    string img_fixed_basename = img_basename + "_fixed";
    string img_dir = fs::path(img_filepath).parent_path().string();
    string img_fixed_filepath = img_dir + img_fixed_basename + img_extension;

    Mat img = imread(img_filepath, IMREAD_COLOR);

    int height = img.rows;
    int width = img.cols;

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


    Mat img_fixed = img.clone();

    String face_cascade_name = "./haarcascade_frontalface_alt.xml";
    String eyes_cascade_name = "./haarcascade_eye_tree_eyeglasses.xml";

    CascadeClassifier face_cascade;
    CascadeClassifier eyes_cascade;

    // Load detection cascade
    if (!face_cascade.load(face_cascade_name))
    {
        printf("Error loading face detection cascade!\n");
        return -1;
    };
    if (!eyes_cascade.load(eyes_cascade_name))
    {
        printf("Error loading eyes detection cascade!\n");
        return -1;
    };

    std::vector<Rect> eyes;

    eyes_cascade.detectMultiScale(img, eyes, 1.1, 3, 0, Size(height/50, width/50), Size(height/2, width/2));

    // Face detection could be employed to eliminated some false positives of eye detection. 
    // But this is not necessary.
    /*
    std::vector<Rect> faces;
    face_cascade.detectMultiScale(img, faces);
    if (faces.empty())
    {
        cout << "No face detected!" << endl;
    }
    */


    /*
    for( size_t j = 0; j < faces.size(); j++ )
    {
        Point center(faces[j].x + faces[j].width*0.5, faces[j].y + faces[j].height*0.5 );
        int radius = cvRound( (faces[j].width + faces[j].height)*0.25 );
        circle( img, center, radius, Scalar( 0, 255, 0 ), 4, 8, 0 );
    }


    for( size_t j = 0; j < eyes.size(); j++ )
    {
        Point center(eyes[j].x + eyes[j].width*0.5, eyes[j].y + eyes[j].height*0.5 );
        int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
        circle( img, center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );
    }
    */

    // Sometimes there will be eye decection false positives
    // One way to remove some false positives is to make sure that the center of eye is above the center of the face

    //namedWindow("Display Image", WINDOW_NORMAL );
    //imshow("Display Image", img);
    //waitKey(0);


    for (size_t j = 0; j < eyes.size(); j++)
    {
        Mat eye = img(eyes[j]);

        // Split image to BGR channels
        Mat bgr[3];
        split(eye, bgr);

        // Red eye detection
        // R > 150 and R > (B + G)
        // The satisfied region will be 255, and the rest will be 0.
        Mat mask = (bgr[2] > red_eye_threshold) & (bgr[2] > (bgr[1] + bgr[0]));

        // Fill holes in mask
        fillHoles(mask);

        // Dilate https://docs.opencv.org/4.0.1/d4/d86/group__imgproc__filter.html#ga4ff0f3318642c4f469d0e11f242f3b6c
        // The masked ROI becomes a little bit larger.
        // ROI is the red eye region.
        dilate(mask, mask, Mat(), Point(-1, -1), 3, 1, 1);

        // Fix red eye effect.
        Mat mean = (bgr[0] + bgr[1]) / 2;
        mean.copyTo(bgr[0], mask);
        mean.copyTo(bgr[1], mask);
        mean.copyTo(bgr[2], mask);

        Mat eye_fixed;
        merge(bgr, 3, eye_fixed);
        eye_fixed.copyTo(img_fixed(eyes[j]));
    }

    imwrite(img_fixed_filepath, img_fixed);

    namedWindow("Corrected Image", WINDOW_NORMAL);
    imshow("Corrected Image", img_fixed);
    waitKey(0);

    return 0;
}