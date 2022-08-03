#pragma once

class class_OpenCV_cusmtom
{

public:
	class_OpenCV_cusmtom(void);
	void custom_imshow(Mat image);
	Mat ImshowTest(); // OpenCV 제대로 나
	Mat ImageShow();
	LPVOID main;

	Mat hconcat_vconcat(Mat image_1, Mat image_2);


	CImage cimage_mfc;
	void DisplayImage_BitBit(Mat Displayimage);

};
