#include <iostream>
#include <fstream>
#include "mustache.hpp"
#define PRECISION 16

class YamlGen{
private:
	kainjow::mustache::data data;
	kainjow::mustache::data coorduv=kainjow::mustache::data::type::list;
	kainjow::mustache::data coordxy=kainjow::mustache::data::type::list;

	int vlrrows=0, ylrrows=0;
public:
	std::string to_string_with_precision(const double value);
	void addvlr(double v, double l, double r, bool last);
	void addylr(double y1, double x1, double y2, double x2, bool last);
	std::string readFile(std::string name);
	void generate();
};
