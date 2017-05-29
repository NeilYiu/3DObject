#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <GL/glut.h>
#include <vector>
using namespace std;

GLint numVert = 5;
GLint xmin = -600, xmax = 600, ymin = -600, ymax = 600;
GLdouble radius = 500;
GLdouble curentPentagonCentre[2];
GLdouble points[5][2];
GLdouble resetz = -200;
GLdouble counter;
GLdouble angleAdjustment = 49.95;
GLdouble scalingFactor = 0.8;

int option;
int singleFrame;

int objectOption = 0;
GLint displayOption = 2;
GLdouble height = 60;
GLdouble baseWidth = 60;
GLdouble yOffset = 0;
GLint replay = 0;

GLdouble xmin1 = -800, xmax1 = 800, ymin1 = -800, ymax1 = 800, znear1 = -500, zfar1 = 500;

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
void idleNeil()
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
}
void animationNeil()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

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
			for (int i = 1; i < totalTriangleCountOfOneRow; i++)
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

			for (int i = 0; i < totalTriangleCountOfOneRow; i++)
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
			for (int i = 0; i < totalTriangleCountOfOneRow; i++)
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
			for (int i = 0; i < totalTriangleCountOfOneRow; i++)
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
}

//Marks **********************************************************************************

void getPentagonPoints(GLdouble centre[], GLint NumVert)
{
	GLdouble angle = 0.0;
	GLint k = 0;
	for (k = 0; k < NumVert; k++) {
		angle = 2.0 * M_PI * k / NumVert;

		//shift the angle the lines are drawn along so that the pentagon sits with its base on the x axis
		points[k][0] = centre[0] + radius * cos((angle - angleAdjustment));
		points[k][1] = centre[1] + radius * sin((angle - angleAdjustment));
	}
}

//Pentagon Drawing function, similar to the Polygon drawing function give by Helen Lu in week 3
void drawOriginalPentagon(GLdouble verts[5][2]) {
	glBegin(GL_POLYGON);
	for (int i = 0; i < 5; i++)
	{
		GLdouble x = verts[i][0];
		GLdouble y = verts[i][1];
		glVertex2d(x, y);
	}
	glEnd();
	glFlush();


}


void displayFrozenFrameFunction()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLoadIdentity();
	glPushMatrix();
	GLdouble movementDistance = 0;
	curentPentagonCentre[0] = 0;
	curentPentagonCentre[1] = 0;


	for (int i = 0; i < 20; i++)
	{
		movementDistance += 4;

		//called backwards, cause that's the way matrixes work
		glTranslated(curentPentagonCentre[0] + movementDistance, curentPentagonCentre[1] + movementDistance, 0);
		glTranslated(curentPentagonCentre[0], curentPentagonCentre[1], 0);
		glScaled(scalingFactor, scalingFactor, 0);
		glTranslated(-curentPentagonCentre[0], -curentPentagonCentre[1], 0);
		curentPentagonCentre[0] += movementDistance;
		curentPentagonCentre[1] += movementDistance;
		glColor3f(0.0, 0.0, 1.0);



		glLineWidth(3);
		drawOriginalPentagon(points);

	}

	glPopMatrix();
	glutSwapBuffers();

	glFlush();
}


void displayAnimationFunction() {


	//glClear(GL_COLOR_BUFFER_BIT); 
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLoadIdentity();
	glPushMatrix();
	GLdouble movementDistance = 0;

	curentPentagonCentre[0] = 0;
	curentPentagonCentre[1] = 0;
	for (int i = 0; i < counter; i++)
	{
		movementDistance += 4;

		//called backwards, cause that's the way matrixes work
		glTranslated(curentPentagonCentre[0] + movementDistance, curentPentagonCentre[1] + movementDistance, 0);
		glTranslated(curentPentagonCentre[0], curentPentagonCentre[1], 0);
		glScaled(scalingFactor, scalingFactor, 0);
		glTranslated(-curentPentagonCentre[0], -curentPentagonCentre[1], 0);
		curentPentagonCentre[0] += movementDistance;
		curentPentagonCentre[1] += movementDistance;
		glColor3f(0.0, 0.0, 1.0);



		glLineWidth(3);
		drawOriginalPentagon(points);
	}
	glPopMatrix();

	//Sleep(200);
	//glutSwapBuffers();

	//glFlush();
}
//Kevin's knot******************************************************************************
GLdouble tyTotalKevin = 0;
GLdouble tyTotal1Kevin = 0;
GLdouble tyStepKevin = 20;
GLdouble faceData1[4][2] = { { 150, -300 },{ 300, -450 } ,{ 150, -600 },{ 0, -450 } };
GLdouble faceData2[4][2] = { { -150 ,300 },{ -300, 450 } ,{ -150 , 600 },{ 0 , 450 } };
//Display Left Quad
void displayQuad1()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(1, 0, 0);
	glBegin(GL_QUADS);
	for (GLint i = 0; i < 4; i++)
	{
		glVertex2dv(faceData2[i]);

	}
	glEnd();
}

