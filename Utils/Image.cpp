#include "Image.h"

#include <fstream>

Image::Image() {
	image = nullptr;
}

Image::Image(int w, int h) {
	width = w;
	height = h;
	image = new Color[w * h];
	for (int i = 0; i < w * h; i++) {
		image[i].r = 0;
		image[i].g = 0;
		image[i].b = 0;
	}
}

Image::Image(const Image & image) : Image(image.width, image.height) {
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			set(i, j, image.get(i, j));
		}
	}
}

Image::~Image() {
	delete[] image;
}

Image Image::operator=(const Image & img) {
	width = img.width;
	height = img.height;
	image = new Color[img.width * img.height];
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			set(i, j, img.get(i, j));
		}
	}
	return *this;
}

int Image::index(int i, int j) const {
	return j * width + i;
}

Color Image::get(int i, int j) const {
	return get(index(i, j));
}

Color Image::get(int index) const {
	return image[index];
}

void Image::set(int i, int j, const Color& c) {
	set(index(i, j), c);
}

void Image::set(int index, const Color& c) {
	image[index] = c;
}

void Image::set(int i, int j, byte r, byte g, byte b) {
	set(index(i, j), r, g, b);
}

void Image::set(int index, byte r, byte g, byte b) {
	image[index].r = r;
	image[index].g = g;
	image[index].b = b;
}

Image Image::stretch(int xScale, int yScale) {
	if (xScale <= 0 || yScale <= 0) {
		return Image();
	}
	Image res(width * xScale, height * yScale);
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			for (int k = 0; k < xScale; k++) {
				for (int l = 0; l < yScale; l++) {
					res.set(xScale * i + k, yScale * j + l, get(i, j));
				}
			}
		}
	}
	return res;
}

Image Image::shrink(int xScale, int yScale) {
	if (xScale <= 0 || yScale <= 0) {
		return Image();
	}
	Image res(width / xScale, height / yScale);
	for (int i = 0; i < res.width; i++) {
		for (int j = 0; j < res.height; j++) {
			int count = 0;
			int r = 0, g = 0, b = 0;
			for (int k = 0; k < xScale && i * xScale + k < width; k++) {
				for (int l = 0; l < yScale && j * yScale + l < height; l++) {
					Color c = get(i * xScale + k, j * yScale + l);
					r += c.r;
					g += c.g;
					b += c.b;
					count++;
				}
			}
			r /= count;
			g /= count;
			b /= count;
			res.set(i, j, Color(r, g, b));
		}
	}
	return res;
}

void Image::save(std::string name) {
	std::ofstream imageFile(name, std::ios::binary);
	imageFile << "P6\n" << width << " " << height << "\n" << "255\n";
	for (int i = 0; i < width * height; i++) {
		imageFile.write((char*)&(image[i].r), sizeof(char));
		imageFile.write((char*)&(image[i].g), sizeof(char));
		imageFile.write((char*)&(image[i].b), sizeof(char));
	}
	imageFile.close();
}
