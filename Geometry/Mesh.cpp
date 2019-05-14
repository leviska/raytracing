#include "Mesh.h"
#include "Plane.h"
#include "../PLY.h"

#include <cmath>
#include <algorithm>
#include <iostream>

//-------------------------------------------
//Vertex

Vertex::Vertex() {

}

Vertex::Vertex(Vector _pos, Color _color) {
	pos = _pos;
	color = _color;
}


//-------------------------------------------
//Axis-Aligned Cuboid

CuboidAA::CuboidAA(Vector _a, Vector _b) {
	a = _a;
	b = _b;
}

IntersectData CuboidAA::intersect(const Line & line) {
	IntersectData res;
	double txmin = (std::min(a.x, b.x) - line.org.x) / line.dir.x;
	double txmax = (std::max(a.x, b.x) - line.org.x) / line.dir.x;
	double tymin = (std::min(a.y, b.y) - line.org.y) / line.dir.y;
	double tymax = (std::max(a.y, b.y) - line.org.y) / line.dir.y;

	if (txmin > txmax) std::swap(txmin, txmax);
	if (tymin > tymax) std::swap(tymin, tymax);
	if ((txmin > tymax) || (tymin > txmax)) {
		return res;
	}

	if (tymin > txmin)
		txmin = tymin;
	if (tymax < txmax)
		txmax = tymax;

	double tzmin = (std::min(a.z, b.z) - line.org.z) / line.dir.z;
	double tzmax = (std::max(a.z, b.z) - line.org.z) / line.dir.z;

	if (tzmin > tzmax) std::swap(tzmin, tzmax);
	if ((txmin > tzmax) || (tzmin > txmax)) {
		return res;
	}

	res.koef = 1;
	return res;
}

IntersectData CuboidAA::intersect(const CuboidAA & s) {
	IntersectData res;
	if (!(b.x < s.a.x || s.b.x < a.x || b.y < s.a.y || s.b.y < a.y || b.z < s.a.z || s.b.z < a.z)) {
		res.koef = 1;
	}
	return res;
}


//-------------------------------------------
//TriangleMesh

TriangleMesh::TriangleMesh() {
	doubleSided = false;
	material = Material::None;
}

TriangleMesh::TriangleMesh(Vertex _a, Vertex _b, Vertex _c) {
	a = _a;
	b = _b;
	c = _c;

	doubleSided = false;
}

Vector TriangleMesh::getBarycentric(Vector p) const {
	Vector answ;
	Vector v0 = b.pos - a.pos;
	Vector v1 = c.pos - a.pos;
	Vector v2 = p - a.pos;
	double d00 = v0 * v0;
	double d01 = v0 * v1;
	double d11 = v1 * v1;
	double d20 = v2 * v0;
	double d21 = v2 * v1;
	double denom = d00 * d11 - d01 * d01;
	answ.y = (d11 * d20 - d01 * d21) / denom;
	answ.z = (d00 * d21 - d01 * d20) / denom;
	answ.x = 1 - answ.y - answ.z;
	return answ;
}

int sign(double v) {
	return (std::abs(v) < eps ? 0 : (v < 0 ? -1 : 1));
}

IntersectData TriangleMesh::intersect(const Line & line) {
	IntersectData data;
	Plane plane(a.pos, ((b.pos - a.pos) / (c.pos - b.pos)).makeUnit(), Vector());
	IntersectData res = plane.intersect(line);
	if (res.koef >= 0) {
		Vector point = line.org + res.koef * line.dir;
		Vector arr[4] = { a.pos, b.pos, c.pos, a.pos };
		int sgn = 0;
		bool inside = true;
		for (int i = 0; i < 3; i++) {
			int cur = sign(((point - arr[i]) / (arr[i + 1] - arr[i])) * res.normal);
			if (sgn == 0) {
				sgn = cur;
			}
			if (sgn * cur < 0) {
				inside = false;
			}
		}
		if (inside) {
			data = res;
			Vector q = getBarycentric(point);

			data.color = a.color * q.x + b.color * q.y + c.color * q.z;
			data.normal = a.norm * q.x + b.norm * q.y + c.norm * q.z;
			data.normal = data.normal.makeUnit();
		}
	}
	return data;
}

void TriangleMesh::precalc() {
}

void TriangleMesh::initFromNode(const XML::Node* node) {
}


//-------------------------------------------
//Mesh

Mesh::Mesh() {
	doubleSided = false;
	material = Material::None;

	for (int i = 0; i < CUBE_DIVISION; i++) {
		for (int j = 0; j < CUBE_DIVISION; j++) {
			for (int k = 0; k < CUBE_DIVISION; k++) {
				cubeDivision[i][j][k] = std::vector<int>();
			}
		}
	}
}

