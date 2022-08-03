#include "pch.h"
#include "OpencvMat.h"
class_OpenCV_cusmtom::class_OpenCV_cusmtom(void)
{
	cout << "test" << endl;
}



void class_OpenCV_cusmtom::custom_imshow(Mat image)
{
	image = image * 0.5;
	imshow("test", image);
}

Mat class_OpenCV_cusmtom::ImshowTest()
{
	CString strFolderPath;
	strFolderPath = "res\\aloeL.jpg";

	cv::Mat img; // 이미지 처리를 위해 Mat 클래스 선언
	img = cv::imread(string(CT2CA(strFolderPath))); // 이미지 읽기

	// 이미지가 없을 경우 예외 처리
	if (img.empty())
	{
		std::cerr << "Image load failed" << std::endl;
		return Mat();
	}

	cv::namedWindow("image"); // 이미지를 보여주기 위한 빈 창
	cv::imshow("image", img); // "image"라는 이름의 창에 이미지를 넣어 보여줌


	return img;
}

Mat class_OpenCV_cusmtom::ImageShow()
{
	CString strFolderPath;
	strFolderPath = "res\\aloeL.jpg";

	cv::Mat img; // 이미지 처리를 위해 Mat 클래스 선언
	img = cv::imread(string(CT2CA(strFolderPath))); // 이미지 읽기

	// 이미지가 없을 경우 예외 처리
	if (img.empty())
	{
		std::cerr << "Image load failed" << std::endl;
		return Mat();
	}


	return img;

	//	DisplayImage_BitBit(img);
}

Mat class_OpenCV_cusmtom::hconcat_vconcat(Mat imgl, Mat imgr)
{
	// resize 2 images to small (540x270)
	Mat img1, img2;
	resize(imgl, img1, Size(540, 270));
	resize(imgr, img2, Size(540, 270));

	// create new 2 image more
	Mat img3, img4;
	img3 = img1 + img2;
	img4 = 0.5 * img1 + 0.5 * img2;

	if (!imgl.empty() || !imgr.empty())
	{
		Mat main;
		Mat h1, h2;

		hconcat(img1, img2, h1);
		hconcat(img3, img4, h2);
		vconcat(h1, h2, main);
		imshow("main", main);
		waitKey(0);
	}
	else
	{
		cout << "image not found " << CV_VERSION << endl;
	}
	return Mat();
}

