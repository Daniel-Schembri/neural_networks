/*
Class for displaying graphics. Could later be replaced by a newer Open-GL-Version
Daniel Schembri 2014-09-14
*/
#include "graphics.h"
#define OBJ_TRIANGLE 0
#define OBJ_RECT 1
#define OBJ_POLYGON 2
#define OBJ_CIRCLE 3


	graphics::graphics()
	{
		ScreenZ = -20.0f; ScreenX = 0.0f; ScreenY = 0.0f;
		xPos = 0.0f; yPos = 0.0f;
	}
	graphics::~graphics()
	{

	}

	int graphics::InitGL()							// All Setup For OpenGL Goes Here
	{
		glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
		glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
		glClearDepth(1.0f);									// Depth Buffer Setup
		glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
		glEnable(GL_BLEND);									// Needed for Transparency 
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
 	        glFlush();  //just used in Ubuntu/Linux Mint
		return true;										// Initialization Went OK
	}

	void graphics::ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
	{
		if (height == 0)										// Prevent A Divide By Zero By
		{
			height = 1;										// Making Height Equal One
		}

		glViewport(0, 0, width, height);						// Reset The Current Viewport

		glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
		glLoadIdentity();									// Reset The Projection Matrix

		// Calculate The Aspect Ratio Of The Window
		gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f); //Sets the borders of displaying objects

		glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
		glLoadIdentity();									// Reset The Modelview Matrix
	}
	

	void graphics::change_Screenz(GLfloat sz)
	{
		ScreenZ += sz;
	}
	void graphics::change_Screenx(GLfloat sx)
	{
		ScreenX += sx;
	}
	void graphics::change_Screeny(GLfloat sy)
	{
		ScreenY += sy;
	}

	int graphics::DrawGL_Screen()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
		glLoadIdentity();									// Reset The Current Modelview Matrix

		glTranslatef(ScreenX, ScreenY, ScreenZ);						// Move Left 1.5 Units And Into The Screen 6.0
		//glRotatef(Screenangle, 0.0f, 0.0f, 1.0f);//optional rotation. correct Axis??
		//Draw all the Triangles (Creatures)
		return 0;
	}
//    glXSwapBuffers(GLWin.dpy, GLWin.win);
	//int graphics::DrawGLScene(GLvoid)									// Here's Where We Do All The Drawing
	int graphics::DrawGLScene(GLfloat x, GLfloat y, b2Vec2* p_shape, int p_shapepoints, GLfloat angle, int objecttype)									// Here's Where We Do All The Drawing
	{
		glPushMatrix();
		glTranslatef(x, y, 0.0f);  //move object
		glRotatef(angle, 0.0f, 0.0f, 1.0f); //rotate object
		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		if (OBJ_TRIANGLE == objecttype)
		{
		  glBegin(GL_TRIANGLES);							// Start Drawing A Triangle
		  glVertex3f(p_shape[0].x, p_shape[0].y, 0.0f);					    // First Point Of The Triangle
		  glVertex3f(p_shape[1].x, p_shape[1].y, 0.0f);					// Second Point Of The Triangle
		  glVertex3f(p_shape[2].x, p_shape[2].y, 0.0f);					// Third Point Of The Triangle
		  glEnd();
		}
		else if (OBJ_RECT == objecttype)
		{
			glBegin(GL_QUADS);							    // Start Drawing A RECT
			glVertex3f(p_shape[0].x, p_shape[0].y, 0.0f);					// First Point Of The RECT
			glVertex3f(p_shape[1].x, p_shape[1].y, 0.0f);					// Second Point Of The RECT
			glVertex3f(p_shape[2].x, p_shape[2].y, 0.0f);					// Third Point Of The RECT
			glVertex3f(p_shape[3].x, p_shape[3].y, 0.0f);					// Fourth Point Of The RECT
			glEnd();
		}
		else if (OBJ_POLYGON == objecttype)		//TODO
		{
			glColor4f(0.0f, 0.0f, 1.0f, 0.1f);
			glBegin(GL_POLYGON);
			for (int i = 0; i < p_shapepoints; i++)
			{
				glVertex3f(p_shape[i].x, p_shape[i].y, 0.0f);					// 
			}
			glEnd();
		}
		else    // CIRCLE TODO
		{

		}
		glPopMatrix();
	//	glLoadIdentity();									// Reset The Current Modelview Matrix
		return true;										// Keep Going
	}

	int graphics::refresh_objectpositions(GLfloat x, GLfloat y)
	{
		return 0;
	}
