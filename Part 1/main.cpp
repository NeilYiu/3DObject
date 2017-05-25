#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <GL/glut.h>
#include <vector>
using namespace std;

GLint displayOption = 2;
GLdouble height = 60;
GLdouble baseWidth = 60;
GLdouble yOffset = 0;

GLdouble initialTriangleVertices[3][2] = { { 0,0 - yOffset },{ baseWidth*0.5,height - yOffset },{ -baseWidth*0.5,height - yOffset } };
int currentTriangleRoundCount = 0;
int totalTriangleCountOfOneRow = 6;//Can't be changed for the moment
int totalTriangleRowCount = 4;
GLdouble scaleFactor = 1.2;

GLdouble txStep = 7, tyStep = 7, tzStep = 0.1;  //tzStep is for the rotation animation
GLdouble sxStep = 0.1, syStep = 0.1, szStep = 0.1;

vector<GLdouble> txs, tys, tzs;
vector<GLdouble> sxs, sys, szs;
vector<GLdouble> rxs, rys, rzs;

vector<bool> completedTranslates, completedScales, completedRotations;

vector<GLdouble> totalTranslations;
vector<GLdouble> totalScales;
vector<GLdouble> totalRotations;

bool rowStartToDrawFlag;

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	glOrtho(-800, 800, -800, 800, 1.0, -1.0);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
}

void drawTriangle()
{
	glColor3f(0.0, 1.0, 1.0);
	glBegin(GL_TRIANGLES);
	glVertex2d(initialTriangleVertices[0][0], initialTriangleVertices[0][1]);
	glVertex2d(initialTriangleVertices[1][0], initialTriangleVertices[1][1]);
	glVertex2d(initialTriangleVertices[2][0], initialTriangleVertices[2][1]);
	glEnd();
	//glFlush();
}

GLdouble calculateTranslation(int index)
{
	if (index == 0)
	{
		return 0;
	}
	GLdouble multiplier = 1;
	for (int k = index; k >= 2; k--)
	{
		multiplier = multiplier + pow(scaleFactor, k - 1);
	}
	return multiplier * height;
}

void calculateTransformations()
{
	GLdouble finalRotation = 0;
	if (currentTriangleRoundCount == 0)
	{
		finalRotation = 0;
	}
	if (currentTriangleRoundCount == 1)
	{
		finalRotation = M_PI_2;
	}
	if (currentTriangleRoundCount == 2)
	{
		finalRotation = M_PI;
	}
	if (currentTriangleRoundCount == 3)
	{
		finalRotation = 3 * M_PI_2;
		//finalRotation = -M_PI_2;
	}
	totalTranslations.clear();
	totalScales.clear();
	totalRotations.clear();
	for (int i = 0; i < totalTriangleCountOfOneRow; i++)
	{
		totalTranslations.push_back(calculateTranslation(i));
		totalScales.push_back(pow(scaleFactor, i));
		totalRotations.push_back(finalRotation);
	}
}

void initializeUnitTransformations()
{
	txs.clear();
	tys.clear();
	tzs.clear();
	sxs.clear();
	sys.clear();
	szs.clear();
	rxs.clear();
	rys.clear();
	rzs.clear();
	completedTranslates.clear();
	completedScales.clear();
	completedRotations.clear();
	for (int i = 0; i<totalTriangleCountOfOneRow; i++)
	{
		txs.push_back(0);
		tys.push_back(0);
		tzs.push_back(0);
		sxs.push_back(1);
		sys.push_back(1);
		szs.push_back(1);
		rxs.push_back(0);
		rys.push_back(0);
		rzs.push_back(0);
		completedTranslates.push_back(false);
		completedScales.push_back(false);
		completedRotations.push_back(false);
	}
}

