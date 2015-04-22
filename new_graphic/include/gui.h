#ifndef GUI_H
#define GUI_H

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "NeuralWorld.h"
#include "evolutionary.h"

#include "glui/glui.h"

struct parameter;

class gui
{
protected:
	static gui *instance;

public:


private:
	//create_evoluion_window components
	/*
	GLUI_Spinner* popSizeSpinner;
	GLUI_Spinner* amountObjectSpinner;
	GLUI_Spinner* evolveTimeSpinner;
	GLUI_Spinner* fieldsizeSpinner;
	GLUI_Spinner* mutationRateSpinner;
	GLUI_Spinner* simulated_annealingSpinner;
	
	GLUI_RadioGroup* evolve_algorithms_radiogroup;
	*/

	bool created;

	NeuralWorld* test = NULL;  //was Test* before
	evolutionary *evolution_control = NULL;
	std::vector<unsigned> topology;

	Settings settings;
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
	struct parameter sim_parameter;
	struct parameter temp_sim_parameter;

	std::vector< std::vector<float> > trainingdata;

	//For Singleplayer Recording
	std::vector< std::vector<double> > record_inputs;
	std::vector< std::vector<double> > record_outputs;


	int x, y, width, height;

	char * title;

public:
	static void setInstance(gui * pinstance);

	static void Wrapper_KeyboardUp(unsigned char key, int x, int y);
	static void Wrapper_Mouse(int32 button, int32 state, int32 x, int32 y);
	static void Wrapper_MouseMotion(int32 x, int32 y);
#ifdef FREEGLUT
	static void Wrapper_MouseWheel(int wheel, int direction, int x, int y);
#endif
	static void Wrapper_new_evolution(int);
	static void Wrapper_create_evolution(int);
	static void Wrapper_Restart_setVal(int);
	static void Wrapper_glui_create_hide(int);
	static void Wrapper_Pause(int);
	static void Wrapper_Exit(int code);
	static void Wrapper_SingleStep(int);
	static void Wrapper_Resize(int w, int h);
	static void Wrapper_Keyboard(unsigned char key, int x, int y);
	static void Wrapper_KeyboardSpecial(int key, int x, int y);
	static b2Vec2 Wrapper_ConvertScreenToWorld(int32 x, int32 y);
	static void Wrapper_plotting();
	static void Wrapper_Timer(int);
	static void Wrapper_Timer2(int);
	static void Wrapper_Restart(int);
	static void Wrapper_Resize_Panel(int32 w, int32 h);
	static void Wrapper_Keyboard_Panel(unsigned char key, int x, int y);
	static void Wrapper_SimulationLoop();

	static void Wrapper_glui_noclose();

	void KeyboardUp(unsigned char key, int x, int y);
	void Mouse(int32 button, int32 state, int32 x, int32 y);
	void MouseMotion(int32 x, int32 y);
#ifdef FREEGLUT
	void MouseWheel(int wheel, int direction, int x, int y);
#endif
	void new_evolution(int);
	void create_evolution(int);
	void Restart_setVal(int);
	void glui_create_hide(int);
	void Pause(int);
	void Exit(int code);
	void SingleStep(int);
	void Resize(int w, int h);
	void Keyboard(unsigned char key, int x, int y);
	void KeyboardSpecial(int key, int x, int y);
	b2Vec2 ConvertScreenToWorld(int32 x, int32 y);
	void plotting(); // function used by second window ds
	void Timer(int);
	void Timer2(int);
	void Restart(int);
	void Resize_Panel(int32 w, int32 h);
	void Keyboard_Panel(unsigned char key, int x, int y);
	void SimulationLoop();

	void glui_noclose();

	void Start();

	//File Operations
	void load_trainingData(const char * path);
	void load_Simulation(const char * path);
	void refresh_Library();


	//
	void Createevolution_win();
	void Create_Panel();
	void Create_plotterwin();

	gui();
	gui(char * ptitle, int px, int py, int pwidth, int pheight, int argc, char** argv);

	void create_subwindow(int id);

	void init();

private:


};
#endif