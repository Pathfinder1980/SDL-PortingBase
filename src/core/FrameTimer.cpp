#include "FrameTimer.h"

namespace porting_base
{
    FrameTimer::FrameTimer(double fixedTimeStep, double maxFrameSeconds) : 
    m_FixedTimeStep(fixedTimeStep), 
    m_MaxFrameSeconds(maxFrameSeconds)
    {
    }

    int FrameTimer::Advance(double frameSeconds)
    {
        if (frameSeconds < 0.0) 
        { 
            frameSeconds = 0.0; 
        }
        if (frameSeconds > m_MaxFrameSeconds)
        {
            frameSeconds = m_MaxFrameSeconds;
        }

        m_AccumulatedTime += frameSeconds;

        int steps = static_cast<int>(m_AccumulatedTime / m_FixedTimeStep );
        m_TimeSimulated += steps * m_FixedTimeStep;
        m_AccumulatedTime -= steps * m_FixedTimeStep;
        m_AlphaTime = m_AccumulatedTime / m_FixedTimeStep;
        return steps;
    }

    double FrameTimer::GetAlphaTime() const
    {
        return m_AlphaTime;
    }
} // namespace porting_base
