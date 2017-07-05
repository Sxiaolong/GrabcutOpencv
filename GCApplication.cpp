#include "GCApplication.h"

const Scalar RED = Scalar(0, 0, 255);
const Scalar BLUE = Scalar(255, 0, 0);


static void getBinMask(const Mat& comMask, Mat& binMask)
{
	binMask.create(comMask.size(), CV_8UC1);
	binMask = comMask & 1;
}

void GCApplication::getRect()
{
	rect = Rect(44, 119, 325, 154);

	
	CV_Assert(!mask.empty());
	mask.setTo(GC_BGD);   
	rect.x = max(0, rect.x);
	rect.y = max(0, rect.y);
	rect.width = min(rect.width, image->cols - rect.x);
	rect.height = min(rect.height, image->rows - rect.y);
	(mask(rect)).setTo(Scalar(GC_PR_FGD)); 

	isInitialized = true;
}



void GCApplication::Init(const Mat& _image, const string& _winName)
{
	if (_image.empty() || _winName.empty())
		return;
	image = &_image;
	winName = &_winName;
	mask.create(image->size(), CV_8UC1);   

	bgdPxls.clear(); fgdPxls.clear();

	fgdState = NOT_SET;  
	bgdState = NOT_SET;  
	iterCount = 0;
}

void GCApplication::showImage() const
{
	if (image->empty() || winName->empty())
		return;

	Mat res;
	Mat binMask;
	
	if (!isInitialized)
		image->copyTo(res);
	else
	{
		getBinMask(mask, binMask);
		image->copyTo(res, binMask); 
	}

	vector<Point>::const_iterator it;
	for (it = bgdPxls.begin(); it != bgdPxls.end(); ++it)
	
		circle(res, *it, radius, BLUE, thickness);
	for (it = fgdPxls.begin(); it != fgdPxls.end(); ++it)
		circle(res, *it, radius, RED, thickness);

	imshow(*winName, res);
}


void GCApplication::setLblsInMask(int flags, Point p)
{
	vector<Point> *bpxls, *fpxls;

	bpxls = &bgdPxls;
	fpxls = &fgdPxls;

	
	if (flags &CV_EVENT_FLAG_RBUTTON)
	{
		bpxls->push_back(p);
	}
	
	if (flags & CV_EVENT_FLAG_LBUTTON)
	{
		fpxls->push_back(p);
	}
}



void GCApplication::mouseClick(int event, int x, int y, int flags, void*)
{
	// TODO add bad args check
	switch (event)
	{
	case EVENT_LBUTTONDOWN:
	{
		fgdState = IN_PROCESS;
	}
	break;
	case EVENT_RBUTTONDOWN:
	{
		bgdState = IN_PROCESS;
	}
	break;
	
	case EVENT_LBUTTONUP:
		
		if (fgdState == IN_PROCESS)
		{
			setLblsInMask(flags, Point(x, y));  
			fgdState = SET;
			showImage();
		}
		break;
	
	case EVENT_RBUTTONUP:
		
		if (bgdState == IN_PROCESS)
		{
			setLblsInMask(flags, Point(x, y)); 
			bgdState = SET;
			showImage();
		}
		break;
	
	case EVENT_MOUSEMOVE:
		if (fgdState == IN_PROCESS)
		{
			setLblsInMask(flags, Point(x, y));
			showImage();
		}
		else if (bgdState == IN_PROCESS)
		{
			setLblsInMask(flags, Point(x, y));
			showImage();
		}
		break;
	}
}


int GCApplication::nextIter()
{
	if (fgdState == SET || bgdState == SET)
		grabCut(*image, mask, rect, bgdModel, fgdModel, 1, GC_INIT_WITH_MASK);
	else
		
		grabCut(*image, mask, rect, bgdModel, fgdModel, 1, GC_INIT_WITH_RECT);

	iterCount++;

	bgdPxls.clear(); fgdPxls.clear();

	return iterCount;
}