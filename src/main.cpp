#include "transform/matrix.hpp"
#include "context.h"
#include "rasterize/rasterize.h"
#include "app/depth_test.h"
#include "app/phong_shading.h"
#include "lib/SDL_wrapper.h"
#include "SDL/SDL.h"

#include <ctime>

using SoftwareRenderer::Transform::Matrix44f;
using namespace SoftwareRenderer::Rasterize;
using std::cout;

static const int SCR_WIDTH = 512, SCR_HEIGHT = 512;
static bool running = false;
Context context;
static Scene* scenes[] = { nullptr, nullptr };
static Scene* currentScene;
static int currentSceneIndex = 0;

static bool Initialize();
static void Clean();

int main(int argc, char* argv[])
{
	(void)argc;
	(void)argv;

	if (!Initialize()) {
		return EXIT_FAILURE;
	}

	//float tmp[] = {
	//	1.0f, 0.0f, 0.0f, 0.0f,
	//	0.0f, 0.7071068f, -0.7071068f, 0.0f,
	//	0.0f, 0.7071068f, 0.7071068f, 0.0f,
	//	0.0f, 0.0f, 0.0f, 1.0f
	//};
	//Matrix44f m(tmp);
	//Matrix44f m1 = m.transposed();
	//cout << m * m1 << std::endl;

	currentScene = scenes[0] = new DepthTest();
	scenes[1] = new PhongShading();

	clock_t clocks;
	int frames = 0;
	clocks = clock();
	while (running) {
		Predraw();

		/*struct VertexPC<float> v0 { { 256.0f, 384.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } };
		struct VertexPC<float> v1 { { 128.0f, 128.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } };
		struct VertexPC<float> v2 { { 512-128.0f, 128.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } };
		Triangle<float>(v0, v1, v2);*/
		scenes[currentSceneIndex]->Render();

		Postdraw();
		HandleEvents();

		frames++;
		if (clock() - clocks >= CLOCKS_PER_SEC * 2) {
			std::cout << frames / 2.0 << " fps" << std::endl;
			frames = 0;
			clocks = clock();
		}
	}

	delete scenes[0];
	delete scenes[1];

	return 0;
}

bool Initialize()
{
	bool result = true;

	result = result && CreateWindow(SCR_WIDTH, SCR_HEIGHT);
	running = result;
	if (result) {
		atexit(Clean);
	}

	onQuit = [](void)->void {
		running = false;
	};
	onLeftKeyDown = [](void)->void {
		currentScene->LeftKeyDown();
	};
	onRightKeyDown = [](void)->void {
		currentScene->RightKeyDown();
	};
	onUpKeyDown = [](void)->void {
		currentScene->UpKeyDown();
	};
	onDownKeyDown = [](void)->void {
		currentScene->DownKeyDown();
	};
	onCKeyDown = [](void)->void {
		currentScene->CKeyDown();
	};
	onDKeyDown = [](void)->void {
		currentScene->DKeyDown();
	};
	onSpaceKeyDown = [&](void)->void {
		currentSceneIndex = (currentSceneIndex + 1) % 2;
		currentScene = scenes[currentSceneIndex];
	};

	return result;
}

void Clean()
{
	Clear();
}
