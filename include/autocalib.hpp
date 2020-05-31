#include <iostream>
#include <vector>
#include <cmath>
#include <opencv2/opencv.hpp>
#include "auxmath.hpp"
#define LOG std::cerr<<">>> "<<__FILE__<<"["<<__LINE__<<"]:"<<__func__<<"();"<<std::endl;

class Autocalib {
private:
	double f, h, w, b;
	cv::Point2d HW;
	cv::Point2d pivot;
	std::vector<std::pair<cv::Point2d, cv::Point2d>> boxes, screenworld;

public:
	Autocalib() {}
	Autocalib(cv::Point2d HW, cv::Point2d pivot, double h, double b, double f, double w)
		{ init(HW, pivot, h, b, f, w); }
	void init(cv::Point2d _HW, cv::Point2d _pivot, double _h, double _b, double _f, double _w)
		{ HW=_HW; pivot=_pivot; h=_h; b=_b; f=_f; w=_w; }

	cv::Point2d m2d(const cv::Point2d m);
	void addbox(std::pair<cv::Point2d, cv::Point2d> box);
	std::pair<cv::Point2d, cv::Point2d> createT(std::pair<cv::Point2d, cv::Point2d> box);
	void generatePointsMatrix(int depth_grid_size);

};
