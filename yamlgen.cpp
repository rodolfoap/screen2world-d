#include "yamlgen.hpp"

std::string YamlGen::to_string_with_precision(const double value) {
	std::ostringstream out;
	out.precision(PRECISION);
	out<<std::fixed<<value;
	return out.str();
}

void YamlGen::addvlr(double v, double l, double r, bool last) {
	vlrrows+=2;
	kainjow::mustache::data pair;
	pair.set("x", to_string_with_precision(l));
	pair.set("y", to_string_with_precision(v));
	pair.set("last", false);
	coorduv<<kainjow::mustache::data{pair};
	pair.set("x", to_string_with_precision(r));
	pair.set("y", to_string_with_precision(v));
	pair.set("last", last);
	coorduv<<kainjow::mustache::data{pair};
}

void YamlGen::addylr(double y1, double x1, double y2, double x2, bool last) {
	ylrrows+=2;
	kainjow::mustache::data pair;
	pair.set("x", to_string_with_precision(x1));
	pair.set("y", to_string_with_precision(y1));
	pair.set("last", false);
	coordxy<<kainjow::mustache::data{pair};
	pair.set("x", to_string_with_precision(x2));
	pair.set("y", to_string_with_precision(y2));
	pair.set("last", last);
	coordxy<<kainjow::mustache::data{pair};
}

std::string YamlGen::readFile(std::string name){
	std::ifstream ifs(name, std::ios::binary);
	std::stringstream buffer;
	buffer<<ifs.rdbuf();
	ifs.close();
	return buffer.str();
}

void YamlGen::generate(){
	data.set("rows", std::to_string(vlrrows));
	data.set("coorduv", coorduv);
	data.set("rows", std::to_string(ylrrows));
	data.set("coordxy", coordxy);
	kainjow::mustache::mustache tpl{readFile("points-matrix.tpl")};
	// Generate yaml file
	std::ofstream yaml;
        yaml.open ("points_matrix.yaml");
        yaml<<tpl.render(data);
        yaml.close();
}
