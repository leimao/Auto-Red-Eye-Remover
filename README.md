# Auto Red Eye Remover

Lei Mao

## Introduction

This is a simple automatic red eye remover implemented using OpenCV C++. It uses a naive red eye detection and correction algorithm. More complicated algorithms could be found in literatures. 

## Dependencies

* GNU g++ 5.5.0 +
* CMake 3.5.1 +
* OpenCV 4.0.0 +
* Boost 1.58 +

## Usage

```bash
./main <image_filepath> [threshold]
```

The red eye removed image will be saved in the same directory to the image provided. 

``threshold`` is optional. But sometimes red eye removal will fail or the image after removal looks abnormal due to the default threshold is not appropriate. Try a value between 0 to 255 to see whether an optimal removal could be achieved. 

The algorithm uses eye detection algorithm to first detect eyes. If eye detection fails on the provided image, no red eye removal will happen. 

## Demo

Raw | Processed |
:-------------------------:|:-------------------------:
![](images/sample_1.jpg) | ![](images/sample_1_fixed.jpg)


## Reference

https://www.learnopencv.com/automatic-red-eye-remover-using-opencv-cpp-python/

http://www.ijeert.org/pdf/v2-i2/10.pdf