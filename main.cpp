#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "ComputeTime.h"
#include "GCApplication.h"

#include <iostream>

using namespace std;
using namespace cv;

GCApplication gcapp;

// �����Ӧ����
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
	// ����һ�����ڣ��ڶ�����������ΪWINDOW_AUTOSIZE��ʾ���ڴ�С���Զ���������Ӧ����ʾ��ͼ�񣬲��Ҳ����ֶ��ı䴰�ڴ�С��
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
		case '\x1b':    // Esc�����˳�
			cout << "Exiting ..." << endl;
			// ������ת�����
			goto exit_main;
		case 'n':    // ��һ�ε�����

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