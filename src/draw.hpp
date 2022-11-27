#pragma once
#include <Windows.h>
#include <stdio.h>
#include <gl\GL.h>
#pragma comment(lib, "OpenGL32.lib")

namespace rgb
{
	const GLubyte red[3] = { 255,0,0 };
	const GLubyte green[3] = { 0,255,0 };
	const GLubyte gray[3] = { 55,55,55 };
	const GLubyte lightgray[3] = { 192,192,192 };
	const GLubyte black[3] = { 0,0,0 };
}

struct Vec2 { float x, y; };
struct Vec3 { float x, y, z; };
struct Vec4 { float x, y, z, w; };

namespace GL
{
	//https://guidedhacking.com/threads/opengl-hooking-drawing-text-rendering.14460/
	class Font
	{
	public:
		bool bBuilt = false;
		unsigned int base;
		HDC hdc = nullptr;
		int height;
		int width;

		void Build(int height);
		void Print(float x, float y, const unsigned char color[3], const char* format, ...);

		Vec3 centerText(float x, float y, float width, float height, float textWidth, float textHeight);
		float centerText(float x, float width, float textWidth);
	};

	void SetupOrtho();
	void RestoreGL();

	void DrawFilledRect(float x, float y, float width, float height, const GLubyte color[3]);
	void DrawOutline(float x, float y, float width, float height, float lineWidth, const GLubyte color[3]);
	//inline void DrawOutline(float x, float y, float width, float height, float lineWidth, const GLubyte color[3]);
}