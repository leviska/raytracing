#pragma once
#include <ostream>

class Vector {
public:
	double x;
	double y;
	double z;

	Vector();
	Vector(double x, double y, double z);

	friend Vector operator+(const Vector& first, const Vector& second);
	friend Vector operator-(const Vector& first, const Vector& second);
	friend Vector operator*(const Vector& first, double k);
	friend Vector operator/(const Vector& first, double k);
	friend Vector operator*(double k, const Vector& first);
	friend Vector operator/(double k, const Vector& first);
	friend double operator*(const Vector& first, const Vector& second);
	friend Vector operator/(const Vector& first, const Vector& second);
	double operator()() const;
	friend std::ostream& operator<<(std::ostream& out, const Vector& first);

	double min() const;
	double max() const;
	double absmin() const;
	double absmax() const;

	bool zero() const;

	Vector rotate(const Vector& axis, double angle) const;
	Vector rotate(const Vector& Ax, const Vector& Ay, const Vector& Az) const;

	void generateRotationMatrix(Vector& Ax, Vector& Ay, Vector& Az) const;

	Vector makeUnit() const;
};