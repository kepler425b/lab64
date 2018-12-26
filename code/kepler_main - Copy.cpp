#define _CRT_SECURE_NO_WARNINGS
#define GLEW_STATIC
#include <Windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <glm.hpp>
#include <GL\glew.h>
#include <gl/GL.h>
#include "Model.h"
#include "Camera.h"

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cassert>
#include <math.h>

SDL_Event Event;
SDL_Renderer *Renderer = NULL;
SDL_Window *Window = NULL;
SDL_Surface *Screen = NULL;
float dt = 1.0 / 60;
__int64 FPS = 0;
const UINT8 *keyState;
bool quit = false;
int SCW = 1024;
int SCH = 768;

GLuint model_list;		
Model model_object;		
Camera camera_object;	
				
static int isWire = 0; 
static int isFog = 1; 
int isOrtho = 0;



using namespace std;
using namespace glm;

#define right SDL_SCANCODE_RIGHT
#define left SDL_SCANCODE_LEFT 
#define up SDL_SCANCODE_UP 
#define down SDL_SCANCODE_DOWN 

std::vector<vec3> s_data;

void sdldie(const char *msg)
{
	printf("%s: %s\n", msg, SDL_GetError());
	SDL_Quit();
	exit(1);
}

void checkSDLError(int line = -1)
{
#ifndef NDEBUG
	const char *error = SDL_GetError();
	if (*error != '\0')
	{
		printf("SDL Error: %s\n", error);
		if (line != -1)
			printf(" + line: %i\n", line);
		SDL_ClearError();
	}
#endif
}

void close()
{
	SDL_DestroyWindow(Window);
	SDL_DestroyRenderer(Renderer);
	Window = NULL;
	Renderer = NULL;
	SDL_Quit();
	IMG_Quit();
}

void Model::loadFile(int argc, char **argv) {
	string fileObjectLine;
	ifstream fileObject;
	assert(argc == 2);				
	fileObject = ifstream(argv[1]);		

	if (fileObject.is_open()) {
		while (!fileObject.eof()) {
			getline(fileObject, fileObjectLine);

			if (fileObjectLine.c_str()[0] == 'v') {				// if the line starts with 'v', it's declaring a vertice
																//cout << fileObjectLine << endl;
				float x, y, z;
				fileObjectLine[0] = ' ';						// get rid of 'v'
				sscanf(fileObjectLine.c_str(), "%f %f %f ", &x, &y, &z);		// assign the vertice's values to x,y,z
				model_object.vertex_list.push_back(x);						// save the values into the vector vertex_list of
				model_object.vertex_list.push_back(y);						// object model_object
				model_object.vertex_list.push_back(z);
				//cout << "line is " << fileObjectLine << endl;
				//cout << model_object.vertex_list.size() << endl;
				continue;													// skip to next iteration
			}
		}

		model_list = glGenLists(1);				// init one display list
		glNewList(model_list, GL_COMPILE);		// start of display list

												// NOTE: this will only run once, after all the vertices have been added and right before polygons/faces are added
												//applyTransfToMatrix();
												//model_object.applyTransfToMatrix();

												// go back to beginning of the file
		fileObject.clear();
		fileObject.seekg(0, ios::beg);



#if 1
		glBegin(GL_POINTS);
		while (!fileObject.eof()) {
			getline(fileObject, fileObjectLine);

			// scanning for 'f' and adding polygons to display list
			if (fileObjectLine.c_str()[0] == 'f') {				// if the line starts with 'f', it's declaring a face/polygon

				s_list.push_back(fileObjectLine);
				fileObjectLine[0] = ' ';
				uint vertexIndex[3], uvIndex[3], normalIndex[3];
				uint matches = sscanf(fileObjectLine.c_str(), "%d/%d/%d %d/%d/%d %d/%d/%d",
					&vertexIndex[0], &uvIndex[0], &normalIndex[0],
					&vertexIndex[1], &uvIndex[1], &normalIndex[1],
					&vertexIndex[2], &uvIndex[2], &normalIndex[2]);
				if (matches != 9) {

				}
				vec3 temp;
				temp.x = model_object.vertex_list.at(vertexIndex[0] - 1);
				temp.y = model_object.vertex_list.at(vertexIndex[1] - 1);
				temp.z = model_object.vertex_list.at(vertexIndex[2] - 1);
				s_data.push_back(temp);
				glVertex3f(temp.x, temp.y, temp.z);

				continue;
			}
		}
		glEnd();
		glEndList();
	}
}

