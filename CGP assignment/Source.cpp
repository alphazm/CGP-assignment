
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <Math.h>

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLU32.lib")

#define WINDOW_TITLE "OpenGL Window"
const float PI = 3.141592f, speed = 0.1;

void weapon();

void weaponLine();

//drawing
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

bool weaponSwitch = false, styleSwitch = true;

color black = { 0.01,0.01,0.01 }, white = { 0.99,0.99,0.99 };
color purple = { 0.5,0.0,1.0 }, darkPurple = { 0.25,0.0,0.5 };
color lime = { 0.0,1.0,0.5 };


//---------------------------------------------------------------
//projection
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
Vec3 cameraPosition = { 0.0f, 0.0f, 50.0f }; // Camera starting position
Vec3 target = { 0.0f, 0.0f, 0.0f };         // Point the camera looks at
Vec3 upVector = { 0.0f, 1.0f, 0.0f };       // Up direction
float radius = 50.0f;              // Distance from camera to target
float yaw = 0.0f;                 // Horizontal angle (in radians)
float pitch = 0.0f;               // Vertical angle (in radians)
GLenum style_glu= GLU_LINE,style_gl=GL_LINE_LOOP;
int style_switch=0;
float tx, ty, tz,angle;
float Oner=-100, Ofar=100, Pner=0.1, Pfar=100;
bool ortho=false;

//lighting
float ambL[] = { 1.0, 1.0,1.0 };
float difL[] = { 1.0, 1.0, 1.0 };
float posA[] = { 0,3,0 };
float posB[] = { 0,3,0 };
float ambM[] = { 0.2, 0.2, 0.2, 1.0 }; // Low ambient reflection
float difM[] = { 0.5, 0.5, 0.5, 1.0 }; // Strong diffuse reflection
bool lightSwitch = false;
bool changeMaterial = false;

//animation
float finger_max_angle = 45;
float finger_min_angle = 0;
float finger_current_angle = finger_min_angle;
float hand_max_angle = -90;
float hand_min_angle = 0;
float hand_left_current_angle = hand_min_angle;
float hand_right_current_angle = hand_min_angle;
float arm_upper_max_angle_z = 80;
float arm_upper_min_angle_z = -90;
float arm_upper_right_current_angle_z = 0;
float arm_upper_left_current_angle_z = 0;
float arm_upper_max_angle_y = -90;
float arm_upper_min_angle_y = 0;
float arm_upper_left_current_angle_y = arm_upper_min_angle_y;
float arm_upper_right_current_angle_y = arm_upper_min_angle_y;
float arm_lower_max_angle = -120;
float arm_lower_min_angle = 0;
float arm_lower_left_current_angle = arm_lower_min_angle;
float arm_lower_right_current_angle = arm_lower_min_angle;
float body_max_angle = 45;
float body_min_angle = -45;
float body_current_angle = 0;
float head_max_angle = 45;
float head_min_angle = -45;
float head_current_angle = 0;

//Rotation for lower body animation
float waistLeftThighRotation = 0, waistRightThighRotation = 0, waistThighMinRotation = -45, waistThighMaxRotation = 90;
float thighLeftCalfRotation = 0, thighRightCalfRotation = 0, thighCalfMinRotation = -45, thighCalfMaxRotation = 0;
float calfLeftLegRotation = 0, calfRightLegRotation = 0, calfLegMinRotation = -30, calfLegMaxRotation = 30;

//Lower body part translation
float thightX = 1.75, thighY = -0.25, thighZ = 0;
float calfX = 1.25, calfY = -4.5, calfZ = 0;
float legX = 1.75, legY = -3.75, legZ = 0;
float thighTranslationX = 1.75, thighTranslationY = thighY, thighTranslationZ = 0;
float calfTranslationX = 1.25, calfTranslationY = thighTranslationY + calfY, calfTranslationZ = 0;
float legTranslationX = 1.75, legTranslationY = calfTranslationY + legY, legTranslationZ = 0;

bool walk = false;

//texture
BITMAP BMP;
HBITMAP hBMP = NULL;
bool textureSwitch = false;
int textureCount=0;
GLuint textureArr[4];

//color 
float r, g, b;
float colorR = 0, colorG = 0, colorB = 0;
bool colorSwitch = true;

GLUquadricObj* obj = NULL;

void walkAnimation() {
	if (walk)
	{

	}
}

LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == 'P') {
			style_switch++;
			style_switch %= 2;
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
			default:
				break;
			}
			styleSwitch = !styleSwitch;
		}
		if (wParam == 'W') { walk = !walk; }
		if (wParam == 'I') { weaponSwitch = !weaponSwitch; }
		if (wParam == 'R') { textureCount++; textureCount %= 3; }
		if (wParam == 'T') { textureSwitch = !textureSwitch; }
		if (wParam == 'C') { colorSwitch = !colorSwitch; }
		if (wParam == 'M') { changeMaterial = !changeMaterial; }
		if (wParam == 'L') { lightSwitch = !lightSwitch; }
		if (wParam == 'O') { ortho = !ortho; }
		if (wParam == 'B') {
			/*if (arm_upper_current_angle_y > arm_upper_max_angle_y)
				arm_upper_current_angle_y -= speed+5;*/

			/*if (arm_upper_current_angle_z < arm_upper_max_angle_z)
				arm_upper_current_angle_z += speed + 5;*/

			/*if (hand_right_current_angle > hand_max_angle)
				hand_right_current_angle -= speed + 5;*/
			 
			/*if (arm_lower_current_angle > arm_lower_max_angle)
				arm_lower_current_angle -= speed + 5;*/

			/*if (finger_current_angle < finger_max_angle)
				finger_current_angle += speed + 5;*/

			/*if (body_current_angle < body_max_angle)
				body_current_angle += speed + 5;*/

			/*if (head_current_angle < head_max_angle)
				head_current_angle += speed + 5;*/
		}
		if (wParam == 'N') {
			/*if (arm_upper_current_angle_y < arm_upper_min_angle_y)
				arm_upper_current_angle_y += speed+5;*/

			/*if (arm_upper_current_angle_z > arm_upper_min_angle_z)
				arm_upper_current_angle_z -= speed + 5;*/

			/*if (hand_right_current_angle < hand_min_angle)
				hand_right_current_angle += speed + 5;*/

			/*if (arm_lower_current_angle < arm_lower_min_angle)
				arm_lower_current_angle += speed + 5;*/

			/*if (finger_current_angle > finger_min_angle)
				finger_current_angle -= speed + 5;*/

			/*if (body_current_angle > body_min_angle)
				body_current_angle -= speed + 5;*/

			/*if (head_current_angle > head_min_angle)
				head_current_angle -= speed + 5;*/

		}
		if (wParam == VK_ESCAPE) PostQuitMessage(0);
		if (wParam == VK_UP) { pitch += speed; }
		if (wParam == VK_DOWN) { pitch -= speed; }
		if (wParam == VK_LEFT) { yaw -= speed; }
		if (wParam == VK_RIGHT) { yaw += speed; }
		if (wParam == VK_OEM_PLUS) { radius -= 1.0f; }
		if (wParam == VK_OEM_MINUS) { radius += 1.0f; }
		if (wParam == VK_SPACE) {
			radius = 50.0f;              
			yaw = 0.0f;                 
			pitch = 0.0f;
			ty = tx = tx = angle = 0;
		}
		cameraPosition.x = target.x + radius * cos(yaw) * cos(pitch);
		cameraPosition.y = target.y + radius * sin(pitch);
		cameraPosition.z = target.z + radius * sin(yaw) * cos(pitch);
		if (colorSwitch) {
			colorR = colorG = colorB = r = g = b = 1;
			
		}
		else
		{
			colorR = colorG = colorB = r = g = b = 0;
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
void rect(float x, float y, float z,GLenum style) {

	//bottom
	glBegin(style);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.0f, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, 0.0f, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 0.0f, 0.0f);
	glEnd();
	//left
	glBegin(style);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0, y, 0.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0, y, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 0.0f, z);
	
	glEnd();
	//front
	glBegin(style);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.0f, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, 0.0f, z);
	glEnd();
	//right
	glBegin(style);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, 0.0f, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, 0.0f, 0.0f);
	glEnd();
	//back
	glBegin(style);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0, y, 0.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y, 0.0f);
	glEnd();
	//top 
	glBegin(style);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0, y, 0.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, y, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x , y, z);
	glEnd();
}

void sphere(float radius, float slice, float stack, GLenum style) {
	obj = gluNewQuadric();
	gluQuadricDrawStyle(obj, style);
	gluQuadricTexture(obj, textureSwitch);
	gluSphere(obj, radius, slice, stack);
	gluDeleteQuadric(obj);
}
void cylinder(float bottom, float top, float height, int slice, int stack, GLenum style) {
	obj = gluNewQuadric();
	gluQuadricDrawStyle(obj, style);
	gluQuadricTexture(obj, textureSwitch);
	gluCylinder(obj, bottom, top, height, slice, stack);
	gluDeleteQuadric(obj);
}

