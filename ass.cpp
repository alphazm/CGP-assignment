
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <math.h>

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLU32.lib")

#define WINDOW_TITLE "OpenGL Window"

struct color
{
	float r = 0.9, g = 0.9, b = 0.9;
};

struct position
{
	float x = 0.0, y = 0.0, z = 0.0;
};

struct fourIrregularPoint
{
	//f = front, b = back
	//l = left, r = right
	//u = up, d = down
	position flu, fld, fru, frd, blu, bld, bru, brd;
};

float rx = 0.0, ry = 0.0, rz = 0.0, rs = 1.0;

bool weaponSwitch = false, styleSwitch = false;

color black = { 0.01,0.01,0.01 }, white = { 0.99,0.99,0.99 };
color purple = { 0.5,0.0,1.0 }, darkPurple = { 0.25,0.0,0.5 };
color lime = { 0.0,1.0,0.5 };

LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) PostQuitMessage(0);
		else if (wParam == VK_SPACE)		//reset
		{
			rx = 0.0f;
			ry = 0.0f;
			rz = 0.0f;
			glLoadIdentity();
		}
		else if (wParam == VK_RIGHT)
		{
			rx -= rs;
		}
		else if (wParam == VK_LEFT)
		{
			rx += rs;
		}
		else if (wParam == VK_UP)
		{
			ry -= rs;
		}
		else if (wParam == VK_DOWN)
		{
			ry += rs;
		}
		else if (wParam == 0x57)		//press w
		{
			rz += rs;
		}
		else if (wParam == 0x53)		//press s
		{
			rz -= rs;
		}
		else if (wParam == 0x43)
		{
			weaponSwitch = !weaponSwitch;
		}
		else if (wParam == 0x50)
		{
			styleSwitch = !styleSwitch;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
//--------------------------------------------------------------------

bool initPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.cAlphaBits = 8;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 0;

	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;

	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;

	// choose pixel format returns the number most similar pixel format available
	int n = ChoosePixelFormat(hdc, &pfd);

	// set pixel format returns whether it sucessfully set the pixel format
	if (SetPixelFormat(hdc, n, &pfd))
	{
		return true;
	}
	else
	{
		return false;
	}
}
//--------------------------------------------------------------------

//formula
void rotationDirection()
{
	glBegin(GL_LINES);

	//y-axis
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0, 0, 0);
	glVertex3f(0.0, 1.0, 0.0);

	//x-axis
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0, 0, 0);
	glVertex3f(1.0, 0.0, 0.0);

	//z-axis
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0, 0, 0);
	glVertex3f(0.0, 0.0, -1.0);

	glEnd();
}

//Ba = Back, F = Front
fourIrregularPoint setBaToFPoint(fourIrregularPoint point)
{
	point.fld.x = point.bld.x, point.fld.y = point.bld.y, point.fld.z = point.bld.z;
	point.flu.x = point.blu.x, point.flu.y = point.blu.y, point.flu.z = point.blu.z;
	point.fru.x = point.bru.x, point.fru.y = point.bru.y, point.fru.z = point.bru.z;
	point.frd.x = point.brd.x, point.frd.y = point.brd.y, point.frd.z = point.brd.z;

	return point;
}

//L = Left, F = Front
fourIrregularPoint setLToFPoint(fourIrregularPoint point)
{
	point.fld.x = point.fld.x, point.fld.y = point.fld.y, point.fld.z = point.fld.z;
	point.flu.x = point.flu.x, point.flu.y = point.flu.y, point.flu.z = point.flu.z;
	point.fru.x = point.blu.x, point.fru.y = point.blu.y, point.fru.z = point.blu.z;
	point.frd.x = point.bld.x, point.frd.y = point.bld.y, point.frd.z = point.bld.z;

	return point;
}

//R = Right, F = Front
fourIrregularPoint setRToFPoint(fourIrregularPoint point)
{
	point.fld.x = point.frd.x, point.fld.y = point.frd.y, point.fld.z = point.frd.z;
	point.flu.x = point.fru.x, point.flu.y = point.fru.y, point.flu.z = point.fru.z;
	point.fru.x = point.bru.x, point.fru.y = point.bru.y, point.fru.z = point.bru.z;
	point.frd.x = point.brd.x, point.frd.y = point.brd.y, point.frd.z = point.brd.z;

	return point;
}

//F = Front, L = Left
fourIrregularPoint setFToLPoint(fourIrregularPoint point)
{
	point.fld.x = point.fld.x, point.fld.y = point.fld.y, point.fld.z = point.fld.z;
	point.flu.x = point.flu.x, point.flu.y = point.flu.y, point.flu.z = point.flu.z;
	point.blu.x = point.fru.x, point.blu.y = point.fru.y, point.blu.z = point.fru.z;
	point.bld.x = point.frd.x, point.bld.y = point.frd.y, point.bld.z = point.frd.z;

	return point;
}

//F = Front, R = Right
fourIrregularPoint setFToRPoint(fourIrregularPoint point)
{
	point.frd.x = point.fld.x, point.frd.y = point.fld.y, point.frd.z = point.fld.z;
	point.fru.x = point.flu.x, point.fru.y = point.flu.y, point.fru.z = point.flu.z;
	point.bru.x = point.fru.x, point.bru.y = point.fru.y, point.bru.z = point.fru.z;
	point.brd.x = point.frd.x, point.brd.y = point.frd.y, point.brd.z = point.frd.z;

	return point;
}

//F = Front, Ba = Back
fourIrregularPoint setFToBaPoint(fourIrregularPoint point)
{
	point.bld.x = point.fld.x, point.bld.y = point.fld.y, point.bld.z = point.fld.z;
	point.blu.x = point.flu.x, point.blu.y = point.flu.y, point.blu.z = point.flu.z;
	point.bru.x = point.fru.x, point.bru.y = point.fru.y, point.bru.z = point.fru.z;
	point.brd.x = point.frd.x, point.brd.y = point.frd.y, point.brd.z = point.frd.z;

	return point;
}

//R = Right, Ba = Back
fourIrregularPoint setRToBaPoint(fourIrregularPoint point)
{
	point = setRToFPoint(point);
	point = setFToBaPoint(point);

	return point;
}

//L = Left, Ba = Back
fourIrregularPoint setLToBaPoint(fourIrregularPoint point)
{
	point = setLToFPoint(point);
	point = setFToBaPoint(point);

	return point;
}

//F = Front, L = Left
fourIrregularPoint setBaToLPoint(fourIrregularPoint point)
{
	point = setBaToFPoint(point);
	point = setLToFPoint(point);

	return point;
}

//F = Front, R = Left
fourIrregularPoint setBaToRPoint(fourIrregularPoint point)
{
	point = setBaToFPoint(point);
	point = setRToFPoint(point);

	return point;
}

//Bo = Bottom, T = Top
fourIrregularPoint setBoToTPoint(fourIrregularPoint point)
{
	point.fld.x = point.flu.x, point.fld.y = point.flu.y, point.fld.z = point.flu.z;
	point.frd.x = point.fru.x, point.frd.y = point.fru.y, point.frd.z = point.fru.z;
	point.bld.x = point.blu.x, point.bld.y = point.blu.y, point.bld.z = point.blu.z;
	point.brd.x = point.bru.x, point.brd.y = point.bru.y, point.brd.z = point.bru.z;

	return point;
}

//T = Top, Bo = Bottom
fourIrregularPoint setTToBoPoint(fourIrregularPoint point)
{
	point.flu.x = point.fld.x, point.flu.y = point.fld.y, point.flu.z = point.fld.z;
	point.fru.x = point.frd.x, point.fru.y = point.frd.y, point.fru.z = point.frd.z;
	point.blu.x = point.bld.x, point.blu.y = point.bld.y, point.blu.z = point.bld.z;
	point.bru.x = point.brd.x, point.bru.y = point.brd.y, point.bru.z = point.brd.z;

	return point;
}

void drawCylinder(float baseR, float topR, float h)
{
	GLUquadricObj* cylinder = NULL;		//create quadric obj pointer
	cylinder = gluNewQuadric();		//create quadric obj in the memory
	gluQuadricDrawStyle(cylinder, GLU_FILL);
	gluCylinder(cylinder, baseR, topR, h, 30, 30);
	gluDeleteQuadric(cylinder);
}

void drawLineCylinder(float baseR, float topR, float h)
{
	GLUquadricObj* cylinder = NULL;		//create quadric obj pointer
	cylinder = gluNewQuadric();		//create quadric obj in the memory
	gluQuadricDrawStyle(cylinder, GLU_LINE);
	gluCylinder(cylinder, baseR, topR, h, 30, 30);
	gluDeleteQuadric(cylinder);
}

void drawHalfCylinder(float baseR, float topR, float length, float numSteps = 30)
{
	float a = 0.0f;
	float step = /*6.2831852*/ 3.1415926 / numSteps;

	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0; i <= numSteps; ++i)
	{
		float x = (float)cos(a) * baseR;
		float y = (float)sin(a) * baseR;
		glVertex3f(x, y, 0.0f);

		x = (float)cos(a) * topR;
		y = (float)sin(a) * topR;
		glVertex3f(x, y, length);
		a += step;
	}
	glEnd();
}

