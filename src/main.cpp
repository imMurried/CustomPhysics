#include "Physics_Walkthrough_App.h"

int main() {
	
	auto app = new Physics_Walkthrough_App();
	app->run("AIE", 1280, 720, false);
	delete app;

	return 0;
}