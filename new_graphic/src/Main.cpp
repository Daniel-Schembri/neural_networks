/*
* Copyright (c) 2006-2007 Erin Catto http://www.box2d.org
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#include "Render.h"
#include "Test.h"
#include "glui/glui.h"
#include <stdio.h>
#include <vector>
#include "NeuralWorld.h"
#include "evolutionary.h"


namespace
{
	NeuralWorld* test = NULL;  //was Test* before
	evolutionary *evolution_control = NULL;
	std::vector<unsigned> topology;

	Settings settings;
	int32 width = 1280;
	int32 height = 928;
	int32 framePeriod = 16;
	int32 mainWindow;
	int32 plotterWindow; //ds
	int32 Createevolution_Window;
    int ptx, pty, ptw, pth;
    int reset_sim;
	float settingsHz = 60.0;
	GLUI *glui;
	GLUI *glui_createevolution;
	float32 viewZoom = 5.0f;
    float32 viewZoom_panel = 3.0f;
	int tx, ty, tw, th;
	bool rMouseDown;
	b2Vec2 lastp;
	DebugDraw plot_debugDraw;


}

static void Resize(int32 w, int32 h)
{
	width = w;
	height = h;

	GLUI_Master.get_viewport_area(&tx, &ty, &tw, &th);
	glViewport(tx, ty, tw, th);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float32 ratio = float32(tw) / float32(th);

	b2Vec2 extents(ratio * 25.0f, 25.0f);
	extents *= viewZoom;

	b2Vec2 lower = settings.viewCenter - extents;
	b2Vec2 upper = settings.viewCenter + extents;

	// L/R/B/T
	gluOrtho2D(lower.x, upper.x, lower.y, upper.y);
}

static void Resize_Panel(int32 w, int32 h)
{
	width = w;
	height = h;

	GLUI_Master.get_viewport_area(&ptx, &pty, &ptw, &pth);
	glViewport(ptx, pty, ptw, pth);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float32 ratio = float32(ptw) / float32(pth);

	b2Vec2 extents(ratio * 25.0f, 25.0f);
	extents *= viewZoom_panel;

	b2Vec2 lower = settings.viewCenter_plotter - extents;
	b2Vec2 upper = settings.viewCenter_plotter + extents;

	// L/R/B/T
	gluOrtho2D(lower.x, upper.x, lower.y, upper.y);
}


static b2Vec2 ConvertScreenToWorld(int32 x, int32 y)
{
	float32 u = x / float32(tw);
	float32 v = (th - y) / float32(th);

	float32 ratio = float32(tw) / float32(th);
	b2Vec2 extents(ratio * 25.0f, 25.0f);
	extents *= viewZoom;

	b2Vec2 lower = settings.viewCenter - extents;
	b2Vec2 upper = settings.viewCenter + extents;

	b2Vec2 p;
	p.x = (1.0f - u) * lower.x + u * upper.x;
	p.y = (1.0f - v) * lower.y + v * upper.y;
	return p;
}



// This is used to control the frame rate (60Hz).
static void Timer(int)
{
	glutSetWindow(mainWindow);
	glutPostRedisplay();
	glutTimerFunc(framePeriod, Timer, 0);
}

// This is used to control the frame rate (60Hz).
static void Timer2(int)
{
	glutSetWindow(plotterWindow);
	glutPostRedisplay();
	glutTimerFunc(framePeriod, Timer2, 0);
}

static void plotting() // function used by second window ds
{
	if (test != NULL)
	{
		if (!settings.pause)
		{
			b2Vec2 oldCenter;
			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			oldCenter = settings.viewCenter_plotter;

			//Plot BestFitness-Graph
			for (unsigned int i = 1; i < evolution_control->best_fitnesses.size(); i++)
				plot_debugDraw.DrawLine(b2Vec2(((i - 1) / 1.0f) - 30.0f, evolution_control->best_fitnesses[i - 1]), b2Vec2((i / 1.0f) - 30.0f, evolution_control->best_fitnesses[i]), b2Color(1.0f, 0.0f, 0.0f));
			//Plot AverageFitness-Graph
			for (unsigned int i = 1; i < evolution_control->average_fitnesses.size(); i++)
				plot_debugDraw.DrawLine(b2Vec2(((i - 1) / 1.0f) - 30.0f, evolution_control->average_fitnesses[i - 1]), b2Vec2((i / 1.0f) - 30.0f, evolution_control->average_fitnesses[i]), b2Color(0.0f, 0.0f, 0.0f));

			//Plot Statistics
			plot_debugDraw.DrawString(5, 20, "Mode: %d", settings.mode);
			plot_debugDraw.DrawString(5, 45, "Evolve_Algorithm: %d", settings.evolve_algorithm);
			plot_debugDraw.DrawString(5, 70, "generation: %d", evolution_control->generations);
			plot_debugDraw.DrawString(5, 95, "iterations: %d", evolution_control->iterationsteps);
			plot_debugDraw.DrawString(5, 120, "mutation_rate: %f", evolution_control->mutation_rate);
			plot_debugDraw.DrawString(5, 145, "annealing_rate: %f", evolution_control->annealing_rate);
			plot_debugDraw.DrawString(5, 170, "Actual Best_Fitness: %d", evolution_control->best_fitnesses.back());
			plot_debugDraw.DrawString(5, 185, "Actual Average_Fitness: %f", evolution_control->average_fitnesses.back());
			plot_debugDraw.DrawString(5, 210, "Best_Fitness overall: %d", evolution_control->get_bestFitness_overall());


			if (oldCenter.x != settings.viewCenter_plotter.x || oldCenter.y != settings.viewCenter_plotter.y)
			{
				Resize_Panel(width, height);
			}

			glutSwapBuffers();
		}
	}
}

static void SimulationLoop()
{
	if (test != NULL)
	{
		if (!settings.pause)
		{
			b2Vec2 oldCenter;

			if (!settings.turbo)
			{
				glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				oldCenter = settings.viewCenter;
			}
			settings.hz = settingsHz;
			test->Step(&settings);
			test->move_bodies(evolution_control->process(test->get_sensor_vectors()));  //TODO: Remove encapsulation
			reset_sim = evolution_control->evolve(settings.evolve_algorithm);


			if (settings.turbo)
			{
				for (int i = 0; i < 20; i++)
				{
					test->Step(&settings);
					test->move_bodies(evolution_control->process(test->get_sensor_vectors()));  //TODO: Remove encapsulation
					reset_sim = evolution_control->evolve(settings.evolve_algorithm);
					if (reset_sim) break;
				}
			}
			if (reset_sim)
			{
				reset_sim = 0;
				evolution_control->save_bestFitness();
				evolution_control->save_averageFitness();
				test->Reset();

			}

			if (!settings.turbo)
			{
				if (oldCenter.x != settings.viewCenter.x || oldCenter.y != settings.viewCenter.y)
				{
					Resize(width, height);
				}

				glutSwapBuffers();

			}
		}
	}
}

static void Keyboard(unsigned char key, int x, int y)
{
	
	if (test != NULL)
	{
		B2_NOT_USED(x);
		B2_NOT_USED(y);

		switch (key)
		{
		case 27:
#ifndef __APPLE__
			// freeglut specific function
			glutLeaveMainLoop();
#endif
			exit(0);
			break;

			// Press 'z' to zoom out.
		case 'y':
			viewZoom = b2Min(1.1f * viewZoom, 20.0f);
			Resize(width, height);
			break;

			// Press 'x' to zoom in.
		case 'x':
			viewZoom = b2Max(0.9f * viewZoom, 0.02f);
			Resize(width, height);
			break;
		case 't':  //turbo ds
			if (settings.turbo){ settings.turbo = false; }
			else{ settings.turbo = true; }
			break;

			// Press 'r' to reset.
		case 'r':
		//	Restart(0);
			break;

		case 'p':
			settings.pause = !settings.pause;
			break;

		default:
			if (test)
			{
				test->Keyboard(key);
			}
		}
	}
}

static void KeyboardSpecial(int key, int x, int y)
{
	if (test != NULL)
	{
		B2_NOT_USED(x);
		B2_NOT_USED(y);

		int mod = glutGetModifiers();

		switch (key)
		{

			//Todo: Here: Get Player Inputs for movement	

			// Press left to pan left.
		case GLUT_KEY_LEFT:
			if (mod == GLUT_ACTIVE_CTRL)
			{
				b2Vec2 newOrigin(-5.0f, 0.0f);
				test->ShiftOrigin(newOrigin);
			}
			else
			{
				settings.viewCenter.x -= 2.0f;
				Resize(width, height);
			}
			break;

			// Press right to pan right.
		case GLUT_KEY_RIGHT:
			if (mod == GLUT_ACTIVE_CTRL)
			{
				b2Vec2 newOrigin(5.0f, 0.0f);
				test->ShiftOrigin(newOrigin);
			}
			else
			{
				settings.viewCenter.x += 2.0f;
				Resize(width, height);
			}
			break;

			// Press down to pan down.
		case GLUT_KEY_DOWN:
			if (mod == GLUT_ACTIVE_CTRL)
			{
				b2Vec2 newOrigin(0.0f, -5.0f);
				test->ShiftOrigin(newOrigin);
			}
			else
			{
				settings.viewCenter.y -= 2.0f;
				Resize(width, height);
			}
			break;

			// Press up to pan up.
		case GLUT_KEY_UP:
			if (mod == GLUT_ACTIVE_CTRL)
			{
				b2Vec2 newOrigin(0.0f, 5.0f);
				test->ShiftOrigin(newOrigin);
			}
			else
			{
				settings.viewCenter.y += 2.0f;
				Resize(width, height);
			}
			break;

			// Press home to reset the view.
		case GLUT_KEY_HOME:
			viewZoom = 1.0f;
			settings.viewCenter.Set(0.0f, 20.0f);
			Resize(width, height);
			break;
		}
	}
}

static void Keyboard_Panel(unsigned char key, int x, int y)
{
	if (test != NULL)
	{
		B2_NOT_USED(x);
		B2_NOT_USED(y);

		switch (key)
		{
		case 27:
#ifndef __APPLE__
			// freeglut specific function
			glutLeaveMainLoop();
#endif
			exit(0);
			break;

			// Press 'z' to zoom out.
		case 'y':
			viewZoom_panel = b2Min(1.1f * viewZoom_panel, 20.0f);
			Resize_Panel(width, height);
			break;

			// Press 'x' to zoom in.
		case 'x':
			viewZoom_panel = b2Max(0.9f * viewZoom_panel, 0.02f);
			Resize_Panel(width, height);
			break;
		case 't':  //turbo ds
			if (settings.turbo){ settings.turbo = false; }
			else{ settings.turbo = true; }
			break;

			// Press 'r' to reset.
		case 'r':
		//	Restart(0);
			break;

		case 'p':
			settings.pause = !settings.pause;
			break;

		default:
			if (test)
			{
				test->Keyboard(key);
			}
		}
	}
}

static void KeyboardUp(unsigned char key, int x, int y)
{
	if (test != NULL)
	{
		B2_NOT_USED(x);
		B2_NOT_USED(y);

		if (test)
		{
			test->KeyboardUp(key);
		}
	}
}

static void Mouse(int32 button, int32 state, int32 x, int32 y)
{
	if (test != NULL)
	{
		// Use the mouse to move things around.
		if (button == GLUT_LEFT_BUTTON)
		{
			int mod = glutGetModifiers();
			b2Vec2 p = ConvertScreenToWorld(x, y);
			if (state == GLUT_DOWN)
			{
				b2Vec2 p = ConvertScreenToWorld(x, y);
				if (mod == GLUT_ACTIVE_SHIFT)
				{
					test->ShiftMouseDown(p);
				}
				else
				{
					test->MouseDown(p);
				}
			}

			if (state == GLUT_UP)
			{
				test->MouseUp(p);
			}
		}
		else if (button == GLUT_RIGHT_BUTTON)
		{
			if (state == GLUT_DOWN)
			{
				lastp = ConvertScreenToWorld(x, y);
				rMouseDown = true;
			}

			if (state == GLUT_UP)
			{
				rMouseDown = false;
			}
		}
	}
}

static void MouseMotion(int32 x, int32 y)
{
	if (test != NULL)
	{
		b2Vec2 p = ConvertScreenToWorld(x, y);
		test->MouseMove(p);

		if (rMouseDown)
		{
			b2Vec2 diff = p - lastp;
			settings.viewCenter.x -= diff.x;
			settings.viewCenter.y -= diff.y;
			Resize(width, height);
			lastp = ConvertScreenToWorld(x, y);
		}
	}
}

#ifdef FREEGLUT
static void MouseWheel(int wheel, int direction, int x, int y)
{
	if (test != NULL)
	{
		B2_NOT_USED(wheel);
		B2_NOT_USED(x);
		B2_NOT_USED(y);
		if (direction > 0)
		{
			viewZoom /= 1.1f;
		}
		else
		{
			viewZoom *= 1.1f;
		}
		Resize(width, height);
	}
}
#endif

static void Restart(int)
{
	if (test != NULL)
	{
		delete test;
		test = NULL;
	}
	if (evolution_control != NULL)
	{
		delete evolution_control;
		evolution_control = NULL;
	}

	evolution_control = new evolutionary(settings.population_size, settings.amount_of_food, 4, settings.mutation_rate, (settings.evolve_time*50), topology);
	test = new NeuralWorld(&(evolution_control->population), evolution_control->amount_of_food, evolution_control->amount_of_obstacles, (float) settings.field_size);
	glui_createevolution->hide();
	Resize(width, height);
}

static void new_evolution()
{
	/*
	if (test != NULL)
	{
		delete test;
		test = NULL;
	}
		
	if (evolution_control != NULL)
	{
		delete evolution_control;
		evolution_control = NULL;
	}
	*/
	glui_createevolution->show();
	//evolution_control = new evolutionary(10, 40, 10, 0.3f, topology);
	//test = new NeuralWorld(&(evolution_control->population), evolution_control->amount_of_food, evolution_control->amount_of_obstacles);

}

