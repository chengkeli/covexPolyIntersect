#ifndef _KL_COMMON_H_
#define _KL_COMMON_H_

//OpenCV
#include <opencv2/opencv.hpp>
#ifdef _DEBUG
#pragma comment(lib, "opencv_core243d.lib")
#pragma comment(lib, "opencv_highgui243d.lib")
#pragma comment(lib, "opencv_imgproc243d.lib")
#pragma comment(lib, "opencv_video243d.lib")
#pragma comment(lib, "opencv_objdetect243d.lib")
#else
#pragma comment(lib, "opencv_core243.lib")
#pragma comment(lib, "opencv_highgui243.lib")
#pragma comment(lib, "opencv_imgproc243.lib")
#pragma comment(lib, "opencv_video243.lib")
#pragma comment(lib, "opencv_objdetect243.lib")
#endif // _DEBUG

using namespace cv;

#endif