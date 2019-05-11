#pragma once
#include <string>

typedef unsigned char byte;

struct Color {
	byte r;
	byte g;
	byte b;

	Color() {
		r = 0;
		g = 0;
		b = 0;
	}

	Color(byte _r, byte _g, byte _b) {
		r = _r;
		g = _g;
		b = _b;
	}

	Color operator+(const Color& second) const {
		return Color(r + second.r, g + second.g, b + second.b);
	}

	Color operator*(double v) const {
		return Color(r * v, g * v, b * v);
	}
};

class Image {
	int width;
	int height;
	Color* image;
public:
	Image();
	Image(int w, int h);
	Image(const Image& image);
	~Image();

	Image operator=(const Image& image);

	int index(int i, int j) const;
	Color get(int i, int j) const;
	Color get(int index) const;
	void set(int i, int j, const Color& c);
	void set(int index, const Color& c);
	void set(int i, int j, byte r, byte g, byte b);
	void set(int index, byte r, byte g, byte b);

	Image stretch(int xScale, int yScale);
	Image shrink(int xScale, int yScale);

	void save(std::string name);
};