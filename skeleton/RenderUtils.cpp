#include <vector>

#include "PxPhysicsAPI.h"

//#include "core.hpp"
#include "RenderUtils.hpp"
#include "ScreenSizeConstants.hpp";
#include <iostream>
#include "GameObject.hpp"

#include "mouse_pos.hpp"

using namespace physx;

extern GameObject* get_rendering_obj();
extern void initPhysics(bool interactive);
extern void stepPhysics(bool interactive, double t);	
extern void cleanupPhysics(bool interactive);
extern void keyPress(unsigned char key);
extern void keyRelease(unsigned char key);
extern void mousePressed(uint8_t button);
extern void mouseReleased(uint8_t button);
extern void mousePosUpdated(float x, float y);
extern PxPhysics* gPhysics;
extern PxMaterial* gMaterial;

std::vector<const RenderItem*> gRenderItems;

double PCFreq = 0.0;
__int64 CounterStart = 0;
__int64 CounterLast = 0;

void StartCounter()
{
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
		return;

	PCFreq = double(li.QuadPart);

	QueryPerformanceCounter(&li);
	CounterStart = li.QuadPart;
	CounterLast = CounterStart;
}

double GetCounter()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	double t = double(li.QuadPart - CounterLast) / PCFreq;
	CounterLast = li.QuadPart;
	return t;
}

namespace
{
	Camera*	sCamera;

void motionCallback(int x, int y)
{
	//From 0 to 1
	float x_float = float(x)/WINDOW_WIDTH;
	float y_float = float(y)/WINDOW_HEIGHT;
	x_float = max(0.0f, min(1, x_float));
	y_float = max(0.0f, min(1, y_float));
	mouse_pos_x = x_float;
	mouse_pos_y = y_float;
	mousePosUpdated(x_float, y_float);
}

void keyboardCallback(unsigned char key, int x, int y)
{
	if(key==27)
		exit(0);

	keyPress(key);
}
void keyboardUpCallback(unsigned char key, int x, int y) {
	keyRelease(key);
}

void mouseCallback(int button, int state, int x, int y)
{
	//sCamera->handleMouse(button, state, x, y);
	float x_float = float(x) / WINDOW_WIDTH;
	float y_float = float(y) / WINDOW_HEIGHT;
	x_float = max(0.0f, min(1, x_float));
	y_float = max(0.0f, min(1, y_float));
	if (state == GLUT_UP) {
		mouseReleased(button);
	}
	else {
		mousePressed(button);
	}
}

void idleCallback()
{
	glutPostRedisplay();
}

float stepTime = 0.0f;
//#define FIXED_STEP

void renderCallback()
{
	update(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	
	double t = GetCounter();
#ifdef FIXED_STEP
	if (t < (1.0f / 30.0f))
	{
		fprintf(stderr, "Time: %f\n", stepTime);
		stepTime += t;
	}
	else
		stepTime = 1.0f / 30.0f;

	if (stepTime >= (1.0f / 30.0f))
	{
		stepPhysics(true, stepTime);
		stepTime = 0.0f;
	}
#else
	stepPhysics(true, t);
#endif
	hud_rendering_obj = get_rendering_obj();

	startRender(sCamera->getEye(), sCamera->getDir(), sCamera->getUp());

	hud_rendering_obj->render3D();

	renderHUD();

	finishRender();
}

void exitCallback(void)
{
	delete sCamera;
	cleanupPhysics(true);
}
}

void renderLoop()
{
	StartCounter();
	sCamera = new Camera(PxVec3(50.0f, 50.0f, 50.0f), PxVec3(-0.6f,-0.2f,-0.7f));

	setupDefaultWindow("Simulacion Fisica Videojuegos", WINDOW_WIDTH, WINDOW_HEIGHT);
	setupDefaultRenderState();

	initPhysics(true);

	glutIdleFunc(idleCallback);
	glutDisplayFunc(renderCallback);
	glutKeyboardFunc(keyboardCallback);
	glutKeyboardUpFunc(keyboardUpCallback);
	glutMouseFunc(mouseCallback);
	glutPassiveMotionFunc(motionCallback);
	glutMotionFunc(motionCallback);
	motionCallback(0,0);
	glutIgnoreKeyRepeat(1);//If its 0 it will not ignore them

	atexit(exitCallback);

	glutMainLoop();
}

void RegisterRenderItem(const RenderItem* _item)
{
	gRenderItems.push_back(_item);
}

void DeregisterRenderItem(const RenderItem* _item)
{
	auto it = find(gRenderItems.begin(), gRenderItems.end(), _item);
	gRenderItems.erase(it);
}

double GetLastTime()
{
	double t = double(CounterLast - CounterStart) / PCFreq;
	return t;
}

Camera* GetCamera()
{
	return sCamera;
}

PxShape* CreateShape(const PxGeometry& geo, const PxMaterial* mat)
{
	if (mat == nullptr)
		mat = gMaterial; // Default material

	PxShape* shape = gPhysics->createShape(geo, *mat);
	return shape;
}