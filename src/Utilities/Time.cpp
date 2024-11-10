//
// Created by Roger on 10/11/2024.
//
#include "Utilities/Time.h"

using namespace std::chrono;

duration<float> Time::timeDelta = seconds(0);

time_point<steady_clock, milliseconds> Time::startTime;

long long Time::GetTime()
{
    return since(startTime).count();
}

float Time::GetDeltaTime()
{
    return timeDelta.count();
}

void Time::UpdateDeltaTime(duration<float> const& delta)
{
    timeDelta = delta;
}
