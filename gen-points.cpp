#include <opencv2/opencv.hpp>
#include "autocalib.hpp"
#include "screen2worldK.hpp"
#define LOG std::cerr<<">>> "<<__FILE__<<"["<<__LINE__<<"]:"<<__func__<<"();"<<std::endl;

#include <iostream>
#include <fstream>
#include <iomanip>
#include <unistd.h>
#include <set>

// The only required lines are the commented ones,
// the rest is just the process of reading the CSV file.
int main(int argc, char** args) {
	std::string detLine;
	std::istringstream ss;
	std::ifstream detectionFile;
	char ch;
	int id, dummy;
	float x1, y1, x2, y2;

	if(argc<1) { std::cerr<<"Usage: "<<args[0]<<" [CSV_DETECTION_FILE INTRINSICS_MATRIX.yaml]"<<std::endl; exit(1); }

	// Create the calibration object
	Autocalib ac(cv::Point2d(1.74, 0.87), cv::Point2d(43.571582, 1.425247), 5, 30, 921.43, 1280);

	std::cerr<<"Processing "<<args[1]<<"..."<<std::endl;
	detectionFile.open(args[1]);
	if (!detectionFile.is_open()) { std::cerr<<"Error: cannot find "<<args[1]<<std::endl; return 1; }
	while (getline(detectionFile, detLine)) {
		ss.str(detLine);
		ss>>id>>ch>>dummy>>ch>>x1>>ch>>y1>>ch>>x2>>ch>>y2;
		ss.str("");

		// Add detections for calibration
		ac.addbox(std::pair(cv::Point2d(y1, x1), cv::Point2d(y2, x2)));
	}
	std::cerr<<"Done. Generating matrix..."<<std::endl;
	detectionFile.close();

	// Get the result, 5 pairs
	ac.generatePointsMatrix(5);

	// Generate extrinsics
	char filename[]="points_matrix.yaml";
	Screen2World s2w(args[2], filename);

	std::cerr<<"Done."<<std::endl;
	return 0;
}