void drawHalfLineCylinder(float baseR, float topR, float length, float numSteps = 30)
{
	float a = 0.0f;
	float step = /*6.2831852*/ 3.1415926 / numSteps;

	glBegin(GL_LINE_STRIP);
	for (int i = 0; i <= numSteps; ++i)
	{ 
		float x = (float)cos(a) * baseR;
		float y = (float)sin(a) * baseR;
		glVertex3f(x, y, 0.0f);

		x = (float)cos(a) * topR;
		y = (float)sin(a) * topR;
		glVertex3f(x, y, length);
		a += step; 
	}
	glEnd();
}

void drawSphere(float radius)
{
	GLUquadricObj* sphere = NULL;		//create quadric obj pointer
	sphere = gluNewQuadric();		//create quadric obj in the memory
	gluQuadricDrawStyle(sphere, GLU_FILL);		//change to full color or draw line
	gluQuadricTexture(sphere, false);		//disable texture
	gluSphere(sphere, radius, 30, 10);		//draw a sphere
	gluDeleteQuadric(sphere);
}

void drawLineSphere(float radius)
{
	GLUquadricObj* sphere = NULL;		//create quadric obj pointer
	sphere = gluNewQuadric();		//create quadric obj in the memory
	gluQuadricDrawStyle(sphere, GLU_LINE);		//change to full color or draw line
	gluQuadricTexture(sphere, false);		//disable texture
	gluSphere(sphere, radius, 30, 10);		//draw a sphere
	gluDeleteQuadric(sphere);
}

void drawDisk(float inr, float outr, float slices = 30, float loops = 30)
{
	GLUquadricObj* disk = gluNewQuadric();
	gluQuadricDrawStyle(disk, GLU_FILL);
	gluDisk(disk, inr, outr, slices, loops);
	gluDeleteQuadric(disk);
}

void drawLineDisk(float inr, float outr, float slices = 30, float loops = 30)
{
	GLUquadricObj* disk = gluNewQuadric();
	gluQuadricDrawStyle(disk, GLU_LINE);
	gluDisk(disk, inr, outr, slices, loops);
	gluDeleteQuadric(disk);
}

void fourPointIrregularShape(fourIrregularPoint point, bool fill = true, bool oriColor = true,
	color fC = { 0.9,0.9,0.9 }, color rC = { 0.9,0.9,0.9 }, color boC = { 0.9,0.9,0.9 },
	color lC = { 0.9,0.9,0.9 }, color uC = { 0.9,0.9,0.9 }, color baC = { 0.9,0.9,0.9 })
{
	if (oriColor)
	{
		if (fill)
		{
			glBegin(GL_QUADS);
		}
		else
		{
			glBegin(GL_LINE_LOOP);
		}

		//front
		glColor3f(1.0, 0.0, 0.0);		//red
		glVertex3f(point.fld.x, point.fld.y, point.fld.z);
		glVertex3f(point.flu.x, point.flu.y, point.flu.z);
		glVertex3f(point.fru.x, point.fru.y, point.fru.z);
		glVertex3f(point.frd.x, point.frd.y, point.frd.z);

		//right
		glColor3f(0.0, 1.0, 0.0);		//green
		glVertex3f(point.frd.x, point.frd.y, point.frd.z);
		glVertex3f(point.fru.x, point.fru.y, point.fru.z);
		glVertex3f(point.bru.x, point.bru.y, point.bru.z);
		glVertex3f(point.brd.x, point.brd.y, point.brd.z);

		//bottom
		glColor3f(0.0, 0.0, 1.0);		//blue
		glVertex3f(point.brd.x, point.brd.y, point.brd.z);
		glVertex3f(point.frd.x, point.frd.y, point.frd.z);
		glVertex3f(point.fld.x, point.fld.y, point.fld.z);
		glVertex3f(point.bld.x, point.bld.y, point.bld.z);

		//left
		glColor3f(1.0, 1.0, 0.0);		//yellow
		glVertex3f(point.bld.x, point.bld.y, point.bld.z);
		glVertex3f(point.fld.x, point.fld.y, point.fld.z);
		glVertex3f(point.flu.x, point.flu.y, point.flu.z);
		glVertex3f(point.blu.x, point.blu.y, point.blu.z);

		//up
		glColor3f(1.0, 0.0, 1.0);		//magenta
		glVertex3f(point.blu.x, point.blu.y, point.blu.z);
		glVertex3f(point.flu.x, point.flu.y, point.flu.z);
		glVertex3f(point.fru.x, point.fru.y, point.fru.z);
		glVertex3f(point.bru.x, point.bru.y, point.bru.z);

		//back
		glColor3f(0.0, 1.0, 1.0);		//lime
		glVertex3f(point.bru.x, point.bru.y, point.bru.z);
		glVertex3f(point.brd.x, point.brd.y, point.brd.z);
		glVertex3f(point.bld.x, point.bld.y, point.bld.z);
		glVertex3f(point.blu.x, point.blu.y, point.blu.z);

		glEnd();
	}
	else
	{
		if (fill)
		{
			glBegin(GL_QUADS);
		}
		else
		{
			glBegin(GL_LINE_LOOP);
		}

		//front
		glColor3f(fC.r, fC.g, fC.b);
		glVertex3f(point.fld.x, point.fld.y, point.fld.z);
		glVertex3f(point.flu.x, point.flu.y, point.flu.z);
		glVertex3f(point.fru.x, point.fru.y, point.fru.z);
		glVertex3f(point.frd.x, point.frd.y, point.frd.z);

		//right
		glColor3f(rC.r, rC.g, rC.b);
		glVertex3f(point.frd.x, point.frd.y, point.frd.z);
		glVertex3f(point.fru.x, point.fru.y, point.fru.z);
		glVertex3f(point.bru.x, point.bru.y, point.bru.z);
		glVertex3f(point.brd.x, point.brd.y, point.brd.z);

		//bottom
		glColor3f(boC.r, boC.g, boC.b);
		glVertex3f(point.brd.x, point.brd.y, point.brd.z);
		glVertex3f(point.frd.x, point.frd.y, point.frd.z);
		glVertex3f(point.fld.x, point.fld.y, point.fld.z);
		glVertex3f(point.bld.x, point.bld.y, point.bld.z);

		//left
		glColor3f(lC.r, lC.g, lC.b);
		glVertex3f(point.bld.x, point.bld.y, point.bld.z);
		glVertex3f(point.fld.x, point.fld.y, point.fld.z);
		glVertex3f(point.flu.x, point.flu.y, point.flu.z);
		glVertex3f(point.blu.x, point.blu.y, point.blu.z);

		//up
		glColor3f(uC.r, uC.g, uC.b);
		glVertex3f(point.blu.x, point.blu.y, point.blu.z);
		glVertex3f(point.flu.x, point.flu.y, point.flu.z);
		glVertex3f(point.fru.x, point.fru.y, point.fru.z);
		glVertex3f(point.bru.x, point.bru.y, point.bru.z);

		//back
		glColor3f(baC.r, baC.g, baC.b);
		glVertex3f(point.bru.x, point.bru.y, point.bru.z);
		glVertex3f(point.brd.x, point.brd.y, point.brd.z);
		glVertex3f(point.bld.x, point.bld.y, point.bld.z);
		glVertex3f(point.blu.x, point.blu.y, point.blu.z);

		glEnd();
	}
}

void drawCube(float size)
{
	glBegin(GL_QUADS);
	//Face 1: Bottom
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, size);
	glVertex3f(size, 0.0f, size);
	glVertex3f(size, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	//Face 2 : Left
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, size, 0.0f);
	glVertex3f(0.0f, size, size);
	glVertex3f(0.0f, 0.0f, size);
	//Face 3 : Front
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, size);
	glVertex3f(0.0f, size, size);
	glVertex3f(size, size, size);
	glVertex3f(size, 0.0f, size);
	//Face 4 : Right
	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(size, 0.0f, size);
	glVertex3f(size, size, size);
	glVertex3f(size, size, 0.0f);
	glVertex3f(size, 0.0f, 0.0f);
	//Face 5 : Back
	glColor3f(0.0f, 1.0f, 1.0f);
	glVertex3f(size, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, size, 0.0f);
	glVertex3f(size, size, 0.0f);
	//Face 6 : Top
	glColor3f(1.0f, 0.0f, 1.0f);
	glVertex3f(size, size, 0.0f);
	glVertex3f(0.0f, size, 0.0f);
	glVertex3f(0.0f, size, size);
	glVertex3f(size, size, size);

	glEnd();
}

//component of head
void neck()
{
	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.3);
	glRotatef(-135, 1.0, 0.0, 0.0);

	//inner neck
	glColor3f(0.18f, 0.18f, 0.18f);		//black-ish grey
	drawCylinder(0.2, 0.1, 0.2);

	//outer neck
	glColor3f(1.0f, 0.7f, 0.0f);		//golden yellow
	drawHalfCylinder(0.2, 0.175, 0.2, 30);

	glPopMatrix();
}

