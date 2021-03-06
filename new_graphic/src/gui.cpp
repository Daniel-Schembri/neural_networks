/*
 * Copyright (c) 2015 Daniel Schembri https://github.com/Daniel-Schembri/neural_networks
 *
 * gui.cpp
 *
 * Some methods are based on Erin Cattos Box2D Testbed
 *
 * new_graphic
 * This is a graphical user interface to evolve object collecting agents 
 * The agent have two neural networks for steering angle and velocity.
 *
 * This project includes the neural net framework, called logic,
 * of Jonathan Schwarz https://github.com/jonathan-schwarz/neural_networks
 *
 * new_graphic also uses the Box2D-library by Erin Catto
 *
 * At following the license for Box2D:
 *
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

#include "gui.h"

gui * gui::instance = 0;
void gui::setInstance(gui * pinstance)
{
    instance = pinstance;
}

//Wrapper Methods
void gui::Wrapper_KeyboardUp(unsigned char key, int x, int y)
{
    instance->KeyboardUp(key, x, y);
}

void gui::Wrapper_Mouse(int32 button, int32 state, int32 x, int32 y)
{
    instance->Mouse(button, state, x, y);
}

void gui::Wrapper_MouseMotion(int32 x, int32 y)
{
    instance->MouseMotion(x, y);
}

#ifdef FREEGLUT
void gui::Wrapper_MouseWheel(int wheel, int direction, int x, int y)
{
    instance->MouseWheel(wheel, direction, x, y);
}
#endif

void gui::Wrapper_new_evolution(int)
{
    instance->new_evolution(0);
}

void gui::Wrapper_create_evolution(int)
{
    instance->create_evolution(0);
}

void gui::Wrapper_Restart_setVal(int)
{
    instance->Restart_setVal(0);
}

void gui::Wrapper_glui_noclose()
{
    instance->glui_noclose();
}

void gui::Wrapper_glui_create_hide(int)
{
    instance->glui_create_hide(0);
}

void gui::Wrapper_Pause(int)
{
    instance->Pause(0);
}

void gui::Wrapper_Exit(int code)
{
    instance->Exit(0);
}

void gui::Wrapper_SingleStep(int)
{
    instance->SingleStep(0);
}

void gui::Wrapper_Resize(int w, int h)
{
    instance->Resize(w, h);
}

void gui::Wrapper_Keyboard(unsigned char key, int x, int y)
{
    instance->Keyboard(key, x, y);
}

void gui::Wrapper_KeyboardSpecial(int key, int x, int y)
{
    instance->KeyboardSpecial(key, x, y);
}

b2Vec2 gui::Wrapper_ConvertScreenToWorld(int32 x, int32 y)
{
    return instance->ConvertScreenToWorld(x, y);
}

void gui::Wrapper_Timer(int)
{
    instance->Timer(0);
}

void gui::Wrapper_Timer2(int)
{
    instance->Timer2(0);
}

void gui::Wrapper_plotting() // function used by second window ds
{
    instance->plotting();
}
void gui::Wrapper_Restart(int)
{
    instance->Restart(0);
}

void gui::Wrapper_Resize_Panel(int32 w, int32 h)
{
    instance->Resize_Panel(w, h);
}

void gui::Wrapper_Keyboard_Panel(unsigned char key, int x, int y)
{
    instance->Keyboard_Panel(key, x, y);
}

void gui::Wrapper_SimulationLoop()
{
    instance->SimulationLoop();
}

void gui::Wrapper_save_simulation(int)
{
    instance->save_simulation(0);
}


void gui::glui_noclose()
{
    glui_createevolution->hide();
    return;
}

// This is used to control the frame rate (60Hz).
void gui::Timer(int)
{
    glutSetWindow(mainWindow);
    glutPostRedisplay();
    glutTimerFunc(framePeriod, Wrapper_Timer, 0);
}

// This is used to control the frame rate (60Hz).
void gui::Timer2(int)
{
    glutSetWindow(plotterWindow);
    glutPostRedisplay();
    glutTimerFunc(framePeriod, Wrapper_Timer2, 0);
}

void gui::plotting() // function used by second window ds
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
            float best_xmax = evolution_control->best_fitnesses.size();
            float best_ymax = evolution_control->get_bestFitness_overall();
            if (0 == best_xmax)
                best_xmax = 1;
            if (0 == best_ymax)
                best_ymax = 1;

            for (unsigned int i = 1; i < best_xmax; ++i)
            {
                b2Vec2 V1 = b2Vec2(float( (i-1) / best_xmax)*40.0f - 30.0f, float(evolution_control->best_fitnesses[i-1] / best_ymax)*40.0f - 30.0f);
                b2Vec2 V2 = b2Vec2(float( (i) / best_xmax)*40.0f - 30.0f, float(evolution_control->best_fitnesses[i] / best_ymax)*40.0f - 30.0f);
                plot_debugDraw.DrawLine(V1, V2, b2Color(1.0f, 0.0f, 0.0f));
            }
            //use xmax and ymax also for average fitness to get the same scale
            //Plot AverageFitness-Graph
            for (unsigned int i = 1; i < evolution_control->average_fitnesses.size(); ++i)
            {
                b2Vec2 V1 = b2Vec2(float( (i-1) / best_xmax)*40.0f - 30.0f, float(evolution_control->average_fitnesses[i-1] / best_ymax)*40.0f - 30.0f);
                b2Vec2 V2 = b2Vec2(float( (i) / best_xmax)*40.0f - 30.0f, float(evolution_control->average_fitnesses[i] / best_ymax)*40.0f - 30.0f);
                plot_debugDraw.DrawLine(V1, V2, b2Color(0.0f, 0.0f, 0.0f));
            }

            //Plot Statistics
            if (0 == sim_parameter.mode)
            {
                plot_debugDraw.DrawString(5, 20, "Mode: Singleplayer");
            }
            else if (1 == sim_parameter.mode)
            {
                plot_debugDraw.DrawString(5, 20, "Mode: Evolution");
            }
            else if (2 == sim_parameter.mode)
            {
                plot_debugDraw.DrawString(5, 20, "Mode: Best Agents ");
            }

            //Plot Statistics
            if (0 == sim_parameter.evolve_algorithm)
            {
                plot_debugDraw.DrawString(5, 45, "Evolve_Algorithm: Hillclimber");
            }
            else if (1 == sim_parameter.evolve_algorithm)
            {
                plot_debugDraw.DrawString(5, 45, "Evolve_Algorithm: Simulated Annealing");
            }
            else if (2 == sim_parameter.evolve_algorithm)
            {
                plot_debugDraw.DrawString(5, 45, "Evolve_Algorithm: Supervised Learning");
            }
            else if (3 == sim_parameter.evolve_algorithm)
            {
                plot_debugDraw.DrawString(5, 45, "Evolve_Algorithm: Crossover");
            }
            else if (4 == sim_parameter.evolve_algorithm)
            {
                plot_debugDraw.DrawString(5, 45, "Evolve_Algorithm: Script");
            }

            plot_debugDraw.DrawString(5, 70, "generation: %d", evolution_control->generations);
            plot_debugDraw.DrawString(5, 95, "iterations: %d / %d", evolution_control->iterationsteps, evolution_control->evolvesteps);
            plot_debugDraw.DrawString(5, 120, "mutation_rate: %f", evolution_control->sim_parameter.mutation_rate);
            plot_debugDraw.DrawString(5, 145, "annealing_rate: %d", evolution_control->sim_parameter.annealing_rate);
            plot_debugDraw.DrawString(5, 170, "Actual Best_Fitness: %d", evolution_control->best_fitnesses.back());
            plot_debugDraw.DrawString(5, 185, "Actual Average_Fitness: %f", evolution_control->average_fitnesses.back());
            plot_debugDraw.DrawString(5, 210, "Best_Fitness overall: %d", evolution_control->get_bestFitness_overall());

            /*
               plot_debugDraw.DrawString(5, 550, "settings.viewcenter.x: %f", settings.viewCenter.x);
               plot_debugDraw.DrawString(5, 575, "settings.viewcenter.y: %f", settings.viewCenter.y);		
               plot_debugDraw.DrawString(5, 600, "Settings.viewcenter_plotter.x: %f", settings.viewCenter_plotter.x);
               plot_debugDraw.DrawString(5, 625, "Settings.viewcenter_plotter.y: %f", settings.viewCenter_plotter.y);
               plot_debugDraw.DrawString(5, 650, "viewzoom: %f", viewZoom);
               plot_debugDraw.DrawString(5, 675, "viewzoom_panel: %f", viewZoom_panel);
               plot_debugDraw.DrawString(5, 700, "ptx : %d, pty : %d, ptw: %d , pwh : %d", ptx, pty, ptw, pth);
               plot_debugDraw.DrawString(5, 725, "tx : %d, ty : %d, tw: %d , th : %d", tx, ty, tw, th);
               */
            if (oldCenter.x != settings.viewCenter_plotter.x || oldCenter.y != settings.viewCenter_plotter.y)
            {
                Resize_Panel(width, height);
            }

            glutSwapBuffers();
        }
    }
    else
    {
        //Clear 1st GLUT-Window
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glutSwapBuffers();
    }
}
void gui::Resize(int w, int h)
{
    width = w;
    height = h;

    GLUI_Master.get_viewport_area(&tx, &ty, &tw, &th);
    glViewport(tx, ty, tw, th);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float ratio = float32(tw) / float32(th);

    b2Vec2 extents(ratio * 25.0f, 25.0f);
    extents *= viewZoom;

    b2Vec2 lower = settings.viewCenter - extents;
    b2Vec2 upper = settings.viewCenter + extents;

    // L/R/B/T
    gluOrtho2D(lower.x, upper.x, lower.y, upper.y);
}

