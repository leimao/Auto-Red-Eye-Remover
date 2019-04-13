#include "utils.h"

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

Mat removeRedEye(Mat &img, int red_eye_threshold)
{
    int height = img.rows;
    int width = img.cols;

    Mat img_fixed = img.clone();

    //string face_cascade_name = "./models/haarcascade_frontalface_alt.xml";
    //string eyes_cascade_name = "./models/haarcascade_eye_tree_eyeglasses.xml";
    string face_cascade_name = MODEL_FACE_FILEPATH;
    string eyes_cascade_name = MODEL_EYE_FILEPATH;

    CascadeClassifier eyes_cascade;

    // Load detection cascade.
    assert(eyes_cascade.load(eyes_cascade_name) && "Eyes detection model loading failed.");

    /*
    CascadeClassifier face_cascade;

    assert(face_cascade.load(face_cascade_name) && "Face detection model loading failed.");

    if (!face_cascade.load(face_cascade_name))
    {
        printf("Error loading face detection cascade!\n");
        return -1;
    };
    if (!eyes_cascade.load(eyes_cascade_name))
    {
        printf("Error loading eyes detection cascade!\n");
        return -1;
    };*/

    vector<Rect> eyes;

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

    return img_fixed;
}