void disk(float iner, float outer, int slice, int stack,  GLenum style) {
	obj = gluNewQuadric();
	gluQuadricDrawStyle(obj, style);
	gluQuadricTexture(obj, textureSwitch);
	gluDisk(obj, iner, outer, slice, stack);
	gluDeleteQuadric(obj);
}

void drawSphereWithoutGLU(GLfloat radius = 0.35, int sliceNo = 30, int stackNo = 30, float sliceA = 0, float stackA = 0)
{
	GLfloat x, y, z, slice, stack;
	GLfloat u, v;

	for (slice = sliceA; slice < 2 * PI; slice += PI / sliceNo)
	{
		glBegin(GL_TRIANGLE_STRIP); // Use GL_TRIANGLE_STRIP for smoother rendering
		for (stack = stackA; stack <= 2 * PI; stack += PI / stackNo)
		{
			// First vertex
			x = radius * cos(stack) * sin(slice);
			y = radius * sin(stack) * sin(slice);
			z = radius * cos(slice);
			u = stack / (2 * PI); // Map longitude to [0, 1]
			v = slice / (2 * PI); // Map latitude to [0, 1]
			glTexCoord2f(u, v);
			glVertex3f(x, y, z);

			// Second vertex
			x = radius * cos(stack) * sin(slice + PI / sliceNo);
			y = radius * sin(stack) * sin(slice + PI / sliceNo);
			z = radius * cos(slice + PI / sliceNo);
			u = stack / (2 * PI); // Map longitude to [0, 1]
			v = (slice + PI / sliceNo) / (2 * PI); // Adjust latitude
			glTexCoord2f(u, v);
			glVertex3f(x, y, z);
		}
		glEnd();
	}
}

void drawSphereWithoutGLUAdvanced(GLfloat xRadius = 0.35, GLfloat yRadius = 0.35, GLfloat zRadius = 0.35, int sliceNo = 30, int stackNo = 30)
{
	GLfloat x, y, z, sliceA, stackA;
	GLfloat u, v;

	for (sliceA = 0.0; sliceA < 2 * PI; sliceA += PI / sliceNo)
	{
		glBegin(style_gl);
		for (stackA = 0.0; stackA < PI; stackA += PI / stackNo)
		{
			x = xRadius * cos(stackA) * sin(sliceA);
			y = yRadius * sin(stackA) * sin(sliceA);
			z = zRadius * cos(sliceA);
			u = stackA / (2 * PI); // Map longitude to [0, 1]
			v = sliceA / (2 * PI); // Map latitude to [0, 1]
			glTexCoord2f(u, v);
			glVertex3f(x, y, z);
			x = xRadius * cos(stackA) * sin(sliceA + PI / stackNo);
			y = yRadius * sin(stackA) * sin(sliceA + PI / sliceNo);
			z = zRadius * cos(sliceA + PI / sliceNo);
			u = stackA / (2 * PI); // Map longitude to [0, 1]
			v = (sliceA + PI / sliceNo) / (2 * PI); // Adjust latitude
			glTexCoord2f(u, v);
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

void light() {
	if (lightSwitch)
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambL);
	glLightfv(GL_LIGHT0, GL_POSITION, posA);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT1, GL_DIFFUSE, difL);
	glLightfv(GL_LIGHT1, GL_POSITION, posB);
	glEnable(GL_LIGHT1);



	if (changeMaterial) {
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambM);
	}
	else
	{
		glMaterialfv(GL_FRONT, GL_DIFFUSE, difM);
	}
	
}

GLuint loadTexture(LPCSTR filename) {

	GLuint texture = 0;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	HBITMAP hBMP = (HBITMAP)LoadImage(GetModuleHandle(NULL),
		filename, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION |
		LR_LOADFROMFILE);
	GetObject(hBMP, sizeof(BMP), &BMP);
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BMP.bmWidth, BMP.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);
	DeleteObject(hBMP);
	return texture;
}

void texture() {
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); // Disable glColor3f effect
	if (textureSwitch)
		switch (textureCount)
		{
		case 0:
			textureArr[0] = loadTexture("waffle.bmp");

			break;
		case 1:
			break;
		case 2:
			break;
		default:
			break;
		}
		
	else
		glDisable(GL_TEXTURE_2D);
	
}

void destory() {
	glDeleteTextures(1, &textureArr[0]);
	glDeleteTextures(1, &textureArr[1]);
	glDeleteTextures(1, &textureArr[2]);
	glDeleteTextures(1, &textureArr[3]);
	glDisable(GL_TEXTURE_2D);
}

void camera() {
	gluLookAt(cameraPosition.x, cameraPosition.y, cameraPosition.z,
		target.x, target.y, target.z,
		upVector.x,upVector.y,upVector.z);
	glTranslatef(tx, ty, tz);
	glRotatef(angle, 0, 0, 1);
}
//--------------body start-------------------
void cheast_frame1() {
	
	glBegin(style_gl);//frame1
	glColor3f(r,0,0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0,2,0.5);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.6, 3.3, -1.2);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.6, 1.8, -1);
	glEnd();

	glBegin(style_gl);//top
	glColor3f(0, g, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 2, 0.5);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.6, 3.3, -1.2);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.5, 3.4, -1.25);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0, 2.2, 0.4);
	glEnd();

	glBegin(style_gl);//top2
	glColor3f(r, g, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.5, 3.4, -1.25);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0, 2.2, 0.4);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0, 2.1, -0.13);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.5, 3.4, -1.5);
	glEnd();

	glBegin(style_gl);//bottom
	glColor3f(0, 0, b);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.6, 1.8, -1);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.6, 1.74, -1.1);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0, -0.04, -0.1);
	glEnd();

	glBegin(style_gl);//back
	glColor3f(r, 0, b);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 2.1, -0.13);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.5, 3.4, -1.5);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.6, 1.74, -1.1);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0, -0.04, -0.1);
	glEnd();
}

