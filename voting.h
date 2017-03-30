#include "CImg.h"
using namespace cimg_library;
#include<vector>
using namespace std;

class voting {
private:
	CImg<float> hough;
	CImg<float> img;
	int r_max;
	int* map[500];
	float threshold;
	int max_hough;
	CImg<int> ouputline;
	CImg<float> forme;
	vector<pair<int, int >> point;
	vector<pair<int, int >> lines;
	CImg<float> pointmap;
public:
	voting(CImg<float>);
	void toHough();
	void Calthreshold(int, int);
	void drawLine(int, int, int);
	vector<pair<int, int >> getPoints();
	vector<pair<int, int >> getLines();
	int getR();
};