void neckLine()
{
	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.3);
	glRotatef(-135, 1.0, 0.0, 0.0);

	//inner neck
	glColor3f(0.18f, 0.18f, 0.18f);		//black-ish grey
	drawLineCylinder(0.2, 0.1, 0.2);

	//outer neck
	glColor3f(1.0f, 0.7f, 0.0f);		//golden yellow
	drawHalfLineCylinder(0.2, 0.175, 0.2, 30);

	glPopMatrix();
}

void necklace()
{
	fourIrregularPoint point, another;

	point.fld.x = -0.75, point.fld.y = -0.1, point.fld.z = 0;
	point.flu.x = -0.75, point.flu.y = -0.01, point.flu.z = 0;
	point.fru.x = -0.575, point.fru.y = 0.1, point.fru.z = 0;
	point.frd.x = -0.55, point.frd.y = 0.09, point.frd.z = 0;

	point.bld.x = -0.825, point.bld.y = -0.15, point.bld.z = 0.15;
	point.blu.x = -0.825, point.blu.y = -0.03, point.blu.z = 0.15;
	point.bru.x = -0.575, point.bru.y = 0.1, point.bru.z = 0.15;
	point.brd.x = -0.55, point.brd.y = 0.09, point.brd.z = 0.15;

	fourPointIrregularShape(point, true, false, purple, purple, purple, purple, purple, purple);

	point = setBaToFPoint(point);

	point.bld.x = -0.775, point.bld.y = -0.15, point.bld.z = 0.4;
	point.blu.x = -0.775, point.blu.y = -0.03, point.blu.z = 0.4;
	point.bru.x = -0.575, point.bru.y = 0.1, point.bru.z = 0.25;
	point.brd.x = -0.55, point.brd.y = 0.075, point.brd.z = 0.225;

	fourPointIrregularShape(point, true, false, purple, purple, purple, purple, purple, purple);

	point = setBaToFPoint(point);

	point.bld.x = -0.7, point.bld.y = -0.15, point.bld.z = 0.5;
	point.blu.x = -0.675, point.blu.y = -0.03, point.blu.z = 0.5;
	point.bru.x = -0.475, point.bru.y = 0.1, point.bru.z = 0.25;
	point.brd.x = -0.45, point.brd.y = 0.075, point.brd.z = 0.225;

	fourPointIrregularShape(point, true, false, purple, purple, purple, purple, purple, purple);

	//joint
	point = setBaToFPoint(point);

	point.bld.x = -0.375, point.bld.y = 0.1, point.bld.z = 0.5;
	point.blu.x = -0.375, point.blu.y = 0.125, point.blu.z = 0.5;
	point.bru.x = -0.35, point.bru.y = 0.125, point.bru.z = 0.15;
	point.brd.x = -0.35, point.brd.y = 0.1, point.brd.z = 0.125;

	fourPointIrregularShape(point, true, false, purple, purple, purple, purple, purple, purple);

	another = setRToBaPoint(point);

	//front to back view, we are in -z view to +z view
	another.bld.x = -0.5, another.bld.y = 0.15, another.bld.z = 0;
	another.blu.x = -0.475, another.blu.y = 0.175, another.blu.z = 0;
	another.bru.x = -0.35, another.bru.y = 0.15, another.bru.z = 0;
	another.brd.x = -0.35, another.brd.y = 0.125, another.brd.z = 0;

	fourPointIrregularShape(another, true, false, purple, purple, purple, purple, purple, purple);

	point = setLToFPoint(point);

	point.bld.x = -0.7, point.bld.y = -0.05, point.bld.z = 0.75;
	point.blu.x = -0.675, point.blu.y = 0.07, point.blu.z = 0.75;
	point.bru.x = -0.375, point.bru.y = 0.2, point.bru.z = 0.75;
	point.brd.x = -0.35, point.brd.y = 0.175, point.brd.z = 0.75;

	fourPointIrregularShape(point, true, false, purple, purple, purple, purple, purple, purple);

	point = setBaToFPoint(point);

	point.bld.x = -0.575, point.bld.y = 0.05, point.bld.z = 1.375;
	point.blu.x = -0.55, point.blu.y = 0.17, point.blu.z = 1.375;
	point.bru.x = -0.325, point.bru.y = 0.3, point.bru.z = 1.0;
	point.brd.x = -0.3, point.brd.y = 0.275, point.brd.z = 1.0;

	fourPointIrregularShape(point, true, false, purple, purple, purple, purple, purple, purple);

	point = setBaToFPoint(point);

	point.bld.x = 0, point.bld.y = 0.15, point.bld.z = 1.675;
	point.blu.x = 0, point.blu.y = 0.27, point.blu.z = 1.675;
	point.bru.x = 0, point.bru.y = 0.4, point.bru.z = 1.25;
	point.brd.x = 0, point.brd.y = 0.375, point.brd.z = 1.25;

	fourPointIrregularShape(point, true, false, purple, purple, purple, purple, purple, purple);

	//just mirror the left side
	point.fld.x = 0.75, point.fld.y = -0.1, point.fld.z = 0;
	point.flu.x = 0.75, point.flu.y = -0.01, point.flu.z = 0;
	point.fru.x = 0.575, point.fru.y = 0.1, point.fru.z = 0;
	point.frd.x = 0.55, point.frd.y = 0.09, point.frd.z = 0;

	point.bld.x = 0.825, point.bld.y = -0.15, point.bld.z = 0.15;
	point.blu.x = 0.825, point.blu.y = -0.03, point.blu.z = 0.15;
	point.bru.x = 0.575, point.bru.y = 0.1, point.bru.z = 0.15;
	point.brd.x = 0.55, point.brd.y = 0.09, point.brd.z = 0.15;

	fourPointIrregularShape(point, true, false, purple, purple, purple, purple, purple, purple);

	point = setBaToFPoint(point);

	point.bld.x = 0.775, point.bld.y = -0.15, point.bld.z = 0.4;
	point.blu.x = 0.775, point.blu.y = -0.03, point.blu.z = 0.4;
	point.bru.x = 0.575, point.bru.y = 0.1, point.bru.z = 0.25;
	point.brd.x = 0.55, point.brd.y = 0.075, point.brd.z = 0.225;

	fourPointIrregularShape(point, true, false, purple, purple, purple, purple, purple, purple);

	point = setBaToFPoint(point);

	point.bld.x = 0.7, point.bld.y = -0.15, point.bld.z = 0.5;
	point.blu.x = 0.675, point.blu.y = -0.03, point.blu.z = 0.5;
	point.bru.x = 0.475, point.bru.y = 0.1, point.bru.z = 0.25;
	point.brd.x = 0.45, point.brd.y = 0.075, point.brd.z = 0.225;

	fourPointIrregularShape(point, true, false, purple, purple, purple, purple, purple, purple);

	//joint
	point = setBaToFPoint(point);

	point.bld.x = 0.375, point.bld.y = 0.1, point.bld.z = 0.5;
	point.blu.x = 0.375, point.blu.y = 0.125, point.blu.z = 0.5;
	point.bru.x = 0.35, point.bru.y = 0.125, point.bru.z = 0.15;
	point.brd.x = 0.35, point.brd.y = 0.1, point.brd.z = 0.125;

	fourPointIrregularShape(point, true, false, purple, purple, purple, purple, purple, purple);

	another = setRToBaPoint(point);

	//front to back view, we are in -z view to +z view
	another.bld.x = 0.5, another.bld.y = 0.15, another.bld.z = 0;
	another.blu.x = 0.475, another.blu.y = 0.175, another.blu.z = 0;
	another.bru.x = 0.35, another.bru.y = 0.15, another.bru.z = 0;
	another.brd.x = 0.35, another.brd.y = 0.125, another.brd.z = 0;

	fourPointIrregularShape(another, true, false, purple, purple, purple, purple, purple, purple);

	point = setLToFPoint(point);

	point.bld.x = 0.7, point.bld.y = -0.05, point.bld.z = 0.75;
	point.blu.x = 0.675, point.blu.y = 0.07, point.blu.z = 0.75;
	point.bru.x = 0.375, point.bru.y = 0.2, point.bru.z = 0.75;
	point.brd.x = 0.35, point.brd.y = 0.175, point.brd.z = 0.75;

	fourPointIrregularShape(point, true, false, purple, purple, purple, purple, purple, purple);

	point = setBaToFPoint(point);

	point.bld.x = 0.575, point.bld.y = 0.05, point.bld.z = 1.375;
	point.blu.x = 0.55, point.blu.y = 0.17, point.blu.z = 1.375;
	point.bru.x = 0.325, point.bru.y = 0.3, point.bru.z = 1.0;
	point.brd.x = 0.3, point.brd.y = 0.275, point.brd.z = 1.0;

	fourPointIrregularShape(point, true, false, purple, purple, purple, purple, purple, purple);

	point = setBaToFPoint(point);

	point.bld.x = 0, point.bld.y = 0.15, point.bld.z = 1.675;
	point.blu.x = 0, point.blu.y = 0.27, point.blu.z = 1.675;
	point.bru.x = 0, point.bru.y = 0.4, point.bru.z = 1.25;
	point.brd.x = 0, point.brd.y = 0.375, point.brd.z = 1.25;

	fourPointIrregularShape(point, true, false, purple, purple, purple, purple, purple, purple);

	//the left turbo
	point.fld.x = -0.6, point.fld.y = 0.075, point.fld.z = -0.1;
	point.flu.x = -0.625, point.flu.y = 0.25, point.flu.z = -0.1;
	point.fru.x = -0.325, point.fru.y = 0.375, point.fru.z = -0.1;
	point.frd.x = -0.3, point.frd.y = 0.15, point.frd.z = -0.1;

	point.bld.x = -0.6, point.bld.y = 0.075, point.bld.z = 0.1;
	point.blu.x = -0.625, point.blu.y = 0.25, point.blu.z = 0.1;
	point.bru.x = -0.325, point.bru.y = 0.375, point.bru.z = 0.1;
	point.brd.x = -0.3, point.brd.y = 0.15, point.brd.z = 0.1;

	fourPointIrregularShape(point, true, false, lime, lime, lime, lime, lime, lime);

	//the right turbo
	point.fld.x = 0.6, point.fld.y = 0.075, point.fld.z = -0.1;
	point.flu.x = 0.625, point.flu.y = 0.25, point.flu.z = -0.1;
	point.fru.x = 0.325, point.fru.y = 0.375, point.fru.z = -0.1;
	point.frd.x = 0.3, point.frd.y = 0.15, point.frd.z = -0.1;

	point.bld.x = 0.6, point.bld.y = 0.075, point.bld.z = 0.1;
	point.blu.x = 0.625, point.blu.y = 0.25, point.blu.z = 0.1;
	point.bru.x = 0.325, point.bru.y = 0.375, point.bru.z = 0.1;
	point.brd.x = 0.3, point.brd.y = 0.15, point.brd.z = 0.1;

	fourPointIrregularShape(point, true, false, lime, lime, lime, lime, lime, lime);

	//left turbo wing
	point.fld.x = -0.9, point.fld.y = 0.1, point.fld.z = -0.1;
	point.flu.x = -0.9, point.flu.y = 0.125, point.flu.z = -0.1;
	point.fru.x = -0.6, point.fru.y = 0.25, point.fru.z = -0.1;
	point.frd.x = -0.6, point.frd.y = 0.2, point.frd.z = -0.1;

	point.bld.x = -0.9, point.bld.y = 0.1, point.bld.z = 0.1;
	point.blu.x = -0.9, point.blu.y = 0.125, point.blu.z = 0.1;
	point.bru.x = -0.6, point.bru.y = 0.25, point.bru.z = 0.1;
	point.brd.x = -0.6, point.brd.y = 0.2, point.brd.z = 0.1;

	fourPointIrregularShape(point, true, false, lime, lime, lime, lime, lime, lime);

	//right turbo wing
	point.fld.x = 0.9, point.fld.y = 0.1, point.fld.z = -0.1;
	point.flu.x = 0.9, point.flu.y = 0.125, point.flu.z = -0.1;
	point.fru.x = 0.6, point.fru.y = 0.25, point.fru.z = -0.1;
	point.frd.x = 0.6, point.frd.y = 0.2, point.frd.z = -0.1;

	point.bld.x = 0.9, point.bld.y = 0.1, point.bld.z = 0.1;
	point.blu.x = 0.9, point.blu.y = 0.125, point.blu.z = 0.1;
	point.bru.x = 0.6, point.bru.y = 0.25, point.bru.z = 0.1;
	point.brd.x = 0.6, point.brd.y = 0.2, point.brd.z = 0.1;

	fourPointIrregularShape(point, true, false, lime, lime, lime, lime, lime, lime);
}

