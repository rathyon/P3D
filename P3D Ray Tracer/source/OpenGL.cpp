#pragma once

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <vector>
#include <ctime>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "Math\Math.h"
#include "Math\Lens.h"
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"
#include "BBox.h"
#include "Grid.h"
#include "Light.h"
#include "AreaLight.h"
#include "StochasticLight.h"
#include "Material.h"
#include "NFFParser.h"

#include "RayTracer.h"

#define CAPTION "Whitted Ray Tracer"

#define VERTEX_COORD_ATTRIB 0
#define COLOR_ATTRIB 1

#define print(x) std::cout << x << std::endl;

// Points defined by 2 attributes: positions which are stored in vertices array and colors which are stored in colors array
float *colors;
float *vertices;

int size_vertices;
int size_colors;

GLfloat m[16];  //projection matrix initialized by ortho function

GLuint VaoId;
GLuint VboId[2];

GLuint VertexShaderId, FragmentShaderId, ProgramId;
GLint UniformId;

//Scene* scene = NULL;
//int RES_X, RES_Y;

int RES_X = 512;
int RES_Y = 512;

/* Draw Mode: 0 - point by point; 1 - line by line; 2 - full frame */
#define DRAWMODE 1
int WindowHandle = 0;

//Time variables
std::clock_t begin;
std::clock_t end;

//------------------ RAY TRACER VARIABLES ------------------//
// these variables are shared between this file and RayTracer.h

Camera camera;
Lens thinLens;
vec3 background_color;
std::vector<Object*> objects;
std::vector<Light*> lights;
Grid grid;

//------------------ PARSER VARIABLES ------------------//

NFFParser parser;


const std::string nffFilename = "source/Nff/boxtest_dbg.nff";



/////////////////////////////////////////////////////////////////////// ERRORS

bool isOpenGLError() {
	bool isError = false;
	GLenum errCode;
	const GLubyte *errString;
	while ((errCode = glGetError()) != GL_NO_ERROR) {
		isError = true;
		errString = gluErrorString(errCode);
		std::cerr << "OpenGL ERROR [" << errString << "]." << std::endl;
	}
	return isError;
}

void checkOpenGLError(std::string error)
{
	if(isOpenGLError()) {
		std::cerr << error << std::endl;
		exit(EXIT_FAILURE);
	}
}
/////////////////////////////////////////////////////////////////////// SHADERs
const GLchar* VertexShader =
{
	"#version 330 core\n"

	"in vec2 in_Position;\n"
	"in vec3 in_Color;\n"
	"uniform mat4 Matrix;\n"
	"out vec4 color;\n"

	"void main(void)\n"
	"{\n"
	"	vec4 position = vec4(in_Position, 0.0, 1.0);\n"
	"	color = vec4(in_Color, 1.0);\n"
	"	gl_Position = Matrix * position;\n"

	"}\n"
};

const GLchar* FragmentShader =
{
	"#version 330 core\n"

	"in vec4 color;\n"
	"out vec4 out_Color;\n"

	"void main(void)\n"
	"{\n"
	"	out_Color = color;\n"
	"}\n"
};

void createShaderProgram()
{
	VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShaderId, 1, &VertexShader, 0);
	glCompileShader(VertexShaderId);

	FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShaderId, 1, &FragmentShader, 0);
	glCompileShader(FragmentShaderId);

	ProgramId = glCreateProgram();
	glAttachShader(ProgramId, VertexShaderId);
	glAttachShader(ProgramId, FragmentShaderId);

	glBindAttribLocation(ProgramId, VERTEX_COORD_ATTRIB, "in_Position");
	glBindAttribLocation(ProgramId, COLOR_ATTRIB, "in_Color");
	
	glLinkProgram(ProgramId);
	UniformId = glGetUniformLocation(ProgramId, "Matrix");

	checkOpenGLError("ERROR: Could not create shaders.");
}

void destroyShaderProgram()
{
	glUseProgram(0);
	glDetachShader(ProgramId, VertexShaderId);
	glDetachShader(ProgramId, FragmentShaderId);

	glDeleteShader(FragmentShaderId);
	glDeleteShader(VertexShaderId);
	glDeleteProgram(ProgramId);

	checkOpenGLError("ERROR: Could not destroy shaders.");
}
/////////////////////////////////////////////////////////////////////// VAOs & VBOs
void createBufferObjects()
{
	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);
	glGenBuffers(2, VboId);
	glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);

	/* N�o � necess�rio fazer glBufferData, ou seja o envio dos pontos para a placa gr�fica pois isso 
	� feito na drawPoints em tempo de execu��o com GL_DYNAMIC_DRAW */

	glEnableVertexAttribArray(VERTEX_COORD_ATTRIB);
	glVertexAttribPointer(VERTEX_COORD_ATTRIB, 2, GL_FLOAT, 0, 0, 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, VboId[1]);
	glEnableVertexAttribArray(COLOR_ATTRIB);
	glVertexAttribPointer(COLOR_ATTRIB, 3, GL_FLOAT, 0, 0, 0);
	