#endif

#if 0
// read file again and process the lines starting with 's'
int num = 0;
while (!fileObject.eof()) {
	getline(fileObject, fileObjectLine);

	// scanning for 'f' and adding polygons to display list
	if (fileObjectLine.c_str()[0] == 'f') {				// if the line starts with 'f', it's declaring a face/polygon

		s_list.push_back(fileObjectLine);
		fileObjectLine[0] = ' ';			// get rid of 'f' from the line string

		istringstream iss(fileObjectLine);


		int x, y, z;							//cout << "line is : " << fileObjectLine << endl;
		x = 0;
		y = 1;
		z = 2;
		int stride = 0;
		int v_size = model_object.vertex_list.size();
		for (int index = 3; index < model_object.vertex_list.size(); index += 3)
		{
			if (x >= v_size) { x = v_size; };
			if (y >= v_size) { y = v_size; };
			if (z >= v_size) { z = v_size; };
			if (num == 0)
			{
				glBegin(GL_LINES);
			}
			glVertex3f(model_object.vertex_list[x], model_object.vertex_list[y], model_object.vertex_list[z]);

			stride += 3;
			x += 3;
			y += 3;
			z += 3;
			num++;
			if (num >= 3) {
				glEnd();
				num = 0;
			}
		}
		glEnd();
	}
}
glEndList();
	}
}
#endif

void drawModelTransf() {
	// model
	// apply model translations
	glTranslatef(model_object.model_x, model_object.model_y, model_object.model_z);

	// translate to z = -2 so model can rotate about its axis
	glTranslatef(0, 0, -2);
	// apply rotation transformations
	glRotatef(model_object.model_rotx, 1, 0, 0);
	glRotatef(model_object.model_roty, 0, 1, 0);
	glRotatef(model_object.model_rotz, 0, 0, 1);
	// translate back to z = 0 where the camera is
	glTranslatef(0, 0, 2);
}

void drawCameraTransf() {
	// camera
	glTranslatef(camera_object.camera_x, camera_object.camera_y, camera_object.camera_z);
	glRotatef(camera_object.camera_rotx, 1, 0, 0);
	glRotatef(camera_object.camera_roty, 0, 1, 0);
	glRotatef(camera_object.camera_rotz, 0, 0, 1);
	//gluLookAt(0.0, 0.0, 0.0, model_object.model_x, model_object.model_y, model_object.model_z, 0.0, 1.0, 0.0);
}

// Drawing (display) routine.
void drawScene(void)
{
	// Clear screen to background color.
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	static int fogMode = GL_LINEAR; // Fog mode
	static float fogStart = 1.0; // Fog start z value.
	static float fogEnd = 5.0; // Fog end z value.
	float fogColor[4] = { 1.0, 1.0, 1.0, 0.0 };
	// Fog controls.
	if (isFog) glEnable(GL_FOG);
	else glDisable(GL_FOG);
	glHint(GL_FOG_HINT, GL_NICEST);
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogi(GL_FOG_MODE, fogMode);
	glFogf(GL_FOG_START, fogStart);
	glFogf(GL_FOG_END, fogEnd);

	// reset transformation matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Set foreground (or drawing) color.
	glColor3f(1.0f, 0.0f, 0.0f);

	// wireframe or not?
	if (isWire) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// apply translations and rotations to model and camera
	drawCameraTransf();
	drawModelTransf();

	// draw display list
	glCallList(model_list);
}