void displayFunction()
{
	glClear(GL_COLOR_BUFFER_BIT);
	//glPushMatrix();

	if (displayOption == 2)
	{
		drawTriangle();

		glPushMatrix();
		const GLdouble rotationMatrix[16] = { cos(M_PI),sin(M_PI),0,0,-sin(M_PI),cos(M_PI),0,0,0,0,1.0,0,0,0,0,1.0 };
		glMultMatrixd(rotationMatrix);
		drawTriangle();
		glPopMatrix();
		glPushMatrix();
		GLdouble rotationMatrix2[16] = { cos(M_PI_2),sin(M_PI_2),0,0,-sin(M_PI_2),cos(M_PI_2),0,0,0,0,1.0,0,0,0,0,1.0 };
		glMultMatrixd(rotationMatrix2);
		drawTriangle();
		glPopMatrix();
		glPushMatrix();
		GLdouble rotationMatrix3[16] = { cos(-M_PI_2),sin(-M_PI_2),0,0,-sin(-M_PI_2),cos(-M_PI_2),0,0,0,0,1.0,0,0,0,0,1.0 };
		glMultMatrixd(rotationMatrix3);
		drawTriangle();
		glPopMatrix();
		for (int i = 0; i < totalTriangleCountOfOneRow; i++)
		{
			//draw the upper half 
			glPushMatrix();
			GLdouble yTranslation = calculateTranslation(i);
			GLdouble translateMatrix[16] = { 1,0,0,0,0,1,0,0,0,0,1,0,0,yTranslation,0,1 };
			GLdouble scaleMatrix[16] = { pow(scaleFactor,i),0,0,0,0,pow(scaleFactor,i),0,0,0,0,pow(scaleFactor,i),0,0,0,0,1.0 };
			glMultMatrixd(translateMatrix);
			glMultMatrixd(scaleMatrix);
			drawTriangle();
			glPopMatrix();

			glPushMatrix();
			GLdouble rotationMatrix[16] = { cos(M_PI),sin(M_PI),0,0,-sin(M_PI),cos(M_PI),0,0,0,0,1.0,0,0,0,0,1.0 };
			glMultMatrixd(rotationMatrix);
			glMultMatrixd(translateMatrix);
			glMultMatrixd(scaleMatrix);
			drawTriangle();
			glPopMatrix();

			glPushMatrix();
			GLdouble rotationMatrix2[16] = { cos(M_PI_2),sin(M_PI_2),0,0,-sin(M_PI_2),cos(M_PI_2),0,0,0,0,1.0,0,0,0,0,1.0 };
			glMultMatrixd(rotationMatrix2);
			glMultMatrixd(translateMatrix);
			glMultMatrixd(scaleMatrix);
			drawTriangle();
			glPopMatrix();

			glPushMatrix();
			GLdouble rotationMatrix3[16] = { cos(-M_PI_2),sin(-M_PI_2),0,0,-sin(-M_PI_2),cos(-M_PI_2),0,0,0,0,1.0,0,0,0,0,1.0 };
			glMultMatrixd(rotationMatrix3);
			glMultMatrixd(translateMatrix);
			glMultMatrixd(scaleMatrix);
			drawTriangle();
			glPopMatrix();
		}
	}

	if (displayOption == 1)
	{
		drawTriangle();

		if (currentTriangleRoundCount == 0)
		{
			for (int i = 1; i<totalTriangleCountOfOneRow; i++)
			{
				glPushMatrix();
				const GLdouble translateMatrix[16] = { 1,0,0,0,0,1,0,0,0,0,1,0,0,tys[i],0,1 };
				GLdouble scaleMatrix[16] = { sxs[i],0,0,0,0,sys[i],0,0,0,0,szs[i],0,0,0,0,1.0 };
				GLdouble rotationMatrix[16] = { cos(rzs[i]),sin(-rzs[i]),0,0,-sin(-rzs[i]),cos(-rzs[i]),0,0,0,0,1.0,0,0,0,0,1.0 };

				glMultMatrixd(translateMatrix);
				glMultMatrixd(scaleMatrix);
				drawTriangle();
				glPopMatrix();
			}
		}
		if (currentTriangleRoundCount == 1)
		{
			for (int i = 1; i < totalTriangleCountOfOneRow; i++)
			{
				//draw the upper half 
				glPushMatrix();
				GLdouble yTranslation = calculateTranslation(i);
				GLdouble translateMatrix[16] = { 1,0,0,0,0,1,0,0,0,0,1,0,0,yTranslation,0,1 };
				GLdouble scaleMatrix[16] = { pow(scaleFactor,i),0,0,0,0,pow(scaleFactor,i),0,0,0,0,pow(scaleFactor,i),0,0,0,0,1.0 };
				glMultMatrixd(translateMatrix);
				glMultMatrixd(scaleMatrix);
				drawTriangle();
				glPopMatrix();
			}

			for (int i = 0; i<totalTriangleCountOfOneRow; i++)
			{
				glPushMatrix();
				GLdouble translateMatrix[16] = { 1,0,0,0,0,1,0,0,0,0,1,0,txs[i],0,0,1 };
				GLdouble scaleMatrix[16] = { sxs[i],0,0,0,0,sys[i],0,0,0,0,szs[i],0,0,0,0,1.0 };
				GLdouble rotationMatrix[16] = { cos(rzs[i]),sin(-rzs[i]),0,0,-sin(-rzs[i]),cos(-rzs[i]),0,0,0,0,1.0,0,0,0,0,1.0 };
				glMultMatrixd(translateMatrix);
				glMultMatrixd(scaleMatrix);
				glMultMatrixd(rotationMatrix);
				drawTriangle();
				glPopMatrix();
			}
		}
		if (currentTriangleRoundCount == 2)
		{
			for (int i = 1; i < totalTriangleCountOfOneRow; i++)
			{
				//draw the upper half 
				glPushMatrix();
				GLdouble yTranslation = calculateTranslation(i);
				GLdouble translateMatrix[16] = { 1,0,0,0,0,1,0,0,0,0,1,0,0,yTranslation,0,1 };
				GLdouble scaleMatrix[16] = { pow(scaleFactor,i),0,0,0,0,pow(scaleFactor,i),0,0,0,0,pow(scaleFactor,i),0,0,0,0,1.0 };
				glMultMatrixd(translateMatrix);
				glMultMatrixd(scaleMatrix);
				drawTriangle();
				glPopMatrix();
			}
			glPushMatrix();
			GLdouble rotationMatrix2[16] = { cos(-M_PI_2),sin(-M_PI_2),0,0,-sin(-M_PI_2),cos(-M_PI_2),0,0,0,0,1.0,0,0,0,0,1.0 };
			glMultMatrixd(rotationMatrix2);
			drawTriangle();
			glPopMatrix();
			for (int i = 1; i < totalTriangleCountOfOneRow; i++)
			{
				//draw the right half 
				glPushMatrix();
				GLdouble yTranslation = calculateTranslation(i);
				GLdouble translateMatrix[16] = { 1,0,0,0,0,1,0,0,0,0,1,0,0,yTranslation,0,1 };
				GLdouble scaleMatrix[16] = { pow(scaleFactor,i),0,0,0,0,pow(scaleFactor,i),0,0,0,0,pow(scaleFactor,i),0,0,0,0,1.0 };
				GLdouble rotationMatrix[16] = { cos(-M_PI_2),sin(-M_PI_2),0,0,-sin(-M_PI_2),cos(-M_PI_2),0,0,0,0,1.0,0,0,0,0,1.0 };
				glMultMatrixd(rotationMatrix);
				glMultMatrixd(translateMatrix);
				glMultMatrixd(scaleMatrix);
				drawTriangle();
				glPopMatrix();
			}
			for (int i = 0; i<totalTriangleCountOfOneRow; i++)
			{
				glPushMatrix();
				GLdouble translateMatrix[16] = { 1,0,0,0,0,1,0,0,0,0,1,0,0,-tys[i],0,1 };
				GLdouble scaleMatrix[16] = { sxs[i],0,0,0,0,sys[i],0,0,0,0,szs[i],0,0,0,0,1.0 };
				GLdouble rotationMatrix[16] = { cos(rzs[i]),sin(-rzs[i]),0,0,-sin(-rzs[i]),cos(-rzs[i]),0,0,0,0,1.0,0,0,0,0,1.0 };
				glMultMatrixd(translateMatrix);
				glMultMatrixd(scaleMatrix);
				glMultMatrixd(rotationMatrix);
				drawTriangle();
				glPopMatrix();
			}
		}
		if (currentTriangleRoundCount == 3)
		{
			for (int i = 1; i < totalTriangleCountOfOneRow; i++)
			{
				//draw the upper half 
				glPushMatrix();
				GLdouble yTranslation = calculateTranslation(i);
				GLdouble translateMatrix[16] = { 1,0,0,0,0,1,0,0,0,0,1,0,0,yTranslation,0,1 };
				GLdouble scaleMatrix[16] = { pow(scaleFactor,i),0,0,0,0,pow(scaleFactor,i),0,0,0,0,pow(scaleFactor,i),0,0,0,0,1.0 };
				glMultMatrixd(translateMatrix);
				glMultMatrixd(scaleMatrix);
				drawTriangle();
				glPopMatrix();
			}
			glPushMatrix();
			GLdouble rotationMatrix2[16] = { cos(-M_PI_2),sin(-M_PI_2),0,0,-sin(-M_PI_2),cos(-M_PI_2),0,0,0,0,1.0,0,0,0,0,1.0 };
			glMultMatrixd(rotationMatrix2);
			drawTriangle();
			glPopMatrix();
			for (int i = 1; i < totalTriangleCountOfOneRow; i++)
			{
				//draw the right half 
				glPushMatrix();
				GLdouble yTranslation = calculateTranslation(i);
				GLdouble translateMatrix[16] = { 1,0,0,0,0,1,0,0,0,0,1,0,0,yTranslation,0,1 };
				GLdouble scaleMatrix[16] = { pow(scaleFactor,i),0,0,0,0,pow(scaleFactor,i),0,0,0,0,pow(scaleFactor,i),0,0,0,0,1.0 };
				GLdouble rotationMatrix[16] = { cos(-M_PI_2),sin(-M_PI_2),0,0,-sin(-M_PI_2),cos(-M_PI_2),0,0,0,0,1.0,0,0,0,0,1.0 };
				glMultMatrixd(rotationMatrix);
				glMultMatrixd(translateMatrix);
				glMultMatrixd(scaleMatrix);
				drawTriangle();
				glPopMatrix();
			}
			glPushMatrix();
			const GLdouble rotationMatrix[16] = { cos(M_PI),sin(M_PI),0,0,-sin(M_PI),cos(M_PI),0,0,0,0,1.0,0,0,0,0,1.0 };
			glMultMatrixd(rotationMatrix);
			drawTriangle();
			glPopMatrix();
			for (int i = 1; i < totalTriangleCountOfOneRow; i++)
			{
				//draw the bottom half 
				glPushMatrix();
				GLdouble yTranslation = calculateTranslation(i);
				GLdouble translateMatrix[16] = { 1,0,0,0,0,1,0,0,0,0,1,0,0,yTranslation,0,1 };
				GLdouble scaleMatrix[16] = { pow(scaleFactor,i),0,0,0,0,pow(scaleFactor,i),0,0,0,0,pow(scaleFactor,i),0,0,0,0,1.0 };
				GLdouble rotationMatrix[16] = { cos(M_PI),sin(M_PI),0,0,-sin(M_PI),cos(M_PI),0,0,0,0,1.0,0,0,0,0,1.0 };
				glMultMatrixd(rotationMatrix);
				glMultMatrixd(translateMatrix);
				glMultMatrixd(scaleMatrix);
				drawTriangle();
				glPopMatrix();
			}
			for (int i = 0; i<totalTriangleCountOfOneRow; i++)
			{
				glPushMatrix();
				GLdouble translateMatrix[16] = { 1,0,0,0,0,1,0,0,0,0,1,0,-txs[i],0,0,1 };
				GLdouble scaleMatrix[16] = { sxs[i],0,0,0,0,sys[i],0,0,0,0,szs[i],0,0,0,0,1.0 };
				GLdouble rotationMatrix[16] = { cos(rzs[i]),sin(-rzs[i]),0,0,-sin(-rzs[i]),cos(-rzs[i]),0,0,0,0,1.0,0,0,0,0,1.0 };
				glMultMatrixd(translateMatrix);
				glMultMatrixd(scaleMatrix);
				glMultMatrixd(rotationMatrix);
				drawTriangle();
				glPopMatrix();
			}
		}
		if (currentTriangleRoundCount == 4)
		{
			glPushMatrix();
			const GLdouble rotationMatrix[16] = { cos(M_PI),sin(M_PI),0,0,-sin(M_PI),cos(M_PI),0,0,0,0,1.0,0,0,0,0,1.0 };
			glMultMatrixd(rotationMatrix);
			drawTriangle();
			glPopMatrix();
			glPushMatrix();
			GLdouble rotationMatrix2[16] = { cos(M_PI_2),sin(M_PI_2),0,0,-sin(M_PI_2),cos(M_PI_2),0,0,0,0,1.0,0,0,0,0,1.0 };
			glMultMatrixd(rotationMatrix2);
			drawTriangle();
			glPopMatrix();
			glPushMatrix();
			GLdouble rotationMatrix3[16] = { cos(-M_PI_2),sin(-M_PI_2),0,0,-sin(-M_PI_2),cos(-M_PI_2),0,0,0,0,1.0,0,0,0,0,1.0 };
			glMultMatrixd(rotationMatrix3);
			drawTriangle();
			glPopMatrix();
			for (int i = 0; i < totalTriangleCountOfOneRow; i++)
			{
				//draw the upper half 
				glPushMatrix();
				GLdouble yTranslation = calculateTranslation(i);
				GLdouble translateMatrix[16] = { 1,0,0,0,0,1,0,0,0,0,1,0,0,yTranslation,0,1 };
				GLdouble scaleMatrix[16] = { pow(scaleFactor,i),0,0,0,0,pow(scaleFactor,i),0,0,0,0,pow(scaleFactor,i),0,0,0,0,1.0 };
				glMultMatrixd(translateMatrix);
				glMultMatrixd(scaleMatrix);
				drawTriangle();
				glPopMatrix();

				glPushMatrix();
				GLdouble rotationMatrix[16] = { cos(M_PI),sin(M_PI),0,0,-sin(M_PI),cos(M_PI),0,0,0,0,1.0,0,0,0,0,1.0 };
				glMultMatrixd(rotationMatrix);
				glMultMatrixd(translateMatrix);
				glMultMatrixd(scaleMatrix);
				drawTriangle();
				glPopMatrix();

				glPushMatrix();
				GLdouble rotationMatrix2[16] = { cos(M_PI_2),sin(M_PI_2),0,0,-sin(M_PI_2),cos(M_PI_2),0,0,0,0,1.0,0,0,0,0,1.0 };
				glMultMatrixd(rotationMatrix2);
				glMultMatrixd(translateMatrix);
				glMultMatrixd(scaleMatrix);
				drawTriangle();
				glPopMatrix();

				glPushMatrix();
				GLdouble rotationMatrix3[16] = { cos(-M_PI_2),sin(-M_PI_2),0,0,-sin(-M_PI_2),cos(-M_PI_2),0,0,0,0,1.0,0,0,0,0,1.0 };
				glMultMatrixd(rotationMatrix3);
				glMultMatrixd(translateMatrix);
				glMultMatrixd(scaleMatrix);
				drawTriangle();
				glPopMatrix();
			}
		}
	}

	if (displayOption == 1)
	{
		Sleep(50);
	}

	glutSwapBuffers();

	//glFlush();
};

