
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <Math.h>

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLU32.lib")

#define WINDOW_TITLE "OpenGL Window"
const float PI = 3.141592f, speed = 0.05;

//---------------------------------------------------------------
struct Vec3 {
	float x, y, z;

	// Add two vectors
	Vec3 operator+(const Vec3& other) const {
		return { x + other.x, y + other.y, z + other.z };
	}

	// Subtract two vectors
	Vec3 operator-(const Vec3& other) const {
		return { x - other.x, y - other.y, z - other.z };
	}

	// Scale a vector
	Vec3 operator*(float scalar) const {
		return { x * scalar, y * scalar, z * scalar };
	}

	// Normalize the vector
	Vec3 normalize() const {
		float len = sqrt(x * x + y * y + z * z);
		return { x / len, y / len, z / len };
	}

	// Cross product
	static Vec3 cross(const Vec3& a, const Vec3& b) {
		return {
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x
		};
	}

	// Dot product
	static float dot(const Vec3& a, const Vec3& b) {
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}
};
Vec3 cameraPosition = { 0.0f, 0.0f, 10.0f }; // Camera starting position
Vec3 target = { 0.0f, 0.0f, 0.0f };         // Point the camera looks at
Vec3 upVector = { 0.0f, 1.0f, 0.0f };       // Up direction
float radius = 10.0f;              // Distance from camera to target
float yaw = 0.0f;                 // Horizontal angle (in radians)
float pitch = 0.0f;               // Vertical angle (in radians)
GLenum style_glu= GLU_LINE,style_gl=GL_LINE_LOOP;
int style_switch;
float tx, ty, tz,angle;
float Oner=-10, Ofar=10, Pner=0.1, Pfar=100;
bool ortho=false;
GLUquadricObj* obj = NULL;
LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == 'P') {
			
			style_switch %= 3;
			switch (style_switch)
			{
			case 0:
				style_glu = GLU_LINE;
				style_gl = GL_LINE_LOOP;
				
				break;
			case 1:
				style_glu = GLU_FILL;
				style_gl = GL_POLYGON;
				break;
			case 2:
				
				style_glu = GLU_POINT;
				style_gl = GL_POINTS;
				break;
			default:
				style_glu = GLU_FILL;
				style_gl = GL_POLYGON;
				break;
			}
			style_switch++;
		}
		if (wParam == 'O') { ortho = !ortho; }
		if (wParam == 'Q') { angle += 5; }
		if (wParam == 'E') { angle -= 5; }
		if (wParam == 'W') { ty += speed; }
		if (wParam == 'A') { tx -= speed; }
		if (wParam == 'S') { ty -= speed; }
		if (wParam == 'D') { tx += speed; }

		if (wParam == VK_ESCAPE) PostQuitMessage(0);
		if (wParam == VK_UP) { pitch += speed; }
		if (wParam == VK_DOWN) { pitch -= speed; }
		if (wParam == VK_LEFT) { yaw -= speed; }
		if (wParam == VK_RIGHT) { yaw += speed; }
		if (wParam == VK_OEM_PLUS) { radius -= 0.1f; }
		if (wParam == VK_OEM_MINUS) { radius += 0.1f; }
		if (wParam == VK_SPACE) {
			radius = 10.0f;              
			yaw = 0.0f;                 
			pitch = 0.0f;
			ty = tx = tx = angle = 0;
		}
		cameraPosition.x = target.x + radius * cos(yaw) * cos(pitch);
		cameraPosition.y = target.y + radius * sin(pitch);
		cameraPosition.z = target.z + radius * sin(yaw) * cos(pitch);
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
void rect(float x, float y, float z,GLenum style) {

	//bottom
	glBegin(style);
	glVertex3f(0.0f, 0.0f, z);
	glVertex3f(x , 0.0f, z);
	glVertex3f(x , 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glEnd();
	//left
	glBegin(style);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0, y, 0.0);
	glVertex3f(0.0, y, z);
	glVertex3f(0.0f, 0.0f, z);
	glEnd();
	//front
	glBegin(style);
	glVertex3f(0.0f, 0.0f, z);
	glVertex3f(0.0f, y, z);
	glVertex3f(x, y, z);
	glVertex3f(x, 0.0f, z);
	glEnd();
	//right
	glBegin(style);
	glVertex3f(x, 0.0f, z);
	glVertex3f(x, y, z);
	glVertex3f(x, y, 0.0f);
	glVertex3f(x, 0.0f, 0.0f);
	glEnd();
	//back
	glBegin(style);
	glVertex3f(x, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0, y, 0.0);
	glVertex3f(x, y, 0.0f);
	glEnd();
	//top 
	glBegin(style);
	glVertex3f(x, y, 0.0f);
	glVertex3f(0.0, y, 0.0);
	glVertex3f(0.0f, y, z);
	glVertex3f(x , y, z);
	glEnd();
}

