// EX5.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "CImg.h"
#include "noiseRemoval.h"
#include "contourDetect.h"
#include "voting.h"
#include "warping.h"
#include<iostream>
using namespace std;
using namespace cimg_library;

int main() {
	const char* filepath = "Dataset/3.bmp";//文件名字
	CImg<double> img(filepath), paint(filepath);//读入图片
	img = RemoveNoise(img);//模糊
	img = sobel(img); //边缘检测
	voting vote(img);
	//vector<pair<int, int>> line = vote.getLines();
	//int r = vote.getR();
	Warp(img, vote.getLines(), vote.getR(), paint);
}

