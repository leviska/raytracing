#pragma once
#include "GeometryPrimitive.h"

class Vertex {
public:
	Vector pos;
	Vector norm;
	Color color;

	Vertex();
	Vertex(Vector pos, Color color);
};

//Utility class for faster intersections, not actual cuboid
class CuboidAA {
public:
	Vector a;
	Vector b;

	CuboidAA() = default;
	CuboidAA(Vector a, Vector b);

	//Checks only fact of collision
	IntersectData intersect(const Line & line);
	IntersectData intersect(const CuboidAA& cuboid);
};

class TriangleMesh : public GeometryPrimitive {
public:
	Vertex a;
	Vertex b;
	Vertex c;

	TriangleMesh();
	TriangleMesh(Vertex a, Vertex b, Vertex c);

	Vector getBarycentric(Vector p) const;

	virtual IntersectData intersect(const Line& line) override;

	virtual void precalc() override;

	virtual void initFromNode(const XML::Node* node) override;
};

struct Face {
	int a;
	int b;
	int c;

	Face() {
		a = 0;
		b = 0;
		c = 0;
	}

	Face(int _a, int _b, int _c) {
		a = _a;
		b = _b;
		c = _c;
	}
};

class Mesh : public GeometryPrimitive {
public:
	std::vector<Vertex> points;
	std::vector<Face> faces;

	std::vector<int> cubeDivision[CUBE_DIVISION][CUBE_DIVISION][CUBE_DIVISION];
	CuboidAA boundingBox;

	Mesh();
	Mesh(std::vector<Vertex> points, std::vector<Face> faces);

	virtual IntersectData intersect(const Line & line) override;

	void move(const Vector& shift);
	void rotate(const Vector& angle);
	void scale(const Vector& factor);
	void scale(double factor);

	void loadFromFile(std::string name);
	void createSphere(int hor, int vert, double r);
	void createCuboid(double x, double y, double z);

	virtual void precalc() override;

	virtual void initFromNode(const XML::Node* node) override;
};