void gui::SimulationLoop()
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
            if (0 < sim_parameter.mode)  //If mode is not singleplayer
            {
                test->move_bodies(evolution_control->process(test->get_sensor_vectors()));
                reset_sim = evolution_control->evolve(sim_parameter.evolve_algorithm);
            }
            else //If Single-Player-Mode enabled 
            {
                /*
                   std::vector<double> local_output;
                   std::vector< std::vector<double> > local_inputs;

                   local_output = test->move_player();
                   record_outputs.push_back(local_output);
                   local_inputs = test->get_sensor_vectors();
                   record_inputs.push_back(local_inputs[0]);
                   */
            }

            if (settings.turbo)
            {
                for (int i = 0; i < 20; i++)
                {
                    test->Step(&settings);
                    if (0 < sim_parameter.mode)  //If mode is not singleplayer
                    {
                        test->move_bodies(evolution_control->process(test->get_sensor_vectors()));
                        reset_sim = evolution_control->evolve(sim_parameter.evolve_algorithm);
                        if (reset_sim) break;
                    }
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
    else
    {
        //Clear 1st GLUT-Window
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glutSwapBuffers();
    }
}

void gui::Resize_Panel(int32 w, int32 h)
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

void gui::Keyboard_Panel(unsigned char key, int x, int y)
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

b2Vec2 gui::ConvertScreenToWorld(int32 x, int32 y)
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

void gui::KeyboardUp(unsigned char key, int x, int y)
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

void gui::Mouse(int32 button, int32 state, int32 x, int32 y)
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

void gui::MouseMotion(int32 x, int32 y)
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
void gui::MouseWheel(int wheel, int direction, int x, int y)
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

//Called if OK-Button pressed
void gui::new_evolution(int)
{
    sim_parameter = temp_sim_parameter;
    Restart(0);

}

void gui::create_evolution(int)
{
    glui_createevolution->close();
    Createevolution_win();

    glui_createevolution->enable();
    glui_createevolution->show();
}

void gui::Restart(int)
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

    evolution_control = new evolutionary(sim_parameter, topology);
    test = new NeuralWorld(&(evolution_control->population), sim_parameter.amount_of_Object, sim_parameter.field_size, sim_parameter.mode);

    evolution_control->set_trainingdataV(trainingdataV);
    evolution_control->set_trainingdataA(trainingdataA);
    glui_createevolution->hide();
    Resize(width, height);
}