void necklaceLine()
{
	fourIrregularPoint point, another;

	point.fld.x = -0.75, point.fld.y = -0.1, point.fld.z = 0;
	point.flu.x = -0.75, point.flu.y = -0.01, point.flu.z = 0;
	point.fru.x = -0.575, point.fru.y = 0.1, point.fru.z = 0;
	point.frd.x = -0.55, point.frd.y = 0.09, point.frd.z = 0;

	point.bld.x = -0.825, point.bld.y = -0.15, point.bld.z = 0.15;
	point.blu.x = -0.825, point.blu.y = -0.03, point.blu.z = 0.15;
	point.bru.x = -0.575, point.bru.y = 0.1, point.bru.z = 0.15;
	point.brd.x = -0.55, point.brd.y = 0.09, point.brd.z = 0.15;

	fourPointIrregularShape(point, false, false, purple, purple, purple, purple, purple, purple);

	point = setBaToFPoint(point);

	point.bld.x = -0.775, point.bld.y = -0.15, point.bld.z = 0.4;
	point.blu.x = -0.775, point.blu.y = -0.03, point.blu.z = 0.4;
	point.bru.x = -0.575, point.bru.y = 0.1, point.bru.z = 0.25;
	point.brd.x = -0.55, point.brd.y = 0.075, point.brd.z = 0.225;

	fourPointIrregularShape(point, false, false, purple, purple, purple, purple, purple, purple);

	point = setBaToFPoint(point);

	point.bld.x = -0.7, point.bld.y = -0.15, point.bld.z = 0.5;
	point.blu.x = -0.675, point.blu.y = -0.03, point.blu.z = 0.5;
	point.bru.x = -0.475, point.bru.y = 0.1, point.bru.z = 0.25;
	point.brd.x = -0.45, point.brd.y = 0.075, point.brd.z = 0.225;

	fourPointIrregularShape(point, false, false, purple, purple, purple, purple, purple, purple);

	//joint
	point = setBaToFPoint(point);

	point.bld.x = -0.375, point.bld.y = 0.1, point.bld.z = 0.5;
	point.blu.x = -0.375, point.blu.y = 0.125, point.blu.z = 0.5;
	point.bru.x = -0.35, point.bru.y = 0.125, point.bru.z = 0.15;
	point.brd.x = -0.35, point.brd.y = 0.1, point.brd.z = 0.125;

	fourPointIrregularShape(point, false, false, purple, purple, purple, purple, purple, purple);

	another = setRToBaPoint(point);

	//front to back view, we are in -z view to +z view
	another.bld.x = -0.5, another.bld.y = 0.15, another.bld.z = 0;
	another.blu.x = -0.475, another.blu.y = 0.175, another.blu.z = 0;
	another.bru.x = -0.35, another.bru.y = 0.15, another.bru.z = 0;
	another.brd.x = -0.35, another.brd.y = 0.125, another.brd.z = 0;

	fourPointIrregularShape(another, false, false, purple, purple, purple, purple, purple, purple);

	point = setLToFPoint(point);

	point.bld.x = -0.7, point.bld.y = -0.05, point.bld.z = 0.75;
	point.blu.x = -0.675, point.blu.y = 0.07, point.blu.z = 0.75;
	point.bru.x = -0.375, point.bru.y = 0.2, point.bru.z = 0.75;
	point.brd.x = -0.35, point.brd.y = 0.175, point.brd.z = 0.75;

	fourPointIrregularShape(point, false, false, purple, purple, purple, purple, purple, purple);

	point = setBaToFPoint(point);

	point.bld.x = -0.575, point.bld.y = 0.05, point.bld.z = 1.375;
	point.blu.x = -0.55, point.blu.y = 0.17, point.blu.z = 1.375;
	point.bru.x = -0.325, point.bru.y = 0.3, point.bru.z = 1.0;
	point.brd.x = -0.3, point.brd.y = 0.275, point.brd.z = 1.0;

	fourPointIrregularShape(point, false, false, purple, purple, purple, purple, purple, purple);

	point = setBaToFPoint(point);

	point.bld.x = 0, point.bld.y = 0.15, point.bld.z = 1.675;
	point.blu.x = 0, point.blu.y = 0.27, point.blu.z = 1.675;
	point.bru.x = 0, point.bru.y = 0.4, point.bru.z = 1.25;
	point.brd.x = 0, point.brd.y = 0.375, point.brd.z = 1.25;

	fourPointIrregularShape(point, false, false, purple, purple, purple, purple, purple, purple);

	//just mirror the left side
	point.fld.x = 0.75, point.fld.y = -0.1, point.fld.z = 0;
	point.flu.x = 0.75, point.flu.y = -0.01, point.flu.z = 0;
	point.fru.x = 0.575, point.fru.y = 0.1, point.fru.z = 0;
	point.frd.x = 0.55, point.frd.y = 0.09, point.frd.z = 0;

	point.bld.x = 0.825, point.bld.y = -0.15, point.bld.z = 0.15;
	point.blu.x = 0.825, point.blu.y = -0.03, point.blu.z = 0.15;
	point.bru.x = 0.575, point.bru.y = 0.1, point.bru.z = 0.15;
	point.brd.x = 0.55, point.brd.y = 0.09, point.brd.z = 0.15;

	fourPointIrregularShape(point, false, false, purple, purple, purple, purple, purple, purple);

	point = setBaToFPoint(point);

	point.bld.x = 0.775, point.bld.y = -0.15, point.bld.z = 0.4;
	point.blu.x = 0.775, point.blu.y = -0.03, point.blu.z = 0.4;
	point.bru.x = 0.575, point.bru.y = 0.1, point.bru.z = 0.25;
	point.brd.x = 0.55, point.brd.y = 0.075, point.brd.z = 0.225;

	fourPointIrregularShape(point, false, false, purple, purple, purple, purple, purple, purple);

	point = setBaToFPoint(point);

	point.bld.x = 0.7, point.bld.y = -0.15, point.bld.z = 0.5;
	point.blu.x = 0.675, point.blu.y = -0.03, point.blu.z = 0.5;
	point.bru.x = 0.475, point.bru.y = 0.1, point.bru.z = 0.25;
	point.brd.x = 0.45, point.brd.y = 0.075, point.brd.z = 0.225;

	fourPointIrregularShape(point, false, false, purple, purple, purple, purple, purple, purple);

	//joint
	point = setBaToFPoint(point);

	point.bld.x = 0.375, point.bld.y = 0.1, point.bld.z = 0.5;
	point.blu.x = 0.375, point.blu.y = 0.125, point.blu.z = 0.5;
	point.bru.x = 0.35, point.bru.y = 0.125, point.bru.z = 0.15;
	point.brd.x = 0.35, point.brd.y = 0.1, point.brd.z = 0.125;

	fourPointIrregularShape(point, false, false, purple, purple, purple, purple, purple, purple);

	another = setRToBaPoint(point);

	//front to back view, we are in -z view to +z view
	another.bld.x = 0.5, another.bld.y = 0.15, another.bld.z = 0;
	another.blu.x = 0.475, another.blu.y = 0.175, another.blu.z = 0;
	another.bru.x = 0.35, another.bru.y = 0.15, another.bru.z = 0;
	another.brd.x = 0.35, another.brd.y = 0.125, another.brd.z = 0;

	fourPointIrregularShape(another, false, false, purple, purple, purple, purple, purple, purple);

	point = setLToFPoint(point);

	point.bld.x = 0.7, point.bld.y = -0.05, point.bld.z = 0.75;
	point.blu.x = 0.675, point.blu.y = 0.07, point.blu.z = 0.75;
	point.bru.x = 0.375, point.bru.y = 0.2, point.bru.z = 0.75;
	point.brd.x = 0.35, point.brd.y = 0.175, point.brd.z = 0.75;

	fourPointIrregularShape(point, false, false, purple, purple, purple, purple, purple, purple);

	point = setBaToFPoint(point);

	point.bld.x = 0.575, point.bld.y = 0.05, point.bld.z = 1.375;
	point.blu.x = 0.55, point.blu.y = 0.17, point.blu.z = 1.375;
	point.bru.x = 0.325, point.bru.y = 0.3, point.bru.z = 1.0;
	point.brd.x = 0.3, point.brd.y = 0.275, point.brd.z = 1.0;

	fourPointIrregularShape(point, false, false, purple, purple, purple, purple, purple, purple);

	point = setBaToFPoint(point);

	point.bld.x = 0, point.bld.y = 0.15, point.bld.z = 1.675;
	point.blu.x = 0, point.blu.y = 0.27, point.blu.z = 1.675;
	point.bru.x = 0, point.bru.y = 0.4, point.bru.z = 1.25;
	point.brd.x = 0, point.brd.y = 0.375, point.brd.z = 1.25;

	fourPointIrregularShape(point, false, false, purple, purple, purple, purple, purple, purple);

	//the left turbo
	point.fld.x = -0.6, point.fld.y = 0.075, point.fld.z = -0.1;
	point.flu.x = -0.625, point.flu.y = 0.25, point.flu.z = -0.1;
	point.fru.x = -0.325, point.fru.y = 0.375, point.fru.z = -0.1;
	point.frd.x = -0.3, point.frd.y = 0.15, point.frd.z = -0.1;

	point.bld.x = -0.6, point.bld.y = 0.075, point.bld.z = 0.1;
	point.blu.x = -0.625, point.blu.y = 0.25, point.blu.z = 0.1;
	point.bru.x = -0.325, point.bru.y = 0.375, point.bru.z = 0.1;
	point.brd.x = -0.3, point.brd.y = 0.15, point.brd.z = 0.1;

	fourPointIrregularShape(point, false, false, lime, lime, lime, lime, lime, lime);

	//the right turbo
	point.fld.x = 0.6, point.fld.y = 0.075, point.fld.z = -0.1;
	point.flu.x = 0.625, point.flu.y = 0.25, point.flu.z = -0.1;
	point.fru.x = 0.325, point.fru.y = 0.375, point.fru.z = -0.1;
	point.frd.x = 0.3, point.frd.y = 0.15, point.frd.z = -0.1;

	point.bld.x = 0.6, point.bld.y = 0.075, point.bld.z = 0.1;
	point.blu.x = 0.625, point.blu.y = 0.25, point.blu.z = 0.1;
	point.bru.x = 0.325, point.bru.y = 0.375, point.bru.z = 0.1;
	point.brd.x = 0.3, point.brd.y = 0.15, point.brd.z = 0.1;

	fourPointIrregularShape(point, false, false, lime, lime, lime, lime, lime, lime);

	//left turbo wing
	point.fld.x = -0.9, point.fld.y = 0.1, point.fld.z = -0.1;
	point.flu.x = -0.9, point.flu.y = 0.125, point.flu.z = -0.1;
	point.fru.x = -0.6, point.fru.y = 0.25, point.fru.z = -0.1;
	point.frd.x = -0.6, point.frd.y = 0.2, point.frd.z = -0.1;

	point.bld.x = -0.9, point.bld.y = 0.1, point.bld.z = 0.1;
	point.blu.x = -0.9, point.blu.y = 0.125, point.blu.z = 0.1;
	point.bru.x = -0.6, point.bru.y = 0.25, point.bru.z = 0.1;
	point.brd.x = -0.6, point.brd.y = 0.2, point.brd.z = 0.1;

	fourPointIrregularShape(point, false, false, lime, lime, lime, lime, lime, lime);

	//right turbo wing
	point.fld.x = 0.9, point.fld.y = 0.1, point.fld.z = -0.1;
	point.flu.x = 0.9, point.flu.y = 0.125, point.flu.z = -0.1;
	point.fru.x = 0.6, point.fru.y = 0.25, point.fru.z = -0.1;
	point.frd.x = 0.6, point.frd.y = 0.2, point.frd.z = -0.1;

	point.bld.x = 0.9, point.bld.y = 0.1, point.bld.z = 0.1;
	point.blu.x = 0.9, point.blu.y = 0.125, point.blu.z = 0.1;
	point.bru.x = 0.6, point.bru.y = 0.25, point.bru.z = 0.1;
	point.brd.x = 0.6, point.brd.y = 0.2, point.brd.z = 0.1;

	fourPointIrregularShape(point, false, false, lime, lime, lime, lime, lime, lime);
}

