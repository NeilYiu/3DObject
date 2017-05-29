#include <iostream>
#define _USE_MATH_DEFINES
#include "SOIL.h"
#include <GL/glut.h>
#include <vector>
#include <math.h>
using namespace std;


//General Globals 

bool shouldUseFlat = true;
int objectOption = 0;

GLfloat pointSize = 10;


GLint option = 0;
GLdouble endPoint1[3] = { 0,0,0 };
GLdouble endPoint2[3] = { 0,0,0 };
GLdouble initialPosition[3] = { 0,0,0 };
GLdouble rotationAxis[3] = { 0,0,0 };
GLdouble lineLengh = pow(endPoint1 - endPoint2, 2);

GLdouble rotationStep = 2;
GLdouble tx = endPoint2[0], ty = endPoint2[1], tz = endPoint2[2];

double stepPortion = 0.007;
GLdouble translationXStep = 0;
GLdouble translationYStep = 0;
GLdouble translationZStep = 0;

GLdouble rotationAngle = 0;
GLdouble xmin1 = -800, xmax1 = 800, ymin1 = -800, ymax1 = 800, zmin1 = -800, zmax1 = 800, znear1 = -800, zfar1 = 800;
GLdouble finalTranslation[3];
GLdouble scaleFactor = 1.0;
GLuint textureID;
GLint border = 0;
const int imageWidth = 10;
const int imageHeight = 10;



//Light Globals 

GLfloat spotLight_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat spotLight_diffuse[] = { 1, 1, 1, 1.0 };
GLfloat spotLight_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat spotLight_position[4] = { initialPosition[0], initialPosition[1] - 200, initialPosition[2] + 800, 1.0 };
GLfloat dirVector[] = { 0.0,0.0,-1.0 };

GLfloat pointLight_position[4] = { -150.0, 0.0, 0.0, 1.0 };
GLfloat pointLight_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat pointLight_diffuse[] = { 1, 1, 1, 1.0 };
GLfloat pointLight_specular[] = { 1.0, 1.0, 1.0, 1.0 };

GLfloat ambientCoef[] = { 0.8f, 0.0f, 0.0f };
GLfloat diffuseCoef[] = { 0.8f, 0.0f, 0.0f };
GLfloat specularCoef[] = { 1.0f, 1.0f, 1.0f };
GLfloat mat_shininess = 128.0f;




// Globals For Mark's Prisim

GLdouble prisimFaceNormal[7][3];
GLdouble prisimVertexNormal[7][3];
GLdouble prisimData[7][3];
GLint triFaceIndex[6][3] = { { 1, 0, 6 },{ 2, 1, 6 },{ 3, 2, 6 },{ 4, 3, 6 },{ 5, 4, 6 },{ 0, 5, 6 } }; //fix name
GLint baseFaceIndex[6] = { 0, 1, 2, 3, 4, 5 }; //fix name 

GLint hexigonBaseVerts = 6;
GLdouble radius = 100;
GLdouble prismHeight = 200;

GLdouble theta;


GLdouble initialPositionMark[3] = { 251.001, 310.254, -182.363 };
GLdouble POne[3] = { 328.825, -66.2508, 282.843 }; //fix name
GLdouble PTwo[3] = { -128.825, 266.251, -282.843 }; //fix name

// End Globals For Mark's Prisim 


GLdouble octahedronFaceNormal[8][3];
GLdouble octahedronVertexNormal[6][3];

bool shouldDisplayTexture1Octahedron = false;
bool shouldDisplayTexture2Octahedron = false;
bool shouldDisplayTexture3Octahedron = false;

GLdouble endPoint1Neil[3] = { 398.974 ,-117.217 ,153.073 };
GLdouble endPoint2Neil[3] = { -198.974 ,317.217 ,-153.073 };
GLdouble initialPositionNeil[3] = { 137.187,409.385,-99.6725 };

GLdouble steps[3] = {};