void cheast_frame2() {
	//front
	glBegin(style_gl);
	glColor3f(r,0,0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.6, 0, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.9, 0.4, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(2, 1.7, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.3, 2.6, 0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.4, 1.7, 0);
	glEnd();

	glBegin(style_gl);
	glColor3f(0, g, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(2, 1.7, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.3, 2.6, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.4, 2.8, -1.3);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.2, 2, -1.8);
	glEnd();

	glBegin(style_gl);
	glColor3f(0, 0, b);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.4, 1.7, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.3, 2.6, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.4, 2.8, -1.3);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.8, 1.85, -1);
	glEnd();

	glBegin(style_gl);
	glColor3f(r, g, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.9, 0.4, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(2, 1.7, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.2, 2, -1.8);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.1, 0.6, -1.8);
	glEnd();

	glBegin(style_gl);
	glColor3f(r, 0, b);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.1, 0.6, -1.8);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.2, -0.25, -1.4);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.6, 0, 0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.9, 0.4, 0);
	glEnd();

	glBegin(style_gl);
	glColor3f(r, g, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0, 0);
	glTexCoord2f(1.0f, 0.0f); 	glVertex3f(1.6, 0, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.2, -0.25, -1.4);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.3, -0.2 , -1);
	glEnd();

	glBegin(style_gl);
	glColor3f(0, g, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.8, 1.85, -1);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.3, -0.2, -1);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0, 0, 0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.4, 1.7, 0);
	glEnd();

	//small pice
	glBegin(style_gl);
	glColor3f(r, g, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.2, -0.2, -0.4);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.48, -0.2, -0.4);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.2, -0.4, -1.4);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.1, -0.4, -1.05);
	glEnd();
	glBegin(style_gl);
	glColor3f(r,0,0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.2, -0.2, -0.4);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.48, -0.2, -0.4);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.48, 0, -0.4);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.2, 0, -0.4);
	glEnd();
	glBegin(style_gl);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.48, -0.2, -0.4);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.2, -0.4, -1.4);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.2, 0, -1.4);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.48, 0, -0.4);
	glEnd();
	glBegin(style_gl);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.2, -0.4, -1.4);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.1, -0.4, -1.05);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.1, 0, -1.05);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.2, 0, -1.4);
	glEnd();
	glBegin(style_gl);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.2, -0.2, -0.4);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.1, -0.4, -1.05);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.1, 0, -1.05);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.2, 0, -0.4);
	glEnd();

	//back
	glBegin(style_gl);
	glColor3f(r, 0, 0);
	glTexCoord2f(0.0f, 0.0f);	glVertex3f(-0.4, 2.8, -1.3);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.8, 1.85, -1);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1, 1.88, -1.55);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.6, 2.7, -2);
	glEnd();

	glBegin(style_gl);
	glColor3f(0, 0, b);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.2, 2, -1.8);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.4, 2.8, -1.3);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.6, 2.7, -2);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.7, 1.7, -2.9);
	glEnd();

	glBegin(style_gl);
	glColor3f(0, g, b);
	glTexCoord2f(0.0f, 0.0f);	glVertex3f(-1, 1.88, -1.55);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.8, 1.85, -1);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.3, -0.2, -1);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.85, 0.3, -2.2);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.06, 1.4, -2.05);
	glEnd();

	glBegin(style_gl);
	glColor3f(r, 0, b);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1, 1.88, -1.55);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.6, 2.7, -2);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.3, 1.6, -2.5);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.06, 1.4, -2.05);
	glEnd();

	glBegin(style_gl);
	glColor3f(0, g, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.6, 2.7, -2);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.7, 1.7, -2.9);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.3, 1.6, -2.5);
	glEnd();

	glBegin(style_gl);
	glColor3f(0, g, b);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.7, 1.7, -2.9);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.2, 2, -1.8);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.1, 0.6, -1.8);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.62, 0.55, -2.9);
	glEnd();

	glBegin(style_gl);
	glColor3f(r, 0, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.62, 0.55, -2.9);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.58, 0.0, -2.9);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.2, -0.25, -1.4);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.1, 0.6, -1.8);
	glEnd();

	glBegin(style_gl);
	glColor3f(0, 0, b);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.7, 1.7, -2.9);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.58, 0.0, -2.9);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.85, 0.3, -2.2);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.06, 1.4, -2.05);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.3, 1.6, -2.5);
	glEnd();

	glBegin(style_gl);
	glColor3f(r, 0, b);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.58, 0.0, -2.9);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.85, 0.3, -2.2);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.3, -0.2, -1);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.2, -0.25, -1.4);
	glEnd();

	//small pice
	glBegin(style_gl);
	glColor3f(r, 0, b);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.2, -0.4, -1.4);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.1, -0.4, -1.05);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.45, -0.1, -2.1);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.68, -0.2, -2.6);
	glEnd();
	glBegin(style_gl);
	glColor3f(0, 0, b);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.2, -0.4, -1.4);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.1, -0.4, -1.05);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.1, 0, -1.05);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.2, 0, -1.4);
	glEnd();
	glBegin(style_gl);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.1, -0.4, -1.05);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.45, -0.1, -2.1);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.45, 0.5, -2.1);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.1, 0, -1.05);
	glEnd();
	glBegin(style_gl);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.45, -0.1, -2.1);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.68, -0.2, -2.6);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.68, 0, -2.6);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.45, 0.5, -2.1);
	glEnd();
	glBegin(style_gl);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.2, -0.4, -1.4);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.68, -0.2, -2.6);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.68, 0, -2.6);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.2, 0, -1.4);
	glEnd();

	//yiling
	//font
	glBegin(style_gl);//left
	glColor3f(r,g,0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5, 2.4, -1.305);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.2, 3.4, -1.305);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5, 3, -0.305);
	glTexCoord2f(0.0f, 1.0f);	glVertex3f(0.1, 2.4, -0.305);
	glEnd();

	glBegin(style_gl);//top
	glColor3f(r, 0, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.1, 3.35, -1.32);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.2, 3.4, -1.305);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5, 3, -0.305);
	glTexCoord2f(0.0f, 1.0f);	glVertex3f(0.6, 2.95, -0.32);
	glEnd();

	glBegin(style_gl);//right
	glColor3f(r, g, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.1, 3.35, -1.32);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.6, 2.95, -0.32);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.3, 2, -0.3);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.6, 2, -1.65);
	glEnd();

	glBegin(style_gl);//back
	glColor3f(r, 0, b);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5, 2.4, -1.305);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.2, 3.4, -1.305);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.1, 3.35, -1.32);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.6, 2, -1.65);
	glEnd();

	glBegin(style_gl);//front
	glColor3f(r, 0, b);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5, 3, -0.305);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.1, 2.4, -0.305);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.3, 2, -0.3);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.6, 2.95, -0.32);
	glEnd();

	//back
	glBegin(style_gl);//left 
	glColor3f(r, g, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5, 2.4, -1.305);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.2, 3.4, -1.305);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.4, 3.1, -2);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.7, 2.3, -1.9);
	glEnd();
	
	glBegin(style_gl);//top 
	glColor3f(r, 0, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.2, 3.4, -1.305);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.1, 3.35, -1.32);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.3, 3.1, -2);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.4, 3.1, -2);
	glEnd();

	glBegin(style_gl);//right 
	glColor3f(r, g, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.1, 3.35, -1.32);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.6, 2, -1.65);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.4, 2, -2.2);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.3, 3.1, -2);
	glEnd();

	glBegin(style_gl);//back 
	glColor3f(r, 0, b);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.4, 2, -2.2);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.7, 2.3, -1.9);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.4, 3.1, -2);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.3, 3.1, -2);
	glEnd();

	//back2
	glBegin(style_gl);//back 
	glColor3f(r, g, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.7, 2.3, -1.9);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.7, 1.7, -2.9);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.6, 2.6, -2.6);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.4, 3.1, -2);
	glEnd();

	glBegin(style_gl);//top 
	glColor3f(r, 0, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.3, 3.1, -2);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.4, 3.1, -2);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.6, 2.6, -2.6);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.7, 2.6, -2.5);
	glEnd();

	glBegin(style_gl);//front  
	glColor3f(r, g, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.3, 3.1, -2);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.4, 2, -2.2);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.85, 1.8, -2.2);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.7, 2.6, -2.5);
	glEnd();

	glBegin(style_gl);//left  
	glColor3f(r, 0, b);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.85, 1.8, -2.2);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.7, 2.6, -2.5);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.6, 2.6, -2.6);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.7, 1.7, -2.9);
	
	glEnd();
	
}

void cheast_middle_detail() {
	glPushMatrix();
	glScalef(0.5,0.5,0.5);
	glRotatef(180, 0, 1, 0);
	glPushMatrix();
	glColor3f(0,g,0);
	glTranslatef(-1.5, 0.1, 1);
	glRotatef(10,1,0.5,0);
	cylinder(0.2,0.2,0.3,10,10,style_glu);
	glPushMatrix();
	glTranslatef(0,0,0.3);
	cylinder(0.2, 0, 0.02, 10, 10, style_glu);
	glPopMatrix();
	glPushMatrix();
	glColor3f(0,0,b);
	glTranslatef(0, 0, -0.05);
	cylinder(0.25, 0.25, 0.05, 10, 10, style_glu);
	disk(0,0.25,10,10,style_glu);
	glPushMatrix();
	glTranslatef(0,0,0.05);
	disk(0, 0.25, 10, 10, style_glu);
	glPopMatrix();
	glPopMatrix();
	glColor3f(r, g, 0);
	glPushMatrix();
	glTranslatef(0,-0.07,-0.6);
	glRotatef(-7, 1, 0, 0);
	cylinder(0.15, 0.15, 0.6, 10, 10, style_glu);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 0.135, -1.32);
	glRotatef(15, 1, 0, 0);
	cylinder(0.15, 0.15, 0.8, 10, 10, style_glu);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, -0.08, -2.02);
	glRotatef(-15, 1, 0, 0);
	cylinder(0.15, 0.15, 0.8, 10, 10, style_glu);
	glPopMatrix();
	glTranslatef(0, -0.66, -3.);
	glRotatef(-30, 1, 0, 0);
	cylinder(0.15, 0.15, 1.2, 10, 10, style_glu);
	glPopMatrix();
	glPopMatrix();
}

void cheast_middle() {
	glPushMatrix();
	glScalef(1.6,1.6,1.6);

	glPushMatrix();
	glColor3f(r,0,0);
	glRotatef(180, 0, 0, 1);
	drawSphereWithoutGLU(1, 20, 20, 3.14, 3.14);
	glPushMatrix();
	glRotatef(90,0,1,0);
	drawSphereWithoutGLU(1, 20, 20, 3.14, 3.14);
	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	drawSphereWithoutGLU(1, 20, 20, 3.14, 3.14);
	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	drawSphereWithoutGLU(1, 20, 20, 3.14, 3.14);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	disk(0,1,20,20,style_glu);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, g, 0);
	glTranslatef(0, 0, -0.5);
	sphere(0.4,20,20,style_glu);
	glPopMatrix();

	// the triangle
	glPushMatrix();
	glColor3f(0,0,b);
	glTranslatef(0,0,-0.4);
	glBegin(style_gl);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0.05, -1);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0, 0.05, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0, -1.3,0 );
	glEnd();

	glBegin(style_gl);//right front
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0.05, -1);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0, -1.3, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.2, -1.3, 0.1);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.2, 0.05, -0.8);
	glEnd();

	glBegin(style_gl);//left front
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0.05, -1);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0, -1.3, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.2, -1.3, 0.1);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.2, 0.05, -0.8);
	glEnd();

	glBegin(style_gl);//right top
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0.05, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.2, 0.05, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.2, 0.05, -0.8);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0, 0.05, -1);
	glEnd();

	glBegin(style_gl);//left top
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0.05, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.2, 0.05, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.2, 0.05, -0.8);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0, 0.05, -1);
	glEnd();

	glBegin(style_gl);//right 
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.2, 0.05, -0.8);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.2, 0.05, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.2, -1.3, 0.1);
	glEnd();
	glBegin(style_gl);//left 
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.2, 0.05, -0.8);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.2, 0.05, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.2, -1.3, 0.1);
	glEnd();

	glPopMatrix();//move the trin back 

	glPopMatrix();//last pop
	
	cheast_middle_detail();

	glPushMatrix();
	glScalef(-1, 1, 1);
	cheast_middle_detail();
	glPopMatrix();
}

