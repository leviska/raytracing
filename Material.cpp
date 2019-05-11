#include "Material.h"

const Material Material::None = { Vector(0.2, 0.2, 0.2), Vector(1, 1, 1), Vector(0, 0, 0), 10, 0, 0, 0, Color(255, 255, 255) };
const Material Material::Ruby = { Vector(0.1745, 0.001175, 0.001175), Vector(0.61424, 0.04136, 0.4136), Vector(0.727811, 0.629959, 0.629959), 76.8, 0, 0.8, 1.51, Color(155, 17, 30) };
const Material Material::Chrome = { Vector(0.25, 0.25, 0.25), Vector(0.4, 0.4, 0.4), Vector(0.774597, 0.774597, 0.774597), 76.8, 0.8, 0, 0, Color(188, 198, 204) };
const Material Material::Silver = { Vector(0.19225, 0.19225, 0.19225), Vector(0.50754, 0.50754, 0.50754), Vector(0.508273, 0.508273, 0.508273), 51.2, 0, 0, 0, Color(192, 192, 192) };
const Material Material::Gold = { Vector(0.24725, 0.1995, 0.1995), Vector(0.75164, 0.60648, 0.22648), Vector(0.628281, 0.555802, 0.366065), 51.2, 0.1, 0, 0, Color(255, 223, 40) };
const Material Material::Plastic = { Vector(0, 0, 0), Vector(0.01, 0.01, 0.01), Vector(0.5, 0.5, 0.5), 32, 0, 0, 0, Color(50, 50, 50) };
const Material Material::Rubber = { Vector(0.02, 0.02, 0.02), Vector(0.01, 0.01, 0.01), Vector(0.4, 0.4, 0.4), 10, 0, 0, 0, Color(50, 50, 50) };
const Material Material::Glass = { Vector(0.1, 0.1, 0.1), Vector(0.6, 0.6, 0.6), Vector(0.5, 0.5, 0.5), 80, 0.1, 1, 1.51, Color(255, 255, 255) };