GLdouble width = 50;
GLdouble height = 100;
GLdouble octahedronVertex[6][3] = { { width,0,-width },{ -width,0,-width },{ -width,0,width },{ width,0,width },{ 0,height,0 },
{ 0,-height,0 } };
GLint octahedronFaceIndex[8][3] = { { 3,0,4 }/*up+x*/,
{ 0,1,4 }/*up-z*/,
{ 1,2,4 }/*up-x*/,
{ 2,3,4 }/*up+z*/,
{ 0,3,5 }/*down+x*/,
{ 1,0,5 }/*down-z*/,
{ 2,1,5 }/*down-x*/,
{ 3,2,5 }/*down+z*/
};
GLint octahedronVertexIndex[6][4] = { { 0,1,4,5 },{ 1,2,5,6 },{ 2,3,7,6 },{ 0,3,4,7 },{ 0,1,2,3 },{ 4,5,6,7 } };


GLubyte texArray1[imageHeight][imageWidth][4];


//Globals for Kevin's house

GLint faceIndexKevin[6][4] = { { 0, 1, 2, 3 },{ 6, 2, 1, 5 },{ 4, 7, 6, 5 },{ 7, 4, 0, 3 },{ 5, 1, 0, 4 },{ 7, 3, 2, 6 }};
GLint vertexIndexKevin[8][3] = { { 0, 3, 4 },{ 0, 1, 4 },{ 0, 1, 5 },{ 0, 3, 5 },{ 2, 3, 4 },{ 1, 2, 4 },{ 1, 2, 5 },{ 2, 3, 5 } };

GLint roofIndexKevin[4][3] = { {0, 4, 1} ,{ 1, 4, 2 } ,{ 2, 4, 3 } ,{ 3, 4, 0 }};
GLint roofFaceIndexKevin[4][3] = { { 0, 4, 1 } ,{ 1, 4, 2 } ,{ 2, 4, 3 } ,{ 3, 4, 0 } };

GLdouble cubeData[8][3];
GLdouble roofData[5][3];

GLdouble faceNormalCubeKevin[6][3];
GLdouble vertexNormalCubeKevin[8][3];

GLdouble faceNormalRoofKevin[4][3];
GLdouble vertexNormalRoofKevin[5][3];

GLint initPx = 43.0533;
GLint initPy = 336.356;
GLint initPz = -132.504;

GLdouble side = 150;

GLdouble endPoint1Kevin[3] = { 214.198, -251.465,  153.073 };
GLdouble endPoint2Kevin[3] = { -14.1978, 451.465, -153.073 };
GLdouble initialPositionKevin[3] = { initPx, initPy, initPz };


//Mark's Prism*************************************************************************************


void CalculatePrisimPoints(GLdouble * centerPoints)
{
	prisimData[6][0] = centerPoints[0];
	prisimData[6][1] = centerPoints[1] + prismHeight / 2;
	prisimData[6][2] = centerPoints[2];

	for (GLint i = 0; i < hexigonBaseVerts; i++)
	{
		theta = 2.0 * M_PI * i / hexigonBaseVerts;
		prisimData[i][0] = centerPoints[0] + radius * cos(theta);
		prisimData[i][1] = centerPoints[1] - prismHeight / 2;
		prisimData[i][2] = centerPoints[2] + radius * sin(theta);
	}
}

