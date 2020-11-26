#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <Windows.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <cstdlib>

using namespace std;
using namespace cv;

Mat hwnd2mat(HWND hwnd)
{
    HDC hwindowDC,hwindowCompatibleDC;

    int height,width,srcheight,srcwidth;
    HBITMAP hbwindow;
    Mat src;
    BITMAPINFOHEADER  bi;

    hwindowDC=GetDC(hwnd);
    hwindowCompatibleDC=CreateCompatibleDC(hwindowDC);
    SetStretchBltMode(hwindowCompatibleDC,COLORONCOLOR);

    RECT windowsize;    // get the height and width of the screen
    GetClientRect(hwnd, &windowsize);

    srcheight = windowsize.bottom;
    srcwidth = windowsize.right;
    height = windowsize.bottom/1;  //change this to whatever size you want to resize to
    width = windowsize.right/1;

    src.create(height,width,CV_8UC4);

    // create a bitmap
    hbwindow = CreateCompatibleBitmap( hwindowDC, width, height);
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
    StretchBlt( hwindowCompatibleDC, 0,0, width, height, hwindowDC, 0, 0,srcwidth,srcheight, SRCCOPY); //change SRCCOPY to NOTSRCCOPY for wacky colors !
    GetDIBits(hwindowCompatibleDC,hbwindow,0,height,src.data,(BITMAPINFO *)&bi,DIB_RGB_COLORS);  //copy from hwindowCompatibleDC to hbwindow

    // avoid memory leak
    DeleteObject (hbwindow);
    DeleteDC(hwindowCompatibleDC);
    ReleaseDC(hwnd, hwindowDC);

    return src;
}

