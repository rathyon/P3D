#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <vector>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "Math\Math.h"
#include "Sphere.h"
#include "Plane.h"
#include "Light.h"
#include "Material.h"
#include "NFFParser.h"

//#include "scene.h"

#define CAPTION "Turner-Whitted Ray Tracer"

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
int draw_mode=1; 

int WindowHandle = 0;

// Scene Variables
#define MAX_DEPTH 6

std::vector<Object*> objects;
std::vector<Light*> lights;

NFFParser parser;
const std::string nffFilename = "source/Nff/default.txt";

///////////////////////////////////////////////////////////////////////  RAY-TRACE SCENE

//Color rayTracing( Ray ray, int depth, float RefrIndex)
//{
//   INSERT HERE YOUR CODE
//}

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

	/* Não é necessário fazer glBufferData, ou seja o envio dos pontos para a placa gráfica pois isso 
	é feito na drawPoints em tempo de execução com GL_DYNAMIC_DRAW */

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
vec3 background_color = vec3(0.078f, 0.361f, 0.753f);

vec3 rayTrace(Ray ray, int depth) {
	float t = MISS;
	float new_t = 0.0f;
	int target;
	for (int i = 0; i < objects.size(); i++) {
		new_t = objects[i]->intersect(ray);

		// ignore if it missed
		if (new_t == MISS) {
			continue;
		}
		// if there was a hit before, grab the smallest t
		else if (t != MISS && new_t < t) {
			t = new_t;
			target = i;
		} 
		// if nothing has been hit, grab the first hit
		else if (t == MISS && new_t > t) {
			t = new_t;
			target = i;
		}
	}

	vec3 color = vec3(0.0f);
	if (t != MISS) {
		for (Light* light : lights) {
			// cast shadow feeler
			vec3 origin = ray.origin() + t*ray.direction();
			vec3 L = normalize(light->pos() - origin);
			Ray feeler = Ray(origin + OFFSET*L, L);
			float light_t = (light->pos() - origin).length();
			float feeler_t = MISS;
			bool in_shadow = false;

			for (int i = 0; i < objects.size(); i++) {
				feeler_t = objects[i]->intersect(feeler);

				if (feeler_t == MISS) {
					continue;
				}
				if (feeler_t != MISS && feeler_t < light_t) {
					in_shadow = true;
					break;
				}
			}
			if (in_shadow) {
				continue;
			}
			else {
				color += objects[target]->shade(*light, ray, t);
			}
		}
	}
	else {
		color = background_color;
	}
	return color;
}