void sphere(float radius, float slice, float stack, GLenum style) {
	obj = gluNewQuadric();
	gluQuadricDrawStyle(obj, style);
	gluSphere(obj, radius, slice, stack);
	gluDeleteQuadric(obj);
}
void cylinder(float bottom, float top, float height, int slice, int stack, GLenum style) {
	obj = gluNewQuadric();
	gluQuadricDrawStyle(obj, style);
	gluCylinder(obj, bottom, top, height, slice, stack);
	gluDeleteQuadric(obj);
}

void disk(float iner, float outer, int slice, int stack,  GLenum style) {
	obj = gluNewQuadric();
	gluQuadricDrawStyle(obj, style);
	gluDisk(obj, iner, outer, slice, stack);
	gluDeleteQuadric(obj);
}

void drawSphereWithoutGLU(GLfloat radius = 0.35 ,int sliceNo = 30,int stackNo = 30)
{
	GLfloat x, y, z, sliceA, stackA;
	
	for (sliceA = 0.0; sliceA < 2 * PI; sliceA += PI / sliceNo)
	{
		glBegin(GL_LINE_LOOP);
		for (stackA = 0.0; stackA < PI; stackA += PI / stackNo)
		{
			x = radius * cos(stackA) * sin(sliceA);
			y = radius * sin(stackA) * sin(sliceA);
			z = radius * cos(sliceA);
			glVertex3f(x, y, z);
			x = radius * cos(stackA) * sin(sliceA + PI / stackNo);
			y = radius * sin(stackA) * sin(sliceA + PI / sliceNo);
			z = radius * cos(sliceA + PI / sliceNo);
			glVertex3f(x, y, z);
		}
		glEnd();
	}
}

void projection() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (!ortho) {
		gluPerspective(40, 800 / 600, Pner, Pfar);
	}
	else
	{
		glOrtho(-10, 10, -10, 10, Oner, Ofar);
	}

}

void camera() {
	gluLookAt(cameraPosition.x, cameraPosition.y, cameraPosition.z,
		target.x, target.y, target.z,
		upVector.x,upVector.y,upVector.z);
	glTranslatef(tx, ty, tz);
	glRotatef(angle, 0, 0, 1);
}

void cheast_frame1() {

	glBegin(style_gl);//1
	glColor3f(1,0,0);
	glVertex3f(0, 0, 0);
	glVertex3f(0,2,0.5);
	glVertex3f(-1.6, 3.3, -1.2);
	glVertex3f(-1.6, 2.1, -1);
	glEnd();



	glBegin(style_gl);//4
	glColor3f(0, 1, 0);
	glVertex3f(0, 2, 0.5);
	glVertex3f(-1.6, 3.3, -1.2);
	glVertex3f(-1.5, 3.4, -1.25);
	glVertex3f(0, 2.2, 0.4);
	glEnd();


}

