#ifndef SIMPLESCRIPT_H
#define SIMPLESCRIPT_H

#define M_PI 3.14159265359
#define DEGTORAD (M_PI/180)

#include "Brain.h"

//Abstract Class to process the inputs
class Simplescript : public Brain
{
//Attributes
public:


private:
	
	

//Methods
public:
	Simplescript();
	virtual std::vector<double> process(std::vector<double> &inputVals);

};
#endif
