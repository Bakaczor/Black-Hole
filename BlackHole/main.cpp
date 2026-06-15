#include "SceneManager.h"

int main(int argc, char* argv[]) {
	SceneManager scene;
	if (0 == scene.init()) {
		return scene.run();
	}
	return 0;
}