void class_OpenCV_cusmtom::DisplayImage_BitBit(Mat Displayimage)
{
	COpenCVMFCDlg* pMainDlg = (COpenCVMFCDlg*)AfxGetMainWnd();
	//화면에 보여주기 위한 처리입니다.
	Mat mat_frame = Displayimage.clone();

	int bpp = 8 * mat_frame.elemSize();
	assert((bpp == 8 || bpp == 24 || bpp == 32));

	int padding = 0;
	//32 bit image is always DWORD aligned because each pixel requires 4 bytes
	if (bpp < 32)
		padding = 4 - (mat_frame.cols % 4);

	if (padding == 4)
		padding = 0;

	int border = 0;
	//32 bit image is always DWORD aligned because each pixel requires 4 bytes
	if (bpp < 32)
	{
		border = 4 - (mat_frame.cols % 4);
	}

	Mat mat_temp;
	if (border > 0 || mat_frame.isContinuous() == false)
	{
		// Adding needed columns on the right (max 3 px)
		cv::copyMakeBorder(mat_frame, mat_temp, 0, 0, 0, border, cv::BORDER_CONSTANT, 0);
	}
	else
	{
		mat_temp = mat_frame;
	}

	RECT r;
	pMainDlg->m_Picture_1.GetClientRect(&r);
	cv::Size winSize(r.right, r.bottom);

	cimage_mfc.Create(winSize.width, winSize.height, 24);


	BITMAPINFO* bitInfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFO) + 256 * sizeof(RGBQUAD));
	bitInfo->bmiHeader.biBitCount = bpp;
	bitInfo->bmiHeader.biWidth = mat_temp.cols;
	bitInfo->bmiHeader.biHeight = -mat_temp.rows;
	bitInfo->bmiHeader.biPlanes = 1;
	bitInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitInfo->bmiHeader.biCompression = BI_RGB;
	bitInfo->bmiHeader.biClrImportant = 0;
	bitInfo->bmiHeader.biClrUsed = 0;
	bitInfo->bmiHeader.biSizeImage = 0;
	bitInfo->bmiHeader.biXPelsPerMeter = 0;
	bitInfo->bmiHeader.biYPelsPerMeter = 0;


	//그레이스케일 인경우 팔레트가 필요
	if (bpp == 8)
	{
		RGBQUAD* palette = bitInfo->bmiColors;
		for (int i = 0; i < 256; i++)
		{
			palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;
			palette[i].rgbReserved = 0;
		}
	}


	// Image is bigger or smaller than into destination rectangle
	// we use stretch in full rect
	if (mat_temp.cols == winSize.width && mat_temp.rows == winSize.height)
	{
		// source and destination have same size
		// transfer memory block
		// NOTE: the padding border will be shown here. Anyway it will be max 3px width

		SetDIBitsToDevice(cimage_mfc.GetDC(),
			//destination rectangle
			0, 0, winSize.width, winSize.height,
			0, 0, 0, mat_temp.rows,
			mat_temp.data, bitInfo, DIB_RGB_COLORS);
	}
	else
	{
		// destination rectangle
		int destx = 0, desty = 0;
		int destw = winSize.width;
		int desth = winSize.height;

		// rectangle defined on source bitmap
		// using imgWidth instead of mat_temp.cols will ignore the padding border
		int imgx = 0, imgy = 0;
		int imgWidth = mat_temp.cols - border;
		int imgHeight = mat_temp.rows;

		StretchDIBits(cimage_mfc.GetDC(),
			destx, desty, destw, desth,
			imgx, imgy, imgWidth, imgHeight,
			mat_temp.data, bitInfo, DIB_RGB_COLORS, SRCCOPY);
	}


	HDC dc = ::GetDC(pMainDlg->m_Picture_1.m_hWnd);
	cimage_mfc.BitBlt(dc, 0, 0);


	::ReleaseDC(pMainDlg->m_Picture_1.m_hWnd, dc);

	cimage_mfc.ReleaseDC();
	cimage_mfc.Destroy();

}

Mat class_OpenCV_cusmtom::hwnd2mat(HWND hwnd)
{
	HDC hwindowDC, hwindowCompatibleDC;

	int height, width, srcheight, srcwidth;
	HBITMAP hbwindow;
	Mat src;
	BITMAPINFOHEADER  bi;

	hwindowDC = GetDC(hwnd);
	hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
	SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

	RECT windowsize;    // get the height and width of the screen
	GetClientRect(hwnd, &windowsize);

	srcheight = windowsize.bottom;
	srcwidth = windowsize.right;
	height = windowsize.bottom / 1;  //change this to whatever size you want to resize to
	width = windowsize.right / 1;

	src.create(height, width, CV_8UC4);

	// create a bitmap
	hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);
	bi.biSize = sizeof(BITMAPINFOHEADER);    //http://msdn.microsoft.com/en-us/library/windows/window/dd183402%28v=vs.85%29.aspx
	bi.biWidth = width;
	bi.biHeight = -height;  //this is the line that makes it draw upside down or not
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	// use the previously created device context with the bitmap
	SelectObject(hwindowCompatibleDC, hbwindow);
	// copy from the window device context to the bitmap device context
	StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, 0, 0, srcwidth, srcheight, SRCCOPY); //change SRCCOPY to NOTSRCCOPY for wacky colors !
	GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, src.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);  //copy from hwindowCompatibleDC to hbwindow

	// avoid memory leak
	DeleteObject(hbwindow);
	DeleteDC(hwindowCompatibleDC);
	ReleaseDC(hwnd, hwindowDC);

	return src;
}


