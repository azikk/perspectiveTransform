
#include "CImg.h"
using namespace cimg_library;

CImg<float> RemoveNoise(CImg<float> img) {
	CImg<float> temp(img.width(), img.height(), 1, 1);
//	img.display();
	cimg_forXY(img, x, y) {
		temp(x, y) = 0.299 * img(x, y, 0) + 0.587*img(x, y, 1) + 0.114*img(x, y, 2);
	}
	temp.blur_box(9);
	

	//temp.display();

	return temp;
}