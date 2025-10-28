#include <vector>

#include "PxPhysicsAPI.h"

//#include "core.hpp"
#include "RenderUtils.hpp"
#include "ScreenSizeConstants.hpp";
#include <iostream>
#include "stb_image.h"


using namespace physx;

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
std::vector<GLuint> textures;
GLuint hud_text;

double PCFreq = 0.0;
__int64 CounterStart = 0;
__int64 CounterLast = 0;

void StartCounter()
{
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
		return;

	PCFreq = double(li.QuadPart) /*/ 1000.0*/;

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
	float x_float = float(x)/WINDOW_LENGTH;
	float y_float = float(y)/WINDOW_HEIGHT;
	x_float = max(0.0f, min(1, x_float));
	y_float = max(0.0f, min(1, y_float));
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

GLuint loadTexture(const char* path) {
	int width, height, numColCh;
	unsigned char* bytes = stbi_load(path, &width, &height, &numColCh, 0);

	GLuint texture;
	glGenTextures(1, &texture);
	textures.push_back(texture);
	//glActiveTexture(GL_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, width, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
	//glGenerateMipMap(GL_TEXTURE_2D);

	stbi_image_free(bytes);
	glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
}

void renderHUD() {
	if (hud_text == 0)
		hud_text = loadTexture("cabina.png");
	glBindTexture(GL_TEXTURE_2D, hud_text);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0);	glVertex2f(0.05, 0.05);
	glTexCoord2f(1.0, 1.0);	glVertex2f(0.3, 0.05);
	glTexCoord2f(1.0, 0.0); glVertex2f(0.3, 0.15);
	glTexCoord2f(0.0, 0.0); glVertex2f(0.05, 0.15);
	glEnd();
}

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
	//RENDER 3D
	startRender(sCamera->getEye(), sCamera->getDir(), sCamera->getUp());

	//fprintf(stderr, "Num Render Items: %d\n", static_cast<int>(gRenderItems.size()));
	for (auto it = gRenderItems.begin(); it != gRenderItems.end(); ++it)
	{
		const RenderItem* obj = (*it);
		auto objTransform = obj->transform;
		if (!objTransform)
		{
			auto actor = obj->actor;
			if (actor)
			{
				renderShape(*obj->shape, actor->getGlobalPose(), obj->color);
				continue;
			}
		}
		renderShape(*obj->shape, objTransform ? *objTransform : physx::PxTransform(PxIdentity), obj->color);
	}
	//RENDER 2D
	renderHUD();

	//PxScene* scene;
	//PxGetPhysics().getScenes(&scene, 1);
	//PxU32 nbActors = scene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC);
	//if (nbActors)
	//{
	//	std::vector<PxRigidActor*> actors(nbActors);
	//	scene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC, reinterpret_cast<PxActor**>(&actors[0]), nbActors);
	//	renderActors(&actors[0], static_cast<PxU32>(actors.size()), true, Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	//}

	finishRender();
}

void exitCallback(void)
{
	delete sCamera;
	for (auto text : textures) {
		glDeleteTextures(1, &text);
	}
	cleanupPhysics(true);
}
}

void renderLoop()
{
	StartCounter();
	sCamera = new Camera(PxVec3(50.0f, 50.0f, 50.0f), PxVec3(-0.6f,-0.2f,-0.7f));

	setupDefaultWindow("Simulacion Fisica Videojuegos", WINDOW_LENGTH, WINDOW_HEIGHT);
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