void brain()
{
	glPushMatrix();

	glTranslatef(0.0, 0.5, 0.675);

	glColor3f(0.8, 0.8, 0.8);		//white-ish grey

	drawSphere(0.25);

	glPopMatrix();
}

void brainLine()
{
	glPushMatrix();

	glTranslatef(0.0, 0.5, 0.675);

	glColor3f(0.8, 0.8, 0.8);		//white-ish grey

	drawLineSphere(0.25);

	glPopMatrix();
}

void faceArmor()
{
	//lower part
	glColor3f(purple.r, purple.g, purple.b);		//purple

	glPushMatrix();

	glTranslatef(0.0, 0.3, -0.1);
	glRotatef(-90, 1.0, 0.0, 0.0);

	drawHalfCylinder(0.3, 0.5, 0.1);

	glPopMatrix();

	//upper part
	glColor3f(darkPurple.r, darkPurple.g, darkPurple.b);		//dark purple

	glPushMatrix();

	glTranslatef(0.0, 0.4, -0.1);
	glRotatef(-90, 1.0, 0.0, 0.0);

	drawHalfCylinder(0.5, 0.3, 0.1);

	glPopMatrix();

	fourIrregularPoint point;

	//left side extend
	point.fld.x = -0.3, point.fld.y = 0.3, point.fld.z = -0.1;
	point.flu.x = -0.5, point.flu.y = 0.4, point.flu.z = -0.1;
	point.fru.x = -0.3, point.fru.y = 0.5, point.fru.z = -0.1;
	point.frd.x = -0.3, point.frd.y = 0.4, point.frd.z = -0.1;

	point.bld.x = -0.3, point.bld.y = 0.35, point.bld.z = 0.1;
	point.blu.x = -0.5, point.blu.y = 0.45, point.blu.z = 0.1;
	point.bru.x = -0.3, point.bru.y = 0.55, point.bru.z = 0.1;
	point.brd.x = -0.3, point.brd.y = 0.45, point.brd.z = 0.1;

	fourPointIrregularShape(point, true, false, purple, purple, purple, purple, purple, purple);

	//right side extend
	point.fld.x = 0.3, point.fld.y = 0.3, point.fld.z = -0.1;
	point.flu.x = 0.5, point.flu.y = 0.4, point.flu.z = -0.1;
	point.fru.x = 0.3, point.fru.y = 0.5, point.fru.z = -0.1;
	point.frd.x = 0.3, point.frd.y = 0.4, point.frd.z = -0.1;

	point.bld.x = 0.3, point.bld.y = 0.35, point.bld.z = 0.1;
	point.blu.x = 0.5, point.blu.y = 0.45, point.blu.z = 0.1;
	point.bru.x = 0.3, point.bru.y = 0.55, point.bru.z = 0.1;
	point.brd.x = 0.3, point.brd.y = 0.45, point.brd.z = 0.1;

	fourPointIrregularShape(point, true, false, darkPurple, darkPurple, darkPurple, darkPurple, darkPurple, darkPurple);
}

