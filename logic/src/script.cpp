#include "script.hpp"
#include <math.h>

#define M_PI 3.14159265358979323846

Script::Script()
{
    idleness_count_ = 0;
    // Start by moving straight forward
    current_angle_  = 0.0f;
}

Script::~Script()
{
}

double Script::processV(std::vector<double> inputvals)
{
    const float radius = 20.0f;
    float hyp = 0.0f;
    float velocity  = 0.0f;

    float x = inputvals[0];
    float y = inputvals[1];


    if(0 == x && 0 == y)
    {
        // Needed to change angle in idleness
        idleness_count_++;
        current_angle_ = 0.0f;

        // Velocity
        return 0.33f;
    }
    else
    {
        idleness_count_ = 0;
    }

    hyp = sqrt(x*x + y*y);

    velocity = hyp / radius;
    // So as to just compute it once
    current_angle_ = atan2(y, x) - (M_PI/2.0f);

    // Let's make sure we won't get too slow
    if(0.2 > velocity)
        velocity = 0.2f;

    return velocity;
}

double Script::processA(std::vector<double> inputvals)
{
    // So as to gradually change rotation in idleness
    if(0 < idleness_count_)
        // Iterates at 60hz -> change angle every 50 ms
        if(0 == idleness_count_ % (unsigned(0.05 * 60)))
        {
            // Angle
            current_angle_ += M_PI/48.0f;
            idleness_count_ = 0;
        }

    return current_angle_;
}