// unbind the VAO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(VERTEX_COORD_ATTRIB);
	glDisableVertexAttribArray(COLOR_ATTRIB);
	checkOpenGLError("ERROR: Could not create VAOs and VBOs.");
}

void destroyBufferObjects()
{
	glDisableVertexAttribArray(VERTEX_COORD_ATTRIB);
	glDisableVertexAttribArray(COLOR_ATTRIB);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glDeleteBuffers(1, VboId);
	glDeleteVertexArrays(1, &VaoId);
	checkOpenGLError("ERROR: Could not destroy VAOs and VBOs.");
}

void drawPoints()
{
	glBindVertexArray(VaoId);
	glUseProgram(ProgramId);

	glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
	glBufferData(GL_ARRAY_BUFFER, size_vertices, vertices, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VboId[1]);
	glBufferData(GL_ARRAY_BUFFER, size_colors, colors, GL_DYNAMIC_DRAW);

	glUniformMatrix4fv(UniformId, 1, GL_FALSE, m);
	glDrawArrays(GL_POINTS, 0, RES_X*RES_Y);
	glFinish();

	glUseProgram(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	checkOpenGLError("ERROR: Could not draw scene.");
}

/////////////////////////////////////////////////////////////////////// CALLBACKS

void renderScene()
{
	grid.computeGrid(objects);
	srand(time(NULL));
	begin = clock();

	int index_pos = 0;
	int index_col = 0;
		
	for (int y = 0; y < RES_Y; y++)
	{
		for (int x = 0; x < RES_X; x++){

			vec3 rgb = rayTrace(x,y);
			float color[3] = {rgb.x, rgb.y, rgb.z};

			vertices[index_pos++] = (float)x;
			vertices[index_pos++] = (float)y;
			colors[index_col++] = color[0];
			colors[index_col++] = color[1];
			colors[index_col++] = color[2];

			if (DRAWMODE == 0) {  // desenhar o conte�do da janela ponto a ponto
				drawPoints();
				index_pos = 0;
				index_col = 0;
			}
		}

		if (DRAWMODE == 1) {  // desenhar o conte�do da janela linha a linha
			drawPoints();
			index_pos = 0;
			index_col = 0;
		}
	}

	if (DRAWMODE == 2) {
		drawPoints();
	}

	end = clock();
	double time = double(end - begin) / CLOCKS_PER_SEC;

	std::cout << "DONE!" << std::endl;
	std::cout << "Elapsed time: " << time << "s" <<std::endl;
	std::cin.ignore();
	exit(0);
}

void cleanup()
{
	destroyShaderProgram();
	destroyBufferObjects();
}

void ortho(float left, float right, float bottom, float top, 
			float nearp, float farp)
{
	m[0 * 4 + 0] = 2 / (right - left);
	m[0 * 4 + 1] = 0.0;
	m[0 * 4 + 2] = 0.0;
	m[0 * 4 + 3] = 0.0;
	m[1 * 4 + 0] = 0.0;
	m[1 * 4 + 1] = 2 / (top - bottom);
	m[1 * 4 + 2] = 0.0;
	m[1 * 4 + 3] = 0.0;
	m[2 * 4 + 0] = 0.0;
	m[2 * 4 + 1] = 0.0;
	m[2 * 4 + 2] = -2 / (farp - nearp);
	m[2 * 4 + 3] = 0.0;
	m[3 * 4 + 0] = -(right + left) / (right - left);
	m[3 * 4 + 1] = -(top + bottom) / (top - bottom);
	m[3 * 4 + 2] = -(farp + nearp) / (farp - nearp);
	m[3 * 4 + 3] = 1.0;
}

void reshape(int w, int h)
{
    glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, w, h);
	ortho(0, (float)RES_X, 0, (float)RES_Y, -1.0, 1.0);
}

/////////////////////////////////////////////////////////////////////// SETUP
void setupCallbacks() 
{
	glutCloseFunc(cleanup);
	glutDisplayFunc(renderScene);
	glutReshapeFunc(reshape);
}

void setupGLEW() {
	glewExperimental = GL_TRUE;
	GLenum result = glewInit() ; 
	if (result != GLEW_OK) { 
		std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
		exit(EXIT_FAILURE);
	} 
	GLenum err_code = glGetError();
	printf ("Vendor: %s\n", glGetString (GL_VENDOR));
	printf ("Renderer: %s\n", glGetString (GL_RENDERER));
	printf ("Version: %s\n", glGetString (GL_VERSION));
	printf ("GLSL: %s\n", glGetString (GL_SHADING_LANGUAGE_VERSION));
}