void faceArmorLine()
{
	//lower part
	glColor3f(purple.r, purple.g, purple.b);		//purple

	glPushMatrix();

	glTranslatef(0.0, 0.3, -0.1);
	glRotatef(-90, 1.0, 0.0, 0.0);

	drawHalfLineCylinder(0.3, 0.5, 0.1);

	glPopMatrix();

	//upper part
	glColor3f(darkPurple.r, darkPurple.g, darkPurple.b);		//dark purple

	glPushMatrix();

	glTranslatef(0.0, 0.4, -0.1);
	glRotatef(-90, 1.0, 0.0, 0.0);

	drawHalfLineCylinder(0.5, 0.3, 0.1);

	glPopMatrix();

	fourIrregularPoint point;

	//left side extend
	point.fld.x = -0.3, point.fld.y = 0.3, point.fld.z = -0.1;
	point.flu.x = -0.5, point.flu.y = 0.4, point.flu.z = -0.1;
	point.fru.x = -0.3, point.fru.y = 0.5, point.fru.z = -0.1;
	point.frd.x = -0.3, point.frd.y = 0.4, point.frd.z = -0.1;

	point.bld.x = -0.3, point.bld.y = 0.35, point.bld.z = 0.1;
	point.blu.x = -0.5, point.blu.y = 0.45, point.blu.z = 0.1;
	point.bru.x = -0.3, point.bru.y = 0.55, point.bru.z = 0.1;
	point.brd.x = -0.3, point.brd.y = 0.45, point.brd.z = 0.1;

	fourPointIrregularShape(point, false, false, purple, purple, purple, purple, purple, purple);

	//right side extend
	point.fld.x = 0.3, point.fld.y = 0.3, point.fld.z = -0.1;
	point.flu.x = 0.5, point.flu.y = 0.4, point.flu.z = -0.1;
	point.fru.x = 0.3, point.fru.y = 0.5, point.fru.z = -0.1;
	point.frd.x = 0.3, point.frd.y = 0.4, point.frd.z = -0.1;

	point.bld.x = 0.3, point.bld.y = 0.35, point.bld.z = 0.1;
	point.blu.x = 0.5, point.blu.y = 0.45, point.blu.z = 0.1;
	point.bru.x = 0.3, point.bru.y = 0.55, point.bru.z = 0.1;
	point.brd.x = 0.3, point.brd.y = 0.45, point.brd.z = 0.1;

	fourPointIrregularShape(point, false, false, darkPurple, darkPurple, darkPurple, darkPurple, darkPurple, darkPurple);
}

void topArmor()
{
	glColor3f(purple.r, purple.g, purple.b);		//purple

	//back piece
	glPushMatrix();

	glTranslatef(0.0, 0.575, 0.45);

	drawHalfCylinder(0.3, 0.25, 0.5);

	glPushMatrix();

	glTranslatef(0.0, 0.0, 0.5);

	drawHalfCylinder(0.25, 0.1, 0.2);

	glPushMatrix();

	glTranslatef(0.0, 0.0, 0.2);

	drawHalfCylinder(0.1, 0.0, 0.05);

	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	//front piece
	glPushMatrix();

	glTranslatef(0.0, 0.575, -0.05);

	drawHalfCylinder(0.2, 0.25, 0.6);

	glPushMatrix();

	glTranslatef(0.0, 0.0, -0.2);

	drawHalfCylinder(0.1, 0.2, 0.2);

	glPushMatrix();

	glTranslatef(0.0, 0.0, -0.05);

	drawHalfCylinder(0.0, 0.1, 0.05);

	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	//the between
	glPushMatrix();

	glTranslatef(0.0, 0.575, 0.5);

	glColor3f(black.r, black.g, black.b);		//black

	drawHalfCylinder(0.0, 0.2825, 0.0);

	glPopMatrix();

	fourIrregularPoint point;

	//left side lower
	point.fld.x = -0.225, point.fld.y = 0.5, point.fld.z = 0.0;
	point.flu.x = -0.1875, point.flu.y = 0.65, point.flu.z = 0.0;
	point.fru.x = -0.175, point.fru.y = 0.65, point.fru.z = 0.0;
	point.frd.x = -0.1875, point.frd.y = 0.5, point.frd.z = 0.0;

	point.bld.x = -0.325, point.bld.y = 0.4, point.bld.z = 0.35;
	point.blu.x = -0.3, point.blu.y = 0.65, point.blu.z = 0.55;
	point.bru.x = -0.275, point.bru.y = 0.65, point.bru.z = 0.55;
	point.brd.x = -0.3, point.brd.y = 0.4, point.brd.z = 0.35;

	fourPointIrregularShape(point, true, false, purple, purple, purple, purple, purple, purple);

	//right side lower
	point.fld.x = 0.225, point.fld.y = 0.5, point.fld.z = 0.0;
	point.flu.x = 0.1875, point.flu.y = 0.65, point.flu.z = 0.0;
	point.fru.x = 0.175, point.fru.y = 0.65, point.fru.z = 0.0;
	point.frd.x = 0.1875, point.frd.y = 0.5, point.frd.z = 0.0;

	point.bld.x = 0.35, point.bld.y = 0.4, point.bld.z = 0.35;
	point.blu.x = 0.325, point.blu.y = 0.65, point.blu.z = 0.55;
	point.bru.x = 0.3, point.bru.y = 0.65, point.bru.z = 0.55;
	point.brd.x = 0.325, point.brd.y = 0.4, point.brd.z = 0.35;

	fourPointIrregularShape(point, true, false, darkPurple, darkPurple, darkPurple, darkPurple, darkPurple, darkPurple);
}

void topArmorLine()
{
	glColor3f(purple.r, purple.g, purple.b);		//purple

	//back piece
	glPushMatrix();

	glTranslatef(0.0, 0.575, 0.45);

	drawHalfLineCylinder(0.3, 0.25, 0.5);

	glPushMatrix();

	glTranslatef(0.0, 0.0, 0.5);

	drawHalfLineCylinder(0.25, 0.1, 0.2);

	glPushMatrix();

	glTranslatef(0.0, 0.0, 0.2);

	drawHalfLineCylinder(0.1, 0.0, 0.05);

	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	//front piece
	glPushMatrix();

	glTranslatef(0.0, 0.575, -0.05);

	drawHalfLineCylinder(0.2, 0.25, 0.6);

	glPushMatrix();

	glTranslatef(0.0, 0.0, -0.2);

	drawHalfLineCylinder(0.1, 0.2, 0.2);

	glPushMatrix();

	glTranslatef(0.0, 0.0, -0.05);

	drawHalfLineCylinder(0.0, 0.1, 0.05);

	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	//the between
	glPushMatrix();

	glTranslatef(0.0, 0.575, 0.5);

	glColor3f(black.r, black.g, black.b);		//black

	drawHalfLineCylinder(0.0, 0.2825, 0.0);

	glPopMatrix();

	fourIrregularPoint point;

	//left side lower
	point.fld.x = -0.225, point.fld.y = 0.5, point.fld.z = 0.0;
	point.flu.x = -0.1875, point.flu.y = 0.65, point.flu.z = 0.0;
	point.fru.x = -0.175, point.fru.y = 0.65, point.fru.z = 0.0;
	point.frd.x = -0.1875, point.frd.y = 0.5, point.frd.z = 0.0;

	point.bld.x = -0.325, point.bld.y = 0.4, point.bld.z = 0.35;
	point.blu.x = -0.3, point.blu.y = 0.65, point.blu.z = 0.55;
	point.bru.x = -0.275, point.bru.y = 0.65, point.bru.z = 0.55;
	point.brd.x = -0.3, point.brd.y = 0.4, point.brd.z = 0.35;

	fourPointIrregularShape(point, false, false, purple, purple, purple, purple, purple, purple);

	//right side lower
	point.fld.x = 0.225, point.fld.y = 0.5, point.fld.z = 0.0;
	point.flu.x = 0.1875, point.flu.y = 0.65, point.flu.z = 0.0;
	point.fru.x = 0.175, point.fru.y = 0.65, point.fru.z = 0.0;
	point.frd.x = 0.1875, point.frd.y = 0.5, point.frd.z = 0.0;

	point.bld.x = 0.35, point.bld.y = 0.4, point.bld.z = 0.35;
	point.blu.x = 0.325, point.blu.y = 0.65, point.blu.z = 0.55;
	point.bru.x = 0.3, point.bru.y = 0.65, point.bru.z = 0.55;
	point.brd.x = 0.325, point.brd.y = 0.4, point.brd.z = 0.35;

	fourPointIrregularShape(point, false, false, darkPurple, darkPurple, darkPurple, darkPurple, darkPurple, darkPurple);
}