void cheast() {
	glPushMatrix();
	glTranslatef(0, 0.5, 1.65);
	glPushMatrix();
	glTranslatef(-3, 1.75, -1.8);
	glRotatef(-20, 0, 0, 1);
	glRotatef(-35, 0, 1, 0);
	glRotatef(20, 1, 0, 0);
	cheast_frame2();
	glPopMatrix();
	cheast_frame1();
	glPopMatrix();
}

void body_upper() {
	glPushMatrix();
	glTranslatef(0, -2, 0);
	glRotatef(-90, 1, 0, 0);

	glPushMatrix();
	glColor3f(0, 0, 0);
	cylinder(1.2, 1.2, 4, 20, 20, style_glu);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(0, 0, 2);
	cylinder(0.9, 1.3, 2, 20, 20, style_glu);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	cylinder(1.3, 0.9, 2, 20, 20, style_glu);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, g, 0);
	glTranslatef(0, 0, 4);
	cylinder(1.3, 1, 1.3, 20, 20, style_glu);
	glPopMatrix();

	for (int i = 0; i < 15; i++) {
		glPushMatrix();
		glRotatef(((360 / 15) * i), 0, 0, 1);
		glColor3f(r, g, b);
		rect(1.2, 0.2, 4, style_gl);
		glPopMatrix();
	}


	glPopMatrix();// r90
}

void body_back() {
	glPushMatrix();
	glRotatef(20, 0, 1, 0);
	glColor3f(r, g, b);
	rect(0.8,1.8,0.2,style_gl);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(0.3,0.4,-0.28);
	glColor3f(r,0,0);
	//pice1
	glBegin(style_gl);//back
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.1, 0.2, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0, 1.5, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5, 1.5, 0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5, 0.2, 0);
	glEnd();
	glColor3f(r, g, 0);
	glBegin(style_gl);//back2
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.1, 0.2, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.1, 0, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5, 0, 0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5, 0.2, 0);
	glEnd();
	glColor3f(0, g, 0);
	glBegin(style_gl);//top
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 1.5, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5, 1.5, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5, 1.55, -0.3);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0, 1.55, -0.3);
	glEnd();
	glColor3f(0, 0, b);
	glBegin(style_gl);//front
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0.2, -0.6);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5, 0.2, -0.6);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5, 1.55, -0.3);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0, 1.55, -0.3);
	glEnd();
	glBegin(style_gl);//front2
	glColor3f(0, g, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0.2, -0.6);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5, 0.2, -0.6);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5, 0, -0.6);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0, 0, -0.6);
	glEnd();
	glBegin(style_gl);//left
	glColor3f(0, g, b);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5, 1.5, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5, 0, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5, 0, -0.6);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5, 0.2, -0.6);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5, 1.55, -0.3);
	glEnd();
	glBegin(style_gl);//right
	glColor3f(0, g, b);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 1.5, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.1, 0.2, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0, 0.2, -0.6);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0, 1.55, -0.3);
	glEnd();
	glColor3f(0, 0, b);//right2
	glBegin(style_gl);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.1, 0, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0, 0, -0.6);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0, 0.2, -0.6);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.1, 0.2, 0);
	glEnd();
	glColor3f(r, 0, b);//bottom
	glBegin(style_gl);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.1, 0, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0, 0, -0.6);
	glTexCoord2f(1.0f, 1.0f);	glVertex3f(0.5, 0, -0.6);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5, 0, 0);
	glEnd();
	//pice2
	glColor3f(r, 0, 0);//left
	glBegin(style_gl);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5, 0, -0.6);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5, 0, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5, -1, 0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5, -1, -0.3);
	glEnd();
	glColor3f(r, 0, 0);//right
	glBegin(style_gl);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0, -0.6);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.1, 0, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0, -1, 0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0, -1, -0.3);
	glEnd();
	glColor3f(0, g, 0);//back
	glBegin(style_gl);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.1, 0, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5, 0, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5, -1, 0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0, -1, 0);
	glEnd();
	glColor3f(0, 0, b);//front
	glBegin(style_gl);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5, 0, -0.6);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5, -1, -0.3);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0, -1, -0.3);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0, 0, -0.6);
	glEnd();
	glColor3f(0, g, 0);//bottom
	glBegin(style_gl);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5, -1, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0, -1, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0, -1, -0.3);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5, -1, -0.3);
	glEnd();
	//pice3
	glPushMatrix();
	glColor3f(r, g, 0);
	glTranslatef(0,0.3,0);
	glBegin(style_gl);//back
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.3, -0.05, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.3, 0.5, 0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0, 1, 0);
	glEnd();
	glBegin(style_gl);//front
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0, -0.1);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.3, -0.05, -0.1);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.3, 0.5, -0.1);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0, 1, -0.1);
	glEnd();
	glColor3f(r, 0, 0);
	glBegin(style_gl);//left
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0, 0, -0.1);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0, 1, -0.1);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0, 1, 0);
	glEnd();
	glBegin(style_gl);//top
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 1, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.3, 0.5, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.3, 0.5, -0.1);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0, 1, -0.1);
	glEnd();
	glBegin(style_gl);//left
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.3, -0.05, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.3, 0.5, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.3, 0.5, -0.1);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.3, -0.05, -0.1);
	glEnd();
	glBegin(style_gl);//bottom
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.3, -0.05, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.3, -0.05, -0.1);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0, 0, -0.1);
	glEnd();
	glPopMatrix();
	//support 
	glBegin(style_gl);//right
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.2, 0.2, -0.6);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.2, 0, -0.6);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.2, 0.2, -0.8);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.2, 0.3, -0.7);
	glEnd();
	glBegin(style_gl);//left
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.3, 0.2, -0.6);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.3, 0, -0.6);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.3, 0.2, -0.8);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.3, 0.3, -0.7);
	glEnd();
	glBegin(style_gl);//front
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.2, 0.2, -0.8);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.3, 0.2, -0.8);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.3, 0.3, -0.7);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.2, 0.3, -0.7);
	glEnd();
	glBegin(style_gl);//top
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.2, 0.2, -0.6);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.3, 0.2, -0.6);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.3, 0.3, -0.7);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.2, 0.3, -0.7);
	glEnd();
	glBegin(style_gl);//bottom
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.2, 0, -0.6);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.3, 0, -0.6);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.3, 0.2, -0.8);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.2, 0.2, -0.8);
	glEnd();
	//plane
	glPushMatrix();
	glRotatef(45, 1, 0, 0);
	glTranslatef(0, -0.7, -0.9);
	glColor3f(0, 0, 0);
	rect(0.5,0.5,0.2,style_gl);
	//pice4
	glPushMatrix();
	glScalef(1.2,1.2,1);
	glTranslatef(0, -0.1, 0);
	glRotatef(15, 0, 1, 0);
	glColor3f(r, 0, b);//front
	glBegin(style_gl);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, -0.1, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0, 0.5, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.2, 0.8, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.7, 0.6, 0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.75, -0.3, 0);
	glEnd();
	glBegin(style_gl);//back
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, -0.1, 0.2);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0, 0.5, 0.2);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.2, 0.8, 0.2);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.7, 0.6, 0.2);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.75, -0.3, 0.2);
	glEnd();
	glColor3f(r, g, 0);
	glBegin(style_gl);//topleft
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0.5, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.2, 0.8, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.2, 0.8, 0.2);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0, 0.5, 0.2);
	glEnd();
	glBegin(style_gl);//topright
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.2, 0.8, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.7, 0.6, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.7, 0.6, 0.2);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.2, 0.8, 0.2);
	glEnd();
	glBegin(style_gl);//left
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, -0.1, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0, 0.5, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0, 0.5, 0.2);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0, -0.1, 0.2);
	glEnd();
	glBegin(style_gl);//right
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.7, 0.6, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.75, -0.3, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.75, -0.3, 0.2);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.7, 0.6, 0.2);
	glEnd();
	glBegin(style_gl);//bottom
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, -0.1, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.75, -0.3, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.75, -0.3, 0.2);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0, -0.1, 0.2);
	glEnd();
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();
}

void arm_upper(bool left) {
	glPushMatrix();
	glTranslatef(2.2, 0, 0);

	if (left) {
		glRotatef(arm_upper_left_current_angle_y, 0, 1, 0);
		glRotatef(arm_upper_left_current_angle_z, 0, 0, 1);
	}
	else
	{
		glRotatef(arm_upper_right_current_angle_y, 0, 1, 0);
		glRotatef(arm_upper_right_current_angle_z, 0, 0, 1);
	}
	
	glPushMatrix();
	glColor3f(0, g, 0);
	glTranslatef(0, 0, -0.7);
	glRotatef(45, 1, 0, 0);
	rect(1.8, 1, 1, style_gl);
	glPopMatrix();

	glPushMatrix();
	glColor3f(r, g, b);
	glRotatef(90, 0, 1, 0);
	cylinder(0.4, 0.4, 2, 10, 10, style_glu);
	glPopMatrix();

	glPushMatrix();//connection part
	glColor3f(r, 0, 0);
	glTranslatef(1.8, -0.4, -0.5);
	rect(0.8,0.8,1,style_gl);
	

	glPushMatrix();
	glColor3f(r, g, 0);
	glTranslatef(0, -0.2, -0.05);
	rect(1.8, 0.2, 1.1, style_gl);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0.8, -0.05);
	rect(1.8, 0.2, 1.1, style_gl);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, g, 0);
	glTranslatef(1.4, 0.9, 0.5);
	glRotatef(90, 1, 0, 0);
	cylinder(0.1,0.1,1,10, 10, style_glu);
	glPopMatrix();

	glPopMatrix();//connection

}

