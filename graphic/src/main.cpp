/*
 *		This Code Was Created By Jeff Molofee 2000
 *		A HUGE Thanks To Fredric Echols For Cleaning Up
 *		And Optimizing The Base Code, Making It More Flexible!
 *		If You've Found This Code Useful, Please Let Me Know.
 *		Visit My Site At nehe.gamedev.net
 */

//#include <windows.h>		// Header File For Windows
//#include <iostream>
//#include <math.h>
#include <stdlib.h>		//For RandomNumber-Generator
#include <time.h>		//For initialising the generator und for timemeasurement
#include "graphics.h"
#include "physics.h"
#include <X11/extensions/xf86vmode.h>
#include <X11/keysym.h>
//#include <GL/glx.h>
//#include <GL/gl.h>			// Header File For The OpenGL32 Library
//#include <gl\glu.h>			// Header File For The GLu32 Library
//# define M_PI           3.14159265358979323846  /* pi */
//#include <gl\glaux.h>		// Header File For The Glaux Library
//gl\glaux.h wird nicht mehr unterstuetzt, deshalb bmp.h verwenden!
//#include "bmp.h"

bool	keys[256];			// Array Used For The Keyboard Routine
int keyCodes[20];     /* array to hold our fetched keycodes */
bool	active=true;		// Window Active Flag Set To TRUE By Default
bool	fullscreen=true;	// Fullscreen Flag Set To Fullscreen Mode By Default

/* stuff about our window grouped together */
typedef struct {
    Display *dpy;
    int screen;
    Window win;
    GLXContext ctx;
    XSetWindowAttributes attr;
    Bool fs;
    XF86VidModeModeInfo deskMode;
    int x, y;
    unsigned int width, height;
    unsigned int depth;
} GLWindow;

/* attributes for a single buffered visual in RGBA format with at least
 * 4 bits per color and a 16 bit depth buffer */
static int attrListSgl[] = {GLX_RGBA, GLX_RED_SIZE, 4,
    GLX_GREEN_SIZE, 4,
    GLX_BLUE_SIZE, 4,
    GLX_DEPTH_SIZE, 16,
    None};

/* attributes for a double buffered visual in RGBA format with at least
 * 4 bits per color and a 16 bit depth buffer */
static int attrListDbl[] = { GLX_RGBA, GLX_DOUBLEBUFFER,
    GLX_RED_SIZE, 4,
    GLX_GREEN_SIZE, 4,
    GLX_BLUE_SIZE, 4,
    GLX_DEPTH_SIZE, 16,
    None };

GLWindow GLWin;
Bool done;
graphics ogl_graphics;