void calculatePrisimFaceNormal(GLdouble normal[7][3])
{
	GLdouble temp;
	GLdouble tempPt[3];
	GLint k, j;

	GLdouble vert1[3], vert2[3], vert3[3];

	for (k = 0; k < 6; k++)
	{
		//Use three temperory vectors to get the three vertices under consideration to simplify the expressions of normal vector components
		for (j = 0; j < 3; j++)
		{
			vert1[j] = prisimData[triFaceIndex[k][0]][j];
			vert2[j] = prisimData[triFaceIndex[k][1]][j];
			vert3[j] = prisimData[triFaceIndex[k][2]][j];
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

	//get three points from the hexigon base for calculating it's normal vector

	vert1[0] = prisimData[3][0];
	vert1[1] = prisimData[3][1];
	vert1[2] = prisimData[3][2];

	vert2[0] = prisimData[4][0];
	vert2[1] = prisimData[4][1];
	vert2[2] = prisimData[4][2];

	vert3[0] = prisimData[5][0];
	vert3[1] = prisimData[5][1];
	vert3[2] = prisimData[5][2];

	//The following three statements are the code version of formula in topic slide 12 (Module 2 part 2)
	tempPt[0] = (vert2[1] - vert1[1])*(vert3[2] - vert1[2]) - (vert2[2] - vert1[2])*(vert3[1] - vert1[1]);
	tempPt[1] = -(vert2[0] - vert1[0])*(vert3[2] - vert1[2]) + (vert2[2] - vert1[2])*(vert3[0] - vert1[0]);
	tempPt[2] = (vert2[0] - vert1[0])*(vert3[1] - vert1[1]) - (vert2[1] - vert1[1])*(vert3[0] - vert1[0]);

	//THis is to calculate the magnitude if the normal vector using the formula mag = sqrt(x^2+y^2+z^2)
	temp = sqrt((tempPt[0] * tempPt[0] + tempPt[1] * tempPt[1] + tempPt[2] * tempPt[2]));

	//The following three statements are to normalise the normal vector to make its magnitude be one unit 
	normal[6][0] = tempPt[0] / temp;
	normal[6][1] = tempPt[1] / temp;
	normal[6][2] = tempPt[2] / temp;


}


void CalculatePrisimVertexNormals(GLdouble vertexNormal[7][3]) {

	vertexNormal[0][0] = (prisimFaceNormal[5][0] + prisimFaceNormal[0][0] + prisimFaceNormal[6][0]) / 3;
	vertexNormal[0][1] = (prisimFaceNormal[5][1] + prisimFaceNormal[0][1] + prisimFaceNormal[6][1]) / 3;
	vertexNormal[0][2] = (prisimFaceNormal[5][2] + prisimFaceNormal[0][2] + prisimFaceNormal[6][2]) / 3;

	for (int i = 1; i < 6; i++)
	{
		vertexNormal[i][0] = (prisimFaceNormal[i - 1][0] + prisimFaceNormal[i][0] + prisimFaceNormal[6][0]) / 3;
		vertexNormal[i][1] = (prisimFaceNormal[i - 1][1] + prisimFaceNormal[i][1] + prisimFaceNormal[6][1]) / 3;
		vertexNormal[i][2] = (prisimFaceNormal[i - 1][2] + prisimFaceNormal[i][2] + prisimFaceNormal[6][2]) / 3;
	}

	vertexNormal[6][0] = 0;
	vertexNormal[6][1] = 0;
	vertexNormal[6][2] = 0;

	for (int i = 0; i < 6; i++)
	{
		vertexNormal[6][0] += prisimFaceNormal[i][0];
		vertexNormal[6][1] += prisimFaceNormal[i][1];
		vertexNormal[6][2] += prisimFaceNormal[i][2];
	}

	vertexNormal[6][0] = vertexNormal[6][0] / 6;
	vertexNormal[6][1] = vertexNormal[6][1] / 6;
	vertexNormal[6][2] = vertexNormal[6][2] / 6;



}

void DrawTriangleSides()
{
	glColor3f(0.0, 0.0, 1.0);
	glEnable(GL_NORMALIZE);
	for (int k = 0; k < 6; k++)
	{
		if (shouldUseFlat == true)
		{
			glNormal3dv(prisimFaceNormal[k]);
		}
		glBegin(GL_TRIANGLES);
		for (int i = 0; i < 3; i++)
		{

			if (shouldUseFlat == false)
			{
				glNormal3dv(prisimVertexNormal[triFaceIndex[i][k]]);;
			}
			glVertex3dv(prisimData[triFaceIndex[k][i]]);
		}


		glEnd();
	}
}

void DrawBase()
{
	glColor3f(0.0, 0.0, 1.0);
	glEnable(GL_NORMALIZE);
	glNormal3dv(prisimFaceNormal[7]);
	glBegin(GL_POLYGON);
	if (shouldUseFlat == true)
	{
		glNormal3dv(prisimFaceNormal[6]);
	}
	for (int i = 0; i < 6; i++)
	{


		if (shouldUseFlat == false)
		{
			glNormal3dv(prisimVertexNormal[i]);
		}

		glVertex3dv(prisimData[baseFaceIndex[i]]);
	}
	glEnd();

}





//End Mark's Prism*************************************************************************************

//Kevin's house************************************************************************************


void calculateCubePoints(GLdouble side)
{

	cubeData[0][0] = initPx - side;
	cubeData[0][1] = initPy - (side / 2);
	cubeData[0][2] = initPz - (side / 2);


	cubeData[1][0] = initPx + side;
	cubeData[1][1] = initPy - (side / 2);
	cubeData[1][2] = initPz - (side / 2);


	cubeData[2][0] = initPx + side;
	cubeData[2][1] = initPy - (side / 2);
	cubeData[2][2] = initPz + (side / 2);

	cubeData[3][0] = initPx - side;
	cubeData[3][1] = initPy - (side / 2);
	cubeData[3][2] = initPz + (side / 2);


	cubeData[4][0] = cubeData[0][0];
	cubeData[4][1] = cubeData[0][1] + side;
	cubeData[4][2] = cubeData[0][2];

	cubeData[5][0] = cubeData[1][0];
	cubeData[5][1] = cubeData[0][1] + side;
	cubeData[5][2] = cubeData[1][2];

	cubeData[6][0] = cubeData[2][0];
	cubeData[6][1] = cubeData[0][1] + side;
	cubeData[6][2] = cubeData[2][2];

	cubeData[7][0] = cubeData[3][0];
	cubeData[7][1] = cubeData[0][1] + side;
	cubeData[7][2] = cubeData[3][2];
}

void calculateRoofPoints(GLdouble side)
{
	//Corner 1
	roofData[0][0] = cubeData[4][0];
	roofData[0][1] = cubeData[4][1];
	roofData[0][2] = cubeData[4][2];

	//Corner 2
	roofData[1][0] = cubeData[5][0];
	roofData[1][1] = cubeData[5][1];
	roofData[1][2] = cubeData[5][2];

	//Corner 3
	roofData[2][0] = cubeData[6][0];
	roofData[2][1] = cubeData[6][1];
	roofData[2][2] = cubeData[6][2];

	//Corner 4
	roofData[3][0] = cubeData[7][0];
	roofData[3][1] = cubeData[7][1];
	roofData[3][2] = cubeData[7][2];

	//Tip of Triangle
	roofData[4][0] = initPx;
	roofData[4][1] = initPy + side;
	roofData[4][2] = initPz;

}

void calculateHouseFaceNormal(GLdouble normal[6][3])
{
	GLdouble temp;
	GLdouble tempPt[3];
	GLint k, j;

	GLdouble vert1[3], vert2[3], vert3[3];

	for (k = 0; k < 6; k++)
	{
		//Use three temperory vectors to get the three vertices under consideration to simplify the expressions of normal vector components
		for (j = 0; j < 3; j++)
		{
			vert1[j] = cubeData[faceIndexKevin[k][0]][j];
			vert2[j] = cubeData[faceIndexKevin[k][1]][j];
			vert3[j] = cubeData[faceIndexKevin[k][2]][j];
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

void calculateHouseVertexNormal(GLdouble vexNormal[8][3])
{
	GLint k, j;

	for (k = 0; k < 8; k++)
	{

		for (j = 0; j < 3; j++)
		{
			vexNormal[k][j] = (faceNormalCubeKevin[vertexIndexKevin[k][0]][j] + faceNormalCubeKevin[vertexIndexKevin[k][1]][j] + faceNormalCubeKevin[vertexIndexKevin[k][2]][j]) / 3.0;
		}
	}
}

void calculateRoofFaceNormal(GLdouble normal[4][3])
{
	GLdouble temp;
	GLdouble tempPt[3];
	GLint k, j;

	GLdouble vert1[3], vert2[3], vert3[3];

	for (k = 0; k < 4; k++)
	{
		//Use three temperory vectors to get the three vertices under consideration to simplify the expressions of normal vector components
		for (j = 0; j < 3; j++)
		{
			vert1[j] = roofData[roofFaceIndexKevin[k][0]][j];
			vert2[j] = roofData[roofFaceIndexKevin[k][4]][j];
			vert3[j] = roofData[roofFaceIndexKevin[k][1]][j];
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

void calculateRoofVertexNormal(GLdouble vexNormal[5][3])
{
	GLint k, j;

	for (k = 0; k < 5; k++)
	{

		for (j = 0; j < 3; j++)
		{
			vexNormal[k][j] = (faceNormalRoofKevin[roofIndexKevin[k][0]][j] + faceNormalRoofKevin[roofIndexKevin[k][1]][j] + faceNormalRoofKevin[roofIndexKevin[k][2]][j]) / 3.0;
		}
	}
}

void displayCubeWithVertexNormal(GLdouble vertData[8][3], GLint faceIndexData[6][4], GLdouble vertNormalData[8][3]) {
	GLint i, j;

	for (GLint i = 0; i < 6; i++)
	{
		glBegin(GL_QUADS);
		for (GLint j = 0; j < 4; j++)
		{
			glNormal3dv(vertNormalData[faceIndexData[i][j]]);
			glVertex3dv(vertData[faceIndexData[i][j]]);
		}

		glEnd();
		glFlush();
	}
}

void displayRoofWithVertexNormal(GLdouble vertData[5][3], GLint faceIndexData[4][3], GLdouble vertNormalData[5][3]) {
	GLint i, j;

	for (GLint i = 0; i < 5; i++)
	{
		glBegin(GL_TRIANGLES);
		for (GLint j = 0; j < 3; j++)
		{
			glNormal3dv(vertNormalData[faceIndexData[i][j]]);
			glVertex3dv(vertData[faceIndexData[i][j]]);
		}

		glEnd();
		glFlush();
	}
}

void quad(GLint face[4]) // arguments are indices of vertives of a quad in vertex array
{
	glEnable(GL_NORMALIZE);

	glBegin(GL_QUADS);
	for (GLint i = 0; i < 4; i++)
	{
		glVertex3dv(cubeData[face[i]]);
	}
	glEnd();
	glFlush();
}

void displayCube() {
	for (GLint i = 0; i < 6; i++)
		quad(faceIndexKevin[i]);
}

void drawRoof(GLint face[3])
{
	glEnable(GL_NORMALIZE);

	glBegin(GL_TRIANGLES);
		for (GLint i = 0; i < 3; i++)
		{
			glVertex3dv(roofData[face[i]]);	
		}
	glEnd();
	glFlush();
}

void displayRoof() {
	for (GLint i = 0; i < 4; i++)
		drawRoof(roofFaceIndexKevin[i]);
}

void drawHouse()
{
	if (shouldUseFlat == true) {
		displayCubeWithVertexNormal(cubeData, faceIndexKevin, vertexNormalCubeKevin);
		displayRoofWithVertexNormal(roofData, roofFaceIndexKevin, vertexNormalRoofKevin);
	}
	else if (shouldUseFlat == false)
	{
		displayCube();
		displayRoof();
	}


}


//End Kevin's house************************************************************************************


//Neil's Octahedron ***********************************************************************************

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
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

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
				glNormal3dv(octahedronVertexNormal[octahedronFaceIndex[i][j]]);
				glVertex3dv(octahedronVertex[octahedronFaceIndex[i][j]]);
			}
			if (j == 1)
			{
				glTexCoord2d(0.0, 1.0);
				glNormal3dv(octahedronVertexNormal[octahedronFaceIndex[i][j]]);
				glVertex3dv(octahedronVertex[octahedronFaceIndex[i][j]]);
			}
			if (j == 2)
			{
				glTexCoord2d(1.0, 1.0);
				glNormal3dv(octahedronVertexNormal[octahedronFaceIndex[i][j]]);
				glVertex3dv(octahedronVertex[octahedronFaceIndex[i][j]]);
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
				glNormal3dv(octahedronVertexNormal[octahedronFaceIndex[i][j]]);
				glVertex3dv(octahedronVertex[octahedronFaceIndex[i][j]]);
			}
			if (j == 1)
			{
				glTexCoord2d(0.0, 1.0);
				glNormal3dv(octahedronVertexNormal[octahedronFaceIndex[i][j]]);
				glVertex3dv(octahedronVertex[octahedronFaceIndex[i][j]]);
			}
			if (j == 2)
			{
				glTexCoord2d(1.0, 1.0);
				glNormal3dv(octahedronVertexNormal[octahedronFaceIndex[i][j]]);
				glVertex3dv(octahedronVertex[octahedronFaceIndex[i][j]]);
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

void displayOctahedron() {
	glEnable(GL_NORMALIZE);

	for (GLint i = 0; i < 8; i++)
	{
		if (shouldUseFlat == true)
		{
			glNormal3dv(octahedronFaceNormal[i]);
		}
		glBegin(GL_TRIANGLES);
		for (GLint j = 0; j < 3; j++)
		{
			if (shouldUseFlat == false)
			{
				glNormal3dv(octahedronVertexNormal[octahedronFaceIndex[i][j]]);
			}
			glVertex3dv(octahedronVertex[octahedronFaceIndex[i][j]]);
		}
		glEnd();
	}
}

void calculateOctahedronVertexNormal(GLdouble vexNormal[6][3])
{
	GLint k, j;

	for (k = 0; k < 6; k++)
	{

		for (j = 0; j < 3; j++)
		{
			vexNormal[k][j] = (octahedronFaceNormal[octahedronVertexIndex[k][0]][j] + octahedronFaceNormal[octahedronVertexIndex[k][1]][j] + octahedronVertexIndex[octahedronFaceIndex[k][2]][j]) / 3.0;
		}
	}
}

void calculateOctahedronFaceNormal(GLdouble normal[8][3])
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
			vert1[j] = octahedronVertex[octahedronFaceIndex[k][0]][j];
			vert2[j] = octahedronVertex[octahedronFaceIndex[k][1]][j];
			vert3[j] = octahedronVertex[octahedronFaceIndex[k][2]][j];
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

//End Neil's Octahedron ********************************************************************************************************

//setup 

void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glFrustum(xmin1, xmax1, ymin1, ymax1, znear1, zfar1);

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

void CalculateFinalTranslationVector()
{
	GLdouble denominator = pow(rotationAxis[0], 2) + pow(rotationAxis[1], 2) + pow(rotationAxis[1], 2);
	GLdouble numerator = -rotationAxis[0] * endPoint1[0] - rotationAxis[1] * endPoint1[1] - rotationAxis[2] * endPoint1[2];
	GLdouble t = numerator / denominator;

	finalTranslation[0] = rotationAxis[0] * t + endPoint1[0];
	finalTranslation[1] = rotationAxis[1] * t + endPoint1[1];
	finalTranslation[2] = rotationAxis[2] * t + endPoint1[2];
}

void CalculateTranslationSteps()
{
	translationXStep = rotationAxis[0] * stepPortion;
	translationYStep = rotationAxis[1] * stepPortion;
	translationZStep = rotationAxis[2] * stepPortion;

	tx = endPoint2[0], ty = endPoint2[1], tz = endPoint2[2];
}


//lighting

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
	glBegin(GL_POINTS);
	glVertex3f(spotLight_position[0], spotLight_position[1], spotLight_position[2]);
	glEnd();
	glFlush();
}


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



void setMaterialProperty()
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientCoef);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseCoef);

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularCoef);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
}

//Display function

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
		glTranslated(initialPosition[0] - finalTranslation[0], initialPosition[1] - finalTranslation[1],
				initialPosition[2] - finalTranslation[2]);
	}

	if (option == 2)
	{
		glTranslated(tx, ty, tz);
		glScaled(scaleFactor, scaleFactor, scaleFactor);

		cout << tx << " , " << ty << " ," << tz << "Final Translations" << endl;
	}

	if (option == 0)
	{
		glTranslated(initialPosition[0], initialPosition[1], initialPosition[2]);
	}

	if (shouldDisplayTexture1Octahedron&&objectOption==2)
	{
		drawTexture1Octahedron();
	}
	else if(shouldDisplayTexture2Octahedron&&objectOption == 2)
	{
		drawTexture2Octahedron();
	}
	else if(shouldDisplayTexture3Octahedron&&objectOption == 2)
	{
		drawTexture3Octahedron();
	}
	else
	{
		if (objectOption == 0)
		{
			
			DrawBase();
			DrawTriangleSides();
		}
		if (objectOption == 1)
		{
			glTranslated(-initPx, -initPy, -initPz);
			drawHouse();
		}
		if (objectOption == 2)
		{
			displayOctahedron();
		}
	}

	glPopMatrix();
	Sleep(20);
	glutSwapBuffers();

	glFlush();
}
//idle 

void idle()
{
	if (option == 1)
	{
		rotationAngle += rotationStep;

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

	glutPostRedisplay();
}



//reshape Function

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

//Controls 

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
		shouldUseFlat = true;
		break;
	case 's':
	case 'S':
		glShadeModel(GL_SMOOTH);
		shouldUseFlat = false;
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



void animationSelection(GLint animationOption)
{
	switch (animationOption) {
	case 1:
		option = 1;
		break;
	case 2:
		option = 2;
		break;
	case 3:
		for (int i = 0; i < 3; i++)
		{
			endPoint1[i] = POne[i];
			endPoint2[i] = PTwo[i];

			initialPosition[i] = initialPositionMark[i];
			rotationAxis[i] = endPoint1[i] - endPoint2[i];
		}

		CalculateTranslationSteps();
		CalculateFinalTranslationVector();

		objectOption = 0;

		scaleFactor = 1.0;
		break;
	case 4:
		for (int i = 0; i < 3; i++)
		{
			endPoint1[i] = endPoint1Neil[i];
			endPoint2[i] = endPoint2Neil[i];

			initialPosition[i] = initialPositionNeil[i];
			rotationAxis[i] = endPoint1[i] - endPoint2[i];
		}
		CalculateTranslationSteps();
		CalculateFinalTranslationVector();

		objectOption = 1;
		scaleFactor = 1.0;

		break;

	case 5:
		for (int i = 0; i < 3; i++)
		{
			endPoint1[i] = endPoint1Neil[i];
			endPoint2[i] = endPoint2Neil[i];
			initialPosition[i] = initialPositionNeil[i];
			rotationAxis[i] = endPoint1[i] - endPoint2[i];
		}
		CalculateTranslationSteps();
		CalculateFinalTranslationVector();

		objectOption = 2;
		scaleFactor = 1.0;

		break;

	default:
		break;
	}
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


int main(int argc, char** argv)
{
	//setup window
	GLint windowID;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowPosition(50, 50);
	glutInitWindowSize(1000, 1000);
	
	windowID = glutCreateWindow("Part 2 - 3D Object");
	makeImage();
	init();
	CalculatePrisimPoints(initialPosition);
	calculateCubePoints(side);
	calculateRoofPoints(side);
	
	for (int i = 0; i < 3; i++)
	{
		endPoint1[i] = POne[i];
		endPoint2[i] = PTwo[i];

		initialPosition[i] = initialPositionMark[i];
		rotationAxis[i] = endPoint1[i] - endPoint2[i];
	}

	CalculateTranslationSteps();
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

	glutAddMenuEntry("Mark's Prism", 3);
	glutAddMenuEntry("Kevin's House", 4);
	glutAddMenuEntry("Neil's Octahedron", 5);

	glutAddMenuEntry("Exit", 7);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	calculateOctahedronFaceNormal(octahedronFaceNormal);
	calculateOctahedronVertexNormal(octahedronVertexNormal);
	calculatePrisimFaceNormal(prisimFaceNormal);
	CalculatePrisimVertexNormals(prisimVertexNormal);
	calculateHouseFaceNormal(faceNormalCubeKevin);
	calculateHouseVertexNormal(vertexNormalCubeKevin);

	calculateRoofFaceNormal(faceNormalRoofKevin);
	calculateRoofVertexNormal(vertexNormalRoofKevin);

	glutDisplayFunc(displayFunction);
	glutSpecialFunc(keyFunc);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshapeFcn);

	glutMainLoop();

	return 0;
}