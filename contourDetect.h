#include "CImg.h"
#include<iostream>
using namespace std;
using namespace cimg_library;

CImg<double> sobel(CImg<double> img) {
	CImg<float> output(img.width(), img.height(), 1, 1);
	output.fill(0);
	double thres = 50;
	int sobelx[3][3] = { { -1, 0, 1 }, { -2, 0, 2 }, { -1, 0, 1 } };
	int sobely[3][3] = { { 1, 2, 1 }, { 0, 0, 0 }, { -1, -2, -1 } };

	/*for (int i = -1; i < 2; i++) {
		for (int t = -1; t < 2; t++) {
			cout << sobelx[i + 1][t + 1] << endl;
		}
	}*/
	
	for (int x = 1; x < img.width() - 1; x++) {
		for (int y = 1; y < img.height() - 1; y++) {
			double gx = 0;
			double gy = 0;
			for (int i = -1; i < 2; i++) {
				for (int t = -1; t < 2; t++) {
					gx += img(x + i, y + t) * sobelx[i + 1][t + 1];
					gy += img(x + i, y + t) * sobely[i + 1][t + 1];
				}
			}
			double sum = abs(gx) + abs(gy);
			if (sum > 50) {
				output(x, y) = 255;
			}
		}
	}

	//output.save("save/sobel-6.bmp");
	return output;
}

