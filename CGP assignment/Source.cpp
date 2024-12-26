
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <Math.h>

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLU32.lib")

#define WINDOW_TITLE "OpenGL Window"
const float PI = 3.141592f, speed = 0.1;

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

int windowWidth = 800, windowHeight = 800;

Vec3 cameraPosition = { 0.0f, 0.0f, 8.0f }; // Camera starting position
Vec3 target = { 0.0f, 0.0f, 0.0f };         // Point the camera looks at
Vec3 upVector = { 0.0f, 1.0f, 0.0f };       // Up direction
float radius = 8.0f;              // Distance from camera to target
float yaw = 0.0f;                 // Horizontal angle (in radians)
float pitch = 0.0f;               // Vertical angle (in radians)
GLenum style_glu = GLU_LINE, style_gl = GL_LINE_LOOP;
int style_switch;
GLUquadricObj* obj = NULL;


//Draw style for lower body
GLenum polygonFaceGLStyle = GL_POLYGON, polygonSideGLStyle = GL_QUADS, sphereGLStyle = GL_TRIANGLE_STRIP;

//Color for lower body
float colorR = 1, colorG = 1, colorB = 1;

//Rotation for lower body animation
float waistThighRotation = 0, waistThighMinRotation = -45, waistThighMaxRotation = 90;
float thighCalfRotation = 0, thighCalfMinRotation = -45, thighCalfMaxRotation = 0;
float calfLegRotation = 0, calfLegMinRotation = -15, calfLegMaxRotation = 15;

//Lower body part translation
float thightX = 1.75, thighY = -0.25, thighZ = 0;
float calfX = 1.25, calfY = -4.5, calfZ = 0;
float legX = 1.75, legY = -4, legZ = 0;
float thighTranslationX = 1.75, thighTranslationY = thighY, thighTranslationZ = 0;
float calfTranslationX = 1.25, calfTranslationY = thighTranslationY + calfY, calfTranslationZ = 0;
float legTranslationX = 1.75, legTranslationY = calfTranslationY + legY, legTranslationZ = 0;

