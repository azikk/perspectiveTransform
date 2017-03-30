#include "stdafx.h"

#include "voting.h"
#include<math.h>
#include<iostream>
#include<vector>

using namespace std;
#define pi 3.1415926




int voting::getR() {
	return r_max;
}


vector<pair<int, int >> voting::getLines() {
	return lines;
}


vector<pair<int, int >> voting::getPoints() {
	return point;
}

voting::voting(CImg<float> in) { //hough voting
	r_max = sqrt(in.width() * in.width() + in.height()*in.height()); //最大R距离 消耗资源↑ 需要考虑
	CImg<float> temp(500, 2*r_max, 1, 1); //储存结果矩阵
	for (int k = 0; k < 500; k++) { //二维数组分布初始化 
		map[k] = new int[2*r_max]();
	}

	CImg<int> output(in.width(), in.height(), 1, 1);
	CImg<float> p(in.width(), in.height(), 1, 1);

	pointmap = p;
	pointmap.fill(-1);
	ouputline = output;
	ouputline.fill(0);

	hough = temp;
	img = in;
	forme = in;
	hough.fill(0);
	threshold = 0.5;

	toHough();
	
}

void voting::toHough() {
	double step = pi / (double)500; //近似值
	cout << "step:" << step << endl;
	int row, col;
	cimg_forXY(img, x, y) {
		if (img(x, y) > 0) {
			cimg_forX(hough, i) { //i = sita, t = p
				int r = (int)x*cos(i*step) + y*sin(i*step); //这个值可能为负数
				r += r_max;
				if (r >= 0 && r < 2*r_max) {
					hough(i, r)++;
					map[i][r]++;
					if (map[i][r] > max_hough) {
						max_hough = map[i][r];
						row = i;
						col = r; //记录第一条线
					}
				}
			}
		}
	}

	cout << endl;
	cout << max_hough << endl;
	Calthreshold(row, col);
	//hough.display("hough");
//	hough.save("save/hough-5.bmp");
}

void voting::Calthreshold(int r, int c) {
	int thres = (int)(max_hough * threshold);
	
	for (int row = 0; row < 500; row++) {
		for (int col = 0; col < 2 * (int)r_max; col++) {

			bool newLines = true;
			int temp = map[row][col];

			if (map[row][col] > thres) {

				for (int k = 0; k < lines.size(); k++) {
					//判断极值
					if ((abs(lines[k].first - row) < 30|| abs((500 - lines[k].first) + row) < 30) && abs(lines[k].second - col) < 1300) {
						if (map[row][col] > map[lines[k].first][lines[k].second]) {
							lines[k].first = row;
							lines[k].second = col;
						}
						newLines = false;
					}
				}

				if (newLines) {
					lines.push_back(make_pair(row, col));
					//cout << "push " << row << " "<< col << endl;
				}
			}

		}
	}

	for (int k = 0; k < 4; k++) {
		cout << "line" << k + 1 << " " << lines[k].first << " " << lines[k].second << endl;
	}

	/*
	for (int k = 0; k < 4; k++) {
		drawLine(lines[k].first, lines[k].second, k);
	}

	
	ouputline.display();
	ouputline.save_bmp("save/line.bmp");

	unsigned char purple[] = { 255, 0, 255 };
	for (int k = 0; k < point.size(); k++) {
		forme.draw_circle(point[k].first, point[k].second, 20, purple);
	}

	forme.display();
	forme.save("save/point.bmp");
	*/
}
	

void voting::drawLine(int s, int p, int k) {
	cout << "draw" << s << " " << p << endl;

	double step = pi / (double)500; //近似值
	float q = -cos(step*s) / sin(step*s);

	
	if (abs(q) < 1) {
		cimg_forX(img, x) {
			int y = (int)(p - r_max) / sin(step*s) - x*cos(step*s) / sin(step*s);
			//	cout << y << endl;
			if (y >= 0 && y+1 < img.height()) {
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
			if (x >= 0 && x+1 < img.width()) {
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
					ouputline(x+1, y) = 255;
	
			}
		}
	}
}
