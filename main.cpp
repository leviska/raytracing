#include "Utils/Image.h"
#include "Config.h"

int main() {
	//Just in case to generate random scenes
	srand(time_t(0));

	Config config;
	config.load("config.xml");
	config.render();

	return 0;
}
