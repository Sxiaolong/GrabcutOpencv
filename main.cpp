#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "ComputeTime.h"
#include "GCApplication.h"

#include <iostream>

using namespace std;
using namespace cv;

GCApplication gcapp;

// 鼠标响应函数
static void on_mouse(int event, int x, int y, int flags, void* param)
{
	gcapp.mouseClick(event, x, y, flags, param);
}

int main(int argc, char** argv)
{
	string filename = "test.jpg";
	Mat image = imread(filename, 1);
	if (image.empty())
	{
		cout << "\n Durn, couldn't read image filename " << filename << endl;
		return 1;
	}


	const string winName = "image";
	// 建立一个窗口，第二个参数设置为WINDOW_AUTOSIZE表示窗口大小会自动调整以适应所显示的图像，并且不能手动改变窗口大小。
	namedWindow(winName, WINDOW_AUTOSIZE);
	setMouseCallback(winName, on_mouse, 0);

	gcapp.Init(image, winName);
	gcapp.showImage();

	gcapp.getRect();


	for (;;)
	{
		int c = waitKey(0);
		switch ((char)c)
		{
		case '\x1b':    // Esc键，退出
			cout << "Exiting ..." << endl;
			// 无条件转移语句
			goto exit_main;
		case 'n':    // 下一次迭代。

			int iterCount = gcapp.getIterCount();
			ComputeTime ct;
			ct.Begin();
			cout << "<" << iterCount << "... ";
			
			int newIterCount = gcapp.nextIter();
			gcapp.showImage();

			cout << ct.End() << ">" << endl;
		
			break;
		}
	}

exit_main:
	destroyWindow(winName);
	return 0;
}