void cheast_frame2() {
	//front
	glBegin(style_gl);
	glColor3f(1,0,0);
	glVertex3f(0, 0, 0);
	glVertex3f(1.6, 0, 0);
	glVertex3f(1.9, 0.4, 0);
	glVertex3f(2, 1.7, 0);
	glVertex3f(0.3, 2.6, 0);
	glVertex3f(-0.4, 1.7, 0);
	glEnd();

	glBegin(style_gl);
	glColor3f(0, 1, 0);
	glVertex3f(2, 1.7, 0);
	glVertex3f(0.3, 2.6, 0);
	glVertex3f(-0.4, 2.8, -1.3);
	glVertex3f(1.2, 2, -1.8);
	glEnd();

	glBegin(style_gl);
	glColor3f(0, 0, 1);
	glVertex3f(-0.4, 1.7, 0);
	glVertex3f(0.3, 2.6, 0);
	glVertex3f(-0.4, 2.8, -1.3);
	glVertex3f(-0.8, 1.85, -1);
	glEnd();

	glBegin(style_gl);
	glColor3f(1, 1, 0);
	glVertex3f(1.9, 0.4, 0);
	glVertex3f(2, 1.7, 0);
	glVertex3f(1.2, 2, -1.8);
	glVertex3f(1.1, 0.6, -1.8);
	glEnd();

	glBegin(style_gl);
	glColor3f(1, 0, 1);
	glVertex3f(1.1, 0.6, -1.8);
	glVertex3f(1.2, -0.25, -1.4);
	glVertex3f(1.6, 0, 0);
	glVertex3f(1.9, 0.4, 0);
	glEnd();

	glBegin(style_gl);
	glColor3f(1, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(1.6, 0, 0);
	glVertex3f(1.2, -0.25, -1.4);
	glVertex3f(-0.3, -0.2 , -1);
	glEnd();

	glBegin(style_gl);
	glColor3f(0, 1, 0);
	glVertex3f(-0.8, 1.85, -1);
	glVertex3f(-0.3, -0.2, -1);
	glVertex3f(0, 0, 0);
	glVertex3f(-0.4, 1.7, 0);
	glEnd();

	//back
	glBegin(style_gl);
	glColor3f(1, 0, 0);
	glVertex3f(-0.4, 2.8, -1.3);
	glVertex3f(-0.8, 1.85, -1);
	glVertex3f(-1, 1.88, -1.55);
	glVertex3f(-0.6, 2.7, -2);
	glEnd();

	glBegin(style_gl);
	glColor3f(0, 0, 1);
	glVertex3f(1.2, 2, -1.8);
	glVertex3f(-0.4, 2.8, -1.3);
	glVertex3f(-0.6, 2.7, -2);
	glVertex3f(0.7, 1.7, -2.9);
	glEnd();

	glBegin(style_gl);
	glColor3f(0, 1, 1);
	glVertex3f(-1, 1.88, -1.55);
	glVertex3f(-0.8, 1.85, -1);
	glVertex3f(-0.3, -0.2, -1);
	glVertex3f(-0.85, 0.3, -2.2);
	glVertex3f(-1.06, 1.4, -2.05);
	glEnd();

	glBegin(style_gl);
	glColor3f(1, 0, 1);
	glVertex3f(-1, 1.88, -1.55);
	glVertex3f(-0.6, 2.7, -2);
	glVertex3f(-0.3, 1.6, -2.5);
	glVertex3f(-1.06, 1.4, -2.05);
	glEnd();

	glBegin(style_gl);
	glColor3f(0, 1, 0);
	glVertex3f(-0.6, 2.7, -2);
	glVertex3f(0.7, 1.7, -2.9);
	glVertex3f(-0.3, 1.6, -2.5);
	glEnd();

	glBegin(style_gl);
	glColor3f(0, 1, 1);
	glVertex3f(0.7, 1.7, -2.9);
	glVertex3f(1.2, 2, -1.8);
	glVertex3f(1.1, 0.6, -1.8);
	glVertex3f(0.62, 0.55, -2.9);
	glEnd();
}

void body_upper() {
	glPushMatrix();
	glTranslatef(0, -2, 0);
	//glRotatef(-90, 1, 0, 0);

	glPushMatrix();
	glColor3f(0, 0, 0);
	cylinder(1, 1, 3, 20, 20, style_glu);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(0, 0, 1);
	cylinder(0.7, 1.2, 2, 20, 20, style_glu);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	cylinder(1.2, 0.7, 2, 20, 20, style_glu);
	glPopMatrix();

	for (int i = 0; i < 15; i++) {
		glPushMatrix();
		glRotatef(((360 / 15) * i), 0, 0, 1);
		glColor3f(0, 0, 0);
		rect(1.18, 0.2, 3, style_gl);
		glPopMatrix();
	}


	glPopMatrix();// r90
}

void display()
{
	glClearColor(0.498, 0.498, 0.498, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	projection();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();
	camera();

	//body_upper();
	cheast_frame2();
	//glPushMatrix();
	//glScalef(-1, 1, 1);
	//cheast_frame2();
	//glPopMatrix();

	glPopMatrix();//camera
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
	glEnable(GL_DEPTH_TEST);

	
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