void setupGLUT(int argc, char* argv[])
{
	glutInit(&argc, argv);
	
	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	
	glutInitWindowPosition(640,100);
	glutInitWindowSize(RES_X, RES_Y);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glDisable(GL_DEPTH_TEST);
	WindowHandle = glutCreateWindow(CAPTION);
	if(WindowHandle < 1) {
		std::cerr << "ERROR: Could not create a new rendering window." << std::endl;
		exit(EXIT_FAILURE);
	}
}

void init(int argc, char* argv[])
{
	setupGLUT(argc, argv);
	setupGLEW();
	std::cerr << "CONTEXT: OpenGL v" << glGetString(GL_VERSION) << std::endl;
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	createShaderProgram();
	createBufferObjects();
	setupCallbacks();
}

int main(int argc, char* argv[])
{
	print("Parsing NFF file...");
	parser = NFFParser(nffFilename);
	parser.ParseObjectsAndLights(lights, objects);

	parser = NFFParser(nffFilename); // it is necessary to reload the file 
	camera = parser.ParseCamera();
	

	//////////////////////////////////Ini Thin lens/////////////////
	//thinLens = Lens(1.5f,3.5f,4,vec3(2.1f, 1.3f, 1.7f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), 45.0f, 0.01f, 1000.0f, 512, 512);
	//thinLens = Lens(1.5f, 3.5, 1, vec3(0.0f, 0.0f, 4.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), 45.0f, 2.51f, 1000.0f, 512, 512);
	thinLens = Lens(1.5f, 3.5f, 8, vec3(0.0f, 0.0f, 4.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), 45.0f, 2.51f, 1000.0f, 512, 512);
	//////////////////////////////////Ini Thin lens/////////////////
	//(const float& radius, const  float& focalDistance, const int& samples, const vec3& pos, const vec3& at, const vec3& up, const float& fovy, const float& near, const float& far, const int& ResX, const int& ResY
	
	vec3 right = thinLens.getRight();
	vec3 up = thinLens.getUp(); //verfiicar o UP
	vec3 lookAt = thinLens.getLookAt();
	float h = thinLens.getHeight();
	float w = thinLens.getWidth();
	float ResX = (float)thinLens.getResX();
	float ResY = (float)thinLens.getResY();

	float radius = thinLens.getRadius();
	float focalDistance = thinLens.getFocalDistance();
	int samples = thinLens.getSamples();
	vec3 pos = thinLens.getPos();
	vec3 at = thinLens.getAt();
	float fov = thinLens.getFOVY();
	float near1 = thinLens.getNear();
	float far1 = thinLens.getFar();
	//std::cerr << "CONTEXT: OpenGL v" << glGetString(GL_VERSION) << std::endl;
	
	std::cerr << "Radius: \n" << radius << std::endl;
	std::cerr << "Focal Distance: \n" << focalDistance << std::endl;
	std::cerr << "Samples: \n" << samples << std::endl;
	std::cerr << "Pos: \n" << pos << std::endl;
	std::cerr << "At: \n" << at << std::endl;
	std::cerr << "Up: \n" << up << std::endl;
	std::cerr << "fovy: \n" << fov << std::endl;
	std::cerr << "Near: \n" << near1 << std::endl;
	std::cerr << "Far: \n" << far1 << std::endl;
	std::cerr << "ResX: \n" << ResX << std::endl;
	std::cerr << "ResY: \n" << ResY << std::endl;
	std::cerr << "Right: \n" << right << std::endl;
	std::cerr << "LookAt: \n" << lookAt << std::endl;
	std::cerr << "h: \n" << h << std::endl;
	std::cerr << "w: \n" << w << std::endl;
	//print("Parsing NFF file..."+);
	
	parser = NFFParser(nffFilename); // it is necessary to reload the file 
	parser.ParseBackgroundColor(background_color);
	

	if(DRAWMODE == 0) { // desenhar o conte�do da janela ponto a ponto
		size_vertices = 2*sizeof(float);
		size_colors = 3*sizeof(float);
		printf("DRAWING MODE: POINT BY POINT\n");
	}
	else if(DRAWMODE == 1) { // desenhar o conte�do da janela linha a linha
		size_vertices = 2*RES_X*sizeof(float);
		size_colors = 3*RES_X*sizeof(float);
		printf("DRAWING MODE: LINE BY LINE\n");
	}
	else if(DRAWMODE == 2) { // preencher o conte�do da janela com uma imagem completa
		size_vertices = 2*RES_X*RES_Y*sizeof(float);
		size_colors = 3*RES_X*RES_Y*sizeof(float);
		printf("DRAWING MODE: FULL IMAGE\n");
	}
	else {
		printf("Draw mode not valid \n");
		exit(0);
	}
	printf("resx = %d  resy= %d.\n", RES_X, RES_Y);

	vertices = (float*)malloc(size_vertices);
    if (vertices==NULL) exit (1);

	colors = (float*)malloc(size_colors);
    if (colors==NULL) exit (1);

	init(argc, argv);
	glutMainLoop();	
	exit(EXIT_SUCCESS);
}
///////////////////////////////////////////////////////////////////////