#include <iostream>
#include <vector>
#include "autocalib.hpp"
#include "yamlgen.hpp"

cv::Point2d Autocalib::m2d(const cv::Point2d m){ return cv::Point2d(m.x*DEG_PER_M, m.y*DEG_PER_M/cos(DEGTORAD(pivot.x))); }
void Autocalib::addbox(std::pair<cv::Point2d, cv::Point2d> box){ boxes.push_back(box); }

std::pair<cv::Point2d, cv::Point2d> Autocalib::createT(std::pair<cv::Point2d, cv::Point2d> box) {
	double Dy=sqrt(pow((HW.x*f/(box.second.x-box.first.x)), 2)-pow(h, 2));
	double L=HW.y*w/(box.second.y-box.first.y);
	cv::Point2d pointLDeg=m2d(rotate(Dy, -L/2., b))+pivot;
	cv::Point2d pointRDeg=m2d(rotate(Dy,  L/2., b))+pivot;
	return std::pair<cv::Point2d, cv::Point2d>(pointLDeg, pointRDeg);
}

void Autocalib::generatePointsMatrix(int depth_grid_size) {
	std::vector<double> x, y, p;
	for(auto pair=boxes.begin(); pair!=boxes.end(); ++pair){
		y.push_back(pair->second.x-pair->first.x);
		x.push_back(pair->second.y-pair->first.y);
		p.push_back(pair->second.x);
	}
	std::pair<double, double> rhght=linereg(p, y);
	std::pair<double, double> ratio=linereg(y, x);
	double  far=*std::min_element(p.begin(), p.end());
	double near=*std::max_element(p.begin(), p.end());
	YamlGen yaml;
	for(double v=far; v<(near+1); v+=(near-far)/(depth_grid_size-1)){
		double heght=lineres(rhght, v);
		double width=lineres(ratio, heght);
		std::pair<cv::Point2d, cv::Point2d> T=createT(std::pair<cv::Point2d, cv::Point2d>(cv::Point2d(v-heght, w/2.0), cv::Point2d(v, w/2.0+width)));
		yaml.addvlr(v, 0.0, w, v>(near-1.));
		yaml.addylr(T.first.y, T.first.x, T.second.y, T.second.x, v>(near-1.));
	}
	yaml.generate();
}