Mesh::Mesh(std::vector<Vertex> _points, std::vector<Face> _faces) : Mesh() {
	points = _points;
	faces = _faces;
	doubleSided = false;
}

IntersectData Mesh::intersect(const Line& line) {
	IntersectData data;
	if (faces.size() > 0 && boundingBox.intersect(line).koef > 0) {
		if (faces.size() < 200) {
			for (int i = 0; i < faces.size(); i++) {
				TriangleMesh triangle(points[faces[i].a], points[faces[i].b], points[faces[i].c]);
				IntersectData temp = triangle.intersect(line);
				if (temp.koef > eps && (data.id < 0 || temp.koef < data.koef - eps)) {
					data = temp;
					data.id = i;
				}
			}
		}
		else {
			Vector boxSize = (boundingBox.b - boundingBox.a + Vector(eps, eps, eps)) / CUBE_DIVISION;
			for (int i = 0; i < CUBE_DIVISION; i++) {
				for (int j = 0; j < CUBE_DIVISION; j++) {
					for (int k = 0; k < CUBE_DIVISION; k++) {
						if (cubeDivision[i][j][k].size()) {
							CuboidAA bb;
							bb.a = boundingBox.a;
							bb.a.x += boxSize.x * i;
							bb.a.y += boxSize.y * j;
							bb.a.z += boxSize.z * k;
							bb.b = boundingBox.a;
							bb.b.x += boxSize.x * (i + 1);
							bb.b.y += boxSize.y * (j + 1);
							bb.b.z += boxSize.z * (k + 1);
							if (bb.intersect(line).koef > 0) {
								for (int p = 0; p < cubeDivision[i][j][k].size(); p++) {
									int face = cubeDivision[i][j][k][p];
									TriangleMesh triangle(points[faces[face].a], points[faces[face].b], points[faces[face].c]);
									IntersectData temp = triangle.intersect(line);
									if (temp.koef > eps && (data.id < 0 || temp.koef < data.koef - eps)) {
										data = temp;
										data.id = i;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return data;
}

void Mesh::move(const Vector & shift) {
	for (size_t i = 0; i < points.size(); i++) {
		points[i].pos = points[i].pos + shift;
	}
}

void Mesh::rotate(const Vector & angle) {
	Vector Ax, Ay, Az;
	angle.generateRotationMatrix(Ax, Ay, Az);
	for (size_t i = 0; i < points.size(); i++) {
		points[i].pos = points[i].pos.rotate(Ax, Ay, Az);
	}
}

void Mesh::scale(const Vector & factor) {
	for (size_t i = 0; i < points.size(); i++) {
		points[i].pos.x *= factor.x;
		points[i].pos.y *= factor.z;
		points[i].pos.z *= factor.z;
	}
}

void Mesh::scale(double factor) {
	scale(Vector(factor, factor, factor));
}

void Mesh::loadFromFile(std::string name) {
	PLY file;
	if (!file.open(name)) {
		throw std::runtime_error("Error loading file " + name);
	}
	points = file.getPoints();
	faces = file.getFaces();
}

void Mesh::createSphere(int M, int N, double r) {
	points.clear();
	faces.clear();
	std::vector<std::vector<int>> id(M + 1, std::vector<int>(N));
	for (int i = 0; i <= M; i++) {
		for (int j = 0; j < N; j++) {
			Vertex v;
			v.pos = Vector(sin(M_PI * i / M) * cos(M_PI * 2 * j / N), sin(M_PI * i / M) * sin(M_PI * 2 * j / N), cos(M_PI * i / M));
			v.color = Color(255, 255, 255);
			v.pos = v.pos * r;
			id[i][j] = points.size();
			points.push_back(v);
			if (i == 0 || i == M) {
				break;
			}
		}
	}
	for (int j = 0; j < N; j++) {
		id[M][j] = points.size() - 1;
	}
	for (int i = 1; i <= M; i++) {
		for (int j = 0; j < N; j++) {
			Face f;
			f.a = id[i][j];
			f.c = id[i - 1][(j + 1) % N];
			if (i < M) {
				f.b = id[i][(j + 1) % N];
				faces.push_back(f);
			}
			if (i > 1) {
				f.b = id[i - 1][j];
				std::swap(f.b, f.c);
				faces.push_back(f);
			}
		}
	}
}

void Mesh::createCuboid(double x, double y, double z) {
	points.clear();
	faces.clear();
	points.push_back(Vertex(Vector(0, 0, 0), Color(255, 255, 255)));
	points.push_back(Vertex(Vector(x, 0, 0), Color(255, 255, 255)));
	points.push_back(Vertex(Vector(x, y, 0), Color(255, 255, 255)));
	points.push_back(Vertex(Vector(0, y, 0), Color(255, 255, 255)));
	points.push_back(Vertex(Vector(0, 0, z), Color(255, 255, 255)));
	points.push_back(Vertex(Vector(x, 0, z), Color(255, 255, 255)));
	points.push_back(Vertex(Vector(x, y, z), Color(255, 255, 255)));
	points.push_back(Vertex(Vector(0, y, z), Color(255, 255, 255)));
	faces.push_back(Face(0, 2, 1));
	faces.push_back(Face(0, 3, 2));
	faces.push_back(Face(4, 5, 6));
	faces.push_back(Face(4, 6, 7));
	faces.push_back(Face(0, 1, 4));
	faces.push_back(Face(1, 5, 4));
	faces.push_back(Face(0, 4, 3));
	faces.push_back(Face(3, 4, 7));
	faces.push_back(Face(2, 3, 6));
	faces.push_back(Face(3, 7, 6));
	faces.push_back(Face(1, 2, 5));
	faces.push_back(Face(2, 6, 5));
}

void Mesh::precalc() {
	//Phong shading model
	std::vector<std::vector<Vector>> normals(points.size());
	for (int i = 0; i < points.size(); i++) {
		points[i].norm = Vector();
	}
	for (int i = 0; i < faces.size(); i++) {
		Vector norm = ((points[faces[i].b].pos - points[faces[i].a].pos) / (points[faces[i].c].pos - points[faces[i].a].pos)).makeUnit();
		normals[faces[i].a].push_back(norm);
		normals[faces[i].b].push_back(norm);
		normals[faces[i].c].push_back(norm);
	}
	for (int i = 0; i < normals.size(); i++) {
		std::vector<Vector> temp;
		for (int j = 0; j < normals[i].size(); j++) {
			Vector& a = normals[i][j];
			bool good = 1;
			for (int k = j + 1; k < normals[i].size(); k++) {
				Vector& b = normals[i][k];
				if (std::abs(b.x - a.x) < eps && std::abs(b.y - a.y) < eps && std::abs(b.z - a.z) < eps) {
					good = 0;
					break;
				}
			}
			if (good) {
				temp.push_back(a);
			}
		}
		normals[i] = temp;
	}
	for (int i = 0; i < normals.size(); i++) {
		for (int j = 0; j < normals[i].size(); j++) {
			points[i].norm = points[i].norm + normals[i][j];
		}
		if (normals[i].size()) {
			points[i].norm = points[i].norm.makeUnit();
		}
	}

	auto bbGen = [](CuboidAA& bb, Vector& v) {
		bb.a.x = std::min(bb.a.x, v.x);
		bb.a.y = std::min(bb.a.y, v.y);
		bb.a.z = std::min(bb.a.z, v.z);
		bb.b.x = std::max(bb.b.x, v.x);
		bb.b.y = std::max(bb.b.y, v.y);
		bb.b.z = std::max(bb.b.z, v.z);
	};

	//Object bounding box
	if (points.size()) {
		boundingBox.a = boundingBox.b = points[0].pos;
		for (int i = 0; i < points.size(); i++) {
			bbGen(boundingBox, points[i].pos);
		}
	}

	//Cube division
	if (faces.size() >= 200) {
		Vector boxSize = (boundingBox.b - boundingBox.a + Vector(eps, eps, eps)) / CUBE_DIVISION;
		for (int p = 0; p < faces.size(); p++) {
			CuboidAA bb;
			bb.a = bb.b = points[faces[p].a].pos;
			bbGen(bb, points[faces[p].b].pos);
			bbGen(bb, points[faces[p].c].pos);
			bb.a = bb.a - boundingBox.a;
			bb.b = bb.b - boundingBox.a;
			int si = bb.a.x / boxSize.x;
			int sj = bb.a.y / boxSize.y;
			int sk = bb.a.z / boxSize.z;
			int ei = bb.b.x / boxSize.x;
			int ej = bb.b.y / boxSize.y;
			int ek = bb.b.z / boxSize.z;
			for (int i = si; i <= ei; i++) {
				for (int j = sj; j <= ej; j++) {
					for (int k = sk; k <= ek; k++) {
						cubeDivision[i][j][k].push_back(p);
					}
				}
			}
		}
	}
}

void Mesh::initFromNode(const XML::Node* node) {
	GeometryPrimitive::initFromNode(node);
	std::string fileName = node->attr("file");
	loadFromFile(fileName);
	Vector shift;
	shift.x = node->attrd("shiftX");
	shift.y = node->attrd("shiftY");
	shift.z = node->attrd("shiftZ");
	move(shift);
}
