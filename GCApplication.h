#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"


#include <iostream>

using namespace std;
using namespace cv;

class GCApplication
{
public:
	enum{ NOT_SET = 0, IN_PROCESS = 1, SET = 2 };
	static const int radius = 2;  
	static const int thickness = -1;  

	void Init(const Mat& _image, const string& _winName);
	void showImage() const;           
	void mouseClick(int event, int x, int y, int flags, void* param);
	int nextIter();
	int getIterCount() const { return iterCount; }
	void getRect();
private:
	void setLblsInMask(int flags, Point p);

	const string* winName;
	const Mat* image;
	Mat mask;
	Mat bgdModel, fgdModel;

	uchar fgdState, bgdState;

	Rect rect;
	vector<Point> fgdPxls, bgdPxls;
	int iterCount;
	bool isInitialized;
};