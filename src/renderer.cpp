#include "renderer.hpp"
#include <Windows.h>
#include <gl/GL.h>
#pragma comment (lib, "opengl32.lib")

void Renderer::Line(int x1, int y1, int x2, int y2)
{
	glBegin(GL_LINES);
	glVertex2i(1, 400);
	glVertex2i(1000, 400);
	glVertex2i(1, 400);
	glVertex2i(1000, 400);
	glEnd();
}
