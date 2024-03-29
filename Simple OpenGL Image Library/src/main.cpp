#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include "SOIL.h"
#include <GL/glut.h>
#include <vector>
using namespace std;

GLdouble faceNormal[8][3];
GLdouble vertexNormal[6][3];

bool shouldDisplayTexture1Octahedron = false;
bool shouldDisplayTexture2Octahedron = false;
bool shouldDisplayTexture3Octahedron = false;

GLfloat pointSize = 10;

GLdouble endPoint1[3] = { 398.974 ,-117.217 ,153.073 };
GLdouble endPoint2[3] = { -198.974 ,317.217 ,-153.073 };
GLdouble initialPosition[3] = { 137.187,409.385,-99.6725 };
//597.948, -434.434, 306.146
GLdouble rotationAxis[3] = { endPoint1[0] - endPoint2[0] ,endPoint1[1] - endPoint2[1],endPoint1[2] - endPoint2[2] };
GLdouble lineLengh = pow(endPoint1 - endPoint2, 2);
GLdouble steps[3] = {};

GLint option = 0;
GLdouble width = 50;
GLdouble height = 100;
GLdouble octahedronVertex[6][3] = { { width,0,-width },{ -width,0,-width },{ -width,0,width },{ width,0,width },{ 0,height,0 },
	{ 0,-height,0 } };
GLint faceIndex[8][3] = { { 3,0,4 }/*up+x*/,
						  { 0,1,4 }/*up-z*/,
						  { 1,2,4 }/*up-x*/,
						  { 2,3,4 }/*up+z*/,
						  { 0,3,5 }/*down+x*/,
						  { 1,0,5 }/*down-z*/,
						  { 2,1,5 }/*down-x*/,
						  { 3,2,5 }/*down+z*/ 
						};
GLint vertexIndex[6][4] = { {0,1,4,5},{1,2,5,6},{2,3,7,6},{0,3,4,7},{0,1,2,3},{4,5,6,7}};

GLdouble rotationStep = 2;
GLdouble tx = endPoint2[0], ty = endPoint2[1], tz = endPoint2[2];

double stepPortion = 0.007;
GLdouble translationXStep = rotationAxis[0] * stepPortion;
GLdouble translationYStep = rotationAxis[1] * stepPortion;
GLdouble translationZStep = rotationAxis[2] * stepPortion;

GLdouble rotationAngle = 0;
GLdouble xmin1 = -800, xmax1 = 800, ymin1 = -800, ymax1 = 800, zmin1 = -800, zmax1 = 800, znear1 = -800, zfar1 = 800;
GLdouble finalTranslation[3];
GLdouble scaleFactor = 1.0;
GLuint textureID;
GLint border = 0;
const int imageWidth = 10;
const int imageHeight = 10;
GLubyte texArray1[imageHeight][imageWidth][4];
void displayFaceNormal()
{
	GLdouble tempPt[3];
	GLint k, j;
	GLdouble scale = 500.0;

	glColor3f(1.0, 0.0, 1.0);

	for (k = 0; k < 6; k++)
	{
		for (j = 0; j < 3; j++)
		{
			tempPt[j] = (octahedronVertex[faceIndex[k][0]][j] + octahedronVertex[faceIndex[k][2]][j]) / 2.0; // average of two opposite vertices
		}

		glBegin(GL_LINES);
		glVertex3dv(tempPt);
		glVertex3d(tempPt[0] + faceNormal[k][0] * scale, tempPt[1] + faceNormal[k][1] * scale, tempPt[2] + faceNormal[k][2] * scale);
		glEnd();
	}

	glFlush();
}

//This function is to display the normal vector for each vertex of the cube which are located at each vertex.
void displayVertexNormal()
{
	GLint k, j;
	GLdouble scale = 50.0;

	glColor3f(1.0, 1.0, 1.0);

	for (k = 0; k < 8; k++)
	{
		glBegin(GL_LINES);
		glVertex3dv(octahedronVertex[k]);
		glVertex3d(octahedronVertex[k][0] + vertexNormal[k][0] * scale, octahedronVertex[k][1] + vertexNormal[k][1] * scale, octahedronVertex[k][2] + vertexNormal[k][2] * scale);
		glEnd();
	}

	glFlush();
}
void makeImage(void)
{
	GLint i, j, c;

	for (i = border; i< imageHeight - border; i++)
	{
		for (j = border; j< imageWidth - border; j++)
		{
			texArray1[i][j][0] = (GLubyte)255;
			texArray1[i][j][1] = (GLubyte)0;
			texArray1[i][j][2] = (GLubyte)0;
			texArray1[i][j][3] = (GLubyte)255;
			if (i == 0 || i == imageHeight / 2 || i == imageHeight - 1)
			{
				texArray1[i][j][0] = (GLubyte)0;
				texArray1[i][j][1] = (GLubyte)0;
				texArray1[i][j][2] = (GLubyte)255;
				texArray1[i][j][3] = (GLubyte)255;
			}
			if (j == 0)
			{
				texArray1[i][j][0] = (GLubyte)0;
				texArray1[i][j][1] = (GLubyte)0;
				texArray1[i][j][2] = (GLubyte)255;
				texArray1[i][j][3] = (GLubyte)255;
			}
		}
	}
}

