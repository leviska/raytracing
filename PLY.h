#pragma once
#include "Geometry.h"

#include <string>
#include <vector>

//only basic format (points and faces) without colors etc

class PLY {
	std::vector<Vertex> points;
	std::vector<Face> faces;
public:
	PLY();

	bool open(std::string fileName);
	std::vector<Vertex> getPoints();
	std::vector<Face> getFaces();
};