void eyes()
{
	fourIrregularPoint point;

	//left eye
	point.fld.x = -0.3, point.fld.y = 0.45, point.fld.z = -0.3;
	point.flu.x = -0.05, point.flu.y = 0.55, point.flu.z = -0.3;
	point.fru.x = -0.025, point.fru.y = 0.6, point.fru.z = -0.3;
	point.frd.x = -0.025, point.frd.y = 0.45, point.frd.z = -0.3;

	point.bld.x = -0.3, point.bld.y = 0.4, point.bld.z = 0.15;
	point.blu.x = -0.125, point.blu.y = 0.65, point.blu.z = 0.15;
	point.bru.x = -0.1, point.bru.y = 0.65, point.bru.z = 0.15;
	point.brd.x = -0.1, point.brd.y = 0.4, point.brd.z = 0.15;

	fourPointIrregularShape(point, true, false, black, black, black, black, black, black);

	//left eye ball

	point.fld.x = -0.2125, point.fld.y = 0.475, point.fld.z = -0.3;
	point.flu.x = -0.0625, point.flu.y = 0.525, point.flu.z = -0.3;
	point.fru.x = -0.0625, point.fru.y = 0.525, point.fru.z = -0.3;
	point.frd.x = -0.2125, point.frd.y = 0.475, point.frd.z = -0.3;

	point.bld.x = -0.2125, point.bld.y = 0.525, point.bld.z = -0.1;
	point.blu.x = -0.1125, point.blu.y = 0.625, point.blu.z = -0.1;
	point.bru.x = -0.1125, point.bru.y = 0.625, point.bru.z = -0.1;
	point.brd.x = -0.2125, point.brd.y = 0.525, point.brd.z = -0.1;

	fourPointIrregularShape(point, true, false, white, white, white, white, white, white);

	//right eye
	point.fld.x = 0.3, point.fld.y = 0.45, point.fld.z = -0.3;
	point.flu.x = 0.05, point.flu.y = 0.55, point.flu.z = -0.3;
	point.fru.x = 0.025, point.fru.y = 0.6, point.fru.z = -0.3;
	point.frd.x = 0.025, point.frd.y = 0.45, point.frd.z = -0.3;

	point.bld.x = 0.3, point.bld.y = 0.4, point.bld.z = 0.15;
	point.blu.x = 0.125, point.blu.y = 0.65, point.blu.z = 0.15;
	point.bru.x = 0.1, point.bru.y = 0.65, point.bru.z = 0.15;
	point.brd.x = 0.1, point.brd.y = 0.4, point.brd.z = 0.15;

	fourPointIrregularShape(point, true, false, black, black, black, black, black, black);

	//right eye ball

	point.fld.x = 0.2125, point.fld.y = 0.475, point.fld.z = -0.3;
	point.flu.x = 0.0625, point.flu.y = 0.525, point.flu.z = -0.3;
	point.fru.x = 0.0625, point.fru.y = 0.525, point.fru.z = -0.3;
	point.frd.x = 0.2125, point.frd.y = 0.475, point.frd.z = -0.3;

	point.bld.x = 0.2125, point.bld.y = 0.525, point.bld.z = -0.1;
	point.blu.x = 0.1125, point.blu.y = 0.625, point.blu.z = -0.1;
	point.bru.x = 0.1125, point.bru.y = 0.625, point.bru.z = -0.1;
	point.brd.x = 0.2125, point.brd.y = 0.525, point.brd.z = -0.1;

	fourPointIrregularShape(point, true, false, white, white, white, white, white, white);

	//eye side
	glPushMatrix();

	glTranslatef(0.0, 0.4, 0.0);
	glRotatef(-90, 1.0, 0.0, 0.0);

	glColor3f(purple.r, purple.g, purple.b);		//purple

	drawHalfCylinder(0.5, 0.1, 0.2);

	glPopMatrix();

	//eye middle
	glPushMatrix();

	glTranslatef(0.0, 0.4, -0.2);
	glRotatef(-90, 1.0, 0.0, 0.0);

	glColor3f(purple.r, purple.g, purple.b);		//purple

	drawHalfCylinder(0.35, 0.0, 0.2);

	glPopMatrix();
}

void eyesLine()
{
	fourIrregularPoint point;

	//left eye
	point.fld.x = -0.3, point.fld.y = 0.45, point.fld.z = -0.3;
	point.flu.x = -0.05, point.flu.y = 0.55, point.flu.z = -0.3;
	point.fru.x = -0.025, point.fru.y = 0.6, point.fru.z = -0.3;
	point.frd.x = -0.025, point.frd.y = 0.45, point.frd.z = -0.3;

	point.bld.x = -0.3, point.bld.y = 0.4, point.bld.z = 0.15;
	point.blu.x = -0.125, point.blu.y = 0.65, point.blu.z = 0.15;
	point.bru.x = -0.1, point.bru.y = 0.65, point.bru.z = 0.15;
	point.brd.x = -0.1, point.brd.y = 0.4, point.brd.z = 0.15;

	fourPointIrregularShape(point, false, false, black, black, black, black, black, black);

	//left eye ball

	point.fld.x = -0.2125, point.fld.y = 0.475, point.fld.z = -0.3;
	point.flu.x = -0.0625, point.flu.y = 0.525, point.flu.z = -0.3;
	point.fru.x = -0.0625, point.fru.y = 0.525, point.fru.z = -0.3;
	point.frd.x = -0.2125, point.frd.y = 0.475, point.frd.z = -0.3;

	point.bld.x = -0.2125, point.bld.y = 0.525, point.bld.z = -0.1;
	point.blu.x = -0.1125, point.blu.y = 0.625, point.blu.z = -0.1;
	point.bru.x = -0.1125, point.bru.y = 0.625, point.bru.z = -0.1;
	point.brd.x = -0.2125, point.brd.y = 0.525, point.brd.z = -0.1;

	fourPointIrregularShape(point, false, false, white, white, white, white, white, white);

	//right eye
	point.fld.x = 0.3, point.fld.y = 0.45, point.fld.z = -0.3;
	point.flu.x = 0.05, point.flu.y = 0.55, point.flu.z = -0.3;
	point.fru.x = 0.025, point.fru.y = 0.6, point.fru.z = -0.3;
	point.frd.x = 0.025, point.frd.y = 0.45, point.frd.z = -0.3;

	point.bld.x = 0.3, point.bld.y = 0.4, point.bld.z = 0.15;
	point.blu.x = 0.125, point.blu.y = 0.65, point.blu.z = 0.15;
	point.bru.x = 0.1, point.bru.y = 0.65, point.bru.z = 0.15;
	point.brd.x = 0.1, point.brd.y = 0.4, point.brd.z = 0.15;

	fourPointIrregularShape(point, false, false, black, black, black, black, black, black);

	//right eye ball

	point.fld.x = 0.2125, point.fld.y = 0.475, point.fld.z = -0.3;
	point.flu.x = 0.0625, point.flu.y = 0.525, point.flu.z = -0.3;
	point.fru.x = 0.0625, point.fru.y = 0.525, point.fru.z = -0.3;
	point.frd.x = 0.2125, point.frd.y = 0.475, point.frd.z = -0.3;

	point.bld.x = 0.2125, point.bld.y = 0.525, point.bld.z = -0.1;
	point.blu.x = 0.1125, point.blu.y = 0.625, point.blu.z = -0.1;
	point.bru.x = 0.1125, point.bru.y = 0.625, point.bru.z = -0.1;
	point.brd.x = 0.2125, point.brd.y = 0.525, point.brd.z = -0.1;

	fourPointIrregularShape(point, false, false, white, white, white, white, white, white);

	//eye side
	glPushMatrix();

	glTranslatef(0.0, 0.4, 0.0);
	glRotatef(-90, 1.0, 0.0, 0.0);

	glColor3f(purple.r, purple.g, purple.b);		//purple

	drawHalfLineCylinder(0.5, 0.1, 0.2);

	glPopMatrix();

	//eye middle
	glPushMatrix();

	glTranslatef(0.0, 0.4, -0.2);
	glRotatef(-90, 1.0, 0.0, 0.0);

	glColor3f(purple.r, purple.g, purple.b);		//purple

	drawHalfLineCylinder(0.35, 0.0, 0.2);

	glPopMatrix();
}

void mouth()
{
	glColor3f(0.8, 0.8, 0.8);		//white-ish, grey

	glPushMatrix();

	glTranslatef(0.0, -0.1, -0.1);
	glRotatef(-90, 1.0, 0.0, 0.0);

	drawHalfCylinder(0.0, 0.3, 0.4);

	glPopMatrix();
}

