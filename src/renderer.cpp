#include "renderer.hpp"
#include <Windows.h>
#include <gl/GL.h>
#pragma comment (lib, "opengl32.lib")

void Renderer::Setup() 
{
	glClearColor(0.0, 0.0, 0.0, 0.0);  //Set the cleared screen colour to black
	glViewport(0, 0, 1024, 768);   //This sets up the viewport so that the coordinates (0, 0) are at the top left of the window

	//Set up the orthographic projection so that coordinates (0, 0) are in the top left
	//and the minimum and maximum depth is -10 and 10. To enable depth just put in
	//glEnable(GL_DEPTH_TEST)
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 1024, 768, 0, -10, 10);

	//Back to the modelview so we can draw stuff 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT or GL_DEPTH_BUFFER_BIT); //Clear the screen and depth buffer

}

void Renderer::Line(int x1, int y1, int x2, int y2)
{
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0);
	glVertex2i(x1, y1);
	glVertex2i(x2, y2);
	glEnd();
}

void Renderer::Box(float x, float y, float width, float height, float lineWidth, const GLubyte color[3])
{
	glLineWidth(lineWidth);
	glBegin(GL_LINE_STRIP);
	glColor3ub(color[0], color[1], color[2]);
	glVertex2f(x - 0.5f, y - 0.5f);
	glVertex2f(x + width + 0.5f, y - 0.5f);
	glVertex2f(x + width + 0.5f, y + height + 0.5f);
	glVertex2f(x - 0.5f, y + height + 0.5f);
	glVertex2f(x - 0.5f, y - 0.5f);
	glEnd();
}