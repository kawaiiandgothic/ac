#include <Windows.h>
#include <gl/GL.h>
#pragma comment (lib, "opengl32.lib")
#include <cstdlib>

namespace rgb
{
    const GLubyte red[3] = { 255,0,0 };
    const GLubyte green[3] = { 0,255,0 };
    const GLubyte gray[3] = { 55,55,55 };
    const GLubyte lightgray[3] = { 192,192,192 };
    const GLubyte black[3] = { 0,0,0 };
}

struct Vec2 { float x, y; };
class Vec3
{
public:

	float x, y, z;

	Vec3() {};
	
	Vec3(const float x, const float y, const float z) : x(x), y(y), z(z) {}
	Vec3 operator + (const Vec3& rhs) const { return Vec3(x + rhs.x, y + rhs.y, z + rhs.z); }
	Vec3 operator - (const Vec3& rhs) const { return Vec3(x - rhs.x, y - rhs.y, z - rhs.z); }
	Vec3 operator * (const float& rhs) const { return Vec3(x * rhs, y * rhs, z * rhs); }
	Vec3 operator / (const float& rhs) const { return Vec3(x / rhs, y / rhs, z / rhs); }
	Vec3& operator += (const Vec3& rhs) { return *this = *this + rhs; }
	Vec3& operator -= (const Vec3& rhs) { return *this = *this - rhs; }
	Vec3& operator *= (const float& rhs) { return *this = *this * rhs; }
	Vec3& operator /= (const float& rhs) { return *this = *this / rhs; }
	float Length() const { return sqrtf(x * x + y * y + z * z); }
	Vec3 Normalize() const { return *this * (1 / Length()); }
	float Distance(const Vec3& rhs) const { return (*this - rhs).Length(); }
};
//struct Vec3 { float x, y, z;             float Distance(Vec3& rhs) { return (*this - rhs).Length(); }};
struct Vec4 { float x, y, z, w; };

namespace Renderer
{
	void Setup();
	void Line(int x1, int y1, int x2, int y2);
	void Box(float x, float y, float width, float height, float lineWidth, const GLubyte color[3]);
}