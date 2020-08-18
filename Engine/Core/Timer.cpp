#include "pch.h"
#include "Timer.h"

void nc::Timer::Reset()
{
    m_timePoint = clock::now();
}

nc::Timer::clock::rep nc::Timer::ElapsedTicks()
{
    clock_duration duration = (clock::now() - m_timePoint);

    return duration.count();
}

nc::Timer::clock::rep nc::Timer::TicksPerSecond()
{
    return clock_duration::period::den;
}

double nc::Timer::ElapsedSeconds()
{
    return ElapsedTicks() / static_cast<double>(TicksPerSecond());
}



void nc::FrameTimer::Tick()
{
    m_dt = static_cast<float>(ElapsedSeconds());
    m_timePoint = clock::now();
}

