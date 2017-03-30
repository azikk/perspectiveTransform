#include "CImg.h"
#include<vector>
#include <thread>
#include "matrix.h"
#define pi 3.1415926
using namespace std;
using namespace cimg_library;


vector<pair<int, int>> point;
CImg<float> ouputline;
int r_max;



void drawLine(int s, int p, CImg<float> img) {
	cout << "draw" << s << " " << p << endl;

	double step = pi / (double)500; //近似值
	float q = -cos(step*s) / sin(step*s);


	if (abs(q) < 1) {
		cimg_forX(img, x) {
			int y = (int)(p - r_max) / sin(step*s) - x*cos(step*s) / sin(step*s);
			//	cout << y << endl;
			if (y >= 0 && y + 1 < img.height()) {
				if (ouputline(x, y) == 255) {
					bool isPoint = true;
					for (int i = 0; i < point.size(); i++) {
						if (abs(x - point[i].first) < 20 && abs(y - point[i].second) < 20) {
							isPoint = false;
						}
					}
					if (isPoint)
						point.push_back(make_pair(x, y));
				}
				ouputline(x, y) = 255;
				ouputline(x, y + 1) = 255;


			}
		}
	}

	if (abs(q) > 1) {
		cimg_forY(img, y) {
			int x = (int)(p - r_max) / cos(step*s) - y*sin(step*s) / cos(step*s);
			//	cout << y << endl;
			if (x >= 0 && x + 1 < img.width()) {
				if (ouputline(x, y) == 255) {
					bool isPoint = true;
					for (int i = 0; i < point.size(); i++) {
						if (abs(x - point[i].first) < 100 && abs(y - point[i].second) < 100) {
							isPoint = false;
						}
					}
					if (isPoint)
						point.push_back(make_pair(x, y));
				}

				ouputline(x, y) = 255;
				ouputline(x + 1, y) = 255;

			}
		}
	}
}


void displayPaint() {
	ouputline.display();
}