/* this function creates our window and sets it up properly */
/* FIXME: bits is currently unused */
Bool createGLWindow(char* title, int width, int height, int bits,
                    Bool fullscreenflag)
{
    XVisualInfo *vi;
    Colormap cmap;
    int dpyWidth, dpyHeight;
    int i;
    int glxMajorVersion, glxMinorVersion;
    int vidModeMajorVersion, vidModeMinorVersion;
    XF86VidModeModeInfo **modes;
    int modeNum;
    int bestMode;
    Atom wmDelete;
    Window winDummy;
    unsigned int borderDummy;
    
    GLWin.fs = fullscreenflag;
    /* set best mode to current */
    bestMode = 0;
    /* get a connection */
    GLWin.dpy = XOpenDisplay(0);
    GLWin.screen = DefaultScreen(GLWin.dpy);
    XF86VidModeQueryVersion(GLWin.dpy, &vidModeMajorVersion,
        &vidModeMinorVersion);
    printf("XF86VidModeExtension-Version %d.%d\n", vidModeMajorVersion,
        vidModeMinorVersion);
    XF86VidModeGetAllModeLines(GLWin.dpy, GLWin.screen, &modeNum, &modes);
    /* save desktop-resolution before switching modes */
    GLWin.deskMode = *modes[0];
    /* look for mode with requested resolution */
    for (i = 0; i < modeNum; i++)
    {
        if ((modes[i]->hdisplay == width) && (modes[i]->vdisplay == height))
        {
            bestMode = i;
        }
    }
    /* get an appropriate visual */
    vi = glXChooseVisual(GLWin.dpy, GLWin.screen, attrListDbl);
    if (vi == NULL)
    {
        vi = glXChooseVisual(GLWin.dpy, GLWin.screen, attrListSgl);
        printf("Only Singlebuffered Visual!\n");
    }
    else
    {
        printf("Got Doublebuffered Visual!\n");
    }
    glXQueryVersion(GLWin.dpy, &glxMajorVersion, &glxMinorVersion);
    printf("glX-Version %d.%d\n", glxMajorVersion, glxMinorVersion);
    /* create a GLX context */
    GLWin.ctx = glXCreateContext(GLWin.dpy, vi, 0, GL_TRUE);
    /* create a color map */
    cmap = XCreateColormap(GLWin.dpy, RootWindow(GLWin.dpy, vi->screen),
        vi->visual, AllocNone);
    GLWin.attr.colormap = cmap;
    GLWin.attr.border_pixel = 0;

    if (GLWin.fs)
    {
        XF86VidModeSwitchToMode(GLWin.dpy, GLWin.screen, modes[bestMode]);
        XF86VidModeSetViewPort(GLWin.dpy, GLWin.screen, 0, 0);
        dpyWidth = modes[bestMode]->hdisplay;
        dpyHeight = modes[bestMode]->vdisplay;
        printf("Resolution %dx%d\n", dpyWidth, dpyHeight);
        XFree(modes);
    
        /* create a fullscreen window */
        GLWin.attr.override_redirect = True;
        GLWin.attr.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |ButtonPressMask |
            StructureNotifyMask;
        GLWin.win = XCreateWindow(GLWin.dpy, RootWindow(GLWin.dpy, vi->screen),
            0, 0, dpyWidth, dpyHeight, 0, vi->depth, InputOutput, vi->visual,
            CWBorderPixel | CWColormap | CWEventMask | CWOverrideRedirect,
            &GLWin.attr);
        XWarpPointer(GLWin.dpy, None, GLWin.win, 0, 0, 0, 0, 0, 0);
		XMapRaised(GLWin.dpy, GLWin.win);
        XGrabKeyboard(GLWin.dpy, GLWin.win, True, GrabModeAsync,
            GrabModeAsync, CurrentTime);
        XGrabPointer(GLWin.dpy, GLWin.win, True, ButtonPressMask,
            GrabModeAsync, GrabModeAsync, GLWin.win, None, CurrentTime);
    }
    else
    {
        /* create a window in window mode*/
        GLWin.attr.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask |
            StructureNotifyMask;
        GLWin.win = XCreateWindow(GLWin.dpy, RootWindow(GLWin.dpy, vi->screen),
            0, 0, width, height, 0, vi->depth, InputOutput, vi->visual,
            CWBorderPixel | CWColormap | CWEventMask, &GLWin.attr);
        /* only set window title and handle wm_delete_events if in windowed mode */
        wmDelete = XInternAtom(GLWin.dpy, "WM_DELETE_WINDOW", True);
        XSetWMProtocols(GLWin.dpy, GLWin.win, &wmDelete, 1);
        XSetStandardProperties(GLWin.dpy, GLWin.win, title,
            title, None, NULL, 0, NULL);
        XMapRaised(GLWin.dpy, GLWin.win);
    }       
    /* connect the glx-context to the window */
    glXMakeCurrent(GLWin.dpy, GLWin.win, GLWin.ctx);
    XGetGeometry(GLWin.dpy, GLWin.win, &winDummy, &GLWin.x, &GLWin.y,
        &GLWin.width, &GLWin.height, &borderDummy, &GLWin.depth);
    printf("Depth %d\n", GLWin.depth);
    if (glXIsDirect(GLWin.dpy, GLWin.ctx)) 
        printf("Congrats, you have Direct Rendering!\n");
    else
        printf("Sorry, no Direct Rendering possible!\n");
    ogl_graphics.InitGL();
    return True;    
}