void drawTexture1Octahedron()
{
	glEnable(GL_TEXTURE_2D);

	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, texArray1);

	for (GLint i = 0; i < 8; i++)
	{
		glBegin(GL_TRIANGLES);
		for (GLint j = 0; j < 3; j++)
		{
			if (j == 0)
			{
				glTexCoord2d(0.0, 0.0);
				glVertex3dv(octahedronVertex[faceIndex[i][j]]);
			}
			if (j == 1)
			{
				glTexCoord2d(0.0, 1.0);
				glVertex3dv(octahedronVertex[faceIndex[i][j]]);
			}
			if (j == 2)
			{
				glTexCoord2d(1.0, 1.0);
				glVertex3dv(octahedronVertex[faceIndex[i][j]]);
			}
		}
		glEnd();
	}

	glDisable(GL_TEXTURE_2D);
	glFlush();
}
void loadCustomTexture(char fileName[])
{
	textureID = SOIL_load_OGL_texture
	(
		fileName,
		SOIL_LOAD_AUTO,
		textureID,
		//SOIL_CREATE_NEW_ID,
		SOIL_FLAG_DDS_LOAD_DIRECT
		//SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB //SOIL_FLAG_TEXTURE_RECTANGLE  
	);
}
void drawTexturedOctahedron()
{
	glEnable(GL_TEXTURE_2D);
	for (GLint i = 0; i < 8; i++)
	{
		glBegin(GL_TRIANGLES);
		for (GLint j = 0; j < 3; j++)
		{
			if (j == 0)
			{
				glTexCoord2d(0.0, 0.0);
				glVertex3dv(octahedronVertex[faceIndex[i][j]]);
			}
			if (j == 1)
			{
				glTexCoord2d(0.0, 1.0);
				glVertex3dv(octahedronVertex[faceIndex[i][j]]);
			}
			if (j == 2)
			{
				glTexCoord2d(1.0, 1.0);
				glVertex3dv(octahedronVertex[faceIndex[i][j]]);
			}
		}
		glEnd();
	}
	glFlush();
	glDisable(GL_TEXTURE_2D);
}
void drawTexture2Octahedron()
{
	loadCustomTexture("MCR.png");
	drawTexturedOctahedron();
}
void drawTexture3Octahedron()
{
	loadCustomTexture("Lifeline.png");
	drawTexturedOctahedron();
}
void TriangleFace(GLint face[3]) // arguments are indices of four vertices of a TriangleFace
{
	glBegin(GL_TRIANGLES);
	for (GLint i = 0; i < 3; i++)
	{
		glVertex3dv(octahedronVertex[face[i]]);
	}
	glEnd();
}

void CalculateFinalTranslationVector()
{
	GLdouble denominator = pow(rotationAxis[0], 2) + pow(rotationAxis[1], 2) + pow(rotationAxis[1], 2);
	GLdouble numerator = -rotationAxis[0] * endPoint1[0] - rotationAxis[1] * endPoint1[1] - rotationAxis[2] * endPoint1[2];
	GLdouble t = numerator / denominator;
	finalTranslation[0] = rotationAxis[0] * t + endPoint1[0];
	finalTranslation[1] = rotationAxis[1] * t + endPoint1[1];
	finalTranslation[2] = rotationAxis[2] * t + endPoint1[2];
}

//THis function draw the six faces of a cube by calling the TriangleFace function
void displayOctahedron() {
	for (GLint i = 0; i < 8; i++)
	{
		TriangleFace(faceIndex[i]);
	}
}

void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(xmin1, xmax1, ymin1, ymax1, znear1, zfar1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(45, 1, 1, 0);
}

