#include "renderer.hpp"
#include <Windows.h>
#include <gl/GL.h>
#pragma comment (lib, "opengl32.lib")

void Renderer::Line(int x1, int y1, int x2, int y2)
{
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0);
	glVertex2i(x1, y1);
	glVertex2i(x2, y2);
	glEnd();
}
