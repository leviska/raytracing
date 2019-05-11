#pragma once
#include <vector>

#include "Image.h"
#include "Material.h"
#include "Vector.h"

const double eps = 1e-6;
const int CUBE_DIVISION = 8;

class Vertex {
public:
	Vector pos;
	Vector norm;
	Color color;

	Vertex();
	Vertex(Vector pos, Color color);
};

struct IntersectData {
	double koef;
	Vector normal;
	Color color;
	int id;

	IntersectData() : normal(), color() {
		koef = -1;
		id = -1;
	}
};

class Line;


class GeometryPrimitive {
public:
	Material material;
	bool doubleSided;

	virtual IntersectData intersect(const Line& line) = 0;
	virtual void precalc() = 0;
};

class Line : public GeometryPrimitive {
public:
	Vector org;
	Vector dir;

	Line();
	Line(Vector org, Vector dir);

	virtual IntersectData intersect(const Line & line) override;

	virtual void precalc() override;
};

class Sphere : public GeometryPrimitive {
public:
	Vector center;
	double r;

	Sphere();
	Sphere(Vector center, double r);

	virtual IntersectData intersect(const Line & line) override;

	virtual void precalc() override;
};

//Utility class for faster intersections, not actual cuboid
class CuboidAA : public GeometryPrimitive {
public:
	Vector a;
	Vector b;

	CuboidAA();
	CuboidAA(Vector a, Vector b);

	//Checks only fact of collision
	virtual IntersectData intersect(const Line & line) override;
	IntersectData intersect(const CuboidAA& cuboid);

	virtual void precalc() override;
};

class Plane : public GeometryPrimitive {
public:
	Vector center;
	Vector normal;
	Vector edge;

	Plane();
	Plane(Vector center, Vector normal, Vector edge);

	virtual IntersectData intersect(const Line & line) override;

	virtual void precalc() override;
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
};