void Selection(GLint animationOption)
{
	switch (animationOption) {
	case 1:
		displayOption = 1;
		break;
	case 2:
		displayOption = 2;
		break;
	default:
		cout << "invalid option" << endl;
	}
	glutPostRedisplay();
}

void idle()
{
	if (displayOption == 1)
	{
		if (currentTriangleRoundCount == 0)
		{
			for (int i = 0; i < totalTriangleCountOfOneRow; i++)
			{
				if (!completedRotations[i])
				{
					if (rzs[i] >= totalRotations[i])
					{
						rzs[i] = totalRotations[i];
						completedRotations[i] = true;
					}
					else
					{
						rzs[i] += tzStep;
					}
				}
				else if (!completedTranslates[i])
				{
					if (tys[i] >= totalTranslations[i])
					{
						tys[i] = totalTranslations[i];
						completedTranslates[i] = true;
					}
					else
					{
						tys[i] += tyStep;
					}
				}
				else if (!completedScales[i])
				{

					if (sxs[i] >= totalScales[i] && sys[i] >= totalScales[i] && szs[i] >= totalScales[i])
					{
						completedScales[i] = true;
						if (i == totalTriangleCountOfOneRow - 1)
						{
							rowStartToDrawFlag = true;
							currentTriangleRoundCount++;
						}
					}
					else
					{
						sxs[i] += sxStep;
						sys[i] += syStep;
						szs[i] += szStep;
					}
				}
			}
		}
		if (currentTriangleRoundCount == 1)
		{
			if (rowStartToDrawFlag)
			{
				initializeUnitTransformations();
				calculateTransformations();
				rowStartToDrawFlag = false;
			}

			for (int i = 0; i < totalTriangleCountOfOneRow; i++)
			{
				if (!completedRotations[i])
				{
					if (rzs[i] >= totalRotations[i])
					{
						rzs[i] = totalRotations[i];

						completedRotations[i] = true;
					}
					else
					{
						rzs[i] += tzStep;
					}
				}
				else if (!completedTranslates[i])
				{
					if (txs[i] >= totalTranslations[i])
					{
						txs[i] = totalTranslations[i];
						completedTranslates[i] = true;
					}
					else
					{
						txs[i] += txStep;
					}
				}
				else if (!completedScales[i])
				{
					if (sxs[i] >= totalScales[i] && sys[i] >= totalScales[i] && szs[i] >= totalScales[i])
					{
						completedScales[i] = true;
						if (i == totalTriangleCountOfOneRow - 1)
						{
							rowStartToDrawFlag = true;
							currentTriangleRoundCount++;
						}
					}
					else
					{
						sxs[i] += sxStep;
						sys[i] += syStep;
						szs[i] += szStep;
					}
				}
			}
		}
		if (currentTriangleRoundCount == 2)
		{
			if (rowStartToDrawFlag)
			{
				initializeUnitTransformations();
				calculateTransformations();
				rowStartToDrawFlag = false;
			}
			for (int i = 0; i < totalTriangleCountOfOneRow; i++)
			{
				if (!completedRotations[i])
				{
					if (rzs[i] >= totalRotations[i])
					{
						rzs[i] = totalRotations[i];
						completedRotations[i] = true;
					}
					else
					{
						rzs[i] += tzStep;
					}
				}
				else if (!completedTranslates[i])
				{
					if (tys[i] >= totalTranslations[i])
					{
						tys[i] = totalTranslations[i];
						completedTranslates[i] = true;
					}
					else
					{
						tys[i] += tyStep;
					}
				}
				else if (!completedScales[i])
				{
					if (sxs[i] >= totalScales[i] && sys[i] >= totalScales[i] && szs[i] >= totalScales[i])
					{
						completedScales[i] = true;
						if (i == totalTriangleCountOfOneRow - 1)
						{
							rowStartToDrawFlag = true;
							currentTriangleRoundCount++;
						}
					}
					else
					{
						sxs[i] += sxStep;
						sys[i] += syStep;
						szs[i] += szStep;
					}
				}
			}
		}
		if (currentTriangleRoundCount == 3)
		{
			if (rowStartToDrawFlag)
			{
				initializeUnitTransformations();
				calculateTransformations();
				rowStartToDrawFlag = false;
			}
			for (int i = 0; i < totalTriangleCountOfOneRow; i++)
			{
				if (!completedRotations[i])
				{
					if (rzs[i] >= totalRotations[i])
					{
						rzs[i] = totalRotations[i];

						completedRotations[i] = true;
					}
					else
					{
						rzs[i] += tzStep;
					}
				}
				else if (!completedTranslates[i])
				{
					if (txs[i] >= totalTranslations[i])
					{
						txs[i] = totalTranslations[i];
						completedTranslates[i] = true;
					}
					else
					{
						txs[i] += txStep;
					}
				}
				else if (!completedScales[i])
				{
					if (sxs[i] >= totalScales[i] && sys[i] >= totalScales[i] && szs[i] >= totalScales[i])
					{
						completedScales[i] = true;
						if (i == totalTriangleCountOfOneRow - 1)
						{
							rowStartToDrawFlag = true;
							currentTriangleRoundCount++;
						}
					}
					else
					{
						sxs[i] += sxStep;
						sys[i] += syStep;
						szs[i] += szStep;
					}
				}
			}
		}
	}
	glutPostRedisplay();
}

void displayMode(GLint displayOptions)
{
	switch (displayOptions) {
	case 1:
		displayOption = 1;
		glutIdleFunc(idle);
		break;
	case 2:
		displayOption = 2;
		glutIdleFunc(nullptr);
		break;
	}
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	initializeUnitTransformations();
	calculateTransformations();
	GLint subMenu;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(550, 100);

	glutInitWindowSize(800, 800);
	glutCreateWindow("Part 1 - 2D shapes");

	init();

	subMenu = glutCreateMenu(displayMode);
	glutAddMenuEntry("Animation", 1);
	glutAddMenuEntry("Result", 2);

	glutCreateMenu(Selection);

	glutAddSubMenu("Display Mode", subMenu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutDisplayFunc(displayFunction);

	glutMainLoop();

	return 0;
}