static void create_evolution(int)
{

	new_evolution();
}

static void glui_create_hide(int)
{
	glui_createevolution->hide();
}

static void Pause(int)
{
	//TODO: Here prove 	if (test != NULL) ?
	settings.pause = !settings.pause;

}

static void Exit(int code)
{
	// TODO: freeglut is not building on OSX
#ifdef FREEGLUT
	glutLeaveMainLoop();
#endif
	exit(code);
}

static void SingleStep(int)
{
	settings.pause = 1;
	settings.singleStep = 1;
}

int main(int argc, char** argv)
{
    //Init Box2D-Simulation
    settings.velocityIterations = 8;
    settings.positionIterations = 3;
    settingsHz = 60;

	//Init

	//Evolution
	topology.push_back(3); //Input-Layer
	topology.push_back(6); //Hidden-Layer
	topology.push_back(2); //Output-Layer
//	evolution_control = new evolutionary(10, 40, 10, 0.3f, topology);

	//Init end
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowPosition(0, 0);
	glutInitWindowSize(width, height);

	char title[32];
	sprintf(title, "Box2D Version %d.%d.%d", b2_version.major, b2_version.minor, b2_version.revision);
	mainWindow = glutCreateWindow(title);
	//glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutDisplayFunc(SimulationLoop);
	GLUI_Master.set_glutReshapeFunc(Resize);  
	GLUI_Master.set_glutKeyboardFunc(Keyboard);
	GLUI_Master.set_glutSpecialFunc(KeyboardSpecial);
	GLUI_Master.set_glutMouseFunc(Mouse);
#ifdef FREEGLUT
	glutMouseWheelFunc(MouseWheel);
#endif
	glutMotionFunc(MouseMotion);

	glutKeyboardUpFunc(KeyboardUp);

	glui = GLUI_Master.create_glui_subwindow(mainWindow, GLUI_SUBWINDOW_BOTTOM);
  

	GLUI_Panel* drawPanel =	glui->add_panel("Simulation");
	glui->add_button_to_panel(drawPanel, "New Evolution", 0, create_evolution);  //TODO: New Game
	glui->add_column_to_panel(drawPanel, true);
	glui->add_button_to_panel(drawPanel, "Pause/Resume", 0, Pause);  //TODO: Pause and Resume!
    glui->add_column_to_panel(drawPanel, true);
	glui->add_button_to_panel(drawPanel, "Restart", 0, Restart);
    glui->add_column_to_panel(drawPanel, true);
	glui->add_button_to_panel(drawPanel, "Best Creatures");


	glui->add_column_to_panel(drawPanel, true);
	glui->add_button_to_panel(drawPanel, "Quit", 0,(GLUI_Update_CB)Exit);
	glui->add_column_to_panel(drawPanel, true);

	glui->set_main_gfx_window( mainWindow );

	// Use a timer to control the frame rate.
	glutTimerFunc(framePeriod, Timer, 0);

	//Second Window for Plotting ds
//    glutInitWindowPosition(1024, 0);
//	glutInitWindowSize(400, 928);
	
//	plotterWindow = glutCreateWindow("Neural Network Status");
	//TODO: Set Focus on Subwindow that under Freeglut (Win) it gets Keyboard-callback
    plotterWindow = glutCreateSubWindow(mainWindow, 880, 0, 400, 848);	
    glutDisplayFunc(plotting);
	glutKeyboardFunc(Keyboard_Panel);
    glutReshapeFunc(Resize_Panel);
//	glutSpecialFunc(KeyboardSpecial_Panel);
	//glutMouseFunc(Mouse);
	// Use a timer to control the frame rate.
	glutTimerFunc(framePeriod, Timer2, 0);
	
	//Window to create new Evolution
	glui_createevolution = GLUI_Master.create_glui("New Evolution", 0, 300, 300);

	//glui_createevolution->add_statictext("Mode");
	//GLUI_Listbox* testList = glui_createevolution->add_listbox("", &settings.mode);
	//testList->add_item(1, "Evolution");
	//testList->add_item(2, "Single-Training");
	//glui_createevolution->add_separator();

	//TODO Maybe in a later version glui_createevolution->add_checkbox("Random Food Position", &settings.randomfood_pos);
	//TODO Maybe in a later version glui_createevolution->add_checkbox("Random Creature Position", &settings.randomcreature_pos);
	
	glui_createevolution->add_separator();

	glui_createevolution->add_button("OK", 0, Restart);
	glui_createevolution->add_column(false);

	GLUI_Spinner* popSizeSpinner = glui_createevolution->add_spinner("Population Size", GLUI_SPINNER_INT, &settings.population_size);
	popSizeSpinner->set_int_limits(1, 100);
	GLUI_Spinner* amountfoodSpinner = glui_createevolution->add_spinner("Amount of Objects", GLUI_SPINNER_INT, &settings.amount_of_food);
	amountfoodSpinner->set_int_limits(1, 1000);
	GLUI_Spinner* evolveTimeSpinner = glui_createevolution->add_spinner("Evolve Time in Sec", GLUI_SPINNER_INT, &settings.evolve_time);
	evolveTimeSpinner->set_int_limits(5, 1000);

	GLUI_Spinner* fieldsizeSpinner = glui_createevolution->add_spinner("Field Size", GLUI_SPINNER_INT, &settings.field_size);
	fieldsizeSpinner->set_int_limits(40, 200);
	GLUI_Spinner* mutationRateSpinner = glui_createevolution->add_spinner("Mutation Rate", GLUI_SPINNER_FLOAT, &settings.mutation_rate);
	mutationRateSpinner->set_float_limits(0.01f, 1.0f);
	glui_createevolution->add_separator();
	glui_createevolution->add_statictext("Evolutionary algorithm");
	GLUI_Listbox* listevolvealgo = glui_createevolution->add_listbox("", &settings.evolve_algorithm);
	listevolvealgo->add_item(1, "Hillclimber");
	listevolvealgo->add_item(2, "Simulated Annealing");
	listevolvealgo->set_int_val(1);
	glui_createevolution->add_separator();
	glui_createevolution->add_button("Cancel", 0, glui_create_hide);

	/*
	GLUI_Master.set_glutIdleFunc(NULL);
	GLUI_Master.set_glutReshapeFunc(Resize);
	GLUI_Master.set_glutKeyboardFunc(Keyboard);
	GLUI_Master.set_glutSpecialFunc(KeyboardSpecial);
	GLUI_Master.set_glutMouseFunc(Mouse);
	*/
	glui_createevolution->set_main_gfx_window(mainWindow);

	glui_createevolution->hide();

	

    glutMainLoop();

	return 0;
}