void arm_lower(bool left) {
	glPushMatrix();
	glColor3f(r, g, b);
	glTranslatef(3, 0, 0);

	
	if (left) {
		glRotatef(arm_lower_left_current_angle, 0, 1, 0);
	}
	else
	{
		glRotatef(arm_lower_right_current_angle, 0, 1, 0);
	}

	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	cylinder(0.4, 0.4, 3, 10, 10, style_glu);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2.3,-0.5,-0.5);
	rect(0.5,1,1,style_gl);

	glPushMatrix();
	glTranslatef(0.1,1,0.35);
	glColor3f(r, 0, 0);
	rect(0.3, 0.5, 0.3, style_gl);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, b);
	glTranslatef(-2,1.5,-0.5);
	rect(4, 0.3, 2, style_gl);
	glPopMatrix();

	glPopMatrix();

	glPushMatrix();
	glColor3f(0,g,0);
	glTranslatef(0.3,0,-0.7);
	glRotatef(45,1,0,0);
	rect(2,1,1,style_gl);
	glPopMatrix();

	
}

void finger1() {
	// Base connection
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glColor3f(0, 0, 0);
	cylinder(0.2, 0.2, 0.1, 20, 20, style_glu);
	glPushMatrix();
	disk(0, 0.2, 20, 20, style_glu);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 0, 0.1);
	disk(0, 0.2, 20, 20, style_glu);
	glPopMatrix();
	glPopMatrix();

	// First section of the finger
	glPushMatrix();
	glRotatef(finger_current_angle, 0, 1, 0); // Rotate the first section
	glPushMatrix();
	glColor3f(r, 0, 0);
	glTranslatef(0, -0.15, 0);
	rect(0.6, 0.2, 0.4, style_gl);
	glPopMatrix();

	// Connection between sections
	glPushMatrix();
	glTranslatef(0.6, 0, 0.2);
	glRotatef(90, 1, 0, 0);
	glColor3f(0, 0, 0);
	cylinder(0.2, 0.2, 0.1, 20, 20, style_glu);
	glPushMatrix();
	disk(0, 0.2, 20, 20, style_glu);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 0, 0.1);
	disk(0, 0.2, 20, 20, style_glu);
	glPopMatrix();
	glPopMatrix();

	// Second section of the finger
	glPushMatrix();
	glTranslatef(0.7, -0.15, 0);
	glRotatef(finger_current_angle, 0, 1, 0); // Rotate the second section
	glColor3f(r, 0, 0);
	rect(0.5, 0.2, 0.4, style_gl);
	glPopMatrix();

	glPopMatrix(); // End of the first section
}

void finger2() {
	// Base connection
	glPushMatrix();
	glColor3f(0, 0, 0);
	cylinder(0.2, 0.2, 0.1, 20, 20, style_glu);
	glPushMatrix();
	disk(0, 0.2, 20, 20, style_glu);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 0, 0.1);
	disk(0, 0.2, 20, 20, style_glu);
	glPopMatrix();
	glPopMatrix();

	// First section of the finger
	glPushMatrix();
	glTranslatef(0, 0, 0); // Move to the base connection pivot
	glRotatef(finger_current_angle, 0, 0, -1); // Rotate the first section
	glPushMatrix();
	glColor3f(r, 0, 0);
	glTranslatef(0, -0.2, -0.05); // Translate to draw the first section
	rect(0.4, 0.4, 0.2, style_gl);
	glPopMatrix();

	// Connection between first and second section
	glPushMatrix();
	glTranslatef(0.4, 0, 0); // Adjust translation for connection alignment
	glColor3f(0, 0, 0);
	cylinder(0.2, 0.2, 0.1, 20, 20, style_glu);
	glPushMatrix();
	disk(0, 0.2, 20, 20, style_glu);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 0, 0.1);
	disk(0, 0.2, 20, 20, style_glu);
	glPopMatrix();
	glPopMatrix();

	// Second section of the finger
	glPushMatrix();
	glTranslatef(0.4, 0, 0); // Move to the second section pivot
	glRotatef(finger_current_angle, 0, 0, -1); // Rotate the second section
	glPushMatrix();
	glColor3f(r, 0, 0);
	glTranslatef(0, -0.2, -0.05); // Translate to draw the second section
	rect(0.4, 0.4, 0.2, style_gl);
	glPopMatrix();

	// Connection between second and third section
	glPushMatrix();
	glTranslatef(0.4, 0, 0); // Adjust translation for connection alignment
	glColor3f(0, 0, 0);
	cylinder(0.2, 0.2, 0.1, 20, 20, style_glu);
	glPushMatrix();
	disk(0, 0.2, 20, 20, style_glu);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 0, 0.1);
	disk(0, 0.2, 20, 20, style_glu);
	glPopMatrix();
	glPopMatrix();

	// Third section of the finger
	glPushMatrix();
	glTranslatef(0.4, 0, 0); // Move to the third section pivot
	glRotatef(finger_current_angle, 0, 0, -1); // Rotate the third section
	glPushMatrix();
	glColor3f(r, 0, 0);
	glTranslatef(0, -0.2, -0.05); // Translate to draw the third section
	rect(0.4, 0.4, 0.2, style_gl);
	glPopMatrix();

	glPopMatrix(); // Close third section
	glPopMatrix(); // Close second section
	glPopMatrix(); // Close first section
}

void hand() {
	glPushMatrix();
	glTranslatef(-5.2,0,0);

	glPushMatrix();
	glColor3f(r, 0, b);
	glTranslatef(8.2, 0.1, -0.5);
	rect(1, 0.3, 1, style_gl);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(8.2, -0.1, -0.5);
	rect(0.5, 0.3, 1, style_gl);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, g, 0);
	glTranslatef(8.2, 0.4, -0.5);
	glRotatef(-5,0,0,1);
	rect(1.4, 0.1, 1, style_gl);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(8.3, -0.1, 0.6);
	glRotatef(40,0,0,-1);
	finger1();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(8.8, 0.05, 0.4);
	finger2();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(8.8, 0.05, 0.1);
	finger2();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(8.8, 0.05, -0.2);
	finger2();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(8.8, 0.05, -0.5);
	finger2();
	glPopMatrix();

	glPopMatrix();
}

void arm(bool left) {
	glPushMatrix();
	glColor3f(r, g, b);
	glRotatef(90, 0, 1, 0);
	cylinder(0.4, 0.4, 1.5, 10, 10, style_glu);
	glPopMatrix();

	glPushMatrix();
	glColor3f(r, g, 0);
	glTranslatef(0.9, -2, -1.5);
	rect(0.3, 6, 2, style_gl);
	glPopMatrix();

	glPushMatrix();
	glColor3f(r, g, 0);
	glTranslatef(0.9, -1.8, -0.5);
	rect(0.3, 4, 2, style_gl);
	glPopMatrix();

	glPushMatrix();
	glScalef(0.8, 0.8, 0.8);

	glPushMatrix();//join part
	glColor3f(r, 0, 0);
	glTranslatef(1.8, 0, 0);
	sphere(0.6, 10, 10, style_glu);
	glPopMatrix();


	arm_upper(left);
	
	arm_lower(left);

	glPushMatrix();
	if (left) {
		glRotatef(hand_left_current_angle, 1, 0, 0);
	}
	else
	{
		glRotatef(hand_right_current_angle, 1, 0, 0);
		if (weaponSwitch) {
			glPushMatrix();
			glTranslatef(3.4, -0.2, 2);
			glRotatef(90, 1, 0, 0);
			glScalef(5, 5, 5);
			if (!styleSwitch)
				weapon();
			else
				weaponLine();

			glPopMatrix();

			finger_current_angle = finger_max_angle;
		}
	}
	hand();
	
	glPopMatrix(); //hand

	glPopMatrix(); //lower arm
	glPopMatrix(); //upper arm

	glPopMatrix();
}

void body() {
	body_upper();

	glPushMatrix();
	glRotatef(body_current_angle,0,1,0);
	cheast();

	glPushMatrix();
	glScalef(-1, 1, 1);
	cheast();
	glPopMatrix();

	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	glTranslatef(0, 4.5, 0);
	cheast_middle();
	glPopMatrix();

	glPushMatrix();
	glScalef(1.5, 1.5, 1.5);
	glPushMatrix();
	glTranslatef(-0.7,1.2,-1.2);
	body_back();
	glPopMatrix();
	glPushMatrix();
	glScalef(-1, 1, 1);
	glTranslatef(-0.7, 1.2, -1.2);
	body_back();
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2.2,3.5,-0.5);
	arm(true);
	glPopMatrix();
	glPushMatrix();
	glScalef(-1, 1, 1);
	glTranslatef(2.2, 3.5, -0.5);
	arm(false);
	glPopMatrix();
	
	glPopMatrix();
}
//-------------body end-----------------------