void drawAxis(GLdouble xmin, GLdouble xmax, GLdouble ymin, GLdouble ymax, GLdouble zfar)
{
	glLineWidth(3);
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(xmin, 0.0, 0.0);
	glVertex3f(xmax, 0.0, 0.0);

	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, ymin, 0.0);
	glVertex3f(0.0, ymax, 0.0);

	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, -zfar);
	glVertex3f(0.0, 0.0, zfar);
	glEnd();
	glLineWidth(1);
}

GLfloat spotLight_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat spotLight_diffuse[] = { 1, 1, 1, 1.0 };
GLfloat spotLight_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat spotLight_position[4] = { initialPosition[0], initialPosition[1] - 200, initialPosition[2] + 800, 1.0 };
GLfloat dirVector[] = { 0.0,0.0,-1.0 };
void setSpotLightProperty()
{
	glLightfv(GL_LIGHT1, GL_AMBIENT, spotLight_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, spotLight_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, spotLight_specular);
	glLightfv(GL_LIGHT1, GL_POSITION, spotLight_position);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, dirVector);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 60.0);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);

	glColor3f(1.0, 1.0, 1.0);
	//glPointSize(10.0);
	glBegin(GL_POINTS);
	glVertex3f(spotLight_position[0], spotLight_position[1], spotLight_position[2]);
	glEnd();
	//glPointSize(1.0);
	glFlush();
}

GLfloat pointLight_position[4] = { -150.0, 0.0, 0.0, 1.0 };
GLfloat pointLight_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat pointLight_diffuse[] = { 1, 1, 1, 1.0 };
GLfloat pointLight_specular[] = { 1.0, 1.0, 1.0, 1.0 };
void setPointLightProperty()
{
	glLightfv(GL_LIGHT2, GL_POSITION, pointLight_position);
	glLightfv(GL_LIGHT2, GL_AMBIENT, pointLight_ambient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, pointLight_diffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, pointLight_specular);
	glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 0.01);
	glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.0001);
	glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.0001);//important

	glColor3f(1.0, 1.0, 1.0);
	//glPointSize(10.0);
	glBegin(GL_POINTS);
	glVertex3f(pointLight_position[0], pointLight_position[1], pointLight_position[2]);
	glEnd();
	//glPointSize(1.0);
	glFlush();
}

GLfloat ambientCoef[] = { 0.8f, 0.0f, 0.0f };
GLfloat diffuseCoef[] = { 0.8f, 0.0f, 0.0f };
GLfloat specularCoef[] = { 1.0f, 1.0f, 1.0f };
GLfloat mat_shininess = 128.0f;

void setMaterialProperty()
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientCoef);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseCoef);

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularCoef);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
}

void displayFunction()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	drawAxis(xmin1, xmax1, ymin1, ymax1, zfar1);
	glLineWidth(5);
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0);

	glVertex3f(endPoint1[0], endPoint1[1], endPoint1[2]);
	glVertex3f(endPoint2[0], endPoint2[1], endPoint2[2]);

	glEnd();
	glColor3d(1.0, 1.0, 1.0);
	glLineWidth(5);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glPointSize(pointSize);
	setMaterialProperty();
	setSpotLightProperty();
	setPointLightProperty();
	glPushMatrix();

	if (option == 1)
	{
		glTranslated(finalTranslation[0], finalTranslation[1], finalTranslation[2]);
		glRotated(rotationAngle, rotationAxis[0], rotationAxis[1], rotationAxis[2]);
	}
	if (option == 2)
	{
		glTranslated(tx, ty, tz);
		glScaled(scaleFactor, scaleFactor, scaleFactor);
	}
	//else
	//cout << "invalid option" << endl;

	if (option != 2)
	{
		glTranslated(initialPosition[0], initialPosition[1], initialPosition[2]);
	}

	if (shouldDisplayTexture1Octahedron)
	{
		drawTexture1Octahedron();

	}
	else if(shouldDisplayTexture2Octahedron)
	{
		drawTexture2Octahedron();
	}
	else if(shouldDisplayTexture3Octahedron)
	{
		drawTexture3Octahedron();
	}
	else
	{
		displayOctahedron();
	}
	//displayFaceNormal();
	//displayVertexNormal();

	//glDisable(GL_LIGHT1);
	//glDisable(GL_LIGHT2);

	glPopMatrix();
	Sleep(20);
	glutSwapBuffers();

	glFlush();
}

