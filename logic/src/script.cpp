#include "script.hpp"

#define M_PI 3.14159265358979323846

Script::Script()
{

}

void Script::function()
{

}

//TODO: Algorithm correct?
std::vector<double> Script::process(std::vector<double> inputvals)
{
    const float radius = 20.0f;
    float hyp = 0.0f;

    std::vector<double> resultvals;
    float angle = 0.0f;
    float velocity  = 0.0f;

    float x = inputvals[0];
    float y = inputvals[1];


    if(0 == x && 0 == y)
    {
        resultvals.push_back(0);
        resultvals.push_back(0);
        return resultvals;
    }

    hyp = sqrt(x*x + y*y);

    angle = atan2(y, x) - (M_PI/2.0f);
    velocity = hyp / radius;

    std::cout << "Input: ("<< x << "," << y << ") Output: (" << velocity << "," << angle << ")\n";
    resultvals.push_back(velocity);
    resultvals.push_back(angle);

    std::cout << "velocity: " << velocity << std::endl; 
    std::cout << "angle: " << angle << std::endl; 

    //function();

    return resultvals;
}


