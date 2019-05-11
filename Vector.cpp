#include "Vector.h"

#include <algorithm>
#include <cmath>

//-------------------------------------------
//Vector

Vector::Vector() {
	x = 0;
	y = 0;
	z = 0;
}

Vector::Vector(double _x, double _y, double _z) {
	x = _x;
	y = _y;
	z = _z;
}


Vector operator+(const Vector& first, const Vector& second) {
	return Vector(first.x + second.x, first.y + second.y, first.z + second.z);
}

Vector operator-(const Vector& first, const Vector& second) {
	return Vector(first.x - second.x, first.y - second.y, first.z - second.z);
}

Vector operator*(const Vector& first, double k) {
	return Vector(first.x * k, first.y * k, first.z * k);
}

Vector operator/(const Vector& first, double k) {
	return Vector(first.x / k, first.y / k, first.z / k);
}

Vector operator*(double k, const Vector& first) {
	return first * k;
}

Vector operator/(double k, const Vector& first) {
	return first / k;
}

double operator*(const Vector& first, const Vector& second) {
	return first.x * second.x + first.y * second.y + first.z * second.z;
}

Vector operator/(const Vector& first, const Vector& second) {
	return Vector(first.y * second.z - first.z * second.y, first.z * second.x - first.x * second.z, first.x * second.y - first.y * second.x);
}

double Vector::operator()() const {
	return sqrt(x * x + y * y + z * z);
}

double Vector::min() const {
	return std::min(x, std::min(y, z));
}

double Vector::max() const {
	return std::max(x, std::max(y, z));
}

double Vector::absmin() const {
	return std::min(std::abs(x), std::min(std::abs(y), std::abs(z)));
}

double Vector::absmax() const {
	return std::max(std::abs(x), std::max(std::abs(y), std::abs(z)));
}

bool Vector::zero() const {
	return x == 0 && y == 0 && z == 0;
}

Vector Vector::rotate(const Vector& axis, double angle) const {
	const Vector& v = *this;
	return v * cos(angle) + (axis / v) * sin(angle) + axis * (axis * v * (1 - cos(angle)));
}

Vector Vector::rotate(const Vector& Ax, const Vector& Ay, const Vector& Az) const {
	return Vector(Ax.x*x + Ax.y*y + Ax.z*z, Ay.x*x + Ay.y*y + Ay.z*z, Az.x*x + Az.y*y + Az.z*z);
}

void Vector::generateRotationMatrix(Vector & Ax, Vector & Ay, Vector & Az) const {
	double cosa = std::cos(z);
	double sina = std::sin(z);

	double cosb = std::cos(y);
	double sinb = std::sin(y);

	double cosc = std::cos(x);
	double sinc = std::sin(x);

	Ax.x = cosa*cosb;
	Ax.y = cosa*sinb*sinc - sina*cosc;
	Ax.z = cosa*sinb*cosc + sina*sinc;

	Ay.x = sina*cosb;
	Ay.y = sina*sinb*sinc + cosa*cosc;
	Ay.z = sina*sinb*cosc - cosa*sinc;

	Az.x = -sinb;
	Az.y = cosb*sinc;
	Az.z = cosb*cosc;
}

std::ostream& operator<<(std::ostream& out, const Vector& first) {
	out << first.x << " " << first.y << " " << first.z;
	return out;
}


Vector Vector::makeUnit() const {
	return Vector(*this) / (*this)();
}