//Display Right Quad
void displayQuad()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(1, 0, 0);
	glBegin(GL_QUADS);
	for (GLint i = 0; i < 4; i++)
	{
		glVertex2dv(faceData1[i]);

	}
	glEnd();
}

//Translate Quad to go up
void translateQuad(void)
{
	glPushMatrix();

	glTranslated(0, tyTotalKevin, 0);
	displayQuad();

	glPopMatrix();
}

//Translate Quad to go down
void translateQuad1(void)
{
	glPushMatrix();

	glTranslated(0, tyTotal1Kevin, 0);
	displayQuad1();

	glPopMatrix();
}

//Counter for replaying the display, also to refresh screen when done(kinda)
void remakeSquare()
{
	if (replay < 100) {
		replay++;
		tyTotalKevin = 0;
		tyTotal1Kevin = 0;
	}
	else {
		replay = 0;
	}
}

void idleKevin()
{
	if (displayOption == 1)
	{
		for (GLint i = 0; i <= replay; i++) {
			translateQuad();
			tyTotalKevin += tyStepKevin;
			if (tyTotalKevin > ymax1 * 3) {
				tyTotalKevin = -ymax1 * 2;
			}
		}

		for (GLint i = 0; i <= replay; i++) {
			tyTotal1Kevin -= tyStepKevin;
			translateQuad1();

			if (tyTotal1Kevin < ymin1 * 3) {
				tyTotal1Kevin = ymax1 * 2;
			}
		}
	}
	if (displayOption == 1)
	{
		Sleep(50);
	}
}
void animationKevin()
{

	glLineWidth(5);
	//animation
	if (displayOption == 1) {

		for (GLint i = 0; i <= replay; i++) {
			translateQuad();
			tyTotalKevin += tyStepKevin;
			if (tyTotalKevin > ymax1 * 3) {
				tyTotalKevin = -ymax1 * 2;
			}
		}

		for (GLint i = 0; i <= replay; i++) {
			tyTotal1Kevin -= tyStepKevin;
			translateQuad1();

			if (tyTotal1Kevin < ymin1 * 3) {
				tyTotal1Kevin = ymax1 * 2;
			}
		}



	}

	//final image
	if (displayOption == 2) {
		for (GLint i = 0; i <= 100; i++) {
			translateQuad();
			tyTotalKevin += tyStepKevin;
			if (tyTotalKevin > ymax1 * 3) {
				tyTotalKevin = -ymax1 * 2;
			}
		}

		for (GLint i = 0; i <= 100; i++) {
			translateQuad1();
			tyTotal1Kevin -= tyStepKevin;
			if (tyTotal1Kevin < ymin1 * 3) {
				tyTotal1Kevin = ymax1 * 2;
			}
		}
	}

	remakeSquare();
}
//Kevin's knot******************************************************************************
//GLint timeDelay = 5000;
//
//void delay(GLint value) {
//	glutPostRedisplay();
//	glutTimerFunc(timeDelay, delay, 0);
//}

void displayFunction()
{
	glClear(GL_COLOR_BUFFER_BIT);
	if (objectOption == 0) {
		animationNeil();
	}

	if (objectOption == 1)
	{

	}
	if (objectOption == 2)
	{
		glLineWidth(5);
		animationKevin();
	}
	glutSwapBuffers();
};

void idle()
{	
	if (objectOption == 0)
	{
		idleNeil();
	}

	if (objectOption == 1)
	{
		
			if (counter < 20)
			{
				counter++;
			}
			else
				counter = 0;
			glutPostRedisplay();

	}
	if (objectOption==2)
	{
		idleKevin();
	}
	glutPostRedisplay();
}
void Selection(GLint animationOption)
{
	switch (animationOption) {
	case 1:
		displayOption = 1;
		glutIdleFunc(idle);
		break;
	case 2:
		displayOption = 2;
		glutIdleFunc(nullptr);
		break;
	case 3:
		objectOption = 0;
		glutDisplayFunc(displayFunction);
		break;

	case 4:
		objectOption = 1;
		glutDisplayFunc(displayAnimationFunction); 
		break;
	case 5:
		objectOption = 2;
		glutDisplayFunc(displayFunction);
		break;

	default:
		cout << "invalid option" << endl;
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

	curentPentagonCentre[0] = resetz;
	curentPentagonCentre[1] = resetz;
	getPentagonPoints(curentPentagonCentre, numVert);

	//subMenu = glutCreateMenu(displayMode);
	//glutAddMenuEntry("Animation", 1);
	//glutAddMenuEntry("Result", 2);
	//glutAddMenuEntry("Neil's Cross", 3);
	//glutAddMenuEntry("Mark's Wormhole", 4);
	//glutAddMenuEntry("Kevin's Knot", 5);

	glutCreateMenu(Selection);
	glutAddMenuEntry("Animation", 1);
	glutAddMenuEntry("Result", 2);
	glutAddMenuEntry("Neil's Cross", 3);
	glutAddMenuEntry("Mark's Wormhole", 4);
	glutAddMenuEntry("Kevin's Knot", 5);

	//glutAddSubMenu("Display Mode", subMenu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutDisplayFunc(displayFunction);
//	delay(0);

	glutMainLoop();

	return 0;
}