int main(int argc, char **argv)
{
    HWND hwndDesktop = GetDesktopWindow();
    namedWindow("output",WINDOW_NORMAL);
    int key = 0;


char mainWindow[] = "Main";
	char trackbarWindow[] = "Trackbar";
	char thresholdWindow[] = "Threshold";
	int min = 0, max = 1000, min2 = 33;
	int count123;
	int hmin = 106, smin = 66, vmin = 28,
		hmax = 218, smax = 216, vmax = 90,
		hmin2 = 103, smin2 = 68, vmin2 = 25,
		hmax2 = 200, smax2 = 200, vmax2 = 101;
	Mat frame, HSV, threshold, blurred;

	//Создаем окна
	//namedWindow(mainWindow, 0);
	namedWindow(trackbarWindow, 0);
	//namedWindow(thresholdWindow, 0);

	//Создаем трэкбар
	createTrackbar("H min:", trackbarWindow, &hmin, hmax);
	createTrackbar("H max:", trackbarWindow, &hmax, hmax);
	createTrackbar("S min:", trackbarWindow, &smin, smax);
	createTrackbar("S max:", trackbarWindow, &smax, smax);
	createTrackbar("V min:", trackbarWindow, &vmin, vmax);
	createTrackbar("V max:", trackbarWindow, &vmax, vmax);
	createTrackbar("Size min:", trackbarWindow, &min, max);
	createTrackbar("Size max:", trackbarWindow, &max, max);

    createTrackbar("H2 min:", trackbarWindow, &hmin2, hmax2);
	createTrackbar("H2 max:", trackbarWindow, &hmax2, hmax2);
	createTrackbar("S2 min:", trackbarWindow, &smin2, smax2);
	createTrackbar("S2 max:", trackbarWindow, &smax2, smax2);
	createTrackbar("V2 min:", trackbarWindow, &vmin2, vmax2);
	createTrackbar("V2 max:", trackbarWindow, &vmax2, vmax2);


    while( key != 27 )
    {  //BGR    127 105 87       59 32 28      105 72 63
        Mat src = hwnd2mat(hwndDesktop);
        Mat out1 = hwnd2mat(hwndDesktop);

        cvtColor(src, out1, COLOR_BGR2HSV);

        inRange(out1, Scalar(hmin2, smin2, vmin2),Scalar(hmax2, smax2, vmax2),src);

        //-----------------------------------------------------------------------------------------

        for(int y = 834; y < 854; y++){
			for(int x = 1793; x < 1911; x++){  //x=1870 y= 800-924
				int value = src.at<uchar>(y, x);
				if(value == 255){
					Rect rect2;
					int count = floodFill(src, Point(x, y), Scalar(200), &rect2);
					//cout << "y= " << y << " x= " << x << ", rect size = " << rect.width <<  endl;
					if(rect2.width >= min2 && rect2.width <= max
						&& rect2.height >= min2 && rect2.height <= max){
						rectangle(src, rect2, Scalar(hmin2, smin2, vmin2, 4));
                        count123++;

					}
				}
			}

		//------------------------------------------------------------------------------------
		}
		//cout << count123 << endl;

        //-----------------------------------------------------------------------------------------
inRange(out1, Scalar(hmin, smin, vmin),Scalar(hmax, smax, vmax),src);

for(int y = 200; y < 650; y++){
			for(int x = 400; x < 1300; x++){
				int value = src.at<uchar>(y, x);
				if(value == 255){

					Rect rect;
					int count = floodFill(src, Point(x, y), Scalar(200), &rect);
					//cout << "y= " << y << " x= " << x << ", rect size = " << rect.width <<  endl;

					if(rect.width >= min && rect.width <= max
						&& rect.height >= min && rect.height <= max){
						rectangle(src, rect, Scalar(hmin, smin, vmin, 4));
                     if ((rect.width>10)&&(rect.width<19))
                        {
                            if (GetAsyncKeyState(VK_NUMPAD4))
                            {
                            return 0;
                            }

                            SetCursorPos(x, y);
                            INPUT iNPUT = {0};
                            iNPUT.type = INPUT_MOUSE;
                            iNPUT.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
                            SendInput(1,&iNPUT, sizeof(iNPUT));
                            ZeroMemory(&iNPUT, sizeof(iNPUT));
                            iNPUT.type = INPUT_MOUSE;
                            iNPUT.mi.dwFlags = MOUSEEVENTF_LEFTUP;
                            SendInput(1,&iNPUT, sizeof(iNPUT));

                            INPUT ip;
                            ip.type = INPUT_KEYBOARD;
                            ip.ki.wScan = 0;
                            ip.ki.time = 0;
                            ip.ki.dwExtraInfo = 0;

                            ip.ki.wVk = 0x34;
                            ip.ki.dwFlags = 0;
                            SendInput(1, &ip, sizeof(INPUT));

                            Sleep(500);

                            ip.ki.dwFlags = KEYEVENTF_KEYUP;
                            SendInput(1, &ip, sizeof(INPUT));
                            Sleep(2000);
                            //-------------------------------------

                        }
					}
				}
			}
		}

	POINT p;
	if (GetAsyncKeyState(VK_NUMPAD5))
                            {
                            GetCursorPos(&p);
                            cout << "x= " << p.x << " y= " << p.y << endl;
                            }

        if (count123<=2)
                {


                 if (GetAsyncKeyState(VK_NUMPAD4))
                            {
                            return 0;
                            }




                SetCursorPos(960, 580);
                GetCursorPos(&p);
                INPUT iNPUT = {0};
                iNPUT.type = INPUT_MOUSE;
                iNPUT.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
                SendInput(1,&iNPUT, sizeof(iNPUT));
                ZeroMemory(&iNPUT, sizeof(iNPUT));

                Sleep(500);
                SetCursorPos(p.x+800, p.y-50);
                Sleep(500);

                INPUT iNPUT2 = {0};
                iNPUT2.type = INPUT_MOUSE;
                iNPUT2.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
                SendInput(1,&iNPUT2, sizeof(iNPUT2));
                ZeroMemory(&iNPUT2, sizeof(iNPUT2));

                Sleep(1700);

                iNPUT2.type = INPUT_MOUSE;
                iNPUT2.mi.dwFlags = MOUSEEVENTF_LEFTUP;
                SendInput(1,&iNPUT2, sizeof(iNPUT2));

                iNPUT.type = INPUT_MOUSE;
                iNPUT.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
                SendInput(1,&iNPUT, sizeof(iNPUT));

                Sleep(500);

                SetCursorPos(650, 550);
                Sleep(500);
                iNPUT.type = INPUT_MOUSE;
                iNPUT.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
                SendInput(1,&iNPUT, sizeof(iNPUT));
                ZeroMemory(&iNPUT, sizeof(iNPUT));
                iNPUT.type = INPUT_MOUSE;
                iNPUT.mi.dwFlags = MOUSEEVENTF_LEFTUP;
                SendInput(1,&iNPUT, sizeof(iNPUT));
                Sleep(500);
                SetCursorPos(105, 365);
                iNPUT.type = INPUT_MOUSE;
                iNPUT.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
                SendInput(1,&iNPUT, sizeof(iNPUT));
                ZeroMemory(&iNPUT, sizeof(iNPUT));
                iNPUT.type = INPUT_MOUSE;
                iNPUT.mi.dwFlags = MOUSEEVENTF_LEFTUP;
                SendInput(1,&iNPUT, sizeof(iNPUT));
                Sleep(500);
                 for(int y = 570; y < 610; y++){
                        for(int x = 1605; x < 1900; x++){
                                SetCursorPos(x,y);
                                iNPUT.type = INPUT_MOUSE;
                                iNPUT.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
                                SendInput(1,&iNPUT, sizeof(iNPUT));
                                ZeroMemory(&iNPUT, sizeof(iNPUT));
                                iNPUT.type = INPUT_MOUSE;
                                iNPUT.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
                                SendInput(1,&iNPUT, sizeof(iNPUT));
                                x=x+34;
                                Sleep(50);
                                if (GetAsyncKeyState(VK_NUMPAD4))
                            {
                            return 0;
                            }
                        }
                        y=y+34;
                 }

                SetCursorPos(960, 580);
                GetCursorPos(&p);
                iNPUT.type = INPUT_MOUSE;
                iNPUT.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
                SendInput(1,&iNPUT, sizeof(iNPUT));
                ZeroMemory(&iNPUT, sizeof(iNPUT));

                Sleep(500);
                SetCursorPos(p.x-800, p.y-50);
                Sleep(500);

                iNPUT2.type = INPUT_MOUSE;
                iNPUT2.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
                SendInput(1,&iNPUT2, sizeof(iNPUT2));
                ZeroMemory(&iNPUT2, sizeof(iNPUT2));

                Sleep(1700);

                iNPUT2.type = INPUT_MOUSE;
                iNPUT2.mi.dwFlags = MOUSEEVENTF_LEFTUP;
                SendInput(1,&iNPUT2, sizeof(iNPUT2));

                iNPUT.type = INPUT_MOUSE;
                iNPUT.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
                SendInput(1,&iNPUT, sizeof(iNPUT));


                        Sleep(100);
                            INPUT ip;
                            ip.type = INPUT_KEYBOARD;
                            ip.ki.wScan = 0;
                            ip.ki.time = 0;
                            ip.ki.dwExtraInfo = 0;

                            ip.ki.wVk = 0x42;
                            ip.ki.dwFlags = 0;
                            SendInput(1, &ip, sizeof(INPUT));
                            Sleep(500);
                            ip.ki.dwFlags = KEYEVENTF_KEYUP;
                            SendInput(1, &ip, sizeof(INPUT));

                            Sleep(500);
                            ip.type = INPUT_KEYBOARD;
                            ip.ki.wScan = 0;
                            ip.ki.time = 0;
                            ip.ki.dwExtraInfo = 0;

                            ip.ki.wVk = 0x34;
                            ip.ki.dwFlags = 0;
                            SendInput(1, &ip, sizeof(INPUT));

                            Sleep(500);

                            ip.ki.dwFlags = KEYEVENTF_KEYUP;
                            SendInput(1, &ip, sizeof(INPUT));

                            Sleep(500);


        }

        count123=-1;
        // you can do some image processing here

        imshow("output", src);

        key = waitKey(60); // you can change wait time
    }

}