void keyboard(unsigned char key, int x, int y) // for the first display window
{
	GLdouble angleStep = 3;
	switch (key) {
	case 'x':
		glRotatef(-angleStep, 1.0, 0.0, 0.0);
		break;
	case 'X':
		glRotatef(angleStep, 1.0, 0.0, 0.0);
		break;
	case 'y':
		glRotatef(-angleStep, 0.0, 1.0, 0.0);
		break;
	case 'Y':
		glRotatef(angleStep, 0.0, 1.0, 0.0);
		break;
	case 'z':
		glRotatef(-angleStep, 0.0, 0.0, 1.0);
		break;
	case 'Z':
		glRotatef(angleStep, 0.0, 0.0, 1.0);
		break;
	case 'U':
	case 'u':
		if (rotationStep<=10)
		{
			rotationStep += 1;
		}
		break;
	case 'd':
	case 'D':
		if (rotationStep>=2)
		{
			rotationStep -= 1;
		}
		break;
	case 'F':
	case 'f': 
		glShadeModel(GL_FLAT);
		break;
	case 's':
	case 'S':
		glShadeModel(GL_SMOOTH);
		break;
	case 'B':
		spotLight_position[0] -= 10;
		glutPostRedisplay();
		break;
	case 'M':
		spotLight_position[0] += 10;
		break;
	case 'H':
		spotLight_position[1] += 10;
		break;
	case 'N':
		spotLight_position[1] -= 10;
		break;
	case 'G':
		spotLight_position[2] -= 10;
		break;
	case 'J':
		spotLight_position[2] += 10;
		break;
	case 'b':
		pointLight_position[0] -= 10;
		break;
	case 'm':
		pointLight_position[0] += 10;
		break;
	case 'h':
		pointLight_position[1] += 10;
		break;
	case 'n':
		pointLight_position[1] -= 10;
		break;
	case 'g':
		pointLight_position[2] -= 10;
		break;
	case 'j':
		pointLight_position[2] += 10;
		break;
	case 27:
		exit(0);
	default: break;
	}
	cout << "New Spotlight Pos: " << spotLight_position[0] <<","<< spotLight_position[1] << "," << spotLight_position[2]<<endl;
	cout << "New Pointlight Pos: " << pointLight_position[0] << "," << pointLight_position[1] << "," << pointLight_position[2]<< endl;

	glutPostRedisplay();
}