void mouthLine()
{
	glColor3f(0.8, 0.8, 0.8);		//white-ish, grey

	glPushMatrix();

	glTranslatef(0.0, -0.1, -0.1);
	glRotatef(-90, 1.0, 0.0, 0.0);

	drawHalfLineCylinder(0.0, 0.3, 0.4);

	glPopMatrix();
}

void corn()
{
	glColor3f(darkPurple.r, darkPurple.g, darkPurple.b);		//dark purple

	glPushMatrix();

	glTranslatef(0.0, 0.6, -0.2);
	glRotatef(250.0, 1.0, 0.0, 0.0);

	drawCylinder(0.05, 0.025, 0.75);

	glColor3f(lime.r, lime.g, lime.b);		//lime

	glPushMatrix();

	glTranslatef(0.0, 0.0, 0.1875);

	drawCylinder(0.05, 0.05, 0.1875);

	glPopMatrix();

	glColor3f(purple.r, purple.g, purple.b);		//purple

	glPushMatrix();

	glTranslatef(0.0, 0.0, 0.75);

	drawCylinder(0.025, 0.0, 0.0);

	glPopMatrix();
	glPopMatrix();
}

void cornLine()
{
	glColor3f(darkPurple.r, darkPurple.g, darkPurple.b);		//dark purple

	glPushMatrix();

	glTranslatef(0.0, 0.6, -0.2);
	glRotatef(250.0, 1.0, 0.0, 0.0);

	drawLineCylinder(0.05, 0.025, 0.75);

	glColor3f(lime.r, lime.g, lime.b);		//lime

	glPushMatrix();

	glTranslatef(0.0, 0.0, 0.1875);

	drawLineCylinder(0.05, 0.05, 0.1875);

	glPopMatrix();

	glColor3f(purple.r, purple.g, purple.b);		//purple

	glPushMatrix();

	glTranslatef(0.0, 0.0, 0.75);

	drawLineCylinder(0.025, 0.0, 0.0);

	glPopMatrix();
	glPopMatrix();
}

void weapon()
{
	glColor3f(lime.r, lime.g, lime.b);

	//bottom spike lower
	glPushMatrix();

	glTranslatef(0.0, -0.9, 0.0);
	glRotatef(-90, 1.0, 0.0, 0.0);

	drawCylinder(0.0, 0.1, 0.15);

	glPopMatrix();

	//bottom spike upper
	glPushMatrix();

	glTranslatef(0.0, -0.75, 0.0);
	glRotatef(-90, 1.0, 0.0, 0.0);

	drawCylinder(0.1, 0.0, 0.15);

	glPopMatrix();

	glColor3f(purple.r, purple.g, purple.b);

	//handle
	glPushMatrix();

	glTranslatef(0.0, -0.7, 0.0);
	glRotatef(-90, 1.0, 0.0, 0.0);

	drawCylinder(0.05, 0.05, 0.8);

	glPopMatrix();

	//left joint
	glPushMatrix();

	glRotatef(-90, 0.0, 1.0, 0.0);
	glRotatef(-45, 1.0, 0.0, 0.0);

	glColor3f(lime.r, lime.g, lime.b);
	drawSphere(0.075);
	glColor3f(darkPurple.r, darkPurple.g, darkPurple.b);
	drawCylinder(0.05, 0.1, 0.3);

	glPushMatrix();

	glTranslatef(0.0, 0.0, 0.3);
	glRotatef(-56.25, 1.0, 0.0, 0.0);

	glColor3f(lime.r, lime.g, lime.b);
	drawSphere(0.1);
	glColor3f(darkPurple.r, darkPurple.g, darkPurple.b);
	drawCylinder(0.1, 0.0, 0.5);

	glPopMatrix();
	glPopMatrix();

	//right joint
	glPushMatrix();

	glRotatef(-90, 0.0, 1.0, 0.0);
	glRotatef(-135, 1.0, 0.0, 0.0);

	glColor3f(lime.r, lime.g, lime.b);
	drawSphere(0.075);
	glColor3f(darkPurple.r, darkPurple.g, darkPurple.b);
	drawCylinder(0.05, 0.1, 0.3);

	glPushMatrix();

	glTranslatef(0.0, 0.0, 0.3);
	glRotatef(56.25, 1.0, 0.0, 0.0);

	glColor3f(lime.r, lime.g, lime.b);
	drawSphere(0.1);
	glColor3f(darkPurple.r, darkPurple.g, darkPurple.b);
	drawCylinder(0.1, 0.0, 0.5);

	glPopMatrix();
	glPopMatrix();
}

void weaponLine()
{
	glColor3f(lime.r, lime.g, lime.b);

	//bottom spike lower
	glPushMatrix();

	glTranslatef(0.0, -0.9, 0.0);
	glRotatef(-90, 1.0, 0.0, 0.0);

	drawLineCylinder(0.0, 0.1, 0.15);

	glPopMatrix();

	//bottom spike upper
	glPushMatrix();

	glTranslatef(0.0, -0.75, 0.0);
	glRotatef(-90, 1.0, 0.0, 0.0);

	drawLineCylinder(0.1, 0.0, 0.15);

	glPopMatrix();

	glColor3f(purple.r, purple.g, purple.b);

	//handle
	glPushMatrix();

	glTranslatef(0.0, -0.7, 0.0);
	glRotatef(-90, 1.0, 0.0, 0.0);

	drawLineCylinder(0.05, 0.05, 0.8);

	glPopMatrix();

	//left joint
	glPushMatrix();

	glRotatef(-90, 0.0, 1.0, 0.0);
	glRotatef(-45, 1.0, 0.0, 0.0);

	glColor3f(lime.r, lime.g, lime.b);
	drawLineSphere(0.075);
	glColor3f(darkPurple.r, darkPurple.g, darkPurple.b);
	drawLineCylinder(0.05, 0.1, 0.3);

	glPushMatrix();

	glTranslatef(0.0, 0.0, 0.3);
	glRotatef(-56.25, 1.0, 0.0, 0.0);

	glColor3f(lime.r, lime.g, lime.b);
	drawLineSphere(0.1);
	glColor3f(darkPurple.r, darkPurple.g, darkPurple.b);
	drawLineCylinder(0.1, 0.0, 0.5);

	glPopMatrix();
	glPopMatrix();

	//right joint
	glPushMatrix();

	glRotatef(-90, 0.0, 1.0, 0.0);
	glRotatef(-135, 1.0, 0.0, 0.0);

	glColor3f(lime.r, lime.g, lime.b);
	drawLineSphere(0.075);
	glColor3f(darkPurple.r, darkPurple.g, darkPurple.b);
	drawLineCylinder(0.05, 0.1, 0.3);

	glPushMatrix();

	glTranslatef(0.0, 0.0, 0.3);
	glRotatef(56.25, 1.0, 0.0, 0.0);

	glColor3f(lime.r, lime.g, lime.b);
	drawLineSphere(0.1);
	glColor3f(darkPurple.r, darkPurple.g, darkPurple.b);
	drawLineCylinder(0.1, 0.0, 0.5);

	glPopMatrix();
	glPopMatrix();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glLoadIdentity();
	glRotatef(rx, 0.0, 1.0, 0.0);
	glRotatef(ry, 1.0, 0.0, 0.0);
	glRotatef(rz, 0.0, 0.0, 1.0);
	
	if (!styleSwitch)
	{
		if (!weaponSwitch)
		{
			glTranslatef(0.0, -0.5, -0.25);

			neck();

			necklace();

			faceArmor();

			glPushMatrix();

			glRotatef(-5, 1.0, 0.0, 0.0);

			brain();

			topArmor();

			corn();

			glPopMatrix();

			eyes();

			mouth();
		}
		else
		{
			weapon();
		}
	}
	else
	{
		if (!weaponSwitch)
		{
			glTranslatef(0.0, -0.5, -0.25);

			neckLine();

			necklaceLine();

			faceArmorLine();

			glPushMatrix();

			glRotatef(-5, 1.0, 0.0, 0.0);

			brainLine();

			topArmorLine();

			cornLine();

			glPopMatrix();

			eyesLine();

			mouthLine();
		}
		else
		{
			weapon();
		}
	}
}
//--------------------------------------------------------------------

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = WindowProcedure;
	wc.lpszClassName = WINDOW_TITLE;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wc)) return false;

	HWND hWnd = CreateWindow(WINDOW_TITLE, WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
		NULL, NULL, wc.hInstance, NULL);

	//--------------------------------
	//	Initialize window for OpenGL
	//--------------------------------

	HDC hdc = GetDC(hWnd);

	//	initialize pixel format for the window
	initPixelFormat(hdc);

	//	get an openGL context
	HGLRC hglrc = wglCreateContext(hdc);

	//	make context current
	if (!wglMakeCurrent(hdc, hglrc)) return false;

	//--------------------------------
	//	End initialization
	//--------------------------------

	ShowWindow(hWnd, nCmdShow);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		display();

		SwapBuffers(hdc);
	}

	UnregisterClass(WINDOW_TITLE, wc.hInstance);

	return true;
}
//--------------------------------------------------------------------