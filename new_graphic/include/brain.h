#ifndef BRAIN_H
#define BRAIN_H

#define M_PI 3.14159265359
#define DEGTORAD (M_PI/180)

#include "net.hpp"

//Abstract Class to process the inputs
class Brain
{
//Attributes
public:


protected:
	std::vector<double> input;
	std::vector<double> output;
	//void getResults(std::vector<double> &resultVals) const;
	

//Methods
public:
	Brain();
	virtual std::vector<double> process(std::vector<double> &inputVals) = 0;

};
#endif