void Warp(CImg<double> img, vector<pair<int, int>> lines, int r, CImg<double> paint) {
	//vector<pair<int, int>> beforeLines = lines;

	CImg<float> temp(img.width(), img.height(), 1, 1);

	ouputline = temp;
	r_max = r;

	ouputline.fill(0);


	int s1 = lines[0].first;
	for (int i = 1; i < 4; i++) {
		if (abs(lines[i].first - s1) < 100 || abs((500 - lines[i].first) + s1) < 100) {
			if (i == 1) break;
			else {
				pair<int, int> temp = lines[i];
				lines[i] = lines[1];
				lines[1] = temp;
				break;
			}
		}
	}

	for (int i = 0; i < 4; i++) {
		cout << lines[i].first << " " << lines[i].second << endl;
		drawLine(lines[i].first, lines[i].second, img);
	}

	ouputline = paint;

	unsigned char color[4][3] = {{ 255, 255, 255 }, { 255, 0, 0 }, { 0,255,0 }, {0, 0, 255}};
	for (int i = 0; i < 4; i++) {
		string str = i + "";
		const char* cs = str.c_str();
		ouputline.draw_circle(point[i].first, point[i].second, 50, color[i]);
	//	ouputline.draw_text(point[i].first, point[i].second, cs, colo);
		cout << point[i].first << " " << point[i].second << endl;
	}
//	paint.display();
	//ouputline = paint;
	//thread t1(displayPaint);

	cout << "开始一点都不智能的交互过程- -" << endl;
	cout << "请输入点的编号以确定裁剪后的A4纸如何布局。对应颜色序号为白0 红1 绿2 蓝3" << endl;
	cout << "白0 红1 绿2 蓝3" << endl;
	cout << "白0 红1 绿2 蓝3" << endl;
	cout << "白0 红1 绿2 蓝3" << endl;
	cout << "重要事情说四次" << endl;
	cout << "其中第一个点对应A4纸的左上角，第二个点对应右上角，第三个点对应右下角。" << endl;
	cout << "例子： 如现在又四个点颜色对应编号为 白0 红1 绿2 蓝3, 输入顺序为1 2 3 0" << endl;
	cout << "       最后输出图像布局为  1    2" << endl;
	cout << "                           0    3" << endl;
	cout << "这样制作是出于拍摄图片可能角度异于天人……可能骨骼惊奇……好吧我只是想不到比较通用只能的对应方法" << endl;
	cout << "请输入(我们暂时没有是否合法的判断）："; //错误检测

	displayPaint();

	int num[4] = { 2, 3, 1, 0 };
	for (int i = 0; i < 4; i++) {
		cin >> num[i];
	}


	vector<pair<int, int>> P ,newPoint; //P 变换后标准A4的点  newPoint 变换前的点   计算时候计算变换后变换到变换前
	
	for (int i = 0; i < 4; i++) {
		newPoint.push_back(point[num[i]]);
	}

	double len1 = sqrt((newPoint[0].first - newPoint[1].first)*(newPoint[0].first - newPoint[1].first) + (newPoint[0].second - newPoint[1].second) * (newPoint[0].second - newPoint[1].second));
	double len2 = sqrt((newPoint[0].first - newPoint[3].first)*(newPoint[0].first - newPoint[3].first) + (newPoint[0].second - newPoint[3].second) * (newPoint[0].second - newPoint[3].second));

	if (len1 > len2) {
		len2 = len1 * 210 / 297;
	}
	else {
		len2 = len1 * 297 / 210;
	}

	

	CImg<double> outputimg((int)len1, (int)len2, 1, 3);  //标准A4大小的纸

	P.push_back(make_pair(0, 0));
	P.push_back(make_pair((int)len1, 0));
	P.push_back(make_pair((int)len1, (int)len2));
	P.push_back(make_pair(0, (int)len2));

	int uv[8] = { newPoint[0].first, newPoint[0].second,
		newPoint[1].first, newPoint[1].second,
		newPoint[2].first, newPoint[2].second,
		newPoint[3].first, newPoint[3].second };
	
	double src[8][8] =
	{ { P[0].first, P[0].second, 1, 0, 0, 0, -newPoint[0].first*P[0].first, -newPoint[0].first*P[0].second },
	{ 0, 0, 0, P[0].first, P[0].second, 1, -newPoint[0].second*P[0].first, -newPoint[0].second*P[0].second },

	{ P[1].first, P[1].second, 1, 0, 0, 0, -newPoint[1].first*P[1].first, -newPoint[1].first*P[1].second },
	{ 0, 0, 0, P[1].first, P[1].second, 1, -newPoint[1].second*P[1].first, -newPoint[1].second*P[1].second },

	{ P[2].first, P[2].second, 1, 0, 0, 0, -newPoint[2].first*P[2].first, -newPoint[2].first*P[2].second },
	{ 0, 0, 0, P[2].first, P[2].second, 1, -newPoint[2].second*P[2].first, -newPoint[2].second*P[2].second },

	{ P[3].first, P[3].second, 1, 0, 0, 0, -newPoint[3].first*P[3].first, -newPoint[3].first*P[3].second },
	{ 0, 0, 0, P[3].first, P[3].second, 1, -newPoint[3].second*P[3].first, -newPoint[3].second*P[3].second } };



	double matrix_after[N][N]{};
	bool flag = GetMatrixInverse(src, N, matrix_after);
	if (false == flag) {
		cout << "求不出系数" << endl;
		return;
	}


	cout << "逆矩阵：" << endl;

	for (int i = 0; i<8; i++)
	{
		for (int j = 0; j<8; j++)
		{
			cout << matrix_after[i][j] << " ";
			//cout << *(*(matrix_after+i)+j)<<" ";
		}
		cout << endl;
	}

	double xs[8];
	for (int i = 0; i < 8; i++) {
		double sum = 0;
		for (int t = 0; t < 8; t++) {
			sum += matrix_after[i][t] * uv[t];
		}
		xs[i] = sum;
	}

	cimg_forXY(outputimg, x, y) {
		double px = xs[0] * x + xs[1] * y + xs[2];
		double py = xs[3] * x + xs[4] * y + xs[5];
		double p = xs[6] * x + xs[7] * y + 1;

		int u = px / p;
		int v = py / p;

		outputimg(x, y, 0) = paint(u, v, 0);
		outputimg(x, y, 1) = paint(u, v, 1);
		outputimg(x, y, 2) = paint(u, v, 2);
	}

	outputimg.display();
	outputimg.save("save/test.bmp");
	


	return;


}

