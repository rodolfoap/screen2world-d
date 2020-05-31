#ifndef AUXMATH_H
#define AUXMATH_H 1

#include <algorithm>
#include <math.h>
#include <numeric>
#include <vector>

const float PI=3.14159265358979f;
const float DEG_PER_M=0.000008983156581409856519026824f;
template <class T> T RADTODEG(const T Radians) { return Radians * 180.0f / PI; }
template <class T> T DEGTORAD(const T Degrees) { return Degrees * PI / 180.0f; }
template <class T> float random(T min, T max) { return (T)rand()/(RAND_MAX)*(max-min)+min; }
// Rotation
template <class T>
cv::Point_<T> rotate(const T x, const T y, const T angle) {
	T s=sin(DEGTORAD(angle));
	T c=cos(DEGTORAD(angle));
	T xnew=x*c-y*s;
	T ynew=x*s+y*c;
	return cv::Point_<T>(xnew, ynew);
}
// Linear regression
template <class T>
std::pair<T, T> linereg(const std::vector<T>& x, const std::vector<T>& y) {
	const auto n   =x.size();
	const auto s_x =std::accumulate(x.begin(), x.end(), 0.0);
	const auto s_y =std::accumulate(y.begin(), y.end(), 0.0);
	const auto s_x2=std::inner_product(x.begin(), x.end(), x.begin(), 0.0);
	const auto s_xy=std::inner_product(x.begin(), x.end(), y.begin(), 0.0);
	const auto m   =(n*s_xy-s_x*s_y)/(n*s_x2-s_x*s_x);
	const auto b   =(s_y-m*s_x)/n;
	return std::pair<T, T>(m, b);
}
// Solution with linear regression formula
template <class T>
T lineres(const std::pair<T, T>& mb, T x){
	return (mb.first*x+mb.second);
}
#endif