//Texture
GLuint texture;
BITMAP BMP;					//Bitmap structure
HBITMAP hBMP;				//Bitmap handle0

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
				style_glu = GLU_POINT;
				style_gl = GL_POINTS;
				break;
			case 1:
				style_glu = GLU_LINE;
				style_gl = GL_LINE_LOOP;
				polygonFaceGLStyle = GL_LINE_LOOP;
				polygonSideGLStyle = GL_LINE_LOOP;
				sphereGLStyle = GL_LINE_LOOP;
				break;
			case 2:
				style_glu = GLU_FILL;
				style_gl = GL_POLYGON;
				polygonFaceGLStyle = GL_POLYGON;
				polygonSideGLStyle = GL_QUADS;
				sphereGLStyle = GL_TRIANGLE_STRIP;
				break;
			default:
				style_glu = GLU_LINE;
				style_gl = GL_LINE_LOOP;
				break;
			}
			style_switch++;
		}
		if (wParam == VK_ESCAPE) PostQuitMessage(0);
		if (wParam == VK_UP) { pitch += speed; }
		if (wParam == VK_DOWN) { pitch -= speed; }
		if (wParam == VK_LEFT) { yaw -= speed; }
		if (wParam == VK_RIGHT) { yaw += speed; }
		if (wParam == VK_OEM_PLUS) { radius -= 0.2f; }
		if (wParam == VK_OEM_MINUS) { radius += 0.2f; }
		if (wParam == VK_SPACE) {
			radius = 5.0f;              
			yaw = 0.0f;                 
			pitch = 0.0f;
		}

		//Rotation for lower body
		if (wParam == 'W')
		{
			waistThighRotation += 5;
		}
		if (wParam == 'S')
		{
			waistThighRotation -= 5;
		}
		if (wParam == 'E')
		{
			thighCalfRotation += 5;
		}
		if (wParam == 'D')
		{
			thighCalfRotation -= 5;
		}
		if (wParam == 'R')
		{
			calfLegRotation += 5;
		}
		if (wParam == 'F')
		{
			calfLegRotation -= 5;
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

void rect(float x, float y, float z, GLenum style) {

	//bottom
	glBegin(style);
	glTexCoord2f(0, 1);
	glVertex3f(0.0f, 0.0f, z);
	glTexCoord2f(1, 1);
	glVertex3f(x , 0.0f, z);
	glTexCoord2f(1, 0);
	glVertex3f(x , 0.0f, 0.0f);
	glTexCoord2f(0, 0);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glEnd();
	//left
	glBegin(style);
	glTexCoord2f(0, 1);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(1, 1);
	glVertex3f(0.0, y, 0.0);
	glTexCoord2f(1, 0);
	glVertex3f(0.0, y, z);
	glTexCoord2f(0, 0);
	glVertex3f(0.0f, 0.0f, z);
	glEnd();
	//front
	glBegin(style);
	glTexCoord2f(0, 1);
	glVertex3f(0.0f, 0.0f, z);
	glTexCoord2f(1, 1);
	glVertex3f(0.0f, y, z);
	glTexCoord2f(1, 0);
	glVertex3f(x, y, z);
	glTexCoord2f(0, 0);
	glVertex3f(x, 0.0f, z);
	glEnd();
	//right
	glBegin(style);
	glTexCoord2f(0, 1);
	glVertex3f(x, 0.0f, z);
	glTexCoord2f(1, 1);
	glVertex3f(x, y, z);
	glTexCoord2f(1, 0);
	glVertex3f(x, y, 0.0f);
	glTexCoord2f(0, 0);
	glVertex3f(x, 0.0f, 0.0f);
	glEnd();
	//back
	glBegin(style);
	glTexCoord2f(0, 1);
	glVertex3f(x, 0.0f, 0.0f);
	glTexCoord2f(1, 1);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(1, 0);
	glVertex3f(0.0, y, 0.0);
	glTexCoord2f(0, 0);
	glVertex3f(x, y, 0.0f);
	glEnd();
	//top 
	glBegin(style);
	glTexCoord2f(0, 1);
	glVertex3f(x, y, 0.0f);
	glTexCoord2f(1, 1);
	glVertex3f(0.0, y, 0.0);
	glTexCoord2f(1, 0);
	glVertex3f(0.0f, y, z);
	glTexCoord2f(0, 0);
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
	gluQuadricTexture(obj, GL_TRUE);  // Enable texture coordinate generation
	gluCylinder(obj, bottom, top, height, slice, stack);
	gluDeleteQuadric(obj);
}

void disk(float iner, float outer, int slice, int stack, GLenum style) {
	obj = gluNewQuadric();
	gluQuadricDrawStyle(obj, style);
	gluQuadricTexture(obj, GL_TRUE);  // Enable texture coordinate generation
	gluDisk(obj, iner, outer, slice, stack);
	gluDeleteQuadric(obj);
}

void drawSphereWithoutGLU(GLfloat radius = 0.35, int sliceNo = 30, int stackNo = 30)
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

//Sphere with customizable radius in 3 axes
void drawSphereWithoutGLUAdvanced(GLfloat xRadius, GLfloat yRadius, GLfloat zRadius, int sliceNo, int stackNo)
{
	GLfloat x, y, z, u, v, sliceA, stackA;

	for (sliceA = 0.0; sliceA < 2 * PI; sliceA += PI / sliceNo)
	{
		glBegin(sphereGLStyle );
		for (stackA = 0.0; stackA < PI; stackA += PI / stackNo)
		{
			x = xRadius * cos(stackA) * sin(sliceA);
			y = yRadius * sin(stackA) * sin(sliceA);
			z = zRadius * cos(sliceA);

			u = sliceA / (2 * PI);         // u: Longitude as a fraction of 2*PI
			v = stackA / PI;              // v: Latitude as a fraction of PI

			glTexCoord2f(u, v);			// Assign texture coordinate
			glVertex3f(x, y, z);

			x = xRadius * cos(stackA) * sin(sliceA + PI / stackNo);
			y = yRadius * sin(stackA) * sin(sliceA + PI / sliceNo);
			z = zRadius * cos(sliceA + PI / sliceNo);

			u = (sliceA + PI / sliceNo) / (2 * PI); // Adjust u for the next slice
			v = stackA / PI;                        // v stays the same

			glTexCoord2f(u, v);			// Assign texture coordinate
			glVertex3f(x, y, z);
		}
		glEnd();
	}
}

void camera() {
	gluLookAt(cameraPosition.x, cameraPosition.y, cameraPosition.z,
		target.x, target.y, target.z,
		upVector.x,upVector.y,upVector.z);

}

void body_upper() {
	glPushMatrix();
	glTranslatef(0, -2, 0);
	glRotatef(-90, 1, 0, 0);

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

	for (int i = 0; i < 10; i++) {
		glPushMatrix();

		glRotatef((360 / 10) * i, 0, 0, 1);
		glColor3f(0, 0, 0);
		rect(1.18, 0.2, 3, style_gl);
		glPopMatrix();
	}

	glPopMatrix();// r90
}



//Soft variable for lower body - front plate

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
lowerBodyFrontLowerPlateY1 = 1.5, lowerBodyFrontLowerPlateY2 = 0.4,
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

//Soft variable for lower body - tiny prism

float waistPrismX = 0.1,
waistPrismY = 0.1,
waistPrismZ = 0.4;

float side1Prism[3][3] = {
	{0, waistPrismY, waistPrismZ},
	{waistPrismX, 0, waistPrismZ},
	{-waistPrismX, 0, waistPrismZ}
};

float side2Prism[3][3] = {
	{0, waistPrismY, -waistPrismZ},
	{waistPrismX, 0, -waistPrismZ},
	{-waistPrismX, 0, -waistPrismZ}
};

//Soft variable for lower body - back plate

float lowerBodyBackUpperPlateX1 = 0.9, lowerBodyBackUpperPlateX2 = 0.75, lowerBodyBackUpperPlateX3 = 0.2,
lowerBodyBackUpperPlateY1 = 0.8, lowerBodyBackUpperPlateY2 = 0.2,
lowerBodyBackUpperPlateZ = 0.15;

float backPlateFrontHexagon[6][3] = {
	{-lowerBodyBackUpperPlateX3, lowerBodyBackUpperPlateY1, lowerBodyBackUpperPlateZ},
	{lowerBodyBackUpperPlateX3, lowerBodyBackUpperPlateY1, lowerBodyBackUpperPlateZ},
	{lowerBodyBackUpperPlateX2, lowerBodyBackUpperPlateY2, lowerBodyBackUpperPlateZ},
	{lowerBodyBackUpperPlateX1, -lowerBodyBackUpperPlateY1, lowerBodyBackUpperPlateZ},
	{-lowerBodyBackUpperPlateX1, -lowerBodyBackUpperPlateY1, lowerBodyBackUpperPlateZ},
	{-lowerBodyBackUpperPlateX2, lowerBodyBackUpperPlateY2, lowerBodyBackUpperPlateZ}
};

float backPlateBackHexagon[6][3] = {
	{-lowerBodyBackUpperPlateX3, lowerBodyBackUpperPlateY1, -lowerBodyBackUpperPlateZ},
	{lowerBodyBackUpperPlateX3, lowerBodyBackUpperPlateY1, -lowerBodyBackUpperPlateZ},
	{lowerBodyBackUpperPlateX2, lowerBodyBackUpperPlateY2, -lowerBodyBackUpperPlateZ},
	{lowerBodyBackUpperPlateX1, -lowerBodyBackUpperPlateY1, -lowerBodyBackUpperPlateZ},
	{-lowerBodyBackUpperPlateX1, -lowerBodyBackUpperPlateY1, -lowerBodyBackUpperPlateZ},
	{-lowerBodyBackUpperPlateX2, lowerBodyBackUpperPlateY2, -lowerBodyBackUpperPlateZ}
};

float lowerBodyBackLowerPlateX1 = 2, lowerBodyBackLowerPlateX2 = 1.5,
lowerBodyBackLowerPlateY1 = 1.75, lowerBodyBackLowerPlateY2 = 0.5,
lowerBodyBackLowerPlateZ = 0.15;

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

//Soft variable for lower body - buttock plate

float buttockPlateX1 = 0.5, buttockPlateX2 = 0.25,
buttockPlateY = 0.75,
buttockPlateZ = 0.3;

float buttockFrontPlate[6][3] = {
	{-buttockPlateX2, buttockPlateY, buttockPlateZ},
	{buttockPlateX2, buttockPlateY, buttockPlateZ},
	{buttockPlateX1, 0, buttockPlateZ},
	{buttockPlateX2, -buttockPlateY, buttockPlateZ},
	{-buttockPlateX2, -buttockPlateY, buttockPlateZ},
	{-buttockPlateX1, 0, buttockPlateZ}
};

float buttockBackPlate[6][3] = {
	{-buttockPlateX2, buttockPlateY, -buttockPlateZ},
	{buttockPlateX2, buttockPlateY, -buttockPlateZ},
	{buttockPlateX1, 0, -buttockPlateZ},
	{buttockPlateX2, -buttockPlateY, -buttockPlateZ},
	{-buttockPlateX2, -buttockPlateY, -buttockPlateZ},
	{-buttockPlateX1, 0, -buttockPlateZ}
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
frontLegY1 = 1.25, frontLegY2 = 0.2,
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
backLegY = 1.25,
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
	// Calculate the centroid for the front and back polygons
	GLfloat frontCentroid[2] = { 0.0f, 0.0f };
	GLfloat backCentroid[2] = { 0.0f, 0.0f };

	// Calculate the centroid of the front polygon
	for (int i = 0; i < noOfSide; ++i) {
		frontCentroid[0] += frontPolygon[i][0];
		frontCentroid[1] += frontPolygon[i][1];
	}
	frontCentroid[0] /= noOfSide;
	frontCentroid[1] /= noOfSide;

	// Calculate the centroid of the back polygon
	for (int i = 0; i < noOfSide; ++i) {
		backCentroid[0] += backPolygon[i][0];
		backCentroid[1] += backPolygon[i][1];
	}
	backCentroid[0] /= noOfSide;
	backCentroid[1] /= noOfSide;

	//Draw front
	glBegin(polygonFaceGLStyle);
	//glColor3f(1.0f, 0.0f, 0.0f); // Red
	for (int i = 0; i < noOfSide; ++i) {
		GLfloat u = (frontPolygon[i][0] - frontCentroid[0]) * 0.5f + 0.5f;  // Normalize
		GLfloat v = (frontPolygon[i][1] - frontCentroid[1]) * 0.5f + 0.5f;  // Normalize
		glTexCoord2f(u, v);
		glVertex3fv(frontPolygon[i]);
	}
	glEnd();

	// Draw back
	glBegin(polygonFaceGLStyle);
	//glColor3f(0.0f, 1.0f, 0.0f); // Green
	for (int i = 0; i < noOfSide; ++i) {
		GLfloat u = (backPolygon[i][0] - backCentroid[0]) * 0.5f + 0.5f;  // Normalize
		GLfloat v = (backPolygon[i][1] - backCentroid[1]) * 0.5f + 0.5f;  // Normalize
		glTexCoord2f(u, v);
		glVertex3fv(backPolygon[i]);
	}
	glEnd();

	// Draw sides
	glBegin(polygonSideGLStyle);
	//glColor3f(0.0f, 0.0f, 1.0f); // Blue
	for (int i = 0; i < noOfSide; ++i) {
		int next = (i + 1) % noOfSide; // Wrap around to the first vertex
		glTexCoord2f(1, 1);
		glVertex3fv(frontPolygon[i]);
		glTexCoord2f(1, 0);
		glVertex3fv(backPolygon[i]);
		glTexCoord2f(0, 0);
		glVertex3fv(backPolygon[next]);
		glTexCoord2f(0, 1);
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
	glTranslatef(0, -1, 0);
	polygonPlate(5, frontPlateFrontPentagon, frontPlateBackPentagon);
	glPopMatrix();

	//Front plate - tiny prism
	glPushMatrix();
	glTranslatef(0, 0, 0.1);
	glRotatef(90, 1, 0, 0);
	for (int i = 0; i < 3; i++)
	{
		glPushMatrix();
		glRotatef(i * 120, 0, 1, 0);
		glTranslatef(0, 0, 0.75);
		polygonPlate(3, side1Prism, side2Prism);
		glPopMatrix();
	}
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
	glTranslatef(0, 1.4, 0);

	//Left upper plate - polygon
	glPushMatrix();
	glTranslatef(-1.1, 0, 0);
	polygonPlate(6, backPlateFrontHexagon, backPlateBackHexagon);
	glPopMatrix();

	//Right upper plate - polygon
	glPushMatrix();
	glTranslatef(1.1, 0, 0);
	polygonPlate(6, backPlateFrontHexagon, backPlateBackHexagon);
	glPopMatrix();

	glPopMatrix();

	//Lower plate - polygon
	glPushMatrix();
	glTranslatef(0, -1.1, 0);
	polygonPlate(5, backPlateFrontPentagon, backPlateBackPentagon);
	glPopMatrix();



	//Back plate - tiny prism
	glPushMatrix();
	glTranslatef(0, 0, -0.15);
	glRotatef(90, 0, 0, 1);
	glRotatef(-90, 1, 0, 0);
	for (int i = 1; i <= 4; i++)
	{
		glPushMatrix();
		glRotatef(45 + i * 90, 0, 1, 0);
		glTranslatef(1.5, 0, 0);
		polygonPlate(3, side1Prism, side2Prism);
		glPopMatrix();
	}
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
			glPushMatrix();
				glTranslatef(0, 0, 1);
				polygonPlate(6, buttockFrontPlate, buttockBackPlate);
			glPopMatrix();
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
				glScalef(2, 2, 2);
				glTranslatef(0, 0.25, 0.1);
				glRotatef(90, 1, 0, 0);
				glRotatef(-60, 0, 1, 0);
				polygonPlate(3, side1Prism, side2Prism);
			glPopMatrix();

			//Thigh left side plate
			glPushMatrix();
				glScalef(-1, 1, 1);
				glTranslatef(0.9, 0, -0.9);
				glRotatef(60, 0, 1, 0);
				polygonPlate(5, thighSideFrontPlate, thighSideBackPlate);
				glScalef(2, 2, 2);
				glTranslatef(0, 0.25, 0.1);
				glRotatef(90, 1, 0, 0);
				glRotatef(-60, 0, 1, 0);
				polygonPlate(3, side1Prism, side2Prism);
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
	cylinder(1, 1, 0.5, 50, 50, style_glu);
	glPopMatrix();

	//Circle - Knee
	glPushMatrix();
	disk(0, 1, 50, 50, style_glu);
	glTranslatef(0, 0, 0.5);
	disk(0, 1, 50, 50, style_glu);
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
				glScalef(-1, 1, 1);
				glTranslatef(1.4, 0, -0.25);
				glRotatef(30, 0, 1, 0);
				polygonPlate(3, calfArmorSideFront, calfArmorSideBack);
				glPopMatrix();

			glPopMatrix();

			//Calf lower part
			glPushMatrix();

			glTranslatef(0, -3, 0);
				glPushMatrix();
				glTranslatef(-0.5, 0, -0.5);
				rect(1, 2, 1, style_gl);
				glPopMatrix();

				glPushMatrix();
					glRotatef(90, 0, 1, 0);
					for (int i = 0; i < 4; i++)
					{
						glPushMatrix();
							glRotatef(i * 90, 0, 1, 0);
							glTranslatef(-1, -0.5, 0);
							glScalef(0.25, 1, 1);
							polygonPlate(3, frontUpperSide1Leg, frontUpperSide2Leg);
						glPopMatrix();
					}
				glPopMatrix();

			glPopMatrix();

		glPopMatrix();

	glPopMatrix();
}

void lowerBodyLeg()
{
	//Lower body - leg
	glPushMatrix();

		glTranslatef(1.75, -4, 0);
		glRotatef(90, 0, 1, 0);

		glPushMatrix();
			glTranslatef(0, -8.25, 0);

			//Calf and leg connection
			glPushMatrix();
				glTranslatef(-0.25, 0, -0.25);
				rect(0.5, 3, 0.5, style_gl);
			glPopMatrix();

			//Front leg
			glPushMatrix();
				glTranslatef(-1, 0, 0);
				polygonPlate(3, frontUpperSide1Leg, frontUpperSide2Leg);
				//glTranslatef(-1, 0, 0);
				polygonPlate(4, frontLowerSide1Leg, frontLowerSide2Leg);
			glPopMatrix();

			//Back leg
			glPushMatrix();
				glTranslatef(0.75, 0, 0);
				polygonPlate(5, backSide1Leg, backSide2Leg);
			glPopMatrix();

		glPopMatrix();

	glPopMatrix();
}

void lowerBodyLegStructure()
{
	glPushMatrix();
		//Thigh
		glTranslatef(thighTranslationX, thighTranslationY, thighTranslationZ);
		glRotatef(waistThighRotation, 1, 0, 0);
		glTranslatef(-thighTranslationX, -thighTranslationY, -thighTranslationZ);
		lowerBodyThigh();
		glPushMatrix();
			//Knee
			lowerBodyKnee();
			glPushMatrix();
				//Calf
				glTranslatef(calfTranslationX, calfTranslationY, calfTranslationZ);
				glRotatef(thighCalfRotation, 1, 0, 0);
				glTranslatef(-calfTranslationX, -calfTranslationY, -calfTranslationZ);
				lowerBodyCalf();
				glPushMatrix();
					//Leg
					glTranslatef(legTranslationX, legTranslationY, legTranslationZ);
					glRotatef(calfLegRotation, 1, 0, 0);
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
	lowerBodyLegStructure();
	glPopMatrix();

	//Left leg
	glPushMatrix();
	glScalef(-1, 1, 1);
	lowerBodyLegStructure();
	glPopMatrix();
}

GLuint LoadTexture(LPCSTR fileName)
{
	GLuint texture = 0;			//Texture name

	//Initialize texture info
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	HBITMAP hBMP = (HBITMAP)LoadImage(GetModuleHandle(NULL), fileName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	GetObject(hBMP, sizeof(BMP), &BMP);

	//Assign texture to polygon
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BMP.bmWidth,
		BMP.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);

	//Delete texture info
	DeleteObject(hBMP);

	return texture;
}

void display()
{
	glClearColor(0.498, 0.498, 0.498, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	texture = LoadTexture("Box.bmp");

	glPushMatrix();
	camera();

	//body_upper();

	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	glTranslatef(0, 5, 0);
	lowerBody();
	glPopMatrix();

	glDeleteTextures(1, &texture);
	glDisable(GL_TEXTURE_2D);


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
		CW_USEDEFAULT, CW_USEDEFAULT, windowWidth, windowHeight,
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
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, windowWidth / windowHeight, 0.0001, 9999);
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