/* function to release/destroy our resources and restoring the old desktop */
void killGLWindow()
{
    if (GLWin.ctx)
    {
        if (!glXMakeCurrent(GLWin.dpy, None, NULL))
        {
            printf("Could not release drawing context.\n");
        }
        glXDestroyContext(GLWin.dpy, GLWin.ctx);
        GLWin.ctx = NULL;
    }
    /* switch back to original desktop resolution if we were in fs */
    if (GLWin.fs)
    {
        XF86VidModeSwitchToMode(GLWin.dpy, GLWin.screen, &GLWin.deskMode);
        XF86VidModeSetViewPort(GLWin.dpy, GLWin.screen, 0, 0);
    }
    XCloseDisplay(GLWin.dpy);
}

//Variableninit
int Init()
{
	/*

	xPos = 0.0f;
	yPos = 0.0f;
	rtri = 0.0f;
	ScreenZ = -20.0f;
	return 0;
	*/
	return 0;
}

int InitWorld()
{
	return 0;
}

void initKeys()
{
    printf("Initializing keys...\n");
    //get keycode for escape-key
    keyCodes[0] = XKeysymToKeycode(GLWin.dpy, XK_Escape);
    keyCodes[1] = XKeysymToKeycode(GLWin.dpy, XK_F1);
    keyCodes[2] = XKeysymToKeycode(GLWin.dpy, XK_w);
    keyCodes[3] = XKeysymToKeycode(GLWin.dpy, XK_a);
    keyCodes[4] = XKeysymToKeycode(GLWin.dpy, XK_s);
    keyCodes[5] = XKeysymToKeycode(GLWin.dpy, XK_d);
    keyCodes[6] = XKeysymToKeycode(GLWin.dpy, XK_plus);
    keyCodes[7] = XKeysymToKeycode(GLWin.dpy, XK_minus);
    keyCodes[8] = XKeysymToKeycode(GLWin.dpy, XK_Up);
    keyCodes[9] = XKeysymToKeycode(GLWin.dpy, XK_Down);
    keyCodes[10] = XKeysymToKeycode(GLWin.dpy, XK_Left);
    keyCodes[11] = XKeysymToKeycode(GLWin.dpy, XK_Right);
}

void keyAction(Creature &Player)
{
    if (keys[keyCodes[0]])
    {
        done = True;
    }
    if (keys[keyCodes[1]])
    {
        killGLWindow();
        GLWin.fs = !GLWin.fs;
        createGLWindow("NeHe's 3D-World Tutorial", 640, 480, 24, GLWin.fs);
        /* avoid multiple switches and therefore set the keypress back  */
        keys[keyCodes[1]] = False;
    }
    if (keys[keyCodes[2]])
    {
    ogl_graphics.change_Screeny(-0.1f);
     //   keys[keyCodes[2]] = False;
    }
    if (keys[keyCodes[3]])
    {
        ogl_graphics.change_Screenx(0.1f);
        //keys[keyCodes[3]] = False;
    }
    if (keys[keyCodes[4]])
    {
          ogl_graphics.change_Screeny(0.1f);
       // keys[keyCodes[4]] = False;
    }
    if (keys[keyCodes[5]])
    {
        ogl_graphics.change_Screenx(-0.1f);
    }
    if (keys[keyCodes[6]])
    {
	ogl_graphics.change_Screenz(0.02f);
    }
    if (keys[keyCodes[7]])
    {
        ogl_graphics.change_Screenz(-0.02f);
    }
    if (keys[keyCodes[8]])
    {
  	     Player.setSpeed(-sin(Player.getAngle()*M_PI / 180)*0.15f, cos(Player.getAngle()*M_PI / 180)*0.15f);
    }
    if (keys[keyCodes[9]])
    {
       	     Player.setSpeed(sin(Player.getAngle()*M_PI / 180)*0.15f, -cos(Player.getAngle()*M_PI / 180)*0.15f);  
    }
    if (keys[keyCodes[10]])
    {
         Player.changeAngle(0.15f);
    }
    if(keys[keyCodes[11]])
    {
         Player.changeAngle(-0.15f);
    }
}

