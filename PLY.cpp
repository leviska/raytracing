#include "PLY.h"

#include <fstream>

PLY::PLY() : points(), faces() {

}

bool PLY::open(std::string fileName) {
	points.clear();
	faces.clear();
	std::ifstream in(fileName);
	if (!in.good() || in.eof() || in.fail()) {
		return false;
	}
	int pointsSize = 0;
	int facesSize = 0;
	std::string s;
	bool colored = false;
	while (s != "element") {
		in >> s;
	}
	in >> s >> pointsSize;
	while (s != "element" && s != "red") {
		in >> s;
	}
	if (s == "red") {
		colored = true;
		while (s != "element") {
			in >> s;
		}
	}
	in >> s >> facesSize;
	while (s != "end_header") {
		in >> s;
	}
	points.reserve(pointsSize);
	for (int i = 0; i < pointsSize; i++) {
		Vertex v;
		in >> v.pos.x >> v.pos.y >> v.pos.z;
		if (colored) {
			int r, g, b;
			in >> r >> g >> b;
			v.color.r = r;
			v.color.g = g;
			v.color.b = b;
		}
		else {
			v.color.r = v.color.g = v.color.b = 255;
		}
		points.push_back(v);
	}
	faces.reserve(facesSize);
	for (int i = 0; i < facesSize; i++) {
		Face f;
		in >> f.a >> f.a >> f.b >> f.c;
		faces.push_back(f);
	}
	return true;
}

std::vector<Vertex> PLY::getPoints() {
	return points;
}

std::vector<Face> PLY::getFaces() {
	return faces;
}
