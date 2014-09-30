//#include <windows.h>		// Header File For Windows
#include <iostream>
#include "Box2D/Box2D.h"    // Needed for vectorcalculation
#include <math.h>

#include <GL/glx.h>
#include <GL/gl.h>// Header File For The OpenGL32 Library
#include <GL/glu.h>// Header File For The GLu32 Library

# define M_PI           3.14159265358979323846  /* pi */
//WINDOWS: gl\glaux.h wird nicht mehr unterstuetzt, deshalb bmp.h verwenden!
//#include "bmp.h"

#ifndef GRAPHICS_H
#define GRAPHICS_H

class graphics
{
private:
	GLfloat	rtri;				// Angle For The Triangle ( NEW )
	GLfloat xPos, yPos;
	GLfloat ScreenX, ScreenY, ScreenZ; // Height of View on the level
	GLfloat objects[100][100];  //Z-Dimension needed?
public:
	graphics();
	~graphics();
	void ReSizeGLScene(GLsizei width, GLsizei height);	// Resize And Initialize The GL Window
	int DrawGL_Screen();
	int DrawGLScene(GLfloat x, GLfloat y, b2Vec2 *p_shape, int p_shapepoints, GLfloat angle, int objecttype);
	//int DrawGLScene(GLvoid);									// Here's Where We Do All The Drawing
	int InitGL();										// All Setup For OpenGL Goes Here
	int refresh_objectpositions(GLfloat x, GLfloat y);
	void change_Screenz(GLfloat sz);
	void change_Screenx(GLfloat sx);
	void change_Screeny(GLfloat sy);
};
#endif