void reshapeFcn(GLint newWidth, GLint newHeight)
{
	glViewport(0, 0, newWidth, newHeight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(xmin1, xmax1, ymin1, ymax1, znear1, zfar1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(45, 1, 1, 0);
}

void animationSelection(GLint animationOption)
{
	switch (animationOption) {
	case 1:
		option = 1;
		break;
	case 2:
		option = 2;
		break;
	case 7:
		exit(0);
	default:
		cout << "invalid option" << endl;
	}
	glutPostRedisplay();
}

void idle()
{
	if (option == 1)
	{
		rotationAngle += rotationStep;
		/*if (rotationStep >= 10)
		{
			rotationStep = 2;
		}*/
		if (rotationAngle > 360)
		{
			rotationAngle = 0.0;
		}
	}
	else if (option == 2)
	{
		scaleFactor += 0.01;
		tx += translationXStep;
		ty += translationYStep;
		tz += translationZStep;

		if (abs(tx) > abs(endPoint1[0]) && abs(ty) > abs(endPoint1[1]) && abs(tz) > abs(endPoint1[2]))
		{
			scaleFactor = 1.0;
			tx = endPoint2[0];
			ty = endPoint2[1];
			tz = endPoint2[2];
		}
	}
	else
		cout << "invalid option" << endl;

	glutPostRedisplay();
}

void keyFunc(GLint key, GLint xMouse, GLint yMouse)
{
	switch (key)
	{
	case GLUT_KEY_UP: glutIdleFunc(idle); break;
	case GLUT_KEY_DOWN: glutIdleFunc(nullptr); break;
	default: break;
	}
	glutPostRedisplay();
}

void drawOptions(GLint drawOptions)
{
	GLenum mode = GL_LINE;
	switch (drawOptions) {
	case 1:
		mode = GL_POINT;
		glPolygonMode(GL_FRONT_AND_BACK, mode);
		//glPointSize(100);
		break;
	case 2:
		mode = GL_LINE;
		glPolygonMode(GL_FRONT_AND_BACK, mode);
		break;
	case 3:
		mode = GL_FILL;
		glPolygonMode(GL_FRONT_AND_BACK, mode);
	}
	glutPostRedisplay();
}

void textureOptions(GLint drawOptions)
{
	GLenum mode = GL_LINE;
	switch (drawOptions) {
	case 1:
		shouldDisplayTexture2Octahedron = false;
		shouldDisplayTexture1Octahedron = false;
		shouldDisplayTexture3Octahedron = false;
		break;
	case 2:
		shouldDisplayTexture2Octahedron = false;
		shouldDisplayTexture1Octahedron = true;
		shouldDisplayTexture3Octahedron = false;
		break;
	case 3:
		shouldDisplayTexture2Octahedron = true;
		shouldDisplayTexture1Octahedron = false;
		shouldDisplayTexture3Octahedron = false;
		break;
	case 4:
		shouldDisplayTexture2Octahedron = false;
		shouldDisplayTexture1Octahedron = false;
		shouldDisplayTexture3Octahedron = true;
		break;
	}
	glutPostRedisplay();
}

void calculateVertexNormal(GLdouble vexNormal[6][3])
{
	GLint k, j;

	for (k = 0; k < 6; k++)
	{

		for (j = 0; j < 3; j++)
		{
			vexNormal[k][j] = (faceNormal[vertexIndex[k][0]][j] + faceNormal[vertexIndex[k][1]][j] + vertexIndex[faceIndex[k][2]][j]) / 3.0;
		}
	}
}
void calculateFaceNormal(GLdouble normal[8][3])
{
	GLdouble temp;
	GLdouble tempPt[3];
	GLint k, j;

	GLdouble vert1[3], vert2[3], vert3[3];

	for (k = 0; k < 8; k++)
	{
		//Use three temperory vectors to get the three vertices under consideration to simplify the expressions of normal vector components
		for (j = 0; j < 3; j++)
		{
			vert1[j] = octahedronVertex[faceIndex[k][0]][j];
			vert2[j] = octahedronVertex[faceIndex[k][1]][j];
			vert3[j] = octahedronVertex[faceIndex[k][2]][j];
		}

		//The following three statements are the code version of formula in topic slide 12 (Module 2 part 2)
		tempPt[0] = (vert2[1] - vert1[1])*(vert3[2] - vert1[2]) - (vert2[2] - vert1[2])*(vert3[1] - vert1[1]);
		tempPt[1] = -(vert2[0] - vert1[0])*(vert3[2] - vert1[2]) + (vert2[2] - vert1[2])*(vert3[0] - vert1[0]);
		tempPt[2] = (vert2[0] - vert1[0])*(vert3[1] - vert1[1]) - (vert2[1] - vert1[1])*(vert3[0] - vert1[0]);

		//THis is to calculate the magnitude if the normal vector using the formula mag = sqrt(x^2+y^2+z^2)
		temp = sqrt((tempPt[0] * tempPt[0] + tempPt[1] * tempPt[1] + tempPt[2] * tempPt[2]));

		//The following three statements are to normalise the normal vector to make its magnitude be one unit 
		normal[k][0] = tempPt[0] / temp;
		normal[k][1] = tempPt[1] / temp;
		normal[k][2] = tempPt[2] / temp;
	}

}


int main(int argc, char** argv)
{
	GLint windowID;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowPosition(50, 50);
	glutInitWindowSize(1000, 1000);

	windowID = glutCreateWindow("Part 2 - 3D Object");
	makeImage();
	init();
	CalculateFinalTranslationVector();
	GLint displayModeMenu;
	displayModeMenu = glutCreateMenu(drawOptions);
	glutAddMenuEntry("Verteces", 1);
	glutAddMenuEntry("Edges", 2);
	glutAddMenuEntry("Everything", 3);
	GLint textureModeMenu;
	textureModeMenu = glutCreateMenu(textureOptions);
	glutAddMenuEntry("No Texture", 1);
	glutAddMenuEntry("Texture1", 2);
	glutAddMenuEntry("Texture2", 3);
	glutAddMenuEntry("Texture3", 4);

	glutCreateMenu(animationSelection);
	glutAddMenuEntry("Rotate", 1);
	glutAddMenuEntry("Translation", 2);
	glutAddSubMenu("Box Style", displayModeMenu);
	glutAddSubMenu("Texture Style", textureModeMenu);
	glutAddMenuEntry("Exit", 7);
	glutAttachMenu(GLUT_LEFT_BUTTON);

	calculateFaceNormal(faceNormal);
	calculateVertexNormal(vertexNormal);

	glutDisplayFunc(displayFunction);
	glutSpecialFunc(keyFunc);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshapeFcn);

	glutMainLoop();

	return 0;
}