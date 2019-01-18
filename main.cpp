#include <opencv2/core.hpp>
//#include <opencv2/core/utility.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

using namespace std;
using namespace cv;


int main(int argc, char * argv[])
{
    // Red image
    // Mat img = imread(argv[1], IMREAD_COLOR);
    //Mat img = imread("./BoldRedEye.jpeg", IMREAD_COLOR);
    Mat img = imread("./test2.jpg", IMREAD_COLOR);
    // cout << img << endl;

    // namedWindow("Display Image", WINDOW_NORMAL );
    // imshow("Display Image", img);
    // waitKey(0);

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
    std::vector<Rect> faces;

    //eyes_cascade.detectMultiScale( img, eyes, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30) );
    eyes_cascade.detectMultiScale( img, eyes );
    face_cascade.detectMultiScale( img, faces, 1.1, 2, 0);

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

    namedWindow("Display Image", WINDOW_NORMAL );
    imshow("Display Image", img);
    waitKey(0);






    return 0;
}