//----------------head start-----------------
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
	glTranslatef(0.0, -0.8, 0.8);
	glRotatef(-120, 1.0, 0.0, 0.0);

	//inner neck
	glColor3f(0.18f, 0.18f, 0.18f);		//black-ish grey
	drawCylinder(0.2, 0.1, 1.5);

	//outer neck
	glColor3f(1.0f, 0.7f, 0.0f);		//golden yellow
	drawHalfCylinder(0.2, 0.175, 1.5, 30);

	glPopMatrix();
}

void neckLine()
{
	glPushMatrix();
	glTranslatef(0.0, -0.8, 0.8);
	glRotatef(-120, 1.0, 0.0, 0.0);

	//inner neck
	glColor3f(0.18f, 0.18f, 0.18f);		//black-ish grey
	drawLineCylinder(0.2, 0.1, 1.5);

	//outer neck
	glColor3f(1.0f, 0.7f, 0.0f);		//golden yellow
	drawHalfLineCylinder(0.2, 0.175, 1.5, 30);

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

	glTranslatef(0.0, -3.65, 0.0);
	glRotatef(-90, 1.0, 0.0, 0.0);

	drawCylinder(0.0, 0.1, 0.15);

	glPopMatrix();

	//bottom spike upper
	glPushMatrix();

	glTranslatef(0.0, -3.5, 0.0);
	glRotatef(-90, 1.0, 0.0, 0.0);

	drawCylinder(0.1, 0.0, 0.15);

	glPopMatrix();

	glColor3f(purple.r, purple.g, purple.b);

	//handle
	glPushMatrix();

	glTranslatef(0.0, -3.5, 0.0);
	glRotatef(-90, 1.0, 0.0, 0.0);

	drawCylinder(0.05, 0.05, 3.5);

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

	glTranslatef(0.0, -3.65, 0.0);
	glRotatef(-90, 1.0, 0.0, 0.0);

	drawLineCylinder(0.0, 0.1, 0.15);

	glPopMatrix();

	//bottom spike upper
	glPushMatrix();

	glTranslatef(0.0, -3.5, 0.0);
	glRotatef(-90, 1.0, 0.0, 0.0);

	drawLineCylinder(0.1, 0.0, 0.15);

	glPopMatrix();

	glColor3f(purple.r, purple.g, purple.b);

	//handle
	glPushMatrix();

	glTranslatef(0.0, -3.5, 0.0);
	glRotatef(-90, 1.0, 0.0, 0.0);

	drawLineCylinder(0.05, 0.05, 3.5);

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

void head() {
	if (!styleSwitch) {
		glPushMatrix();
		glTranslatef(0.0, -0.5, -0.25);

		neck();
		glPushMatrix();
		glRotatef(head_current_angle,0,1,0);
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
		glPopMatrix();
		glPopMatrix();
	}
	else {
		glPushMatrix();
		glTranslatef(0.0, -0.5, -0.25);

		neckLine();
		glPushMatrix();
		glRotatef(head_current_angle, 0, 1, 0);
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
		glPopMatrix();
		glPopMatrix();
	}
}
//------------head end----------------------

//----------body bottom-------------------
float lowerBodyFrontUpperPlateX1 = 1.2, lowerBodyFrontUpperPlateX2 = 1,
lowerBodyFrontUpperPlateY1 = 0.5, lowerBodyFrontUpperPlateY2 = 0.2,
lowerBodyFrontUpperPlateZ = 0.1;

float frontPlateFrontHexagon[6][3] = {
	{-lowerBodyFrontUpperPlateX1, lowerBodyFrontUpperPlateY1, lowerBodyFrontUpperPlateZ},
	{lowerBodyFrontUpperPlateX1, lowerBodyFrontUpperPlateY1, lowerBodyFrontUpperPlateZ},
	{lowerBodyFrontUpperPlateX1, -lowerBodyFrontUpperPlateY2, lowerBodyFrontUpperPlateZ},
	{lowerBodyFrontUpperPlateX2, -lowerBodyFrontUpperPlateY1, lowerBodyFrontUpperPlateZ},
	{-lowerBodyFrontUpperPlateX2, -lowerBodyFrontUpperPlateY1, lowerBodyFrontUpperPlateZ},
	{-lowerBodyFrontUpperPlateX1, -lowerBodyFrontUpperPlateY2, lowerBodyFrontUpperPlateZ}
};

float frontPlateBackHexagon[6][3] = {
	{-lowerBodyFrontUpperPlateX1, lowerBodyFrontUpperPlateY1, -lowerBodyFrontUpperPlateZ},
	{lowerBodyFrontUpperPlateX1, lowerBodyFrontUpperPlateY1, -lowerBodyFrontUpperPlateZ},
	{lowerBodyFrontUpperPlateX1, -lowerBodyFrontUpperPlateY2, -lowerBodyFrontUpperPlateZ},
	{lowerBodyFrontUpperPlateX2, -lowerBodyFrontUpperPlateY1, -lowerBodyFrontUpperPlateZ},
	{-lowerBodyFrontUpperPlateX2, -lowerBodyFrontUpperPlateY1, -lowerBodyFrontUpperPlateZ},
	{-lowerBodyFrontUpperPlateX1, -lowerBodyFrontUpperPlateY2, -lowerBodyFrontUpperPlateZ}
};

float lowerBodyFrontLowerPlateX1 = 1,
lowerBodyFrontLowerPlateY1 = 1.0, lowerBodyFrontLowerPlateY2 = 0.2,
lowerBodyFrontLowerPlateZ = 0.1;

float frontPlateFrontPentagon[5][3] = {
	{-lowerBodyFrontLowerPlateX1, lowerBodyFrontLowerPlateY1, lowerBodyFrontLowerPlateZ},
	{lowerBodyFrontLowerPlateX1, lowerBodyFrontLowerPlateY1, lowerBodyFrontLowerPlateZ},
	{lowerBodyFrontLowerPlateX1, -lowerBodyFrontLowerPlateY2, lowerBodyFrontLowerPlateZ},
	{0, -lowerBodyFrontLowerPlateY1, lowerBodyFrontLowerPlateZ},
	{-lowerBodyFrontLowerPlateX1, -lowerBodyFrontLowerPlateY2, lowerBodyFrontLowerPlateZ}
};

float frontPlateBackPentagon[5][3] = {
	{-lowerBodyFrontLowerPlateX1, lowerBodyFrontLowerPlateY1, -lowerBodyFrontLowerPlateZ},
	{lowerBodyFrontLowerPlateX1, lowerBodyFrontLowerPlateY1, -lowerBodyFrontLowerPlateZ},
	{lowerBodyFrontLowerPlateX1, -lowerBodyFrontLowerPlateY2, -lowerBodyFrontLowerPlateZ},
	{0, -lowerBodyFrontLowerPlateY1, -lowerBodyFrontLowerPlateZ},
	{-lowerBodyFrontLowerPlateX1, -lowerBodyFrontLowerPlateY2, -lowerBodyFrontLowerPlateZ}
};

//Soft variable for lower body - back plate

float lowerBodyBackUpperPlateX1 = 0.6, lowerBodyBackUpperPlateX2 = 0.2,
lowerBodyBackUpperPlateY1 = 0.6, lowerBodyBackUpperPlateY2 = 0.0,
lowerBodyBackUpperPlateZ = 0.1;

float backPlateFrontHexagon[6][3] = {
	{-lowerBodyBackUpperPlateX2, lowerBodyBackUpperPlateY1, lowerBodyBackUpperPlateZ},
	{lowerBodyBackUpperPlateX2, lowerBodyBackUpperPlateY1, lowerBodyBackUpperPlateZ},
	{lowerBodyBackUpperPlateX1, lowerBodyBackUpperPlateY2, lowerBodyBackUpperPlateZ},
	{lowerBodyBackUpperPlateX1, -lowerBodyBackUpperPlateY1, lowerBodyBackUpperPlateZ},
	{-lowerBodyBackUpperPlateX1, -lowerBodyBackUpperPlateY1, lowerBodyBackUpperPlateZ},
	{-lowerBodyBackUpperPlateX1, lowerBodyBackUpperPlateY2, lowerBodyBackUpperPlateZ}
};

float backPlateBackHexagon[6][3] = {
	{-lowerBodyBackUpperPlateX2, lowerBodyBackUpperPlateY1, -lowerBodyBackUpperPlateZ},
	{lowerBodyBackUpperPlateX2, lowerBodyBackUpperPlateY1, -lowerBodyBackUpperPlateZ},
	{lowerBodyBackUpperPlateX1, lowerBodyBackUpperPlateY2, -lowerBodyBackUpperPlateZ},
	{lowerBodyBackUpperPlateX1, -lowerBodyBackUpperPlateY1, -lowerBodyBackUpperPlateZ},
	{-lowerBodyBackUpperPlateX1, -lowerBodyBackUpperPlateY1, -lowerBodyBackUpperPlateZ},
	{-lowerBodyBackUpperPlateX1, lowerBodyBackUpperPlateY2, -lowerBodyBackUpperPlateZ}
};

float lowerBodyBackLowerPlateX1 = 1.5, lowerBodyBackLowerPlateX2 = 1.25,
lowerBodyBackLowerPlateY1 = 1.5, lowerBodyBackLowerPlateY2 = 0.2,
lowerBodyBackLowerPlateZ = 0.1;

float backPlateFrontPentagon[5][3] = {
	{-lowerBodyBackLowerPlateX1, lowerBodyBackLowerPlateY1, lowerBodyBackLowerPlateZ},
	{lowerBodyBackLowerPlateX1, lowerBodyBackLowerPlateY1, lowerBodyBackLowerPlateZ},
	{lowerBodyBackLowerPlateX2, -lowerBodyBackLowerPlateY2, lowerBodyBackLowerPlateZ},
	{0, -lowerBodyBackLowerPlateY1, lowerBodyBackLowerPlateZ},
	{-lowerBodyBackLowerPlateX2, -lowerBodyBackLowerPlateY2, lowerBodyBackLowerPlateZ}
};

float backPlateBackPentagon[5][3] = {
	{-lowerBodyBackLowerPlateX1, lowerBodyBackLowerPlateY1, -lowerBodyBackLowerPlateZ},
	{lowerBodyBackLowerPlateX1, lowerBodyBackLowerPlateY1, -lowerBodyBackLowerPlateZ},
	{lowerBodyBackLowerPlateX2, -lowerBodyBackLowerPlateY2, -lowerBodyBackLowerPlateZ},
	{0, -lowerBodyBackLowerPlateY1, -lowerBodyBackLowerPlateZ},
	{-lowerBodyBackLowerPlateX2, -lowerBodyBackLowerPlateY2, -lowerBodyBackLowerPlateZ}
};

//Soft variable for lower body - thigh middle plate

float thighMiddlePlateX = 0.5, thighMiddlePlateY = 1.5, thighMiddlePlateZ = 0.1;

float thighMiddleFrontPlate[4][3] = {
	{-thighMiddlePlateX, thighMiddlePlateY, thighMiddlePlateZ},
	{thighMiddlePlateX, thighMiddlePlateY, thighMiddlePlateZ},
	{thighMiddlePlateX, -thighMiddlePlateY, thighMiddlePlateZ},
	{-thighMiddlePlateX, -thighMiddlePlateY, thighMiddlePlateZ}
};

float thighMiddleBackPlate[4][3] = {
	{-thighMiddlePlateX, thighMiddlePlateY, -thighMiddlePlateZ},
	{thighMiddlePlateX, thighMiddlePlateY, -thighMiddlePlateZ},
	{thighMiddlePlateX, -thighMiddlePlateY, -thighMiddlePlateZ},
	{-thighMiddlePlateX, -thighMiddlePlateY, -thighMiddlePlateZ}
};

//Soft variable for lower body - thigh side plate

float thighSidePlateX1 = 1,
thighSidePlateY1 = 1.5, thighSidePlateY2 = 2.5,
thighSidePlateZ = 0.1;

float thighSideFrontPlate[5][3] = {
	{-thighSidePlateX1, thighSidePlateY1, lowerBodyBackLowerPlateZ},
	{thighSidePlateX1, thighSidePlateY2, lowerBodyBackLowerPlateZ},
	{thighSidePlateX1, -thighSidePlateY1, lowerBodyBackLowerPlateZ},
	{0, -thighSidePlateY2, lowerBodyBackLowerPlateZ},
	{-thighSidePlateX1, -thighSidePlateY1, lowerBodyBackLowerPlateZ}
};

float thighSideBackPlate[5][3] = {
	{-thighSidePlateX1, thighSidePlateY1, -lowerBodyBackLowerPlateZ},
	{thighSidePlateX1, thighSidePlateY2, -lowerBodyBackLowerPlateZ},
	{thighSidePlateX1, -thighSidePlateY1, -lowerBodyBackLowerPlateZ},
	{0, -thighSidePlateY2, -lowerBodyBackLowerPlateZ},
	{-thighSidePlateX1, -thighSidePlateY1, -lowerBodyBackLowerPlateZ}
};

//Soft variable for lower body - calf upper part

float calfUpperX1 = 1, calfUpperX2 = 0.5,
calfUpperY = 1.5,
calfUpperZ = 0.75;

float calfUpperFrontPart[6][3] = {
	{-calfUpperX2, calfUpperY, calfUpperZ},
	{calfUpperX2, calfUpperY, calfUpperZ},
	{calfUpperX1, 0, calfUpperZ},
	{calfUpperX2, -calfUpperY, calfUpperZ},
	{-calfUpperX2, -calfUpperY, calfUpperZ},
	{-calfUpperX1, 0, calfUpperZ}
};

float calfUpperBackPart[6][3] = {
	{-calfUpperX2, calfUpperY, -calfUpperZ},
	{calfUpperX2, calfUpperY, -calfUpperZ},
	{calfUpperX1, 0, -calfUpperZ},
	{calfUpperX2, -calfUpperY, -calfUpperZ},
	{-calfUpperX2, -calfUpperY, -calfUpperZ},
	{-calfUpperX1, 0, -calfUpperZ}
};

//Soft variable for lower body - calf shield

float calfArmorX1 = 1, calfArmorX2 = 0.5,
calfArmorY = 2.5,
calfArmorZ = 0.1;

float calfArmorMiddleFront[4][3] = {
	{-calfArmorX1, calfArmorY, calfArmorZ},
	{calfArmorX1, calfArmorY, calfArmorZ},
	{calfArmorX1, -calfArmorY, calfArmorZ},
	{-calfArmorX1, -calfArmorY, calfArmorZ}
};

float calfArmorMiddleBack[4][3] = {
	{-calfArmorX1, calfArmorY, -calfArmorZ},
	{calfArmorX1, calfArmorY, -calfArmorZ},
	{calfArmorX1, -calfArmorY, -calfArmorZ},
	{-calfArmorX1, -calfArmorY, -calfArmorZ}
};

float calfArmorSideFront[3][3] = {
	{-calfArmorX2, calfArmorY, calfArmorZ},
	{calfArmorX2, 0, calfArmorZ},
	{-calfArmorX2, -calfArmorY, calfArmorZ}
};

float calfArmorSideBack[3][3] = {
	{-calfArmorX2, calfArmorY, -calfArmorZ},
	{calfArmorX2, 0, -calfArmorZ},
	{-calfArmorX2, -calfArmorY, -calfArmorZ}
};

//Soft variable for lower body - front upper and lower leg

float frontLegX1 = 2, frontLegX2 = 0.5,
frontLegY1 = 1.5, frontLegY2 = 0.2,
frontLegZ1 = 0.75, frontLegZ2 = 0.5;

float frontLowerSide1Leg[4][3] = {
	{-frontLegX2, -frontLegY2, frontLegZ1},
	{frontLegX1, -frontLegY2, frontLegZ1},
	{frontLegX2, -frontLegY1, frontLegZ1},
	{-frontLegX1, -frontLegY1, frontLegZ1}
};

float frontLowerSide2Leg[4][3] = {
	{-frontLegX2, -frontLegY2, -frontLegZ1},
	{frontLegX1, -frontLegY2, -frontLegZ1},
	{frontLegX2, -frontLegY1, -frontLegZ1},
	{-frontLegX1, -frontLegY1, -frontLegZ1}
};

float frontUpperSide1Leg[3][3] = {
	{-frontLegX2, -frontLegY2, frontLegZ2},
	{frontLegX1, frontLegY1, frontLegZ2},
	{frontLegX1, -frontLegY2, frontLegZ2}
};

float frontUpperSide2Leg[3][3] = {
	{-frontLegX2, -frontLegY2, -frontLegZ2},
	{frontLegX1, frontLegY1, -frontLegZ2},
	{frontLegX1, -frontLegY2, -frontLegZ2}
};

//Soft variable for lower body - back leg

float backLegX1 = 0.5, backLegX2 = 0.25,
backLegY = 1.5,
backLegZ = 0.75;

float backSide1Leg[5][3] = {
	{-backLegX1, backLegY, backLegZ},
	{backLegX2, backLegY, backLegZ},
	{backLegX1, 0, backLegZ},
	{backLegX2, -backLegY, backLegZ},
	{-backLegX1, -backLegY, backLegZ}
};

float backSide2Leg[5][3] = {
	{-backLegX1, backLegY, -backLegZ},
	{backLegX2, backLegY, -backLegZ},
	{backLegX1, 0, -backLegZ},
	{backLegX2, -backLegY, -backLegZ},
	{-backLegX1, -backLegY, -backLegZ} };

void polygonPlate(int noOfSide, GLfloat frontPolygon[][3], GLfloat backPolygon[][3])
{
	//Draw front
	glBegin(style_gl);
	//glColor3f(1.0f, 0.0f, 0.0f); // Red
	for (int i = 0; i < noOfSide; ++i) {
		glVertex3fv(frontPolygon[i]);
	}
	glEnd();

	// Draw back
	glBegin(style_gl);
	//glColor3f(0.0f, 1.0f, 0.0f); // Green
	for (int i = 0; i < noOfSide; ++i) {
		glVertex3fv(backPolygon[i]);
	}
	glEnd();

	// Draw sides
	glBegin(style_gl);
	//glColor3f(0.0f, 0.0f, 1.0f); // Blue
	for (int i = 0; i < noOfSide; ++i) {
		int next = (i + 1) % noOfSide; // Wrap around to the first vertex
		glVertex3fv(frontPolygon[i]);
		glVertex3fv(backPolygon[i]);
		glVertex3fv(backPolygon[next]);
		glVertex3fv(frontPolygon[next]);
	}
	glEnd();
}

void lowerBodyWaist()
{
	//Lower body - Front plate 
	glPushMatrix();

	//Translation to front
	glTranslatef(0, 0.25, 1.5);

	//Upper plate - polygon
	glPushMatrix();
	glTranslatef(0, 0.75, 0);
	polygonPlate(6, frontPlateFrontHexagon, frontPlateBackHexagon);
	glPopMatrix();

	//Lower plate - polygon
	glPushMatrix();
	glTranslatef(0, -0.75, 0);
	polygonPlate(5, frontPlateFrontPentagon, frontPlateBackPentagon);
	glPopMatrix();

	glPopMatrix();



	//Lower body - Middle part
	glPushMatrix();

	//Cuboid
	glTranslatef(-0.5, -0.5, -1.5);
	rect(1, 1, 3, style_gl);

	glPopMatrix();



	//Lower body - Back plate
	glPushMatrix();

	//Translation to back
	glTranslatef(0, 0, -1.6);

	//Upper plate
	glPushMatrix();

	//Translation to up for upper plates
	glTranslatef(0, 1.1, 0);

	//Left upper plate - polygon
	glPushMatrix();
	glTranslatef(-0.9, 0, 0);
	polygonPlate(6, backPlateFrontHexagon, backPlateBackHexagon);
	glPopMatrix();

	//Right upper plate - polygon
	glPushMatrix();
	glTranslatef(0.9, 0, 0);
	polygonPlate(6, backPlateFrontHexagon, backPlateBackHexagon);
	glPopMatrix();

	glPopMatrix();

	//Lower plate - polygon
	glPushMatrix();
	glTranslatef(0, -1, 0);
	polygonPlate(5, backPlateFrontPentagon, backPlateBackPentagon);
	glPopMatrix();

	glPopMatrix();



	//Waist and buttock connection
	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	glTranslatef(-0.5, -0.5, -1.25);
	rect(1, 1, 2.5, style_gl);
	glPopMatrix();
}

void lowerBodyThigh()
{
	//Lower body - buttock and thigh
	glPushMatrix();

	//Translate whole part to right
	glTranslatef(1.75, -0.25, 0);

	//Buttock
	glPushMatrix();
	drawSphereWithoutGLUAdvanced(1, 1.5, 1.25, 50, 50);
	glPopMatrix();

	//Lower body - thigh
	glPushMatrix();

	//Translate to down
	glTranslatef(0, -2.75, 1);

	//Thigh middle plate
	glPushMatrix();
	polygonPlate(4, thighMiddleFrontPlate, thighMiddleBackPlate);
	glPopMatrix();

	//Thigh right side plate
	glPushMatrix();
	glTranslatef(0.9, 0, -0.9);
	glRotatef(60, 0, 1, 0);
	polygonPlate(5, thighSideFrontPlate, thighSideBackPlate);
	glPopMatrix();

	//Thigh left side plate
	glPushMatrix();
	glTranslatef(-0.9, 0, -0.9);
	glRotatef(120, 0, 1, 0);
	polygonPlate(5, thighSideFrontPlate, thighSideBackPlate);
	glPopMatrix();

	//Translate to back
	glTranslatef(-0.5, -1.5, -1.5);

	//Inner thigh
	glPushMatrix();
	rect(1, 3.5, 1, style_gl);
	glPopMatrix();

	glPopMatrix();

	glPopMatrix();
}

void lowerBodyKnee()
{
	//Lower body - knee
	glPushMatrix();

	//Translate to down and rotate
	glTranslatef(1.5, -5, 0);
	glRotatef(90, 0, 1, 0);

	//Cylinder - Knee
	glPushMatrix();
	cylinder(1, 1, 0.5, 50, 50, style_gl);
	glPopMatrix();

	//Circle - Knee
	glPushMatrix();
	disk(0, 1, 50, 50, style_gl);
	glTranslatef(0, 0, 0.5);
	disk(0, 1, 50, 50, style_gl);
	glPopMatrix();

	glPopMatrix();
}

void lowerBodyCalf()
{
	//Lower body - calf
	glPushMatrix();

	glTranslatef(1.75, -4.5, 0);

	glPushMatrix();
	glTranslatef(0, -2.5, 0);

	//Calf upper part
	glPushMatrix();
	polygonPlate(6, calfUpperFrontPart, calfUpperBackPart);
	glPopMatrix();

	//Calf upper part and calf shield connection
	glPushMatrix();
	glTranslatef(0.75, 0, 0);
	rect(0.75, 0.2, 0.2, style_gl);
	glPopMatrix();

	//Calf shield
	glPushMatrix();

	//Translate to right and rotate
	glTranslatef(1.5, 0, 0);
	glRotatef(90, 0, 1, 0);

	//Armor middle part
	glPushMatrix();
	polygonPlate(4, calfArmorMiddleFront, calfArmorMiddleBack);
	glPopMatrix();

	//Armor right part
	glPushMatrix();
	glTranslatef(1.4, 0, -0.25);
	glRotatef(30, 0, 1, 0);
	polygonPlate(3, calfArmorSideFront, calfArmorSideBack);
	glPopMatrix();

	//Armor left part
	glPushMatrix();
	glTranslatef(-1.4, 0, -0.25);
	glRotatef(150, 0, 1, 0);
	polygonPlate(3, calfArmorSideFront, calfArmorSideBack);
	glPopMatrix();

	glPopMatrix();

	//Calf lower part
	glPushMatrix();
	glTranslatef(-0.5, -3, -0.5);
	rect(1, 2, 1, style_gl);
	glPopMatrix();

	glPopMatrix();

	glPopMatrix();
}

void lowerBodyLeg()
{
	//Lower body - leg
	glPushMatrix();

	glTranslatef(1.75, -3.75, 0);
	glRotatef(90, 0, 1, 0);

	glPushMatrix();
	glTranslatef(0, -8, 0);

	//Calf and leg connection
	glPushMatrix();
	glTranslatef(-0.4, 0, -0.4);
	rect(0.8, 2, 0.8, style_gl);
	glPopMatrix();

	//Front leg
	glPushMatrix();
	glTranslatef(-2, 0, 0);
	polygonPlate(4, frontLowerSide1Leg, frontLowerSide2Leg);
	polygonPlate(3, frontUpperSide1Leg, frontUpperSide2Leg);
	glPopMatrix();

	//Back leg
	glPushMatrix();
	glTranslatef(0.75, 0, 0);
	polygonPlate(5, backSide1Leg, backSide2Leg);
	glPopMatrix();

	glPopMatrix();

	glPopMatrix();
}

void lowerBodyLegStructure(bool left)
{
	glPushMatrix();
	//Thigh
	glTranslatef(thighTranslationX, thighTranslationY, thighTranslationZ);

	if(left)
		glRotatef(waistLeftThighRotation, 1, 0, 0);
	else
		glRotatef(waistRightThighRotation, 1, 0, 0);

	glTranslatef(-thighTranslationX, -thighTranslationY, -thighTranslationZ);
	lowerBodyThigh();
	glPushMatrix();
	//Knee
	lowerBodyKnee();
	glPushMatrix();
	//Calf
	glTranslatef(calfTranslationX, calfTranslationY, calfTranslationZ);

	if (left)
		glRotatef(thighLeftCalfRotation, 1, 0, 0);
	else
		glRotatef(thighRightCalfRotation, 1, 0, 0);
	
	glTranslatef(-calfTranslationX, -calfTranslationY, -calfTranslationZ);
	lowerBodyCalf();
	glPushMatrix();
	//Leg
	glTranslatef(legTranslationX, legTranslationY, legTranslationZ);

	if (left)
		glRotatef(calfLeftLegRotation, 1, 0, 0);
	else
		glRotatef(calfLeftLegRotation, 1, 0, 0);
	
	glTranslatef(-legTranslationX, -legTranslationY, -legTranslationZ);
	lowerBodyLeg();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

void lowerBody()
{
	glColor3f(colorR, colorG, colorB);

	//Waist
	lowerBodyWaist();

	//Right leg
	glPushMatrix();
	lowerBodyLegStructure(false);
	glPopMatrix();

	//Left leg
	glPushMatrix();
	glScalef(-1, 1, 1);
	lowerBodyLegStructure(true);
	glPopMatrix();
}
//----------------end---------------------------

void display()
{
	glClearColor(0.498, 0.498, 0.498, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	texture();
	light();

	projection();
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();
	camera();
	
	glPushMatrix();	
	glTranslatef(0,7.5,0);
	glRotatef(180, 0, 1, 0);
	glScalef(3, 3, 3);
	head();
	glPopMatrix();

	body();

	glPushMatrix();
	glTranslatef(0,-2,0);
	lowerBody();
	glPopMatrix();



	glPopMatrix();//camera
	destory();
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