// Initialization routine.
void setup(void)
{
	// Set background (or clearing) color.
	glClearColor(0.0, 0.0, 0.0, 0.0);
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (isOrtho) {
		glOrtho(-1.0, 1.0, -1.0, 1.0, 1, 100.0);
	}
	else {
		glFrustum(-1.0, 1.0, -1.0, 1.0, 1, 100.0);
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


int main(int argc, char* argv[])
{
	SDL_Window *mainWindow;
	SDL_GLContext glContext;
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{

	}
	mainWindow = SDL_CreateWindow("Beaminster", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		512, 512, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (!mainWindow) /* Die if creation failed */
		sdldie("Unable to create window");
	

	
	checkSDLError(__LINE__);

	glContext = SDL_GL_CreateContext(mainWindow);
	SDL_SetWindowResizable(mainWindow, SDL_TRUE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	if (!glewInit() == GLEW_OK) {
		printf("error");
	}

	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	//glBufferData(GL_ARRAY_BUFFER, 3, &)

	checkSDLError(__LINE__);
	SDL_GL_SetSwapInterval(1);
	int w, h;
	SDL_GetWindowSize(mainWindow, &w, &h);
	glViewport(0, 0, w, h);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	/*char *asd = (char *)malloc(256);
	strcpy_s(asd, 256, "asd");
	TTF_Font *font = TTF_OpenFont("font/ProggyClean.ttf", 24);*/


	LARGE_INTEGER Frequency;
	QueryPerformanceFrequency(&Frequency);
	double SecondPerTick = 1.0 / (double)Frequency.LowPart;
	LARGE_INTEGER tick_beforloop;
	QueryPerformanceCounter(&tick_beforloop);
	model_object.loadFile(argc, argv);
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	while (!quit){
		LARGE_INTEGER last_tick;
		QueryPerformanceCounter(&last_tick);
		while (SDL_PollEvent(&Event)){
			if (Event.type == SDL_QUIT){
				quit = true;
			}
			switch (Event.type)
				if (Event.type == SDL_WINDOWEVENT)
				{
					switch (Event.window.event)
					{
					case SDL_WINDOWEVENT_RESIZED:
					{
						resize(Event.window.data1, Event.window.data2);
						break;
					default:
						break;
					}
				}
				case SDL_KEYDOWN:
				{
					switch (Event.key.keysym.sym)
					{
					case SDLK_UP:
						model_object.model_rotx += 10;
						break;

					case SDLK_DOWN:
						model_object.model_rotx -= 10;
						break;
					case SDLK_RIGHT:
						model_object.model_roty += 10;
						break;

					case SDLK_LEFT:
						model_object.model_roty -= 10;
						break;
					default:
						break;
					}
				}
				if(Event.type == SDL_KEYUP)
				{
					switch (Event.key.keysym.sym)
					{
					case SDLK_UP:
			
						break;

					case SDLK_DOWN:
		
						break;
					case SDLK_RIGHT:
	
						break;
					default:
						break;

					case SDLK_LEFT:
	
						break;
					}
				}
			}
			if (Event.type == SDL_MOUSEBUTTONDOWN){
				if (Event.button.button == SDL_BUTTON_LEFT){
				}	
			}
			else if (Event.type == SDL_MOUSEBUTTONUP){
				
			}
		}
		const UINT8* Key = SDL_GetKeyboardState(NULL);
		if (Key[right]){
		}
		if (Key[left]){
		}
		if (Key[up]){
		}
		if (Key[down]){
		}
		
		drawScene();

		LARGE_INTEGER now_tick;
		QueryPerformanceCounter(&now_tick);

		__int64 Interval = now_tick.QuadPart - tick_beforloop.QuadPart;
		double SecondsGoneBy = (double)Interval * SecondPerTick;
	}
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(mainWindow);
	SDL_Quit();
	return 0;
}