void gui::Restart_setVal(int)
{
    if (sim_parameter.population_size)
        Restart(0);
}

void gui::glui_create_hide(int)
{
    glui_createevolution->hide();
}

void gui::Pause(int)
{
    settings.pause = !settings.pause;
}

void gui::Exit(int code)
{
    // TODO: freeglut is not building on OSX
#ifdef FREEGLUT
    glutLeaveMainLoop();
#endif

    //Call destructors
    if (NULL != test)
    {
        delete test;
        test = NULL;
    }

    if (NULL != evolution_control)
    {
        delete evolution_control;
        evolution_control = NULL;
    }

    if (NULL != glui)
    {
        //  delete glui;
        glui->close();
        glui = NULL;
    }

    if (NULL != glui_createevolution)
    {
        //  delete glui_createevolution;
        glui_createevolution->close();
        glui_createevolution = NULL;
    }

    return;
    exit(code); 
}

void gui::SingleStep(int)
{
    settings.pause = 1;
    settings.singleStep = 1;
}

void gui::Keyboard(unsigned char key, int x, int y)
{

    if (test != NULL)
    {
        B2_NOT_USED(x);
        B2_NOT_USED(y);

        switch (key)
        {
            case 27:
                //#ifndef __APPLE__
                // freeglut specific function
                //			glutLeaveMainLoop();
                //#endif
                Exit(0);

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

void gui::KeyboardSpecial(int key, int x, int y)
{
    if (test != NULL)
    {
        B2_NOT_USED(x);
        B2_NOT_USED(y);

        int mod = glutGetModifiers();

        switch (key)
        {
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

gui::gui()
{

}

void gui::Createevolution_win()
{
    //Window to create new Evolution
    glui_createevolution = GLUI_Master.create_glui("New evolution", 0, 300, 300);
    glui_createevolution_id = glui_createevolution->get_glut_window_id(); 
    glui_createevolution->add_separator();

    glui_createevolution->add_statictext("General options");
    GLUI_Spinner* popSizeSpinner = glui_createevolution->add_spinner("Population size", GLUI_SPINNER_INT, &temp_sim_parameter.population_size);
    popSizeSpinner->set_int_limits(1, 100);
    GLUI_Spinner* amountObjectSpinner = glui_createevolution->add_spinner("Amount of objects", GLUI_SPINNER_INT, &temp_sim_parameter.amount_of_Object);
    amountObjectSpinner->set_int_limits(1, 1000);
    GLUI_Spinner* evolveTimeSpinner = glui_createevolution->add_spinner("Evolve time in sec", GLUI_SPINNER_INT, &temp_sim_parameter.evolvetime);
    evolveTimeSpinner->set_int_limits(5, 1000);

    GLUI_Spinner* fieldsizeSpinner = glui_createevolution->add_spinner("Field size", GLUI_SPINNER_INT, &temp_sim_parameter.field_size);
    fieldsizeSpinner->set_int_limits(40, 200);
    GLUI_Spinner* mutationRateSpinner = glui_createevolution->add_spinner("Mutation rate", GLUI_SPINNER_FLOAT, &temp_sim_parameter.mutation_rate);
    mutationRateSpinner->set_float_limits(0.01f, 1.0f);
    glui_createevolution->add_separator();

    glui_createevolution->add_statictext("Simulated annealing options");
    GLUI_Spinner* simulated_annealingSpinner = glui_createevolution->add_spinner("Annealing rate", GLUI_SPINNER_INT, &temp_sim_parameter.annealing_rate);
    simulated_annealingSpinner->set_int_limits(1, 10000);
    glui_createevolution->add_separator();

    glui_createevolution->add_statictext("Crossover options");
    GLUI_Spinner* simulated_eliteSpinner = glui_createevolution->add_spinner("Elite copies", GLUI_SPINNER_INT, &temp_sim_parameter.amount_of_elite_copies);
    simulated_eliteSpinner->set_int_limits(1, 100);
    GLUI_Spinner* simulated_crossoverrateSpinner = glui_createevolution->add_spinner("Crossover rate", GLUI_SPINNER_FLOAT, &temp_sim_parameter.crossover_rate);
    simulated_crossoverrateSpinner->set_float_limits(0.01f, 1.0f);

    glui_createevolution->add_separator();
    glui_createevolution->add_statictext("Evolutionary algorithm");

    GLUI_RadioGroup* evolve_algorithms_radiogroup = glui_createevolution->add_radiogroup(&temp_sim_parameter.evolve_algorithm);

    glui_createevolution->add_radiobutton_to_group(evolve_algorithms_radiogroup, "Hillclimber");
    glui_createevolution->add_radiobutton_to_group(evolve_algorithms_radiogroup, "Simulated annealing");
    glui_createevolution->add_radiobutton_to_group(evolve_algorithms_radiogroup, "Supervised learning");
    glui_createevolution->add_radiobutton_to_group(evolve_algorithms_radiogroup, "Crossover");
    //		glui_createevolution->add_radiobutton_to_group(evolve_algorithms_radiogroup, "Script");

    glui_createevolution->add_separator();


    glui_createevolution->add_statictext("Network type");
    GLUI_RadioGroup* nettype_radiogroup = glui_createevolution->add_radiogroup(&temp_sim_parameter.nettype);
    glui_createevolution->add_radiobutton_to_group(nettype_radiogroup, "Feedforward");
    //		glui_createevolution->add_radiobutton_to_group(nettype_radiogroup, "Simple Recurrent");
    //		glui_createevolution->add_radiobutton_to_group(nettype_radiogroup, "Script");

    glui_createevolution->add_separator();

    glui_createevolution->add_button("OK", 0, Wrapper_new_evolution);

    glui_createevolution->add_button("Cancel", 0, Wrapper_glui_create_hide);

    //Noeff glutCloseFunc(Wrapper_glui_noclose);

    glui_createevolution->set_main_gfx_window(mainWindow);
    glui_createevolution->hide();

    //Clear 2nd GLUT Window
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glutSwapBuffers();
}


void gui::Create_Panel()
{

    glui = GLUI_Master.create_glui_subwindow(mainWindow, GLUI_SUBWINDOW_BOTTOM);


    GLUI_Panel* drawPanel = glui->add_panel("Simulation");
    glui->add_button_to_panel(drawPanel, "New Evolution", 0, Wrapper_create_evolution); 
    glui->add_column_to_panel(drawPanel, true);
    glui->add_button_to_panel(drawPanel, "Pause/Resume", 0, Wrapper_Pause);
    glui->add_column_to_panel(drawPanel, true);
    glui->add_button_to_panel(drawPanel, "Restart", 0, Wrapper_Restart_setVal);
    glui->add_column_to_panel(drawPanel, true);
    glui->add_button_to_panel(drawPanel, "Save simulation", 0, Wrapper_save_simulation);
    glui->add_column_to_panel(drawPanel, true);
    glui->add_button_to_panel(drawPanel, "Quit", 0, (GLUI_Update_CB)Wrapper_Exit);
    glui->add_column_to_panel(drawPanel, true);

    glui->set_main_gfx_window(mainWindow);

    // Use a timer to control the frame rate.
    glutTimerFunc(framePeriod, Wrapper_Timer, 0);

}

//Second Window for Plotting ds
void gui::Create_plotterwin()
{
    plotterWindow = glutCreateSubWindow(mainWindow, 880, 0, 400, 550); //848 before
    glutDisplayFunc(Wrapper_plotting);
    glutKeyboardFunc(Wrapper_Keyboard_Panel);
    glutReshapeFunc(Wrapper_Resize_Panel);
    // Use a timer to control the frame rate.
    glutTimerFunc(framePeriod, Wrapper_Timer2, 0);
}


void gui::init()
{
    created = false;
    glui = NULL;
    glui_createevolution = 0; //was NULL before
    glui_createevolution_id = 0;
    plotterWindow = 0;

    //Init Box2D-Simulation
    settings.velocityIterations = 8;
    settings.positionIterations = 3;
    settingsHz = 60;

    settings.pause = 0;
    ptx = 0; pty = 0; ptw = 0; pth = 0;
    tx = 0; ty = 0; tw = 0; th = 0;
    settings.viewCenter = b2Vec2(0.0f, 0.0f);
    settings.viewCenter_plotter = b2Vec2(0.0f, 0.0f);
    lastp = b2Vec2(0.0f, 0.0f);

    rMouseDown = false;

    //settings.viewCenter = b2Vec2(0.0, 0.0f);

    //Init 
    //Default Values
    sim_parameter.population_size = 0;  // For check of unitialised Varialbes on early Restart/Button press

    sim_parameter.amount_of_Object = 0;
    sim_parameter.evolvetime = 0;
    sim_parameter.evolve_algorithm = 0;
    sim_parameter.field_size = 0;
    sim_parameter.mode = 0;
    sim_parameter.mutation_rate = 0.0f;
    sim_parameter.annealing_rate = 0;
    sim_parameter.nettype = 0; //Feedforward
    sim_parameter.amount_of_elite_copies = 0;
    sim_parameter.crossover_rate = 0.0f;

    temp_sim_parameter.amount_of_Object = 40;
    temp_sim_parameter.evolvetime = 30;
    temp_sim_parameter.evolve_algorithm = 1;
    temp_sim_parameter.field_size = 50;
    temp_sim_parameter.mode = 1;
    temp_sim_parameter.mutation_rate = 0.3f;
    temp_sim_parameter.annealing_rate = 300;
    temp_sim_parameter.population_size = 10;
    temp_sim_parameter.nettype = 0; //Feedforward
    temp_sim_parameter.amount_of_elite_copies = 3;
    temp_sim_parameter.crossover_rate = 0.7f;

    //Evolution
    load_trainingData();
}

gui::gui(char * ptitle, int px, int py, int pwidth, int pheight, int argc, char** argv)
{
    x = px; y = py; width = pwidth; height = pheight;

    title = ptitle;

    init();

    //GUI START
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(width, height);

    mainWindow = glutCreateWindow(title);
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

    glutDisplayFunc(Wrapper_SimulationLoop);
    GLUI_Master.set_glutReshapeFunc(Wrapper_Resize);
    GLUI_Master.set_glutKeyboardFunc(Wrapper_Keyboard);
    GLUI_Master.set_glutSpecialFunc(Wrapper_KeyboardSpecial);
    GLUI_Master.set_glutMouseFunc(Wrapper_Mouse);
#ifdef FREEGLUT
    glutMouseWheelFunc(Wrapper_MouseWheel);
#endif
    glutMotionFunc(Wrapper_MouseMotion);

    glutKeyboardUpFunc(Wrapper_KeyboardUp);

    Createevolution_win();
    Create_Panel();
    Create_plotterwin();

    //Clear 1st GLUT-Window
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glutSwapBuffers();
}

void gui::Start()
{
    setInstance(this);
    glutMainLoop();
}

void gui::load_trainingData()
{
    // Line buffer
    std::string line = "";
    // Network topology
    std::string topology_string = "";

    std::ifstream training_file_V("trainingDataV.txt");
    std::ifstream training_file_A("trainingDataA.txt");

    //Load topology from the first line

    if (training_file_V.is_open())
    {
        //Load topology from the first line
        getline(training_file_V, topology_string);
        int first = 0;
        int last = 0;

        //Skip the description till space
        while (' ' != topology_string[first])
            first++;
        first++;
        last = first;

        for(int i=0; i<3;++i)
        {   
            std::string tmpString = "";
            while (' ' != topology_string[last])
            {
                last++;
            }   
            last--;

            tmpString = topology_string.substr(first, last);
            topology.push_back(atoi(tmpString.c_str()));
            last += 2;
            first = last;
        }
        while (getline(training_file_V, line))
        {
            first = 0; last = 0;
            std::vector<double> single;
            //Skip the description till space
            while (' ' != line[first])
                first++;
            first++;
            last = first;
            //2 Input-Vals
            for(int i=0; i<2;++i)
            {   
                std::string tmpString = "";
                while (' ' != line[last] && '\0' != line[last])
                {
                    last++;
                }   

                tmpString = line.substr(first, last-first); //substr(pos, length)
                single.push_back(atof(tmpString.c_str()));
                last ++;
                first = last;
            }
            //1 Outputval
            getline(training_file_V, line);
            first = 0; last = 0;
            //Skip the description till space
            while (' ' != line[first])
                first++;
            first++;
            last = first;
            std::string tmpString = "";
            while (' ' != line[last] && '\0' != line[last])
            {
                last++;
            }   

            tmpString = line.substr(first, last-first);
            single.push_back(atof(tmpString.c_str()));

            trainingdataV.push_back(single);
        }
        training_file_V.close();
    }
    else
    {
        std::cerr << "Unable to open velocity file";
    }

    if (training_file_A.is_open())
    {
        //Skip the topology, because we already got it
        getline(training_file_A, line);

        int first = 0;
        int last = 0;
        while (getline(training_file_A, line))
        {
            first = 0; last = 0;
            std::vector<double> single;
            //Skip the description till space
            while (' ' != line[first])
                first++;
            first++;
            last = first;
            //2 Input-Vals
            for(int i=0; i<2;++i)
            {   
                std::string tmpString = "";
                while (' ' != line[last] && '\0' != line[last])
                {
                    last++;
                }   

                tmpString = line.substr(first, last-first); //substr(pos, length)
                single.push_back(atof(tmpString.c_str()));
                last ++;
                first = last;
            }
            //1 Outputval
            getline(training_file_A, line);
            first = 0; last = 0;
            //Skip the description till space
            while (' ' != line[first])
                first++;
            first++;
            last = first;
            std::string tmpString = "";
            while (' ' != line[last] && '\0' != line[last])
            {
                last++;
            }   

            tmpString = line.substr(first, last-first);
            single.push_back(atof(tmpString.c_str()));

            trainingdataA.push_back(single);
        }
        training_file_A.close();
    }
    else
    {
        std::cerr << "Unable to open angle file";
    }
}

void gui::save_simulation(int)
{
    if(NULL == evolution_control)
    {
        return;
    }

    std::ofstream sim_ofstream("simulation.txt", std::ofstream::out);
    if(!sim_ofstream)
    {
        std::cerr << "Error opening file to save simulation" << std::endl;
    }
    else
    {
        sim_ofstream << "Topology: ";
        for(int i=0;i<evolution_control->sim_parameter.topology.size();++i)
        {
            sim_ofstream << evolution_control->sim_parameter.topology[i] << " ";
        }
        sim_ofstream << "Network-Type : Feed Forward Net" << std::endl;
        sim_ofstream << "" << std::endl;
        sim_ofstream << "Simulation settings: " << std::endl;
        sim_ofstream << "Population size: " <<  sim_parameter.population_size << std::endl;
        sim_ofstream << "Amount of Objects: " <<  sim_parameter.amount_of_Object<< std::endl;
        sim_ofstream << "Evolvetime(sec): " <<  sim_parameter.evolvetime << std::endl;
        sim_ofstream << "Field size: " <<  sim_parameter.field_size << std::endl;
        sim_ofstream << "Mutation rate: " <<  sim_parameter.mutation_rate << std::endl;
        sim_ofstream << "Annealing rate: " <<  sim_parameter.annealing_rate<< std::endl;
        sim_ofstream << "Amount of elite copies: " <<  sim_parameter.amount_of_elite_copies << std::endl;
        sim_ofstream << "Crossover rate: " <<  sim_parameter.crossover_rate << std::endl;

        if (0 == sim_parameter.evolve_algorithm)
        {
            sim_ofstream << "Evolutionary Algorithm: Hillclimber" << std::endl;
            plot_debugDraw.DrawString(5, 45, "Evolve_Algorithm: Hillclimber");
        }
        else if (1 == sim_parameter.evolve_algorithm)
        {
            sim_ofstream << "Evolutionary Algorithm: Simulated Annealing" << std::endl;
        }
        else if (2 == sim_parameter.evolve_algorithm)
        {
            sim_ofstream << "Evolutionary Algorithm: Supervised Learning" << std::endl;
        }
        else if (3 == sim_parameter.evolve_algorithm)
        {
            sim_ofstream << "Evolutionary Algorithm: Crossover" << std::endl;
        }
        else if (4 == sim_parameter.evolve_algorithm)
        {
            sim_ofstream << "Evolutionary Algorithm: Script" << std::endl;
        }

        sim_ofstream << "" << std::endl;
        sim_ofstream << "Generations simulated: " << evolution_control->generations << std::endl;
        sim_ofstream << "Best fitness over the whole process: " << evolution_control->get_bestFitness_overall()  << std::endl;

        sim_ofstream << "Best fitnesses for each generations: ";
        for(int i=1;i<evolution_control->best_fitnesses.size();++i)
        {
            sim_ofstream << " " << evolution_control->best_fitnesses[i];
        }
        sim_ofstream << std::endl;
        sim_ofstream << "Average fitnesses for each generations: ";
        for(int i=1;i<evolution_control->average_fitnesses.size();++i)
        {
            sim_ofstream << " " << evolution_control->average_fitnesses[i];
        }   
        sim_ofstream << std::endl;
        sim_ofstream << std::endl;
        sim_ofstream << "Each agents fitness and net-weights after the process: " << std::endl;
        for(int i=0;i<sim_parameter.population_size;++i)
        {
            sim_ofstream << "Agent[" << i << "] fitness: " << evolution_control->population[i]->fitness << std::endl;
        }
        sim_ofstream <<  std::endl  << std::endl;


        for(int k=0;k<sim_parameter.population_size;++k)
        {

            sim_ofstream << "Agent[" << k << "] angle_net weights: " << std::endl;

            //Save angle net
            for (unsigned int i=0; i<evolution_control->population[k]->angle_net->m_layers.size();i++)  //Amount of Layers
            {
                for(unsigned int j=0;j<evolution_control->population[k]->angle_net->m_layers[i].size();j++)    //Amount of Neurons
                {
                    for (unsigned int w = 0; w < evolution_control->population[k]->angle_net->m_layers[i][j]->m_outputWeights.size(); w++)
                    {
                        sim_ofstream << evolution_control->population[k]->angle_net->m_layers[i][j]->m_outputWeights[w].weight << " ";
                    }
                }
            }

            sim_ofstream << std::endl;
            sim_ofstream << "velocity_net weights: " << std::endl;

            //Save velocity net
            for (unsigned int i=0; i<evolution_control->population[k]->velocity_net->m_layers.size();i++)  //Amount of Layers
            {
                for(unsigned int j=0;j<evolution_control->population[k]->velocity_net->m_layers[i].size();j++)    //Amount of Neurons
                {
                    for (unsigned int w = 0; w < evolution_control->population[k]->velocity_net->m_layers[i][j]->m_outputWeights.size(); w++)
                    {
                        sim_ofstream << evolution_control->population[k]->velocity_net->m_layers[i][j]->m_outputWeights[w].weight << " ";
                    }
                }
            }
            sim_ofstream << std::endl <<  std::endl;
        }
        sim_ofstream.close();
    }
}