int main(int argc, char **argv)
{
	srand((unsigned)time(NULL)); // Zufallsgenerator initialisieren.

	World world;
	b2Vec2 shape[4];
	shape[0] = b2Vec2(0.0f, 0.0f); shape[1] = b2Vec2(0.0f, 0.5f); shape[2] = b2Vec2(0.5f, 0.5f); shape[3] = b2Vec2(0.5f, 0.0f);
	Object* food[10];
	for (int i = 0; i < 10; i++)
	{
	food[i] = new Object(world.getWorld(), 200 + i, b2Vec2( rand() % 90 - 19, rand() % 90 - 19), shape, 4, 0.0f, OBJ_RECT);
	food[i]->make_Sensor(); //No physics, just collision detection
	}
	shape[0] = b2Vec2(0.0f, 2.0f); shape[1] = b2Vec2(-1.0f, -1.0f); shape[2] = b2Vec2(1.0f, -1.0f); shape[3] = b2Vec2(0.0f, 0.0f);
	Creature Player(world.getWorld(), 1, b2Vec2(1.0f, 1.0f), shape, 3, 0.0f, 100, OBJ_TRIANGLE);
	shape[0] = b2Vec2(-2.5f, -2.5f); shape[1] = b2Vec2(0.0f, -2.5f); shape[2] = b2Vec2(0.0f, 100.0f); shape[3] = b2Vec2(-2.5f, 100.0f);
	Object Wall1(world.getWorld(), 101, b2Vec2(-20.0f, -20.0f), shape,4, 0.0f, OBJ_RECT);//|
	Object Wall3(world.getWorld(), 103, b2Vec2(80.0f, -20.0f), shape, 4, 0.0f, OBJ_RECT);//  |
	shape[0] = b2Vec2(-2.5f, -2.5f); shape[1] = b2Vec2(-2.5f, 0.0f); shape[2] = b2Vec2(100.0f, 0.0f); shape[3] = b2Vec2(100.0f, -2.5f);
	Object Wall0(world.getWorld(), 100, b2Vec2(-20.0f, -20.0f), shape, 4, 0.0f, OBJ_RECT);//_
	Object Wall2(world.getWorld(), 102, b2Vec2(-20.0f, 80.0f), shape, 4, 0.0f, OBJ_RECT);//-

	//Creature creatures[10] = new
	Init();
	InitWorld();

    XEvent event;
    KeySym key;
    done = False;
    /* default to fullscreen */
    GLWin.fs = False;
    createGLWindow("NeHe's Rotation Tutorial", 640, 480, 24, GLWin.fs);
    initKeys();
/* wait for events*/ 
    while (!done)
    {


        /* handle the events in the queue */
        while (XPending(GLWin.dpy) > 0)
        {
            XNextEvent(GLWin.dpy, &event);
            switch (event.type)
            {
                case Expose:
	                if (event.xexpose.count != 0)
	                    break;

	        		ogl_graphics.DrawGL_Screen();
	        		ogl_graphics.DrawGLScene(Player.getPosition().x, Player.getPosition().y, Player.get_Polygonvectors(), Player.get_Polygonsize(), Player.getAngle(), Player.get_Objecttype());
	        			ogl_graphics.DrawGLScene(Player.getPosition().x, Player.getPosition().y, Player.get_Sensorvectors(), 8, Player.getAngle(), OBJ_POLYGON);

	        			for (int i = 0; i < 10; i++)
	        			{
	        				if (food[i]->is_awake())
	        				{
	        					ogl_graphics.DrawGLScene(food[i]->getPosition().x, food[i]->getPosition().y, food[i]->get_Polygonvectors(), food[i]->get_Polygonsize(), food[i]->getAngle(), food[i]->get_Objecttype());
	        				}
	        			}
	        			ogl_graphics.DrawGLScene(Wall0.getPosition().x, Wall0.getPosition().y, Wall0.get_Polygonvectors(), Wall0.get_Polygonsize(), Wall0.getAngle(), Wall0.get_Objecttype());
	        			ogl_graphics.DrawGLScene(Wall1.getPosition().x, Wall1.getPosition().y, Wall1.get_Polygonvectors(), Wall1.get_Polygonsize(), Wall1.getAngle(), Wall1.get_Objecttype());
	        			ogl_graphics.DrawGLScene(Wall2.getPosition().x, Wall2.getPosition().y, Wall2.get_Polygonvectors(), Wall2.get_Polygonsize(), Wall2.getAngle(), Wall2.get_Objecttype());
	        			ogl_graphics.DrawGLScene(Wall3.getPosition().x, Wall3.getPosition().y, Wall3.get_Polygonvectors(), Wall3.get_Polygonsize(), Wall3.getAngle(), Wall3.get_Objecttype());
    glXSwapBuffers(GLWin.dpy, GLWin.win);

         	        break;
	            case ConfigureNotify:
	            /* call resizeGLScene only if our window-size changed */
	                if ((event.xconfigure.width != GLWin.width) || 
	                    (event.xconfigure.height != GLWin.height))
	                {
	                    GLWin.width = event.xconfigure.width;
	                    GLWin.height = event.xconfigure.height;
                        printf("Resize event\n");
	                    ogl_graphics.ReSizeGLScene(event.xconfigure.width,
	                        event.xconfigure.height);
	                }
	                break;
                /* exit in case of a mouse button press */
                case ButtonPress:
                    //done = True;
                    break;
		case KeyPress:
                    keys[event.xkey.keycode] = True;
printf("Press\n");
                    break;
                case KeyRelease:
                    keys[event.xkey.keycode] = False;
		printf("Release\n");
                    break;
                case ClientMessage:
                    if (*XGetAtomName(GLWin.dpy, event.xclient.message_type) == 
                        *"WM_PROTOCOLS")
                    {
                        printf("Exiting sanely...\n");
                        done = True;
                    }
                    break;
                default:
                    break;
            }
        }
       keyAction(Player);
       world.simulate();
        	        		ogl_graphics.DrawGL_Screen();
	        		ogl_graphics.DrawGLScene(Player.getPosition().x, Player.getPosition().y, Player.get_Polygonvectors(), Player.get_Polygonsize(), Player.getAngle(), Player.get_Objecttype());
	        			ogl_graphics.DrawGLScene(Player.getPosition().x, Player.getPosition().y, Player.get_Sensorvectors(), 8, Player.getAngle(), OBJ_POLYGON);

	        			for (int i = 0; i < 10; i++)
	        			{
	        				if (food[i]->is_awake())
	        				{
	        					ogl_graphics.DrawGLScene(food[i]->getPosition().x, food[i]->getPosition().y, food[i]->get_Polygonvectors(), food[i]->get_Polygonsize(), food[i]->getAngle(), food[i]->get_Objecttype());
	        				}
	        			}
	        			ogl_graphics.DrawGLScene(Wall0.getPosition().x, Wall0.getPosition().y, Wall0.get_Polygonvectors(), Wall0.get_Polygonsize(), Wall0.getAngle(), Wall0.get_Objecttype());
	        			ogl_graphics.DrawGLScene(Wall1.getPosition().x, Wall1.getPosition().y, Wall1.get_Polygonvectors(), Wall1.get_Polygonsize(), Wall1.getAngle(), Wall1.get_Objecttype());
	        			ogl_graphics.DrawGLScene(Wall2.getPosition().x, Wall2.getPosition().y, Wall2.get_Polygonvectors(), Wall2.get_Polygonsize(), Wall2.getAngle(), Wall2.get_Objecttype());
	        			ogl_graphics.DrawGLScene(Wall3.getPosition().x, Wall3.getPosition().y, Wall3.get_Polygonvectors(), Wall3.get_Polygonsize(), Wall3.getAngle(), Wall3.get_Objecttype());
    glXSwapBuffers(GLWin.dpy, GLWin.win);

    }
    killGLWindow();
    return 0;
}
