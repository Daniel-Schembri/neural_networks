/*
* Copyright (c) 2015 Daniel Schembri https://github.com/Daniel-Schembri/neural_networks
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
* At following the license for Box2D
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

#include "render.h"
#include "test.h"
#include "glui/glui.h"
#include <stdio.h>
#include <vector>
#include "neuralWorld.h"
#include "evolutionary.hpp"
#include "gui.h"


int main(int argc, char** argv)
{

	gui *ngui = new gui("Simulation of Agents", 0, 0, 1280, 928, argc, argv);

	ngui->Start();

	return 0;
}
