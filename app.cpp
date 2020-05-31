#include <iostream>
#include <vector>
#include <cmath>
#include <opencv2/opencv.hpp>
#include "auxmath.h"
#define LOG std::cerr<<">>> "<<__FILE__<<"["<<__LINE__<<"]:"<<__func__<<"();"<<std::endl;
#define GRID_SIZE 4 // T will be calculated for 5 points

class Autocalib {
private:
	double f, h, w, b;
	cv::Point2d HW;
	cv::Point2d pivot;
	std::vector<std::pair<cv::Point2d, cv::Point2d>> boxes, screenworld;

public:
	Autocalib() {}
	//    HW: person height & width in meters
	// pivot: camera Lat, Lon
	//     h: pole height in meters
	//     b: camera bearing
	//     f: focalY distance in pixels
	//     w: screen width, in pixels
	Autocalib(cv::Point2d HW, cv::Point2d pivot, double h, double b, double f, double w)
		{ init(HW, pivot, h, b, f, w); }
	void init(cv::Point2d _HW, cv::Point2d _pivot, double _h, double _b, double _f, double _w)
		{ HW=_HW; pivot=_pivot; h=_h; b=_b; f=_f; w=_w; }

	// Meters to degrees
	cv::Point2d m2d(const cv::Point2d m){ return cv::Point2d(m.x*DEG_PER_M, m.y*DEG_PER_M/cos(DEGTORAD(pivot.x))); }

	// Add detection box to boxes vector
	void addbox(std::pair<cv::Point2d, cv::Point2d> box){ boxes.push_back(box); }

	// Will create the T structure, read the paper.
	std::pair<cv::Point2d, cv::Point2d> createT(std::pair<cv::Point2d, cv::Point2d> box) {
		// Calculate distance camera-target
		double Dy=sqrt(pow((HW.x*f/(box.second.x-box.first.x)), 2)-pow(h, 2));
		// Calculate ground width at the foot of the box
		double L=HW.y*w/(box.second.y-box.first.y);
		// Left T point
		cv::Point2d pointLDeg=m2d(rotate(Dy, -L/2., b))+pivot;
		// Right T point
		cv::Point2d pointRDeg=m2d(rotate(Dy,  L/2., b))+pivot;
		// Returns the proper WGS84 T pair
		return std::pair<cv::Point2d, cv::Point2d>(pointLDeg, pointRDeg);
	}

	// Loops the vector to generate some T points
	void generatePairs() {
		std::vector<double> x, y, p;
		// Calculate formulae
		for(auto pair=boxes.begin(); pair!=boxes.end(); ++pair){
			y.push_back(pair->second.x-pair->first.x); // Used to calculate height linear regression
			x.push_back(pair->second.y-pair->first.y); // Used to calculate width linear regression
			p.push_back(pair->second.x);               // Relative to...
		}
		// Relative height (height depending on foot position in screen coords, v)
		std::pair<double, double> rhght=linereg(p, y);
		// Width to height ratio
		std::pair<double, double> ratio=linereg(y, x);
		// Box points are discarded, better to do a linear distribution
		double  far=*std::min_element(p.begin(), p.end());
		double near=*std::max_element(p.begin(), p.end());
		for(double v=far; v<(near+1); v+=(near-far)/GRID_SIZE){
			double heght=lineres(rhght, v);
			double width=lineres(ratio, heght);
			std::pair<cv::Point2d, cv::Point2d> T=createT(std::pair<cv::Point2d, cv::Point2d>(cv::Point2d(v-heght, w/2.0), cv::Point2d(v, w/2.0+width)));
			std::cerr<<"------------------"<<std::endl;
			std::cerr<<"       v: "<<v<<std::endl;
			std::cerr<<"  calc_h: "<<heght<<std::endl;
			std::cerr<<"  calc_w: "<<width<<std::endl;
			std::cerr<<" World_L: "<<T.first<<std::endl;
			std::cerr<<"Screen_L: "<<v<<", "<<0<<std::endl;
			std::cerr<<" World_R: "<<T.second<<std::endl;
			std::cerr<<"Screen_R: "<<v<<", "<<w<<std::endl;
		}
		std::cerr<<"------------------"<<std::endl;
	}
};

int main(int argc, char** args) {
	std::cerr.precision(16);
	std::cerr<<std::fixed;

	// Create the calibration object
	Autocalib ac(cv::Point2d(1.74, 0.87), cv::Point2d(43.571582, 1.425247), 5, 30, 921.43, 1280);

	// Add some detections
	ac.addbox(std::pair(cv::Point2d(270, 280), cv::Point2d(310, 300)));
	ac.addbox(std::pair(cv::Point2d(435, 300), cv::Point2d(515, 340)));
	ac.addbox(std::pair(cv::Point2d(600, 100), cv::Point2d(720, 160)));

	// Get the result
	ac.generatePairs();

	return 0;
}
