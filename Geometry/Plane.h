#pragma once
#include "GeometryPrimitive.h"

class Plane : public GeometryPrimitive {
public:
	Vector center;
	Vector normal;
	Vector edge;

	Plane();
	Plane(Vector center, Vector normal, Vector edge);

	virtual IntersectData intersect(const Line & line) override;

	virtual void precalc() override;

	virtual void initFromNode(const XML::Node* node) override;
};