// Render function by primary ray casting from the eye towards the scene's objects
void renderScene()
{

	parser = NFFParser(nffFilename);
	parser.ParseObjectsAndLights(lights, objects);

	int index_pos=0;
	int index_col=0;

	// same as nff file
	Camera testcam = Camera(vec3(2.1f, 1.3f, 1.7f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), 45.0f, 0.01f, 1000.0f, 512, 512);

	Material sphereMat = Material(vec3(1.0f, 0.9f, 0.7f), 0.5f, 0.5f, 30.0827f, 0.0f, 1.0f);
	Material planeMat = Material(vec3(1.0f, 0.9f, 0.7f), 0.5f, 0.5f, 100000.0f, 0.0f, 1.0f);

	//Sphere* sphere1 = new Sphere(vec3(0.0f, 0.0f, 0.0f), 0.5f, sphereMat);
	/*Sphere* sphere2 = new Sphere(vec3(0.272166f, 0.272166f, 0.544331f), 0.166667f, sphereMat);
	Sphere* sphere3 = new Sphere(vec3(0.643951f, 0.172546f, 1.11022e-16), 0.166667f, sphereMat);
	Sphere* sphere4 = new Sphere(vec3(0.172546f, 0.643951f, 1.11022e-16), 0.166667f, sphereMat);
	Sphere* sphere5 = new Sphere(vec3(-0.371785f, 0.0996195f, 0.544331f), 0.166667f, sphereMat);
	Sphere* sphere6 = new Sphere(vec3(-0.471405f, 0.471405f, 1.11022e-16), 0.166667f, sphereMat);
	Sphere* sphere7 = new Sphere(vec3(-0.643951f, -0.172546f, 1.11022e-16), 0.166667f, sphereMat);
	Sphere* sphere8 = new Sphere(vec3(0.0996195f, -0.371785f, 0.544331f), 0.166667f, sphereMat);
	Sphere* sphere9 = new Sphere(vec3(-0.172546f, -0.643951f, 1.11022e-16), 0.166667f, sphereMat);
	Sphere* sphere10 = new Sphere(vec3(0.471405f, -0.471405f, 1.11022e-16), 0.166667f, sphereMat);*/
	Plane* plane = new Plane(vec3(12.0f, 12.0f, -0.5f), vec3(-12.0f, 12.0f, -0.5f), vec3(-12.0f, -12.0f, -0.5f));
	
	Light* light1 = new Light(vec3(4.0f, 3.0f, 2.0f), vec3(1.0f, 1.0f, 1.0f));
	Light* light2 = new Light(vec3(1.0f, -4.0f, 4.0f), vec3(1.0f, 1.0f, 1.0f));
	Light* light3 = new Light(vec3(-3.0f, 1.0f, 5.0f), vec3(1.0f, 1.0f, 1.0f));

	plane->setMaterial(planeMat);

	objects.push_back(plane);
	/*//objects.push_back(sphere1);
	objects.push_back(sphere2);
	objects.push_back(sphere3);
	objects.push_back(sphere4);
	objects.push_back(sphere5);
	objects.push_back(sphere6);
	objects.push_back(sphere7);
	objects.push_back(sphere8);
	objects.push_back(sphere9);
	objects.push_back(sphere10);*/

	lights.push_back(light1);
	lights.push_back(light2);
	lights.push_back(light3);

	for (int y = 0; y < RES_Y; y++)
	{
		for (int x = 0; x < RES_X; x++){
			Ray primary = Ray(testcam, x, y);
			vec3 rgb = rayTrace(primary, 0);
			float color[3] = {rgb.x, rgb.y, rgb.z};

			vertices[index_pos++] = (float)x;
			vertices[index_pos++] = (float)y;
			colors[index_col++] = color[0];
			colors[index_col++] = color[1];
			colors[index_col++] = color[2];

			if (draw_mode == 0) {  // desenhar o conteúdo da janela ponto a ponto
				drawPoints();
				index_pos = 0;
				index_col = 0;
			}
		}

		if (draw_mode == 1) {  // desenhar o conteúdo da janela linha a linha
			drawPoints();
			index_pos = 0;
			index_col = 0;
		}
	}

	/*for (int y = 0; y < RES_Y; y++)
	{
		for (int x = 0; x < RES_X; x++)
		{
		
		    YOUR 2 FUNTIONS: 
				ray = calculate PrimaryRay(x, y);
				color=rayTracing(ray, 1, 1.0 );

			vertices[index_pos++]= (float)x;
			vertices[index_pos++]= (float)y;
			colors[index_col++]= (float)color.r;
			colors[index_col++]= (float)color.g;
			colors[index_col++]= (float)color.b;	

			if(draw_mode == 0) {  // desenhar o conteúdo da janela ponto a ponto
				drawPoints();
				index_pos=0;
				index_col=0;
			}
		}
		printf("line %d", y);
		if(draw_mode == 1) {  // desenhar o conteúdo da janela linha a linha
				drawPoints();
				index_pos=0;
				index_col=0;
		}
	}*/

	//if(draw_mode == 2) //preenchar o conteúdo da janela com uma imagem completa
		 //drawPoints();

	printf("Terminou!\n"); 	
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
	glClearColor(0.078f, 0.361f, 0.753f, 1.0f);
	createShaderProgram();
	createBufferObjects();
	setupCallbacks();
}

int main(int argc, char* argv[])
{
    //INSERT HERE YOUR CODE FOR PARSING NFF FILES
	//scene = new Scene();
	//if(!(scene->load_nff("jap.nff"))) return 0;
	//RES_X = scene->GetCamera()->GetResX();
	//RES_Y = scene->GetCamera()->GetResY();

	if(draw_mode == 0) { // desenhar o conteúdo da janela ponto a ponto
		size_vertices = 2*sizeof(float);
		size_colors = 3*sizeof(float);
		printf("DRAWING MODE: POINT BY POINT\n");
	}
	else if(draw_mode == 1) { // desenhar o conteúdo da janela linha a linha
		size_vertices = 2*RES_X*sizeof(float);
		size_colors = 3*RES_X*sizeof(float);
		printf("DRAWING MODE: LINE BY LINE\n");
	}
	else if(draw_mode == 2) { // preencher o conteúdo da janela com uma imagem completa
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