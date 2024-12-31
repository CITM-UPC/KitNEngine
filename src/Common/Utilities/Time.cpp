//
// Created by Roger on 10/11/2024.
//
#include "Game/Utilities/Time.h"

using namespace std::chrono;

duration<float> Time::timeDelta = seconds(0);

time_point<steady_clock, milliseconds> Time::startTime;

float Time::timeScale = 1.0f;

float Time::GetDeltaTime()
{
    return timeDelta.count();
}

void Time::UpdateDeltaTime(duration<float> const& delta)
{
    timeDelta = delta;
}
