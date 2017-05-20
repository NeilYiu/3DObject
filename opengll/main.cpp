#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <GL/glut.h>
#include <vector>
using namespace std;

GLdouble endPoint1[3] = { 398.974 ,-117.217 ,153.073 };
GLdouble endPoint2[3] = { -198.974 ,317.217 ,-153.073 };
GLdouble initialPosition[3] = { 137.187,409.385,-99.6725 };
//597.948, -434.434, 306.146
GLdouble rotationAxis[3] = {endPoint1[0]-endPoint2[0] ,endPoint1[1] - endPoint2[1],endPoint1[2] - endPoint2[2]};
GLdouble lineLengh = pow(endPoint1 - endPoint2,2);
GLdouble steps[3] = {};

GLint option = 1;
GLdouble width = 50;
GLdouble height = 100;
GLdouble octahedronVertex[6][3] = {{width,0,-width },{-width,0,-width },{-width,0,width },{ width,0,width },{0,height,0},{ 0,-height,0 }};
GLint faceIndex[8][3] = { {3,0,4},{0,1,4},{1,2,4},{3,2,4},{3,0,5},{0,1,5},{1,2,5},{3,2,5}};
GLdouble xStep = 5, yStep = 5, zStep = 5;
GLdouble tx = endPoint2[0], ty = endPoint2[1], tz = endPoint2[2];
GLdouble txStep = 5, tyStep = 5, tzStep = 5;
GLdouble xAngle = 0, yAngle = 0, zAngle = 0;
GLdouble xmin1 = -800, xmax1 = 800, ymin1 = -800, ymax1 = 800, zmin1 = -800, zmax1 = 800, znear1 = -800, zfar1 = 800;
GLdouble finalTranslation[3];

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
GLfloat spotLight_position[4] = { initialPosition[0], initialPosition[1]-200, initialPosition[2]+800, 1.0 };
GLfloat dirVector[] = { 0.0,0.0,-1.0 };

void setSpotLightProperty()
{
	glLightfv(GL_LIGHT1, GL_AMBIENT, spotLight_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, spotLight_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, spotLight_specular);
	glLightfv(GL_LIGHT1, GL_POSITION, spotLight_position);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, dirVector);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 1.0);
	glColor3f(1.0, 1.0, 1.0);
	glPointSize(10.0);
	glBegin(GL_POINTS);
	glVertex3f(spotLight_position[0], spotLight_position[1], spotLight_position[2]);
	glEnd();
	glPointSize(1.0);
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
	glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1.0);	glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.0001);	glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.00001);
	glColor3f(1.0, 1.0, 1.0);
	glPointSize(10.0);
	glBegin(GL_POINTS);
	glVertex3f(pointLight_position[0], pointLight_position[1], pointLight_position[2]);
	glEnd();
	glPointSize(1.0);
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
	glClear(GL_COLOR_BUFFER_BIT);
	drawAxis(xmin1, xmax1, ymin1, ymax1, zfar1);
	glLineWidth(5);
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0);

	glVertex3f(endPoint1[0], endPoint1[1], endPoint1[2]);
	glVertex3f(endPoint2[0], endPoint2[1], endPoint2[2]);

	glEnd();
	glColor3d(1.0, 1.0, 1.0);
	glLineWidth(5);
	/*glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);*/
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);

	setMaterialProperty();
	setSpotLightProperty();
	setPointLightProperty();
	glPushMatrix();

	if (option == 1)
	{
		glTranslated(finalTranslation[0], finalTranslation[1], finalTranslation[2]);
		glRotated(xAngle, rotationAxis[0], rotationAxis[1], rotationAxis[2]);
	}
	if (option == 2)
	{
		glTranslated(tx,ty,tz);
	}
	//else
		//cout << "invalid option" << endl;

	if (option!=2)
	{
		glTranslated(initialPosition[0], initialPosition[1], initialPosition[2]);
	}

	displayOctahedron();
	//glDisable(GL_LIGHT1);
	//glDisable(GL_LIGHT2);

	glPopMatrix();
	Sleep(50);
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
	case 'l': glEnable(GL_LIGHTING); 		
		break;
	case 'n': glDisable(GL_LIGHTING); break;
	case 'f': glEnable(GL_COLOR_MATERIAL); break;
	case 'c': glDisable(GL_COLOR_MATERIAL); break;
	case 'A':
		spotLight_position[0] -= 10; 
		glutPostRedisplay();
		break;
	case 'D':
		spotLight_position[0] += 10; 
		break;
	case 'W':
		spotLight_position[1] += 10; 
		break;
	case 'S':
		spotLight_position[1] -= 10; 
		break;
	case 'Q':
		spotLight_position[2] -= 10; 
		break;
	case 'E':
		spotLight_position[2] += 10; 
		break;
	case 'a':
		pointLight_position[0] -= 10;
		break;
	case 'd':
		pointLight_position[0] += 10;
		break;
	case 'w':
		pointLight_position[1] += 10;
		break;
	case 's':
		pointLight_position[1] -= 10;
		break;
	case 'q':
		pointLight_position[2] -= 10;
		break;
	case 'e':
		pointLight_position[2] += 10;
		break;
		/*case 'i':
		case 'I':
		init();
		glutPostRedisplay();
		break;*/
	case 27:
		exit(0);
	default: break;
	}
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
		xAngle += xStep;
		if (xAngle > 360)
		{
			xAngle = 0.0;
		}
	}
	else if (option == 2)
	{
		tx += rotationAxis[0]*0.1;
		ty += rotationAxis[1]*0.1;
		tz += rotationAxis[2]*0.1;

		if (abs(tx) > abs(endPoint1[0]) && abs(ty) > abs(endPoint1[1]) && abs(tz) > abs(endPoint1[2]))
		{
			tx = endPoint2[0];
			ty = endPoint2[1];
			tz = endPoint2[2];
		}
	}
	else if (option == 4)
	{
		tx += txStep;

		if (tx > xmax1)
		{
			tx = -xmax1;
		}
	}
	else if (option == 5)
	{
		ty += tyStep;

		if (ty > ymax1)
		{
			ty = -ymax1;
		}
	}

	else if (option == 6)
	{
		tz += tzStep;

		if (tz > zmax1)
		{
			tz = zmax1;
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
int main(int argc, char** argv)
{
	GLint windowID;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	glutInitWindowPosition(50, 50);
	glutInitWindowSize(1000, 1000);

	windowID = glutCreateWindow("Part 2 - 3D Object");

	init();
	CalculateFinalTranslationVector();
	GLint subMenu;
	subMenu = glutCreateMenu(drawOptions);
	glutAddMenuEntry("Verteces", 1);
	glutAddMenuEntry("Edges", 2);
	glutAddMenuEntry("Everything", 3);
	glutCreateMenu(animationSelection);
	glutAddMenuEntry("Rotate", 1);
	glutAddMenuEntry("Translation", 2);
	glutAddSubMenu("Box Style", subMenu);
	glutAddMenuEntry("Exit", 7);
	glutAttachMenu(GLUT_LEFT_BUTTON);

	glutDisplayFunc(displayFunction);
	glutSpecialFunc(keyFunc);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshapeFcn);

	glutMainLoop();

	return 0;
}